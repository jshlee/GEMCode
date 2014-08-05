#include "GEMDigiMatcher.h"
#include "SimHitMatcher.h"

using namespace std;
using namespace matching;

#include "DataFormats/MuonDetId/interface/CSCDetId.h"
#include "DataFormats/MuonDetId/interface/GEMDetId.h"

GEMDigiMatcher::GEMDigiMatcher(SimHitMatcher& sh)
: DigiMatcher(sh)
{
  auto gemDigi_= conf().getParameter<edm::ParameterSet>("gemStripDigi");
  gemDigiInput_ = gemDigi_.getParameter<edm::InputTag>("input");
  minBXGEM_ = gemDigi_.getParameter<int>("minBX");
  maxBXGEM_ = gemDigi_.getParameter<int>("maxBX");
  matchDeltaStrip_ = gemDigi_.getParameter<int>("matchDeltaStrip");
  verboseDigi_ = gemDigi_.getParameter<int>("verbose");
  runGEMDigi_ = gemDigi_.getParameter<bool>("run");

  auto gemPad_= conf().getParameter<edm::ParameterSet>("gemPadDigi");
  gemPadDigiInput_ = gemPad_.getParameter<edm::InputTag>("input");
  minBXGEM_ = gemPad_.getParameter<int>("minBX");
  maxBXGEM_ = gemPad_.getParameter<int>("maxBX");
  verbosePad_ = gemPad_.getParameter<int>("verbose");
  runGEMPad_ = gemPad_.getParameter<bool>("run");

  auto gemCoPad_= conf().getParameter<edm::ParameterSet>("gemCoPadDigi");
  gemCoPadDigiInput_ = gemCoPad_.getParameter<edm::InputTag>("input");
  minBXGEM_ = gemCoPad_.getParameter<int>("minBX");
  maxBXGEM_ = gemCoPad_.getParameter<int>("maxBX");
  verboseCoPad_ = gemCoPad_.getParameter<int>("verbose");
  runGEMCoPad_ = gemCoPad_.getParameter<bool>("run");

  matchDeltaStrip_ = conf().getUntrackedParameter<int>("matchDeltaStripGEM", 1);

  setVerbose(conf().getUntrackedParameter<int>("verboseGEMDigi", 0));

  if (! (gemDigiInput_.label().empty() ||
         gemPadDigiInput_.label().empty() ||
         gemCoPadDigiInput_.label().empty())
     )
  {
    init();
  }
}

GEMDigiMatcher::~GEMDigiMatcher() {}


void
GEMDigiMatcher::init()
{
  edm::Handle<GEMDigiCollection> gem_digis;
  event().getByLabel(gemDigiInput_, gem_digis);
  if (runGEMDigi_) matchDigisToSimTrack(*gem_digis.product());

  edm::Handle<GEMCSCPadDigiCollection> gem_pads;
  event().getByLabel(gemPadDigiInput_, gem_pads);
  if (runGEMPad_) matchPadsToSimTrack(*gem_pads.product());

  edm::Handle<GEMCSCPadDigiCollection> gem_co_pads;
  event().getByLabel(gemPadDigiInput_, gem_co_pads);
  if (runGEMCoPad_) matchCoPadsToSimTrack(*gem_co_pads.product());
}


void
GEMDigiMatcher::matchDigisToSimTrack(const GEMDigiCollection& digis)
{
  auto det_ids = simhit_matcher_->detIdsGEM();
  for (auto id: det_ids)
  {
    GEMDetId p_id(id);
    GEMDetId superch_id(p_id.region(), p_id.ring(), p_id.station(), 1, p_id.chamber(), 0);

    auto hit_strips = simhit_matcher_->hitStripsInDetId(id, matchDeltaStrip_);
    if (verboseDigi_)
    {
      cout<<"hit_strips_fat ";
      copy(hit_strips.begin(), hit_strips.end(), ostream_iterator<int>(cout, " "));
      cout<<endl;
    }

    auto digis_in_det = digis.get(GEMDetId(id));

    for (auto d = digis_in_det.first; d != digis_in_det.second; ++d)
    {
      if (verboseDigi_) cout<<"gdigi "<<p_id<<" "<<*d<<endl;
      // check that the digi is within BX range
      if (d->bx() < minBXGEM_ || d->bx() > maxBXGEM_) continue;
      // check that it matches a strip that was hit by SimHits from our track
      if (hit_strips.find(d->strip()) == hit_strips.end()) continue;
      if (verboseDigi_) cout<<"oki"<<endl;

      auto mydigi = make_digi(id, d->strip(), d->bx(), GEM_STRIP);
      detid_to_digis_[id].push_back(mydigi);
      chamber_to_digis_[ p_id.chamberId().rawId() ].push_back(mydigi);
      superchamber_to_digis_[ superch_id() ].push_back(mydigi);

      //int pad_num = 1 + static_cast<int>( roll->padOfStrip(d->strip()) ); // d->strip() is int
      //digi_map[ make_pair(pad_num, d->bx()) ].push_back( d->strip() );
    }
  }
}


