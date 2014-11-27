#include "GEMCode/GEMValidation/src/TrackMatcher.h"
#include "TLorentzVector.h"
#include <map>

TrackMatcher::TrackMatcher(SimHitMatcher& sh, CSCDigiMatcher& csc_dg, 
                           GEMDigiMatcher& gem_dg, RPCDigiMatcher& rpc_dg, 
                           CSCStubMatcher& csc_st)
: CSCStubMatcher(sh, csc_dg, gem_dg, rpc_dg)
, sh_matcher_(&sh)
, gem_digi_matcher_(&gem_dg)
, csc_digi_matcher_(&csc_dg)
, rpc_digi_matcher_(&rpc_dg)                 
{
  auto tfTrack = conf().getParameter<edm::ParameterSet>("cscTfTrack");
  cscTfTrackInputLabel_ = tfTrack.getParameter<edm::InputTag>("input");
  minBXTFTrack_ = tfTrack.getParameter<int>("minBX");
  maxBXTFTrack_ = tfTrack.getParameter<int>("minBX");
  verboseTFTrack_ = tfTrack.getParameter<int>("verbose");
  deltaRTFTrack_ = tfTrack.getParameter<double>("deltaR");
  
  auto tfCand = conf().getParameter<edm::ParameterSet>("cscTfCand");
  cscTfCandInputLabel_ = tfCand.getParameter<edm::InputTag>("input");
  minBXTFCand_ = tfCand.getParameter<int>("minBX");
  maxBXTFCand_ = tfCand.getParameter<int>("minBX");
  verboseTFCand_ = tfCand.getParameter<int>("verbose");
  deltaRTFCand_ = tfCand.getParameter<double>("deltaR");
  
  auto gmtRegCand = conf().getParameter<edm::ParameterSet>("gmtRegCand");
  gmtRegCandInputLabel_ = gmtRegCand.getParameter<edm::InputTag>("input");
  minBXGMTRegCand_ = gmtRegCand.getParameter<int>("minBX");
  maxBXGMTRegCand_ = gmtRegCand.getParameter<int>("minBX");
  verboseGMTRegCand_ = gmtRegCand.getParameter<int>("verbose");
  deltaRGMTRegCand_ = gmtRegCand.getParameter<double>("deltaR");
  
  auto gmtCand = conf().getParameter<edm::ParameterSet>("gmtCand");
  gmtCandInputLabel_ = gmtCand.getParameter<edm::InputTag>("input");
  minBXGMTCand_ = gmtCand.getParameter<int>("minBX");
  maxBXGMTCand_ = gmtCand.getParameter<int>("minBX");
  verboseGMTCand_ = gmtCand.getParameter<int>("verbose");
  deltaRGMTCand_ = gmtCand.getParameter<double>("deltaR");
  
  auto l1Extra = conf().getParameter<edm::ParameterSet>("l1Extra");
  l1ExtraInputLabel_ = l1Extra.getParameter<edm::InputTag>("input");
  minBXL1Extra_ = l1Extra.getParameter<int>("minBX");
  maxBXL1Extra_ = l1Extra.getParameter<int>("minBX");
  verboseL1Extra_ = l1Extra.getParameter<int>("verbose");
  deltaRL1Extra_ = l1Extra.getParameter<double>("deltaR");

  CSCTFSPset_ = conf().getParameter<edm::ParameterSet>("sectorProcessor");
  ptLUTset_ = CSCTFSPset_.getParameter<edm::ParameterSet>("PTLUT");
  srLUTset_ = CSCTFSPset_.getParameter<edm::ParameterSet>("SRLUT");
  
  simPt = trk().momentum().pt();
  simEta = trk().momentum().eta();
  simPhi = trk().momentum().phi();
  simE = trk().momentum().E();
  simCharge = trk().charge();
  hasMuScales_ = true;
  hasMuPtScale_ = true;

  ptLUT_ = nullptr;
  for(int e=0; e<2; e++) for (int s=0; s<6; s++) my_SPs_[e][s] = nullptr;
  //  CSCSectorReceiverLUT* srLUTs_[5][6][2];
  dtrc_ = nullptr;

  //std::cout<<" TrackMatcher constructor" <<std::endl;
  clear();
  init();
}

