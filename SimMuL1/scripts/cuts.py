from ROOT import TCut

#_______________________________________________________________________________
def ANDtwo(cut1,cut2):
    """AND of two TCuts in PyROOT"""
    return TCut("(%s) && (%s)"%(cut1.GetTitle(),cut2.GetTitle()))


#_______________________________________________________________________________
def ORtwo(cut1,cut2):
    """OR of two TCuts in PyROOT"""
    return TCut("(%s) || (%s)"%(cut1.GetTitle(),cut2.GetTitle()))


#_______________________________________________________________________________
def AND(*arg):
    """AND of any number of TCuts in PyROOT"""
    length = len(arg)
    if length == 0:
        print "ERROR: invalid number of arguments"
        return
    if length == 1:
        return arg[0] 
    if length==2:
        return ANDtwo(arg[0],arg[1])
    if length>2:
        result = arg[0]
        for i in range(1,len(arg)):
            result = ANDtwo(result,arg[i])
        return result


#_______________________________________________________________________________
def OR(*arg): 
    """OR of any number of TCuts in PyROOT"""
    length = len(arg)
    if length == 0:
        print "ERROR: invalid number of arguments"
        return
    if length == 1:
        return arg[0] 
    if length==2:
        return ORtwo(arg[0],arg[1])
    if length>2:
        result = arg[0]
        for i in range(1,len(arg)):
            result = ORtwo(result,arg[i])
        return result


#_______________________________________________________________________________
## ranges of eta partitions GE1/1 and GE2/1L

