## library of input files for the GEM-CSC trigger study

files = {}

## produced on top of https://github.com/gem-sw/cmssw/tree/adb1cc3e0fad25bc0da2e4430e3e0965160a58dc
## 6 partition geometry + post CSC TF pt bug + detailed GEM digi model
eosfiles = {}

## official samples for L1 trigger development
eosfiles['_pt2-50_PU000_6part2019'] = ['/eos/uscms/store/user/jdimasva/SingleMuPt2-50Fwdv2_100K_DIGI_PU000/jdimasva/SingleMuPt2-50Fwdv2_1M/SingleMuPt2-50Fwdv2_100K_DIGI_PU000/36687f2fa27757b21d07bf97e7678b19/']
eosfiles['_pt2-50_PU140_6part2019'] = ['/eos/uscms/store/user/jdimasva/SingleMuPt2-50Fwdv2_100K_DIGI_PU140/jdimasva/SingleMuPt2-50Fwdv2_1M/SingleMuPt2-50Fwdv2_100K_DIGI_PU140/7444237097ec40e1cd737724f1a85642/']
eosfiles['_pt2-50_PU400_6part2019'] = ['/eos/uscms/store/user/aysen/SingleMuPt2-50Fwdv2_100K_DIGI_PU400/aysen/SingleMuPt2-50Fwdv2_1M/SingleMuPt2-50Fwdv2_100K_DIGI_PU400/6bcb78929a3bd07607400097784c7b5b/']

eosfiles['_pt2-50_PU0_SLHC10_2023Muon'] = ['/eos/uscms/store/user/dildick/dildick/SingleMuPt2-50Fwdv2_50k_DIGI_PU0_SLHC10_2023Muon/SingleMuPt2-50Fwdv2_50k_DIGI_PU0_SLHC10_2023Muon/3fc7116e7a0ba79c1b27ffca0468fa34/']


## new samples produced with SLHC11 + fix in CSC digitizer (ask Aysen)
eosfiles['_pt2-50_SLHC11_2023Muon'] = ['/eos/uscms/store/user/lpcgem/dildick/SingleMuPt2-50_1M_SLHC11_2023Muon/SingleMuPt2-50_1M_SLHC11_2023Muon/c7fc9200318437f38716754e18c29ddf/']

## digi samples 
eosfiles['_Nu_SLHC12_2023Muon_PU140'] = ['/eos/uscms/store/user/lpcgem/jlee/SingleNu_cfi_GEN_SIM/SingleNu_SLHC12_2023Muon_DIGI_PU140/3d13cbaabc944c94dfc4b7e14635daa0/']
eosfiles['_Nu_SLHC12_2023Muon_PU140_geonmo'] = ['/eos/uscms/store/user/lpcgem/geonmo/SingleNu_cfi_GEN_SIM/SingleNu_SLHC12_2023Muon_DIGI_PU140v2/6b50ed2a008d8a47a360507d169521e1/']
eosfiles['_Nu_SLHC12_2023Muon_PU400'] = ['/eos/uscms/store/user/lpcgem/aysen/SingleNu_cfi_GEN_SIM/SingleNu_SLHC11_2023Muon_DIGI_PU400_v3/6869af1b28a0650484eca6fece69b429/']

eosfiles['_pt2-50_SLHC12_2023Muon_PU140_tao'] = ['/eos/uscms/store/user/lpcgem/tahuang/SingleMuPt2-50_1M_SLHC11_2023Muon/SingleMuon_SLHC12_2023Muon_DIGI_PU140/464f837479844427abae84188ceec4af/']

## Sven's samples for bending angle calculations
eosfiles['_SingleMuPt3_SLHC12_2023Muon_PU0_L1'] = ['/eos/uscms/store/user/lpcgem/dildick/SingleMuPt3_SLHC12_GEN_SIM_DIGI_L1/SingleMuPt3_SLHC12_GEN_SIM_DIGI_L1/d1c0a6121a5b67989597420fd3bcad81/']
eosfiles['_SingleMuPt5_SLHC12_2023Muon_PU0_L1'] = ['/eos/uscms/store/user/lpcgem/dildick/SingleMuPt5_SLHC12_GEN_SIM_DIGI_L1/SingleMuPt5_SLHC12_GEN_SIM_DIGI_L1/7bd82d6abf1b247a8e77eabb8d0358b4/']
eosfiles['_SingleMuPt7_SLHC12_2023Muon_PU0_L1'] = ['/eos/uscms/store/user/lpcgem/dildick/SingleMuPt7_SLHC12_GEN_SIM_DIGI_L1/SingleMuPt7_SLHC12_GEN_SIM_DIGI_L1/82bc934ea88643c7d53d811e1ce09a7e/']
eosfiles['_SingleMuPt10_SLHC12_2023Muon_PU0_L1'] = ['/eos/uscms/store/user/lpcgem/dildick/SingleMuPt10_SLHC12_GEN_SIM_DIGI_L1/SingleMuPt10_SLHC12_GEN_SIM_DIGI_L1/f216a8c46e3ad5c04305450b1e11f485/']
eosfiles['_SingleMuPt15_SLHC12_2023Muon_PU0_L1'] = ['/eos/uscms/store/user/lpcgem/dildick/SingleMuPt15_SLHC12_GEN_SIM_DIGI_L1/SingleMuPt15_SLHC12_GEN_SIM_DIGI_L1/1fd2b692590369b313166b87828a5e55/']
eosfiles['_SingleMuPt20_SLHC12_2023Muon_PU0_L1'] = ['/eos/uscms/store/user/lpcgem/dildick/SingleMuPt20_SLHC12_GEN_SIM_DIGI_L1/SingleMuPt20_SLHC12_GEN_SIM_DIGI_L1/9f43e5aa0db40ecf4c6ad4f76ff3ba57/']
eosfiles['_SingleMuPt30_SLHC12_2023Muon_PU0_L1'] = ['/eos/uscms/store/user/lpcgem/dildick/SingleMuPt30_SLHC12_GEN_SIM_DIGI_L1/SingleMuPt30_SLHC12_GEN_SIM_DIGI_L1/4c222e5f541a2a909af90d52da92bbae/']
eosfiles['_SingleMuPt40_SLHC12_2023Muon_PU0_L1'] = ['/eos/uscms/store/user/lpcgem/dildick/SingleMuPt40_SLHC12_GEN_SIM_DIGI_L1/SingleMuPt40_SLHC12_GEN_SIM_DIGI_L1/0f0ae35ebf4e35077884bcb7db7d7ad0/']