TrackMatcher::~TrackMatcher() 
{
 // if(ptLUT_) delete ptLUT_;
 // ptLUT_ = nullptr;
    //std::cout<<" TrackMatcher destructor" <<std::endl;
  delete ptLUT_;
  ptLUT_ = nullptr;

  for(int e=0; e<2; e++) for (int s=0; s<6; s++){
   // if  (my_SPs_[e][s])
       	delete my_SPs_[e][s];
        my_SPs_[e][s] = NULL;
    
  }
  for(int endcap = 1; endcap<=2; endcap++) {
    for(int sector=1; sector<=6; sector++) {
      for(int station=1,fpga=0; station<=4 && fpga<5; station++) {
        if(station==1) for(int subSector=0; subSector<2; subSector++)
          delete srLUTs_[fpga++][sector-1][endcap-1];
        else
          delete srLUTs_[fpga++][sector-1][endcap-1];
      }
    }
  }


  for (auto trk:tfTracks_)  delete trk;

 // if(dtrc_) 
      delete dtrc_;
  dtrc_ = nullptr;
  clear();

//  delete sh_matcher_;
//  delete gem_digi_matcher_;
//  delete csc_digi_matcher_;
//  delete rpc_digi_matcher_;
//  delete csc_stub_matcher_;
}

void 
TrackMatcher::clear()
{
  tfTracks_.clear();
  tfCands_.clear();
  gmtRegCands_.clear();
  gmtCands_.clear();
  l1Extras_.clear();
}

void 
TrackMatcher::init()
{  
  try {
    eventSetup().get<L1MuTriggerScalesRcd>().get(muScalesHd_);
    muScales_ = &*muScalesHd_;
  } catch (edm::eventsetup::NoProxyException<L1MuTriggerScalesRcd>& e) {
    hasMuScales_ = false;
    LogDebug("TrackMatcher") << "+++ Info: L1MuTriggerScalesRcd is unavailable. +++\n";
  }

  try {
    eventSetup().get<L1MuTriggerPtScaleRcd>().get(muPtScaleHd_);
    muPtScale_ = &*muPtScaleHd_;
  } catch (edm::eventsetup::NoProxyException<L1MuTriggerPtScaleRcd>& e) {
    hasMuPtScale_ = false;
    LogDebug("TrackMatcher") << "+++ Info: L1MuTriggerPtScaleRcd is unavailable. +++\n";
  }

  if (ptLUT_) delete ptLUT_;
  ptLUT_ = new CSCTFPtLUT(ptLUTset_, muScalesHd_.product(), muPtScaleHd_.product());

  /*// Sector Processor and Receiver
  for(int e=0; e<2; e++) {
    for (int s=0; s<6; s++){
      my_SPs_[e][s] = new CSCTFSectorProcessor(e+1, s+1, CSCTFSPset_, true, muScalesHd_.product(), muPtScaleHd_.product());
      my_SPs_[e][s]->initialize(eventSetup());
    }
  }*/

 const bool TMB07(true);  
  for(int endcap = 1; endcap<=2; endcap++) {
    for(int sector=1; sector<=6; sector++) {
      for(int station=1,fpga=0; station<=4 && fpga<5; station++) {
        if(station==1) for(int subSector=0; subSector<2; subSector++)
          srLUTs_[fpga++][sector-1][endcap-1] = new CSCSectorReceiverLUT(endcap, sector, subSector+1, station, srLUTset_, TMB07);
        else
          srLUTs_[fpga++][sector-1][endcap-1] = new CSCSectorReceiverLUT(endcap, sector, 0, station, srLUTset_, TMB07);
      }
    }
  }

  dtrc_ = new CSCTFDTReceiver();
  
  // tracks produced by TF
  edm::Handle<L1CSCTrackCollection> hl1Tracks;
  event().getByLabel(cscTfTrackInputLabel_,hl1Tracks);
  matchTfTrackToSimTrack(*hl1Tracks.product());
  propagateSimTrack();
  propagationInterStation();
  
  // L1 muon candidates after CSC sorter
  edm::Handle<std::vector<L1MuRegionalCand> > hl1TfCands;
  event().getByLabel(cscTfCandInputLabel_, hl1TfCands);
  const std::vector<L1MuRegionalCand>* l1TfCands = hl1TfCands.product();
  matchTfCandToSimTrack(l1TfCands);
}

