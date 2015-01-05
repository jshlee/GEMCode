#include "GEMCode/GEMValidation/src/TFTrack.h"

TFTrack::TFTrack(const csc::L1Track* t, const CSCCorrelatedLCTDigiCollection* lcts)
{
  l1track_ = t;
  nstubs = 0;

  for (auto detUnitIt = lcts->begin(); detUnitIt != lcts->end(); detUnitIt++) {
    const CSCDetId& id = (*detUnitIt).first;
    //std::cout << "DetId " << id << std::endl;
    const auto range = (*detUnitIt).second;
    for (auto digiIt = range.first; digiIt != range.second; digiIt++) {
      if (!(*digiIt).isValid()) continue;
      //std::cout << "Digi " << *digiIt << std::endl;
      addTriggerDigi(&(*digiIt));
      addTriggerDigiId(id);
      nstubs++;
    }
  }
  

}

TFTrack::TFTrack(const TFTrack& rhs)
{}

TFTrack::~TFTrack()
{
//  std::cout<<" deconstrcution of TFTrack"<< std::endl;
  triggerDigis_.clear();
  triggerIds_.clear();
  triggerEtaPhis_.clear();
  triggerStubs_.clear();
  mplcts_.clear();
  ids_.clear(); // chamber ids_

}

void 
TFTrack::init(CSCTFPtLUT* ptLUT,
	      edm::ESHandle< L1MuTriggerScales > &muScales,
	      edm::ESHandle< L1MuTriggerPtScale > &muPtScale)
{
  // This section is copied from L1Trigger/CSCTrackFinder/interface/CSCTFMuonSorter.h                                                               
  unsigned gbl_phi(l1track_->localPhi() + ((l1track_->sector() - 1)*24) + 6);
  if(gbl_phi > 143) gbl_phi -= 143;
  phi_packed_ = gbl_phi & 0xff;

  const unsigned eta_sign(l1track_->endcap() == 1 ? 0 : 1);
  const int gbl_eta(l1track_->eta_packed() | eta_sign << (L1MuRegionalCand::ETA_LENGTH - 1));
  eta_packed_  = gbl_eta & 0x3f;

  unsigned gpt = 0, quality = 0;
  csc::L1Track::decodeRank(l1track_->rank(), gpt, quality);
  q_packed_ = quality & 0x3;
  pt_packed_ = gpt & 0x1f;

  // calculate pt, eta and phi (don't forget to store the sign)                                                                                   
  const int sign(l1track_->endcap()==1 ? 1 : -1);
  pt_ = muPtScale->getPtScale()->getLowEdge(pt_packed_) + 1.e-6;
  eta_ = muScales->getRegionalEtaScale(2)->getCenter(l1track_->eta_packed()) * sign;
  phi_ = normalizedPhi(muScales->getPhiScale()->getLowEdge(phi_packed_));
}

void 
TFTrack::setDR(double dr)
{
  dr_ = dr;
}

bool 
TFTrack::hasStubEndcap(int st) const
{
  if(st==1 and l1track_->me1ID() > 0) return true;
  if(st==2 and l1track_->me2ID() > 0) return true;
  if(st==3 and l1track_->me3ID() > 0) return true;
  if(st==4 and l1track_->me4ID() > 0) return true;
  return false;
}

bool
TFTrack::hasStubBarrel() const
{
  return l1track_->mb1ID() > 0;
}

bool 
TFTrack::hasStubStation(int st) const
{
  if(st==0 and hasStubBarrel())       return true;
  if(st==1 and l1track_->me1ID() > 0) return true;
  if(st==2 and l1track_->me2ID() > 0) return true;
  if(st==3 and l1track_->me3ID() > 0) return true;
  if(st==4 and l1track_->me4ID() > 0) return true;
  return false;
}


bool 
TFTrack::hasStubCSCOk(int st) const
{
   if (!hasStubEndcap(st)) return false;
//   bool cscok = 0;
//   for (size_t s=0; s<ids__.size(); s++) {
//     if (ids__[s].station() == st and mplcts__[s]->deltaOk) { 
//       cscok = 1; 
//       break; 
//     }
//   }
//   return cscok;
  return true;
}


unsigned int 
TFTrack::nStubs(bool mb1, bool me1, bool me2, bool me3, bool me4) const
{
  return ( (mb1 and hasStubStation(0)) + (me1 and hasStubStation(1)) + 
	   (me2 and hasStubStation(2)) + (me3 and hasStubStation(3)) + 
	   (me4 and hasStubStation(4)) );
}


unsigned int 
TFTrack::nStubsCSCOk(bool me1, bool me2, bool me3, bool me4) const
{
  return ( (me1 and hasStubCSCOk(1)) + (me2 and hasStubCSCOk(2)) + 
	   (me3 and hasStubCSCOk(3)) + (me4 and hasStubCSCOk(4)) );
}