eta_partitions = {
    'GE11-8-8' : {
    },
    'GE11-9-10' : {
        1: {'even' : {'min' : -99,     'max' : -99    }, 'odd' : {'min' : 1.54160, 'max' : 1.59633} },
        2: {'even' : {'min' : 1.59245, 'max' : 1.65334}, 'odd' : {'min' : 1.59652, 'max' : 1.65744} },
        3: {'even' : {'min' : 1.65354, 'max' : 1.71946}, 'odd' : {'min' : 1.65765, 'max' : 1.72360} },
        4: {'even' : {'min' : 1.71968, 'max' : 1.77729}, 'odd' : {'min' : 1.72382, 'max' : 1.78146} },
        5: {'even' : {'min' : 1.77753, 'max' : 1.83933}, 'odd' : {'min' : 1.78170, 'max' : 1.84353} },
        6: {'even' : {'min' : 1.83959, 'max' : 1.89512}, 'odd' : {'min' : 1.84379, 'max' : 1.89934} },
        7: {'even' : {'min' : 1.89539, 'max' : 1.95468}, 'odd' : {'min' : 1.89961, 'max' : 1.95892} },
        8: {'even' : {'min' : 1.95497, 'max' : 2.01682}, 'odd' : {'min' : 1.95922, 'max' : 2.02108} },
        9: {'even' : {'min' : 2.01713, 'max' : 2.08337}, 'odd' : {'min' : 2.02140, 'max' : 2.08765} },
       10: {'even' : {'min' : 2.08371, 'max' : 2.14738}, 'odd' : {'min' : 2.08799, 'max' : 2.15167} },
    },
    'GE21S' : {
        1: {'even' : {'min' : 1.63089, 'max' : 1.67609}, 'odd' : {'min' : 1.63089, 'max' : 1.67609} },
        2: {'even' : {'min' : 1.67639, 'max' : 1.72423}, 'odd' : {'min' : 1.67639, 'max' : 1.72423} },
        3: {'even' : {'min' : 1.72455, 'max' : 1.77531}, 'odd' : {'min' : 1.72455, 'max' : 1.77531} },
        4: {'even' : {'min' : 1.77565, 'max' : 1.82965}, 'odd' : {'min' : 1.77565, 'max' : 1.82965} },
        5: {'even' : {'min' : 1.83001, 'max' : 1.88763}, 'odd' : {'min' : 1.83001, 'max' : 1.88763} },
        6: {'even' : {'min' : 1.88802, 'max' : 1.94971}, 'odd' : {'min' : 1.88802, 'max' : 1.94971} },
        7: {'even' : {'min' : 1.95013, 'max' : 2.01644}, 'odd' : {'min' : 1.95013, 'max' : 2.01644} },
        8: {'even' : {'min' : 2.01689, 'max' : 2.08849}, 'odd' : {'min' : 2.01689, 'max' : 2.08849} },
    },
    'GE21L' : {
        1: {'even' : {'min' : 1.64917, 'max' : 1.69498}, 'odd' : {'min' : 1.64917, 'max' : 1.69498} },
        2: {'even' : {'min' : 1.69529, 'max' : 1.74379}, 'odd' : {'min' : 1.69529, 'max' : 1.74379} },
        3: {'even' : {'min' : 1.74412, 'max' : 1.79560}, 'odd' : {'min' : 1.74412, 'max' : 1.79560} },
        4: {'even' : {'min' : 1.79595, 'max' : 1.85074}, 'odd' : {'min' : 1.79595, 'max' : 1.85074} },
        5: {'even' : {'min' : 1.85111, 'max' : 1.90962}, 'odd' : {'min' : 1.85111, 'max' : 1.90962} },
        6: {'even' : {'min' : 1.91002, 'max' : 1.97270}, 'odd' : {'min' : 1.91002, 'max' : 1.97270} },
        7: {'even' : {'min' : 1.97313, 'max' : 2.04057}, 'odd' : {'min' : 1.97313, 'max' : 2.04057} },
        8: {'even' : {'min' : 2.04103, 'max' : 2.11393}, 'odd' : {'min' : 2.04103, 'max' : 2.11393} },
        9: {'even' : {'min' : 2.11443, 'max' : 2.19365}, 'odd' : {'min' : 2.11443, 'max' : 2.19365} },
       10: {'even' : {'min' : 2.19419, 'max' : 2.28085}, 'odd' : {'min' : 2.19419, 'max' : 2.28085} },
       11: {'even' : {'min' : 2.28144, 'max' : 2.37696}, 'odd' : {'min' : 2.28144, 'max' : 2.37696} },
       12: {'even' : {'min' : 2.37762, 'max' : 2.48389}, 'odd' : {'min' : 2.37762, 'max' : 2.48389} },
    },
    'RE31' : {
        1: {'even' : {'min' : 1.82706, 'max' : 1.92791}, 'odd' : {'min' : 1.82274, 'max' : 1.92354} },
        2: {'even' : {'min' : 1.93091, 'max' : 2.04512}, 'odd' : {'min' : 1.92654, 'max' : 2.04072} },
        3: {'even' : {'min' : 2.04854, 'max' : 2.17972}, 'odd' : {'min' : 2.04414, 'max' : 2.17529} },
        4: {'even' : {'min' : 2.18369, 'max' : 2.33720}, 'odd' : {'min' : 2.17925, 'max' : 2.33273} },
        5: {'even' : {'min' : 2.34189, 'max' : 2.52619}, 'odd' : {'min' : 2.33742, 'max' : 2.52169} },
    },
    'RE41' : {
        1: {'even' : {'min' : 1.90357, 'max' : 1.99096}, 'odd' : {'min' : 1.89958, 'max' : 1.98695} },
        2: {'even' : {'min' : 1.99292, 'max' : 2.08996}, 'odd' : {'min' : 1.98891, 'max' : 2.08592} },
        3: {'even' : {'min' : 2.09215, 'max' : 2.20097}, 'odd' : {'min' : 2.08811, 'max' : 2.19691} },
        4: {'even' : {'min' : 2.20344, 'max' : 2.32703}, 'odd' : {'min' : 2.19938, 'max' : 2.32295} },
        5: {'even' : {'min' : 2.32986, 'max' : 2.47251}, 'odd' : {'min' : 2.32577, 'max' : 2.46841} },
    },
}

#_______________________________________________________________________________
nocut = TCut("")

ok_eta = TCut("TMath::Abs(eta)>1.64 && TMath::Abs(eta)<2.14")
ok_pt = TCut("pt > 20.")

def ok_partition(station, p, even):
    stationStrings = {'ME1b' : 'GE11-9-10', 'ME21' : 'GE21L', 'ME31' : 'RE31', 'ME41' : 'RE41' }
    eta_min = eta_partitions[stationStrings[station]][p][even]['min']
    eta_max = eta_partitions[stationStrings[station]][p][even]['max']
    return TCut('TMath::Abs(eta) >= %f && TMath::Abs(eta) <= %f'%(eta_min, eta_max))