void 
TrackMatcher::matchTfTrackToSimTrack(const L1CSCTrackCollection& tracks)
{

  for (L1CSCTrackCollection::const_iterator trk = tracks.begin(); trk != tracks.end(); trk++) {
     verboseTFTrack_ = 0;
    TFTrack *track = new TFTrack(&trk->first,&trk->second);
    track->init(ptLUT_, muScalesHd_, muPtScaleHd_);
    
    float dR = 10.0;
    TLorentzVector templ1muon;
    templ1muon.SetPtEtaPhiM(track->pt(), track->eta(), track->phi(), 0.1057);

    
    if (simEta*track->eta() < 0) continue;
    // calculate the deltaR using the simhits in the 2nd CSC station -- reference station
    auto p(sh_matcher_->chamberIdsCSC(CSC_ME21));  
    auto pp(sh_matcher_->chamberIdsCSC(CSC_ME22));  
    p.insert(pp.begin(),pp.end());

    TLorentzVector simmuon;
       
    if (verboseTFTrack_ > 1) std::cout << "----------------------------------------------------------" << std::endl;
    if (verboseTFTrack_ > 1) std::cout << "detids " << std::endl;
    
    if (p.size()!=0) {
      if (verboseTFTrack_ > 1) std::cout << CSCDetId(*p.begin()) << std::endl;  
      auto hits(sh_matcher_->hitsInChamber(*p.begin()));
      auto gp(sh_matcher_->simHitsMeanPosition(hits));
      if (verboseTFTrack_ > 1) std::cout << gp << std::endl;
      // simmuon.SetPtEtaPhiM(simPt, gp.eta(), gp.phi(), 1.057);
    }
    
   //propagate simtrack to station2
   float phi_cor = phiHeavyCorr(simPt, simEta, simPhi, simCharge); 
   simmuon.SetPtEtaPhiM(simPt, simEta, phi_cor, 0.1057);
   dR = simmuon.DeltaR(templ1muon);
   /*  auto lcts2(lctsInStation(2));
    auto lcts3(lctsInStation(3));
    lcts2.insert(lcts2.end(),lcts3.begin(),lcts3.end());

    if (lcts2.size() != 0) {
    
	TLorentzVector simmuon;
        for (auto stub : lcts2)
        {
	    if (verboseTFTrack_ > 1)   std::cout <<"stubs in st2,3 "<< stub << std::endl;
	    auto EtaPhi(intersectionEtaPhi(digi_id(stub), digi_wg(stub), digi_channel(stub)));
            //simmuon.SetPtEtaPhiE(simPt, simEta, simPhi, simE);
            simmuon.SetPtEtaPhiE(simPt, EtaPhi.first, EtaPhi.second, simE);
	    if (simmuon.DeltaR(templ1muon) < dR)  dR = simmuon.DeltaR(templ1muon);
	}
    
    }
    else continue;//if no stubs available in station2,or 3 that can match to simtrack, then continue;???
     */

    if (verboseTFTrack_ > 1) std::cout << "----------------------------------------------------------" << std::endl;
    
   
    track->setDR(dR);

    // debugging
    /*if (track->nStubs()==0)
    {
        std::cout << "nstubs == 0" << std::endl;
	verboseTFTrack_ = 2;
    }*/
    //debug
    //if (!(track->hasStubEndcap(1) and track->hasStubEndcap(2)) and track->dr()< deltaRTFTrack_)
   // {
    //     std::cout <<"no stubs in station 1 or 2" << std::endl;
//	 verboseTFTrack_ = 2;
  //  }

    if (verboseTFTrack_){
      std::cout << "\tL1CSC TFTrack "<< trk-tracks.begin() << " information:" << std::endl;
      std::cout << "\tpt (GeV/c) = " << track->pt() << ", eta = " << track->eta() 
                << "\t, phi = " << track->phi() << ", dR(sim,L1) = " << track->dr() 
		<<" nStubs = "<< track->nStubs() << ", deltaphi12 = "<< track->dPhi12() <<", deltaphi23 = "<<track->dPhi23() <<std::endl;
      std::cout << " pt_packed " << track->ptPacked()  << " eta_packed " << track->etaPacked() << " phi_packed " << track->phiPacked() << std::endl;        
      std::cout << "simTrack \t simpt " << simPt << " simeta "<< simEta << " simPhi "<< simPhi <<" simenergy "<< simE << std::endl;
    }
    
    // check the stubs
    if (verboseTFTrack_ > 1){
      std::cout << " \t\tStubs:" << std::endl;
    }
    for (CSCCorrelatedLCTDigiCollection::DigiRangeIterator detUnitIt = trk->second.begin(); 
         detUnitIt != trk->second.end(); detUnitIt++) {
      const CSCDetId& id = (*detUnitIt).first;
      if (verboseTFTrack_ > 1){
        std::cout << "\t\tDetId: " << id << std::endl;
      }
      const CSCCorrelatedLCTDigiCollection::Range& range = (*detUnitIt).second;
      for (CSCCorrelatedLCTDigiCollection::const_iterator digiIt = range.first; digiIt != range.second; digiIt++) {
        auto lct(*digiIt);
        // check for valid stubs
        if (!(lct.isValid())) continue;
      //  track.addTriggerDigi(&lct);
      //  track.addTriggerDigiId(id);
	auto EtaPhi(intersectionEtaPhi(id, lct.getKeyWG(), lct.getStrip()));
        track->addTriggerEtaPhi(EtaPhi);
        track->addTriggerStub(buildTrackStub(lct, id));
        if (verboseTFTrack_ > 1 ){
	//    auto EtaPhi(intersectionEtaPhi(id, lct.getKeyWG(), lct.getStrip()));
          std::cout << "\t\tLCT" << digiIt-range.first<<" eta:"<< EtaPhi.first
	      <<" phi:"<< EtaPhi.second << ": " << lct << std::endl;
        }
      }
    }

    //check the dR of simtrack and tftrack
    if (track->dr() < deltaRTFTrack_) {
      tfTracks_.push_back(track);
      if (verboseTFTrack_) std::cout<<" dR(sim, L1) is ok "<<" deltaRTFTrack"<< deltaRTFTrack_ <<" real dr"<<track->dr()<< std::endl;
    }

  }
  int i=0;
  for (auto tftrk : tfTracks_)
  {
   if (verboseTFTrack_)  std::cout<<" track that can match to simtrack "<< i << " track pt " << tftrk->pt() <<" track eta " << tftrk->eta()
                                  <<" nstubs:"<< tftrk->nStubs()<<" dr:" << tftrk->dr() <<std::endl;
   i++;
  }
  
}

