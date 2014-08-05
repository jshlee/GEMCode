#include "GEMCode/GEMValidation/src/GEMRecHitMatcher.h"
#include "GEMCode/GEMValidation/src/SimHitMatcher.h"

using namespace std;
using namespace matching;

GEMRecHitMatcher::GEMRecHitMatcher(SimHitMatcher& sh)
  : BaseMatcher(sh.trk(), sh.vtx(), sh.conf(), sh.event(), sh.eventSetup())
  , simhit_matcher_(&sh)
{
  auto gemRecHit_= conf().getParameter<edm::ParameterSet>("gemRecHit");
  gemRecHitInput_ = gemRecHit_.getParameter<edm::InputTag>("input");
  minBXGEM_ = gemRecHit_.getParameter<int>("minBX");
  maxBXGEM_ = gemRecHit_.getParameter<int>("maxBX");
  matchDeltaStrip_ = gemRecHit_.getParameter<int>("matchDeltaStrip");
  verboseGEMRecHit_ = gemRecHit_.getParameter<int>("verbose");
  runGEMRecHit_ = gemRecHit_.getParameter<bool>("run");

  if (!(gemRecHitInput_.label().empty()))
  {
    init();
  }
}

GEMRecHitMatcher::~GEMRecHitMatcher() {}


void
GEMRecHitMatcher::init()
{
  edm::Handle<GEMRecHitCollection> gem_rechits;
  event().getByLabel(gemRecHitInput_, gem_rechits);
  if (runGEMRecHit_) matchRecHitsToSimTrack(*gem_rechits.product());
}


void
GEMRecHitMatcher::matchRecHitsToSimTrack(const GEMRecHitCollection& rechits)
{
  
  auto det_ids = simhit_matcher_->detIdsGEM();
  for (auto id: det_ids)
  {
    GEMDetId p_id(id);
    GEMDetId superch_id(p_id.region(), p_id.ring(), p_id.station(), 1, p_id.chamber(), 0);

    auto hit_strips = simhit_matcher_->hitStripsInDetId(id, matchDeltaStrip_);
    if (verboseGEMRecHit_)
    {
      cout<<"hit_strips_fat ";
      copy(hit_strips.begin(), hit_strips.end(), ostream_iterator<int>(cout, " "));
      cout<<endl;
    }

    auto rechits_in_det = rechits.get(GEMDetId(id));

    for (auto d = rechits_in_det.first; d != rechits_in_det.second; ++d)
    {
      if (verboseGEMRecHit_) cout<<"recHit "<<p_id<<" "<<*d<<endl;
      // check that the rechit is within BX range
      if (d->BunchX() < minBXGEM_ || d->BunchX() > maxBXGEM_) continue;
      // check that it matches a strip that was hit by SimHits from our track

      int firstStrip = d->firstClusterStrip();
      int cls = d->clusterSize();
      bool stripFound = false;

      for(int i = firstStrip; i < (firstStrip + cls); i++){

	if (hit_strips.find(i) != hit_strips.end()) stripFound = true;
        //std::cout<<i<<" "<<firstStrip<<" "<<cls<<" "<<stripFound<<std::endl;
	
      }

      if (!stripFound) continue;
      if (verboseGEMRecHit_) cout<<"oki"<<endl;

      auto myrechit = make_digi(id, d->firstClusterStrip(), d->BunchX(), GEM_STRIP, d->clusterSize());
      detid_to_recHits_[id].push_back(myrechit);
      chamber_to_recHits_[ p_id.chamberId().rawId() ].push_back(myrechit);
      superchamber_to_recHits_[ superch_id() ].push_back(myrechit);

    }
  }
  
}


std::set<unsigned int>
GEMRecHitMatcher::detIds() const
{
  std::set<unsigned int> result;
  for (auto& p: detid_to_recHits_) result.insert(p.first);
  return result;
}


std::set<unsigned int>
GEMRecHitMatcher::chamberIds() const
{
  std::set<unsigned int> result;
  for (auto& p: chamber_to_recHits_) result.insert(p.first);
  return result;
}

std::set<unsigned int>
GEMRecHitMatcher::superChamberIds() const
{
  std::set<unsigned int> result;
  for (auto& p: superchamber_to_recHits_) result.insert(p.first);
  return result;
}


const GEMRecHitMatcher::RecHitContainer&
GEMRecHitMatcher::recHitsInDetId(unsigned int detid) const
{
  if (detid_to_recHits_.find(detid) == detid_to_recHits_.end()) return no_recHits_;
  return detid_to_recHits_.at(detid);
}

const GEMRecHitMatcher::RecHitContainer&
GEMRecHitMatcher::recHitsInChamber(unsigned int detid) const
{
  if (chamber_to_recHits_.find(detid) == chamber_to_recHits_.end()) return no_recHits_;
  return chamber_to_recHits_.at(detid);
}

const GEMRecHitMatcher::RecHitContainer&
GEMRecHitMatcher::recHitsInSuperChamber(unsigned int detid) const
{
  if (superchamber_to_recHits_.find(detid) == superchamber_to_recHits_.end()) return no_recHits_;
  return superchamber_to_recHits_.at(detid);
}

int
GEMRecHitMatcher::nLayersWithRecHitsInSuperChamber(unsigned int detid) const
{
  set<int> layers;
  /*
  auto recHits = recHitsInSuperChamber(detid);
  for (auto& d: recHits)
  {
    GEMDetId idd(digi_id(d));
    layers.insert(idd.layer());
  }
  */
  return layers.size();
}


std::set<int>
GEMRecHitMatcher::stripNumbersInDetId(unsigned int detid) const
{
  set<int> result;
  /*
  auto recHits = recHitsInDetId(detid);
  for (auto& d: recHits)
  {
    result.insert( digi_channel(d) );
  }
  */
  return result;
}

std::set<int>
GEMRecHitMatcher::partitionNumbers() const
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

GlobalPoint
GEMRecHitMatcher::recHitPosition(const RecHit& rechit) const
{
  unsigned int id = digi_id(rechit);
  int strip = digi_channel(rechit);
  DigiType t = digi_type(rechit);

  GlobalPoint gp;
  if ( t == GEM_STRIP )
  {
    GEMDetId idd(id);
    LocalPoint lp = gemGeometry_->etaPartition(idd)->centreOfStrip(strip);
    gp = gemGeometry_->idToDet(id)->surface().toGlobal(lp);
  }

  return gp;
}


GlobalPoint
GEMRecHitMatcher::recHitMeanPosition(const RecHitContainer& rechit) const
{
  GlobalPoint point_zero;
  if (rechit.empty()) return point_zero; // point "zero"

  float sumx, sumy, sumz;
  sumx = sumy = sumz = 0.f;
  size_t n = 0;
  for (auto& d: rechit)
  {
    GlobalPoint gp = recHitPosition(d);
    if (gp == point_zero) continue;

    sumx += gp.x();
    sumy += gp.y();
    sumz += gp.z();
    ++n;
  }
  if (n == 0) return GlobalPoint();
  return GlobalPoint(sumx/n, sumy/n, sumz/n);
}

