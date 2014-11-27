import FWCore.ParameterSet.Config as cms

process = cms.Process("GEMCSCANA")

## Standard sequence
process.load('Configuration.StandardSequences.Services_cff')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('Configuration.Geometry.GeometryExtended2023Muon_cff')
process.load('Configuration.Geometry.GeometryExtended2023MuonReco_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_PostLS1_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

## TrackingComponentsRecord required for matchers
process.load('TrackPropagation.SteppingHelixPropagator.SteppingHelixPropagatorOpposite_cfi')
process.load('TrackPropagation.SteppingHelixPropagator.SteppingHelixPropagatorAlong_cfi')

InputFiles = ['file:/uscms_data/d3/tahuang/CMSSW_6_2_0_SLHC16/src/GEMCode/SimMuL1/debug/PU0_10k_Pt20_out_L1.root']
process.source = cms.Source("PoolSource",
	fileNames = cms.untracked.vstring('file:out_L1.root'),
#        skipEvents = cms.untracked.uint32(26)
#     fileNames = cms.untracked.vstring(*InputFiles)
)

#process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32(1000)


#InputDir = ['/eos/uscms/store/user/lpcgem/tahuang/SingleMuPt2-50_1M_SLHC11_2023Muon/SLHC13_100k_L1_PU0_Pt0_2023All/2d6b486b97b36a4f35123274447d2d5e/']
#InputDir = ['/eos/uscms/store/user/lpcgem/tahuang/SingleMuPt2-50_1M_SLHC11_2023Muon/SLHC13_100k_L1_PU140_Pt0_2023All/2d6b486b97b36a4f35123274447d2d5e/']
#InputDir = ['/eos/uscms/store/user/tahuang/SLHC13_100k_L1_PU140_Pt0_2023All_SortByGEMDPhi_v2/']
#InputDir = ['/eos/uscms/store/user/lpcgem/SLHC13_100k_L1_PU140_Pt0_2023All_SortByGEMDPhi_v2/']
#InputDir = ['/eos/uscms/store/user/tahuang/SLHC13_100k_L1_PU140_Pt0_2023All_NoPromote_v3/']
#InputDir = ['/eos/uscms/store/user/tahuang/SLHC13_100k_L1_PU140_Pt0_2023All_SortByGEMDPhi_v3/']
#InputDir = ['/eos/uscms/store/user/tahuang/SLHC13_100k_L1_PU140_Pt0_2023All_LooserMatch/tahuang/SingleMuPt2-50_1M_SLHC11_2023Muon/SLHC13_100k_L1_PU140_Pt0_2023All_LooserMatch/5a549a4810b5acc2831e5eae6e69c904/']
#InputDir = ['/eos/uscms/store/user/tahuang/SLHC13_100k_L1_PU140_Pt0_2023All_SortByQuality/']
#InputDir = ['/eos/uscms/store/user/tahuang/SLHC13_100k_L1_PU140_Pt0_2023All_SortByGEMDPhi_v4/']
#InputDir = ['/eos/uscms/store/user/tahuang/SLHC13_100k_L1_PU140_Pt0_2023All_combined/tahuang/SingleMuPt2-50_1M_SLHC11_2023Muon/SLHC13_100k_L1_PU140_Pt0_2023All_combined/5a549a4810b5acc2831e5eae6e69c904/']
#InputDir = ['/eos/uscms/store/user/tahuang/SLHC13_patch2_200k_L1_PU140_Pt0_2023All_combined/']
#InputDir = ['/eos/uscms/store/user/tahuang/SLHC13_100k_L1_PU140_Pt0_2023All_FixBX/']
#InputDir = ['/eos/uscms/store/user/tahuang/SLHC16_200k_L1_PU0_Pt0_2023All/tahuang/SLHC16_2023_gen_sim_Pt2_50_1M/SLHC16_200k_L1_PU0_Pt0_2023All/3fa846ef32763f40bf20ad84de3c74de/']
#InputDir = ['/eos/uscms/store/user/tahuang/SLHC16_200k_L1_PU140_Pt0_2023All/tahuang/SLHC16_2023_gen_sim_Pt2_50_1M/SLHC16_200k_L1_PU140_Pt0_2023All/3fa846ef32763f40bf20ad84de3c74de/']
#InputDir = ['/eos/uscms/store/user/tahuang/SLHC16_200k_L1_PU140_Pt0_2023All_fixdphi/']
#InputDir = ['/eos/uscms/store/user/tahuang/SLHC16_200k_L1_PU0_Pt0_2023All_fixdphi/']
#InputDir = ['/eos/uscms/store/user/tahuang/SLHC16_200k_L1_PU140_Pt2-50_2023All_fixdphi/']
#InputDir = ['/eos/uscms/store/user/tahuang/SLHC16_200k_L1_PU0_Pt2-50_2023All_fixdphi/']
#InputDir = ['/eos/uscms/store/user/tahuang/SLHC16_200k_L1_PU140_Pt2-50_2023All_GE21dphi/']
#InputDir = ['/eos/uscms/store/user/tahuang/SLHC16_200k_L1_PU140_Pt2-50_2023All_GE21dphi_v3/']
InputDir = ['/eos/uscms/store/user/tahuang/SLHC16_200k_L1_PU140_Pt2-50_2023All_switchPositive/']
## input
from GEMCode.SimMuL1.GEMCSCTriggerSamplesLib import *
from GEMCode.GEMValidation.InputFileHelpers import *
process = useInputDir(process, InputDir, True)
#process = useInputDir(process, files['_gem98_pt2-50_PU0_pt0_new'], False)

process.TFileService = cms.Service("TFileService",
    fileName = cms.string("PU140_200k_Pt2-50_test_GEMCSC.root")
#    fileName = cms.string("test_GEMCSC.root")
)

## global tag for upgrade studies
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:upgradePLS3', '')

# the analyzer configuration
def enum(*sequential, **named):
  enums = dict(zip(sequential, range(len(sequential))), **named)
  return type('Enum', (), enums)
Stations = enum('ALL','ME11','ME1a','ME1b','ME12','ME13','ME21','ME22','ME31','ME32','ME41','ME42')

from GEMCode.GEMValidation.simTrackMatching_cfi import SimTrackMatching
process.GEMCSCAnalyzer = cms.EDAnalyzer("GEMCSCAnalyzer",
    verbose = cms.untracked.int32(0),
    stationsToUse = cms.vint32(Stations.ALL,Stations.ME11,Stations.ME1a,Stations.ME1b,
                              Stations.ME21,Stations.ME31,Stations.ME41),
    simTrackMatching = SimTrackMatching
)
matching = process.GEMCSCAnalyzer.simTrackMatching
matching.simTrack.minPt = 1.5
matching.matchprint = cms.bool(False)
matching.gemRecHit.input = ""
"""
matching.cscTfTrack.input = ""
matching.tfCand.input = ""
matching.gmtCand.input = ""
matching.l1Extra.input = ""
"""
doGem = True
if doGem:
  matching.cscSimHit.minNHitsChamber = 3
  matching.cscStripDigi.minNHitsChamber = 3
  matching.cscWireDigi.minNHitsChamber = 3
  matching.cscCLCT.minNHitsChamber = 3
  matching.cscALCT.minNHitsChamber = 3
  matching.cscLCT.minNHitsChamber = 3
  matching.cscLCT.matchAlctGem = True
  matching.cscMPLCT.minNHitsChamber = 3

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )

process.p = cms.Path(process.GEMCSCAnalyzer)

## messages
print
#print 'Input files:'
print '----------------------------------------'
#print process.source.fileNames
print
print 'Output file:'
print '----------------------------------------'
print process.TFileService.fileName
print