void 
TrackMatcher::matchTfCandToSimTrack(const std::vector<L1MuRegionalCand>* tracks)
{
  for (std::vector<L1MuRegionalCand>::const_iterator trk = tracks->begin(); trk != tracks->end(); trk++) {
    TFCand track(&*trk);
    track.init(ptLUT_, muScalesHd_, muPtScaleHd_);
    // calculate the DR
//     track.setDR(this->trk());

    // debugging
    if (verboseTFCand_){
      std::cout << "\tL1CSC TFCand "<< trk-tracks->begin() << " information:" << std::endl;
      std::cout << "\tpt (GeV/c) = " << track.pt() << ", eta = " << track.eta() 
                << "\t, phi = " << track.phi() << ", dR(sim,L1) = " << track.dr() << std::endl;      
    }
//     // match this one if the dR < matchDeltaR
//     if (track.dr() < deltaRTFCand_) {
//       tfCands_.push_back(track);
//     }
  }

}

void 
TrackMatcher::matchGmtRegCandToSimTrack(const L1MuRegionalCand& tracks)
{
}

void 
TrackMatcher::matchGmtCandToSimTrack(const L1MuGMTExtendedCand& tracks)
{
}

TFTrack* 
TrackMatcher::bestTFTrack(bool sortPtFirst) const
{
  //return 0;
  
  if (tfTracks_.size()==0) return NULL;
  
  // determine max # of matched stubs in the TFTrack collection
  unsigned int maxNMatchedMPC = 0;
  float minDeltaR = 10;
  unsigned index = 0;
  for (unsigned i=0; i<tfTracks_.size(); i++) {
    TFTrack* track = tfTracks_.at(i);
    unsigned int nStubs = track->nStubs();
    if (nStubs >= maxNMatchedMPC){
       if (track->dr()<minDeltaR)
       { 
	   minDeltaR = track->dr();
	   maxNMatchedMPC = nStubs;
	   index = i;
       }
    
    }
  }


  return tfTracks_.at(index);
 /* // collect tracks with max # of matched stubs
  std::vector<unsigned> bestMatchedTracks;
  for (unsigned i=0; i<tfTracks_.size(); i++) {
    int nst=0;
    for (size_t s=0; s<tfTracks_.at(i).ids.size(); s++) 
      if (tfTracks_.at(i).mplcts[s]->deltaOk) nst++;
    if (nst==maxNMatchedMPC) bestMatchedTracks.push_back(i);
  }
  
  // already found the best TFTrack
  if (bestMatchedTracks.size()==1) return &(tfTracks_[bestMatchedTracks[0]]);
  
  // case when you have more than 1 best TFTrack
  // first sort by quality
  double qBase  = 1000000.;
  // then sort by Pt inside the cone (if sortPtFirst), then sort by DR
  double ptBase = 0.;
  if (sortPtFirst) ptBase = 1000.;
  unsigned maxI = 99;
  double maxRank = -999999.;
  for (unsigned i=0; i<tfTracks_.size(); i++) {
    if (bestMatchedTracks.size()) {
      bool gotit=0;
      for (unsigned m=0;m<bestMatchedTracks.size();m++) if (bestMatchedTracks[m]==i) gotit=1;
      if (!gotit) continue;
    }
    double rr = qBase*tfTracks_.at(i).q_packed + ptBase*tfTracks_.at(i).pt_packed + 1./(0.01 + tfTracks_.at(i).dr);
    if (rr > maxRank) { maxRank = rr; maxI = i;}
  }
  if (maxI==99) return NULL;
  return &(tfTracks_.at(maxI));
  */
}


