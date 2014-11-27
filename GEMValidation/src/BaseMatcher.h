#ifndef GEMValidation_BaseMatcher_h
#define GEMValidation_BaseMatcher_h

/**\class BaseMatcher

  Base for Sim and Trigger info matchers for SimTrack in CSC & GEM

 Original Author:  "Vadim Khotilovich"

*/

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include <SimDataFormats/Track/interface/SimTrackContainer.h>
#include <SimDataFormats/Vertex/interface/SimVertexContainer.h>

#include "MagneticField/Engine/interface/MagneticField.h"
#include "TrackingTools/GeomPropagators/interface/Propagator.h"

#include "Geometry/Records/interface/MuonGeometryRecord.h"
#include "Geometry/GEMGeometry/interface/GEMGeometry.h"
#include "Geometry/GEMGeometry/interface/ME0Geometry.h"
#include "Geometry/RPCGeometry/interface/RPCGeometry.h"
#include "Geometry/CSCGeometry/interface/CSCGeometry.h"
#include "Geometry/CSCGeometry/interface/CSCLayerGeometry.h"
#include "Geometry/DTGeometry/interface/DTGeometry.h"

#include "DataFormats/MuonDetId/interface/CSCDetId.h"
#include "DataFormats/MuonDetId/interface/GEMDetId.h"
#include "DataFormats/MuonDetId/interface/RPCDetId.h"
#include "DataFormats/MuonDetId/interface/ME0DetId.h"
#include "DataFormats/MuonDetId/interface/DTWireId.h"

inline bool is_dt(unsigned int detId) {
  return (DetId(detId)).subdetId() == MuonSubdetId::DT;
}

inline bool is_gem(unsigned int detId) {
  return (DetId(detId)).subdetId() == MuonSubdetId::GEM;
}

inline bool is_csc(unsigned int detId) {
  return (DetId(detId)).subdetId() == MuonSubdetId::CSC;
}

inline bool is_rpc(unsigned int detId) {
  return (DetId(detId)).subdetId() == MuonSubdetId::RPC;
}

inline bool is_me0(unsigned int detId) {
  return (DetId(detId)).subdetId() == MuonSubdetId::ME0;
}

int chamber(const DetId& id);

static const float AVERAGE_GEM_Z(568.6); // [cm]

static const float AVERAGE_GE11_ODD_Z(568.6); // [cm]
static const float AVERAGE_GE11_EVEN_Z(568.6); // [cm]

static const float AVERAGE_GE21_LONG_Z(568.6); // [cm]
static const float AVERAGE_GE21_SHORT_Z(568.6); // [cm]

static const float AVERAGE_ME11_EVEN_Z(585); // [cm]
static const float AVERAGE_ME11_ODD_Z(615); // [cm]

static const float AVERAGE_ME21_EVEN_Z(820); // [cm]
static const float AVERAGE_ME21_ODD_Z(835); // [cm]

static const float AVERAGE_ME0_Z(568.6); // [cm]

class BaseMatcher
{
public:
  
  /// CSC chamber types, according to CSCDetId::iChamberType()
  enum CSCType {CSC_ALL = 0, CSC_ME1a, CSC_ME1b, CSC_ME12, CSC_ME13,
                CSC_ME21, CSC_ME22, CSC_ME31, CSC_ME32, CSC_ME41, CSC_ME42};

  /// GEM chamber types
  enum GEMType {GEM_ALL = 0, GEM_ME11, GEM_ME21};
  
  /// RPC endcap chamber types
  enum RPCType {RPC_ALL = 0, RPC_ME12, RPC_ME13, RPC_ME22, RPC_ME23, 
                RPC_ME31, RPC_ME32, RPC_ME33, RPC_ME41, RPC_ME42, RPC_ME43};

  /// DT chamber types
  enum DTType { DT_ALL = 0, DT_MB10, DT_MB11, DT_MB12, DT_MB20, DT_MB21, 
		DT_MB22, DT_MB30, DT_MB31, DT_MB32, DT_MB40, DT_MB41, DT_MB42};