## CSC simhits & digis
ok_sh1 = TCut("(has_csc_sh&1) > 0")
ok_sh2 = TCut("(has_csc_sh&2) > 0")
ok_st1 = TCut("(has_csc_strips&1) > 0")
ok_st2 = TCut("(has_csc_strips&2) > 0")
ok_w1 = TCut("(has_csc_wires&1) > 0")
ok_w2 = TCut("(has_csc_wires&2) > 0")
ok_digi1 = AND(ok_st1,ok_w1)
ok_digi2 = AND(ok_st2,ok_w2)

## CSC stub
ok_lct1 = TCut("(has_lct&1) > 0")
ok_lct2 = TCut("(has_lct&2) > 0")
ok_alct1 = TCut("(has_alct&1) > 0")
ok_alct2 = TCut("(has_alct&2) > 0")
ok_clct1 = TCut("(has_clct&1) > 0")
ok_clct2 = TCut("(has_clct&2) > 0")
ok_lct_hs_min = TCut("hs_lct_odd > 4")
ok_lct_hs_max = TCut("hs_lct_odd < 125")
ok_lct_hs = AND(ok_lct_hs_min,ok_lct_hs_max)
ok_lcths1 = AND(ok_lct1,ok_lct_hs)
ok_lcths2 = AND(ok_lct2,ok_lct_hs)

## GEM simhit
ok_gsh1 = TCut("(has_gem_sh&1) > 0")
ok_gsh2 = TCut("(has_gem_sh&2) > 0")
ok_g2sh1 = TCut("(has_gem_sh2&1) > 0")
ok_g2sh2 = TCut("(has_gem_sh2&2) > 0")


## GEM digi
ok_gdg1 = TCut("(has_gem_dg&1) > 0")
ok_gdg2 = TCut("(has_gem_dg&2) > 0")
ok_pad1 = TCut("(has_gem_pad&1) > 0")
ok_pad2 = TCut("(has_gem_pad&2) > 0")
ok_rpcstrip1 = TCut("(has_rpc_dg&1) > 0")
ok_rpcstrip2 = TCut("(has_rpc_dg&2) > 0")

ok_dphi1 = TCut("dphi_pad_odd < 10.")
ok_dphi2 = TCut("dphi_pad_even < 10.")

ok_pad1_lct1 = AND(ok_pad1,ok_lct1)
ok_pad2_lct2 = AND(ok_pad2,ok_lct2)

ok_rpcstrip1_lct1 = AND(ok_rpcstrip1,ok_lct1)
ok_rpcstrip2_lct2 = AND(ok_rpcstrip2,ok_lct2)

ok_pad1_dphi1 = AND(ok_pad1,ok_dphi1)
ok_pad2_dphi2 = AND(ok_pad2,ok_dphi2)

ok_lct1_eta = AND(ok_eta,ok_lct1)
ok_lct2_eta = AND(ok_eta,ok_lct2)

ok_pad1_lct1_eta = AND(ok_pad1,ok_lct1,ok_eta)
ok_pad2_lct2_eta = AND(ok_pad2,ok_lct2,ok_eta)

ok_gsh1_lct1_eta = AND(ok_gsh1,ok_lct1,ok_eta)
ok_gsh2_lct2_eta = AND(ok_gsh2,ok_lct2,ok_eta)

ok_gsh1_eta = AND(ok_gsh1,ok_eta)
ok_gsh2_eta = AND(ok_gsh2,ok_eta)

ok_gdg1_eta = AND(ok_gdg1,ok_eta)
ok_gdg2_eta = AND(ok_gdg2,ok_eta)

ok_2pad1 = TCut("(has_gem_pad2&1) > 0")
ok_2pad2 = TCut("(has_gem_pad2&2) > 0")

ok_pad1_overlap = OR(ok_pad1,AND(ok_lct2,ok_pad2))
ok_pad2_overlap = OR(ok_pad2,AND(ok_lct1,ok_pad1))

ok_copad1 = TCut("(has_gem_copad&1) > 0")
ok_copad2 = TCut("(has_gem_copad&2) > 0")

ok_Qp = TCut("charge > 0")
ok_Qn = TCut("charge < 0")

ok_lct1_eta_Qn = AND(ok_lct1,ok_eta,ok_Qn)
ok_lct2_eta_Qn = AND(ok_lct2,ok_eta,ok_Qn)

ok_lct1_eta_Qp = AND(ok_lct1,ok_eta,ok_Qp)
ok_lct2_eta_Qp = AND(ok_lct2,ok_eta,ok_Qp)

Ep = TCut("endcap > 0")
En = TCut("endcap < 0")