TFCand* 
TrackMatcher::bestTFCand(bool sortPtFirst) const
{
  /*
    if (cands.size()==0) return NULL;

    // determine max # of matched stubs
    int maxNMatchedMPC = 0;
    for (unsigned i=0; i<cands.size(); i++) 
    {
        int nst=0;
        if (cands[i].tftrack==0) continue;
        for (size_t s=0; s<cands[i].tftrack->ids.size(); s++) 
            if (cands[i].tftrack->mplcts[s]->deltaOk) nst++;
        if (nst>maxNMatchedMPC) maxNMatchedMPC = nst;
    }

    // collect tracks with max # of matched stubs
    std::vector<unsigned> bestMatchedTracks;
    if (maxNMatchedMPC>0) {
        for (unsigned i=0; i<cands.size(); i++) 
        {
            int nst=0;
            if (cands[i].tftrack==0) continue;
            for (size_t s=0; s<cands[i].tftrack->ids.size(); s++) 
                if (cands[i].tftrack->mplcts[s]->deltaOk) nst++;
            if (nst==maxNMatchedMPC) bestMatchedTracks.push_back(i);
        }
        if (bestMatchedTracks.size()==1) return &(cands[bestMatchedTracks[0]]);
    }

    // first sort by quality
    double qBase  = 1000000.;
    // then sort by Pt inside the cone (if sortPtFirst), then sort by DR
    double ptBase = 0.;
    if (sortPtFirst) ptBase = 1000.;
    unsigned maxI = 99;
    double maxRank = -999999.;
    for (unsigned i=0; i<cands.size(); i++) 
    {
        if (bestMatchedTracks.size()) {
            bool gotit=0;
            for (unsigned m=0;m<bestMatchedTracks.size();m++) if (bestMatchedTracks[m]==i) gotit=1;
            if (!gotit) continue;
        }
        // quality criterium you apply to get the best TFCand
        double rr = qBase*cands[i].l1cand->quality_packed() + ptBase*cands[i].l1cand->pt_packed() + 1./(0.01 + cands[i].dr);
        if (rr > maxRank) { maxRank = rr; maxI = i;}
    }
OB    if (maxI==99) return NULL;
    return &(cands[maxI]);
  */
  return 0;
}