  const double ME11GEMdPhi[9][3] = {
    {-2 , 1.0, 1.0 },
    {3 , 0.03971647, 0.01710244 },
    {5 , 0.02123785, 0.00928431 },
    {7 , 0.01475524, 0.00650928 },
    {10, 0.01023299, 0.00458796 },
    {15, 0.00689220, 0.00331313 },
    {20, 0.00535176, 0.00276152 },
    {30, 0.00389050, 0.00224959 },
    {40, 0.00329539, 0.00204670 }
  };
  const double ME21GEMdPhi[9][3] = {
    {-2 , 1.0, 1.0 },
    {3 , 0.01832829, 0.01003643 },
    {5 , 0.01095490, 0.00631625 },
    {7 , 0.00786026, 0.00501017 },
    {10, 0.00596349, 0.00414560 },
    {15, 0.00462411, 0.00365550 },
    {20, 0.00435298, 0.00361550 },
    {30, 0.00465160, 0.00335700 },
    {40, 0.00372145, 0.00366262 }
  };

  BaseMatcher(const SimTrack& t, const SimVertex& v,
      const edm::ParameterSet& ps, const edm::Event& ev, const edm::EventSetup& es);

  ~BaseMatcher();

  // non-copyable
  BaseMatcher(const BaseMatcher&) = delete;
  BaseMatcher& operator=(const BaseMatcher&) = delete;

  const SimTrack& trk() const {return trk_;}
  const SimVertex& vtx() const {return vtx_;}

  const edm::ParameterSet& conf() const {return conf_;}

  const edm::Event& event() const {return ev_;}
  const edm::EventSetup& eventSetup() const {return es_;}

  /// check if CSC chamber type is in the used list
  bool useCSCChamberType(int csc_type);
  
  void setVerbose(int v) { verbose_ = v; }
  int verbose() const { return verbose_; }

  /// general interface to propagation
  GlobalPoint propagateToZ(GlobalPoint &inner_point, GlobalVector &inner_vector, float z) const;

  /// propagation for a track starting from a vertex
  GlobalPoint propagateToZ(float z) const;

  /// propagate the track to average GEM z-position                                                                            
  GlobalPoint propagatedPositionGEM() const;

  /// geometry
  void setGEMGeometry(const GEMGeometry *geom) {gemGeometry_ = geom;}
  void setRPCGeometry(const RPCGeometry *geom) {rpcGeometry_ = geom;}
  void setME0Geometry(const ME0Geometry *geom) {me0Geometry_ = geom;}
  void setCSCGeometry(const CSCGeometry *geom) {cscGeometry_ = geom;}
  void setDTGeometry(const DTGeometry *geom) {dtGeometry_ = geom;}

  const GEMGeometry* getGEMGeometry() const {return gemGeometry_;}
  const RPCGeometry* getRPCGeometry() const {return rpcGeometry_;}
  const ME0Geometry* getME0Geometry() const {return me0Geometry_;}
  const CSCGeometry* getCSCGeometry() const {return cscGeometry_;}
  const DTGeometry* getDTGeometry() const {return dtGeometry_;}

  unsigned int gemDetFromCSCDet(unsigned int id,int layer);

  std::pair<unsigned int, unsigned int> gemDetsFromCSCDet(unsigned int id);
  double phiHeavyCorr(double pt, double eta, double phi, double charge) const;
  bool passDPhicut(CSCDetId id, float dphi, float pt) const;

 protected:
  
  const CSCGeometry* cscGeometry_;
  const RPCGeometry* rpcGeometry_;
  const GEMGeometry* gemGeometry_;
  const ME0Geometry* me0Geometry_;
  const DTGeometry* dtGeometry_;

  bool hasGEMGeometry_;
  bool hasRPCGeometry_;
  bool hasME0Geometry_;
  bool hasCSCGeometry_;
  bool hasDTGeometry_; 
  
 private:

  const SimTrack& trk_;
  const SimVertex& vtx_;

  const edm::ParameterSet& conf_;

  const edm::Event& ev_;
  const edm::EventSetup& es_;

  int verbose_;

  // list of CSC chamber types to use
  bool useCSCChamberTypes_[11];

  edm::ESHandle<MagneticField> magfield_;
  edm::ESHandle<Propagator> propagator_;
  edm::ESHandle<Propagator> propagatorOpposite_;
  edm::ESHandle<CSCGeometry> csc_geom;
  edm::ESHandle<RPCGeometry> rpc_geom;
  edm::ESHandle<GEMGeometry> gem_geom;
  edm::ESHandle<ME0Geometry> me0_geom;
  edm::ESHandle<DTGeometry> dt_geom;
};

#endif