bool 
TFTrack::passStubsMatch(double steta, int minLowHStubs, int minMidHStubs, int minHighHStubs) const
{
//    const double steta(match->strk->momentum().eta());
  const int nstubs(nStubs(1,1,1,1,1));
  const int nstubsok(nStubsCSCOk(1,1,1,1));
  if (fabs(steta) <= 1.2)      return nstubsok >=1 and nstubs >= minLowHStubs;
  else if (fabs(steta) <= 2.1) return nstubsok >=2 and nstubs >= minMidHStubs;
  else                         return nstubsok >=2 and nstubs >= minHighHStubs;
}


void 
TFTrack::print()
{
  
//    std::cout<<"#### TFTRACK PRINT: "<<msg<<" #####"<<std::endl;
    //std::cout<<"## L1MuRegionalCand print: ";
    //l1track_->print();
    //std::cout<<"\n## L1Track Print: ";
    //l1track_->Print();
    //std::cout<<"## TFTRACK:  
    std::cout<<"\tpt_packed: "<<pt_packed_<<"  eta_packed: " << eta_packed_<<"  phi_packed: " << phi_packed_<<"  q_packed: "<< q_packed_<<"  bx: "<<l1track_->bx()<<std::endl;
    std::cout<<"\tpt: "<<pt_<<"  eta: "<<eta_<<"  phi: "<<phi_<<"  sector: "<<l1track_->sector()<<"  dr: "<<dr_<<std::endl;
  /*  std::cout<<"\tMB1 ME1 ME2 ME3 ME4 = "<<l1track_->mb1ID()<<" "<<l1track_->me1ID()<<" "<<l1track_->me2ID()<<" "<<l1track_->me3ID()<<" "<<l1track_->me4ID()
        <<" ("<<hasStub(0)<<" "<<hasStub(1)<<" "<<hasStub(2)<<" "<<hasStub(3)<<" "<<hasStub(4)<<")  "
        <<" ("<<hasStubCSCOk(1)<<" "<<hasStubCSCOk(2)<<" "<<hasStubCSCOk(3)<<" "<<hasStubCSCOk(4)<<")"<<std::endl;*/
    std::cout<<"\tptAddress: 0x"<<std::hex<<l1track_->ptLUTAddress()<<std::dec<<"  dphi12: "<<dPhi12()<<"  dphi23: "<<dPhi23()<<std::endl;
    std::cout<<"\thas "<<triggerDigis_.size()<<" stubs in " << std::endl;
    for (size_t s=0; s<triggerDigis_.size(); s++) 
        std::cout<<CSCDetId(triggerIds_[s])<<" w:"<<triggerDigis_[s]->getKeyWG()+1<<" hs:"<<triggerDigis_[s]->getStrip()+1 <<" p:"<<triggerDigis_[s]->getPattern()<<" bx:"<<triggerDigis_[s]->getBX()<<"; " << std::endl;
   
    std::cout<<"\tstub_etaphis:" << std::endl;
    for (size_t s=0; s<triggerEtaPhis_.size(); s++)
        std::cout<<" eta: "<<triggerEtaPhis_[s].first<<" phi: "<<triggerEtaPhis_[s].second << std::endl;
    /*std::cout<<"\tstub_petaphis:";
    for (size_t s=0; s<triggerStubs_.size(); s++)
        std::cout<<"  "<<triggerStubs_[s].etaPacked()<<" "<<triggerStubs_[s].phiPacked();
    std::cout<<std::endl;*/
/*    std::cout<<"\thas "<<mplcts_.size()<<" associated MPCs in ";
    for (size_t s=0; s<ids_.size(); s++) 
        std::cout<<ids_[s]<<" w:"<<mplcts_[s]->trgdigi->getKeyWG()<<" s:"<<mplcts_[s]->trgdigi->getStrip()/2 + 1<<" Ok="<<mplcts_[s]->deltaOk<<"; " << std::endl;
    std::cout<<"\tMPCs meEtap and mePhip: ";
    for (size_t s=0; s<ids_.size(); s++) std::cout<<mplcts_[s]->meEtap<<", "<<mplcts_[s]->mePhip<<";  ";
    std::cout<<std::endl;*/
    std::cout<<"#### TFTRACK END PRINT #####"<<std::endl;
  
}

unsigned int TFTrack::digiInME(int st, int ring)
{
  if (triggerDigis_.size() != triggerIds_.size()) std::cout<<" BUG " <<std::endl;
  for (unsigned int i=0; i<triggerDigis_.size(); i++)
  {
     auto id(triggerIds_.at(i));
     if (id.station()==st && id.ring()==ring) return i;
     else continue;  
  }
  return 999;//invalid return, larger than triggerDigis_.size();

}

void 
TFTrack::addTriggerDigi(const CSCCorrelatedLCTDigi* digi)
{
  triggerDigis_.push_back(digi);
}

void 
TFTrack::addTriggerDigiId(const CSCDetId& id)
{
  triggerIds_.push_back(id);
}

void 
TFTrack::addTriggerEtaPhi(const std::pair<float,float>& p)
{
  triggerEtaPhis_.push_back(p);
}

void 
TFTrack::addTriggerStub(const csctf::TrackStub& st)
{
  triggerStubs_.push_back(st);
}