GMTRegCand* 
TrackMatcher::bestGMTRegCand(bool sortPtFirst) const
{
  // first sort by Pt inside the cone (if sortPtFirst), then sort by DR
  if (gmtRegCands_.size()==0) return nullptr;
  const double ptBase(sortPtFirst ? 1000. : 0.);
  unsigned maxI = 99;
  double maxRank = -999999.;
  for (unsigned i=0; i<gmtRegCands_.size(); i++) {
    // quality criterium to sort the GMT Regional candidates
    const double rank(ptBase*gmtRegCands_.at(i)->pt() + 1./(0.01 + gmtRegCands_.at(i)->dr()));
    if (rank > maxRank) { 
      maxRank = rank; 
      maxI = i;
    }
  }
  if (maxI==99) return nullptr;
  return gmtRegCands_.at(maxI);
}

GMTCand* 
TrackMatcher::bestGMTCand(bool sortPtFirst) const
{
  // first sort by Pt inside the cone (if sortPtFirst), then sort by DR
  if (gmtCands_.size()==0) return nullptr;
  const double ptBase(sortPtFirst ? 1000. : 0.);
  unsigned maxI = 99;
  double maxRank = -999999.;
  for (unsigned i=0; i<gmtCands_.size(); i++) {
    // quality criterium to sort the GMT candidates
    const double rank(ptBase*gmtCands_.at(i)->pt() + 1./(0.01 + gmtCands_.at(i)->dr()));
    if (rank > maxRank) { 
      maxRank = rank; 
      maxI = i;
    }
  }
  if (maxI==99) return nullptr;
  return gmtCands_.at(maxI);
}

L1Extra* 
TrackMatcher::bestL1Extra(bool sortPtFirst) const
{
  return 0;
}

csctf::TrackStub 
TrackMatcher::buildTrackStub(const CSCCorrelatedLCTDigi &d, CSCDetId id)
{
  const unsigned fpga((id.station() == 1) ? CSCTriggerNumbering::triggerSubSectorFromLabels(id) - 1 : id.station());
  CSCSectorReceiverLUT* srLUT = srLUTs_[fpga][id.triggerSector()-1][id.endcap()-1];
  const unsigned cscid(CSCTriggerNumbering::triggerCscIdFromLabels(id));
  const unsigned cscid_special((id.station()==1 && id.ring()==4) ? cscid + 9 : cscid);
  const lclphidat lclPhi(srLUT->localPhi(d.getStrip(), d.getPattern(), d.getQuality(), d.getBend()));
  const gblphidat gblPhi(srLUT->globalPhiME(lclPhi.phi_local, d.getKeyWG(), cscid_special));
  const gbletadat gblEta(srLUT->globalEtaME(lclPhi.phi_bend_local, lclPhi.phi_local, d.getKeyWG(), cscid));

  return csctf::TrackStub(d, id, gblPhi.global_phi, gblEta.global_eta);
}



std::pair<float, float> 
TrackMatcher::intersectionEtaPhi(CSCDetId id, int wg, int hs)
{
  const CSCDetId layerId(id.endcap(), id.station(), id.ring(), id.chamber(), CSCConstants::KEY_CLCT_LAYER);
  const CSCLayer* csclayer(cscGeometry_->layer(layerId));
  const CSCLayerGeometry* layer_geo(csclayer->geometry());

  // LCT::getKeyWG() starts from 0
  const float wire(layer_geo->middleWireOfGroup(wg + 1));

  // half-strip to strip
  // note that LCT's HS starts from 0, but in geometry strips start from 1
  const float fractional_strip(0.5 * (hs + 1) - 0.25);
  const LocalPoint csc_intersect(layer_geo->intersectionOfStripAndWire(fractional_strip, wire));
  const GlobalPoint csc_gp(cscGeometry_->idToDet(layerId)->surface().toGlobal(csc_intersect));
  //std::cout << " simcharge " << simCharge << "  CSCDet Id  " << id << "   csc_gp.phi" << csc_gp.phi() << std::endl;
  return std::make_pair(csc_gp.eta(), csc_gp.phi());
}