void
GEMDigiMatcher::matchPadsToSimTrack(const GEMCSCPadDigiCollection& pads)
{
  auto det_ids = simhit_matcher_->detIdsGEM();
  for (auto id: det_ids)
  {
    GEMDetId p_id(id);
    GEMDetId superch_id(p_id.region(), p_id.ring(), p_id.station(), 1, p_id.chamber(), 0);

    auto hit_pads = simhit_matcher_->hitPadsInDetId(id);
    auto pads_in_det = pads.get(p_id);

    if (verbosePad_)
    {
      cout<<"checkpads "<<hit_pads.size()<<" "<<std::distance(pads_in_det.first, pads_in_det.second)<<" hit_pads: ";
      copy(hit_pads.begin(), hit_pads.end(), ostream_iterator<int>(cout," "));
      cout<<endl;
    }

    for (auto pad = pads_in_det.first; pad != pads_in_det.second; ++pad)
    {
      if (verbosePad_) cout<<"chp "<<*pad<<endl;
      // check that the pad BX is within the range
      if (pad->bx() < minBXGEM_ || pad->bx() > maxBXGEM_) continue;
      if (verbosePad_) cout<<"chp1"<<endl;
      // check that it matches a pad that was hit by SimHits from our track
      if (hit_pads.find(pad->pad()) == hit_pads.end()) continue;
      if (verbosePad_) cout<<"chp2"<<endl;

      auto mydigi = make_digi(id, pad->pad(), pad->bx(), GEM_PAD);
      detid_to_pads_[id].push_back(mydigi);
      chamber_to_pads_[ p_id.chamberId().rawId() ].push_back(mydigi);
      superchamber_to_pads_[ superch_id() ].push_back(mydigi);
    }
  }
}


void
GEMDigiMatcher::matchCoPadsToSimTrack(const GEMCSCPadDigiCollection& co_pads)
{
  auto det_ids = simhit_matcher_->detIdsGEMCoincidences();
  for (auto id: det_ids)
  {
    GEMDetId p_id(id);
    GEMDetId superch_id(p_id.region(), p_id.ring(), p_id.station(), 1, p_id.chamber(), 0);

    auto hit_co_pads = simhit_matcher_->hitCoPadsInDetId(id);
    auto co_pads_in_det = co_pads.get(p_id);

    if (verboseCoPad_)
    {
      cout<<"checkpads "<<hit_co_pads.size()<<" "<<std::distance(co_pads_in_det.first, co_pads_in_det.second)<<" hit_pads: ";
      copy(hit_co_pads.begin(), hit_co_pads.end(), ostream_iterator<int>(cout," "));
      cout<<endl;
    }

    for (auto pad = co_pads_in_det.first; pad != co_pads_in_det.second; ++pad)
    {
      // check that the pad BX is within the range
      if (pad->bx() < minBXGEM_ || pad->bx() > maxBXGEM_) continue;
      // check that it matches a coincidence pad that was hit by SimHits from our track
      if (hit_co_pads.find(pad->pad()) == hit_co_pads.end()) continue;

      auto mydigi = make_digi(id, pad->pad(), pad->bx(), GEM_COPAD);
      detid_to_copads_[id].push_back(mydigi);
      superchamber_to_copads_[ superch_id() ].push_back(mydigi);
    }
  }
}


std::set<unsigned int>
GEMDigiMatcher::detIds() const
{
  std::set<unsigned int> result;
  for (auto& p: detid_to_digis_) result.insert(p.first);
  return result;
}


