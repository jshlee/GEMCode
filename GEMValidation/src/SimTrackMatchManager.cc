#include "GEMCode/GEMValidation/src/SimTrackMatchManager.h"

SimTrackMatchManager::SimTrackMatchManager(const SimTrack& t, const SimVertex& v,
      const edm::ParameterSet& ps, const edm::Event& ev, const edm::EventSetup& es)
: simhits_(t, v, ps, ev, es)
, gem_digis_(simhits_)
, rpc_digis_(simhits_)
, csc_digis_(simhits_)
, stubs_(simhits_, csc_digis_, gem_digis_, rpc_digis_)
, gem_rechits_(simhits_)
, tracks_(simhits_, csc_digis_, gem_digis_, rpc_digis_, stubs_)
{
  //std::cout <<" simTrackMatcherManager constructor " << std::endl;
}

SimTrackMatchManager::~SimTrackMatchManager() {
 // std::cout <<" simTrackMatcherManager destructor " << std::endl;

}