void TrackMatcher::propagateSimTrack()
{
   int endcap = (simEta>0? 1 : 2);
   int ring = 1;
 //  auto it_odd(simTrackPropagateGPs_odd_.begin());
  for (int st=1; st<5; st++)
  {
     int chamber = 1;
     const CSCDetId layerId(endcap, st, ring, chamber, CSCConstants::KEY_CLCT_LAYER);
     const CSCLayer* csclayer(cscGeometry_->layer(layerId));
     const GlobalPoint gp = csclayer->centerOfWireGroup(10);
   //  std::cout <<" layerId " << layerId << "z position: " << gp.z() << std::endl;
     GlobalPoint gp_propagate(propagateToZ(gp.z()));
     //std::cout << "propagate z in TFMatcher:" <<gp_propagate.z() << std::endl;
     //simTrackDummy_[st]=st;
     simTrackPropagateGPs_odd_.push_back(std::make_pair(gp_propagate.eta(), gp_propagate.phi()));
    // std::cout << " CSCDet Id " << layerId << "  propageted_phi " << gp_propagate.phi() << std::endl;
     //simTrackPropagateGPs_odd_[st] = gp_propagate;
    }
  for (int st=1; st<5; st++)
  {
     int chamber = 2;
     const CSCDetId layerId(endcap, st, ring, chamber, CSCConstants::KEY_CLCT_LAYER);
     const CSCLayer* csclayer(cscGeometry_->layer(layerId));
     GlobalPoint gp = csclayer->centerOfWireGroup(10);
     //std::cout <<" layerId " << layerId << "z position: " << gp.z() << std::endl;
     GlobalPoint gp_propagate(propagateToZ(gp.z()));
     simTrackPropagateGPs_even_.push_back(std::make_pair(gp_propagate.eta(), gp_propagate.phi()));
     //std::cout << " CSCDet Id " << layerId << "  propageted_phi " << gp_propagate.phi() << std::endl;
    }

} 




GlobalPoint TrackMatcher::propagationInterStation(int firstSt, int SecondSt, bool odd)
{

  for (auto d: sh_matcher_->chamberIdsCSC(0))
  {
    CSCDetId id(d);
    int chamber(odd ? 1:2);
    const CSCDetId layerId(id.endcap(), SecondSt, 1, chamber, CSCConstants::KEY_CLCT_LAYER);
    const CSCLayer* csclayer(cscGeometry_->layer(layerId));
    const GlobalPoint gpAtSecondSt = csclayer->centerOfWireGroup(10);
    if (id.station()==firstSt)
    {
        const auto& hits = sh_matcher_->hitsInChamber(d);
	if (hits.size()==0) continue;
	//pick up one hit to do propagation
	auto onehit(hits.at(0));
	//std::cout <<" detId " << onehit.detUnitId() << "  momentu "<< onehit.momentumAtEntry() <<" entry point " << onehit.entryPoint() << std::endl;
        LocalPoint lp(onehit.entryPoint());
	GlobalPoint gp(cscGeometry_->idToDet(onehit.detUnitId())->surface().toGlobal(lp));
        LocalVector lv(onehit.momentumAtEntry());
        GlobalVector gv(cscGeometry_->idToDet(onehit.detUnitId())->surface().toGlobal(lv));

        GlobalPoint gp_propagate(propagateToZ(gp, gv, gpAtSecondSt.z()));
	return gp_propagate;
    }
  }
  //error return 
  return  GlobalPoint();
}

 
void TrackMatcher::propagationInterStation()
{

  //auto p(propagationInterStation(1,2, true));
  //std::cout << "propagated global point " << p << std::endl;
  interStatPropagation_odd_[12] = propagationInterStation(1,2,true);
  interStatPropagation_odd_[23] = propagationInterStation(2,3,true);
  interStatPropagation_odd_[13] = propagationInterStation(1,3,true);
  //interStatPropagation_.insert(propagationInterStation(2,3)); 
  interStatPropagation_even_[12] = propagationInterStation(1,2,false);
  interStatPropagation_even_[23] = propagationInterStation(2,3,false);
  interStatPropagation_even_[13] = propagationInterStation(1,3,false);

}