std::set<unsigned int>
GEMDigiMatcher::chamberIds() const
{
  std::set<unsigned int> result;
  for (auto& p: chamber_to_digis_) result.insert(p.first);
  return result;
}

std::set<unsigned int>
GEMDigiMatcher::superChamberIds() const
{
  std::set<unsigned int> result;
  for (auto& p: superchamber_to_digis_) result.insert(p.first);
  return result;
}


std::set<unsigned int>
GEMDigiMatcher::detIdsWithCoPads() const
{
  std::set<unsigned int> result;
  for (auto& p: detid_to_copads_) result.insert(p.first);
  return result;
}

std::set<unsigned int>
GEMDigiMatcher::superChamberIdsWithCoPads() const
{
  std::set<unsigned int> result;
  for (auto& p: superchamber_to_copads_) result.insert(p.first);
  return result;
}




const matching::DigiContainer&
GEMDigiMatcher::digisInDetId(unsigned int detid) const
{
  if (detid_to_digis_.find(detid) == detid_to_digis_.end()) return no_digis_;
  return detid_to_digis_.at(detid);
}

const matching::DigiContainer&
GEMDigiMatcher::digisInChamber(unsigned int detid) const
{
  if (chamber_to_digis_.find(detid) == chamber_to_digis_.end()) return no_digis_;
  return chamber_to_digis_.at(detid);
}

const matching::DigiContainer&
GEMDigiMatcher::digisInSuperChamber(unsigned int detid) const
{
  if (superchamber_to_digis_.find(detid) == superchamber_to_digis_.end()) return no_digis_;
  return superchamber_to_digis_.at(detid);
}

const matching::DigiContainer&
GEMDigiMatcher::padsInDetId(unsigned int detid) const
{
  if (detid_to_pads_.find(detid) == detid_to_pads_.end()) return no_digis_;
  return detid_to_pads_.at(detid);
}

const matching::DigiContainer&
GEMDigiMatcher::padsInChamber(unsigned int detid) const
{
  if (chamber_to_pads_.find(detid) == chamber_to_pads_.end()) return no_digis_;
  return chamber_to_pads_.at(detid);
}

const matching::DigiContainer&
GEMDigiMatcher::padsInSuperChamber(unsigned int detid) const
{
  if (superchamber_to_pads_.find(detid) == superchamber_to_pads_.end()) return no_digis_;
  return superchamber_to_pads_.at(detid);
}


const matching::DigiContainer&
GEMDigiMatcher::coPadsInDetId(unsigned int detid) const
{
  if (detid_to_copads_.find(detid) == detid_to_copads_.end()) return no_digis_;
  return detid_to_copads_.at(detid);
}

const matching::DigiContainer&
GEMDigiMatcher::coPadsInSuperChamber(unsigned int detid) const
{
  if (superchamber_to_copads_.find(detid) == superchamber_to_copads_.end()) return no_digis_;
  return superchamber_to_copads_.at(detid);
}


int
GEMDigiMatcher::nLayersWithDigisInSuperChamber(unsigned int detid) const
{
  set<int> layers;
  auto digis = digisInSuperChamber(detid);
  for (auto& d: digis)
  {
    GEMDetId idd(digi_id(d));
    layers.insert(idd.layer());
  }
  return layers.size();
}


int
GEMDigiMatcher::nLayersWithPadsInSuperChamber(unsigned int detid) const
{
  set<int> layers;
  auto digis = padsInSuperChamber(detid);
  for (auto& d: digis)
  {
    GEMDetId idd(digi_id(d));
    layers.insert(idd.layer());
  }
  return layers.size();
}


int
GEMDigiMatcher::nCoPads() const
{
  int n = 0;
  auto ids = superChamberIdsWithCoPads();
  for (auto id: ids)
  {
    n += coPadsInSuperChamber(id).size();
  }
  return n;
}


int
GEMDigiMatcher::nPads() const
{
  int n = 0;
  auto ids = superChamberIds();
  for (auto id: ids)
  {
    n += padsInSuperChamber(id).size();
  }
  return n;
}


std::set<int>
GEMDigiMatcher::stripNumbersInDetId(unsigned int detid) const
{
  set<int> result;
  auto digis = digisInDetId(detid);
  for (auto& d: digis)
  {
    result.insert( digi_channel(d) );
  }
  return result;
}

std::set<int>
GEMDigiMatcher::padNumbersInDetId(unsigned int detid) const
{
  set<int> result;
  auto digis = padsInDetId(detid);
  for (auto& d: digis)
  {
    result.insert( digi_channel(d) );
  }
  return result;
}

std::set<int>
GEMDigiMatcher::coPadNumbersInDetId(unsigned int detid) const
{
  set<int> result;
  auto digis = coPadsInDetId(detid);
  for (auto& d: digis)
  {
    result.insert( digi_channel(d) );
  }
  return result;
}

std::set<int>
GEMDigiMatcher::partitionNumbers() const
{
  std::set<int> result;

  auto detids = detIds();
  for (auto id: detids)
  {
    GEMDetId idd(id);
    result.insert( idd.roll() );
  }
  return result;
}

std::set<int>
GEMDigiMatcher::partitionNumbersWithCoPads() const
{
  std::set<int> result;

  auto detids = detIdsWithCoPads();
  for (auto id: detids)
  {
    GEMDetId idd(id);
    result.insert( idd.roll() );
  }
  return result;
}

int 
GEMDigiMatcher::extrapolateHsfromGEMPad(unsigned int id, int gempad) const
{
  int result = -1 ;
  
  GEMDetId gem_id(id);
  int endcap = (gem_id.region()>0 ? 1 : 2);
  int station;
  if (gem_id.station() == 3) station = 2;
  else if (gem_id.station() == 2) return result;
  else station = gem_id.station();
  CSCDetId csc_id(endcap, station, gem_id.ring(), gem_id.chamber(), 0);

//  const CSCGeometry* cscGeometry_(DigiMatcher::getCSCGeometry());
//  const GEMGeometry* gemGeometry_(DigiMatcher::getGEMGeometry());

  const CSCChamber* cscChamber(cscGeometry_->chamber(csc_id));
  const CSCLayer* cscKeyLayer(cscChamber->layer(3));
  const CSCLayerGeometry* cscKeyLayerGeometry(cscKeyLayer->geometry());

  const GEMChamber* gemChamber(gemGeometry_->chamber(id));
  auto gemRoll(gemChamber->etaPartition(2));//any roll
  const int nGEMPads(gemRoll->npads());
  if (gempad > nGEMPads or gempad < 0) result = -1;

  const LocalPoint lpGEM(gemRoll->centreOfPad(gempad));
  const GlobalPoint gp(gemRoll->toGlobal(lpGEM));
  const LocalPoint lpCSC(cscKeyLayer->toLocal(gp));
  const float strip(cscKeyLayerGeometry->strip(lpCSC));
  // HS are wrapped-around
  result = (int) (strip - 0.25)/0.5;
  return result;
}


int 
GEMDigiMatcher::extrapolateHsfromGEMStrip(unsigned int id, int gemstrip) const
{
  int result = -1 ;
  
  GEMDetId gem_id(id);//chamberid
  int endcap = (gem_id.region()>0 ? 1 : 2);
  int station;
  if (gem_id.station() == 3) station = 2;
  else if (gem_id.station() == 2) return result;
  else station = gem_id.station();
  CSCDetId csc_id(endcap, station, gem_id.ring(), gem_id.chamber(), 0);

//  const CSCGeometry* cscGeometry_(DigiMatcher::getCSCGeometry());
//  const GEMGeometry* gemGeometry_(DigiMatcher::getGEMGeometry());

  const CSCChamber* cscChamber(cscGeometry_->chamber(csc_id));
  const CSCLayer* cscKeyLayer(cscChamber->layer(3));
  const CSCLayerGeometry* cscKeyLayerGeometry(cscKeyLayer->geometry());

  const GEMChamber* gemChamber(gemGeometry_->chamber(id));
  auto gemRoll(gemChamber->etaPartition(2));//any roll
  const int nGEMStrips(gemRoll->nstrips());
  if (gemstrip > nGEMStrips or gemstrip < 0) result = -1;

  const LocalPoint lpGEM(gemRoll->centreOfStrip(gemstrip));
  const GlobalPoint gp(gemRoll->toGlobal(lpGEM));
  const LocalPoint lpCSC(cscKeyLayer->toLocal(gp));
  const float strip(cscKeyLayerGeometry->strip(lpCSC));
  // HS are wrapped-around
  result = (int) (strip - 0.25)/0.5;
  return result;
}

