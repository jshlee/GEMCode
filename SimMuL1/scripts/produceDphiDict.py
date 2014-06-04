##
## This script produces the dictionary of bending angles
##

from cuts import *

## run quiet mode
import sys
sys.argv.append( '-b' )

from ROOT import *
gROOT.SetBatch(1)

def getTree(fileName,station):
    """Get tree for given filename"""

    analyzer = "GEMCSCAnalyzer"
    trk_eff = "trk_eff_%s"%(station)

    file = TFile.Open(fileName)
    if not file:
        sys.exit('Input ROOT file %s is missing.' %(fileName))

    dir = file.Get(analyzer)
    if not dir:
        sys.exit('Directory %s does not exist.' %(dir))
        
    tree = dir.Get(trk_eff)
    if not tree:
        sys.exit('Tree %s does not exist.' %(tree))

    return tree

def dphiCut(h, fractionToKeep):
    """Get the dPhi cut corresponding to the fractionToKeep [95,98,99]"""

    ax = h.GetXaxis()
    total = h.Integral()
    bin = 1
    fractionToKeep = fractionToKeep/100.
    for b in range(ax.GetNbins()):
        if (h.Integral(0,b)/total > fractionToKeep):
            bin = b - 1
            break

    ## interpolate
    x1 = ax.GetBinUpEdge(bin)
    x2 = ax.GetBinUpEdge(bin + 1)
    y1 = h.Integral(0, bin)/total
    y2 = h.Integral(0, bin + 1)/total
    x = x1 + (fractionToKeep - y1)/(y2-y1)*(x2-x1)
    return x


def produceDphiDict(filesDir, outFileName):
    """Get the libary of dPhi values"""
    
    etas = [8,12,5,5] ## eta partitions
    dphis = [[[[[0 for x in xrange(2)] for x in xrange(len(fr))] for x in xrange(len(pt))] for x in xrange(etas[1])] for x in xrange(len(stations))]
    
    for st in range(len(stations)):
        station = stations[st]
        ## print "Processing station", station, "with", etas[st], "partitions"        
        for m in range(etas[st]):
            ## print "  Processing partition", m+1
            for n in range(len(pt)):
                ## print "    Processing", pt[n]
                t = getTree("%sgem-csc_stub_ana_%s.root"%(filesDir,pt[n]),station)
                ## add eta cuts here
                if (station is 'ME1b') or (station is 'ME21'):
                    t.Draw("TMath::Abs(dphi_pad_odd)>>dphi_odd(600,0.,0.3)", ok_pad1_lct1)
                    t.Draw("TMath::Abs(dphi_pad_even)>>dphi_even(600,0.,0.03)", ok_pad2_lct2)
                else:
                    t.Draw("TMath::Abs(dphi_rpcstrip_odd)>>dphi_odd(600,0.,0.3)", ok_rpcstrip1_lct1)
                    t.Draw("TMath::Abs(dphi_rpcstrip_even)>>dphi_even(600,0.,0.03)", ok_rpcstrip2_lct2)
                h_dphi_odd = TH1F(gDirectory.Get("dphi_odd"))
                h_dphi_even = TH1F(gDirectory.Get("dphi_even"))
                for f in range(len(fr)):
                    ## print "      Processing fraction",fr[f]
                    dphis[st][m][n][f][0] = dphiCut(h_dphi_odd, fr[f])
                    dphis[st][m][n][f][1] = dphiCut(h_dphi_even, fr[f])

    ## print the dphis to a file
    outfile = open("%s"%(outFileName),"w")
    outfile.write("dphi_lct_pad = {\n")
    for st in range(len(stations)):
        station = stations[st]
        outfile.write('    "%s" : {\n'%(station))
        if station == len(stations)-1:
            endchar1 = ""
        else:
            endchar1 = ","
            for m in range(etas[st]):
                outfile.write('        "Eta%d" : {\n'%(m+1))
                if station == len(stations)-1:
                    endchar2 = ""
                else:
                    endchar2 = ","
                for n in range(len(pt)):
                    outfile.write('            "%s" : {\n'%(pt[n]))
                    if n == len(pt)-1:
                        endchar3 = ""
                    else:
                        endchar3 = ","
                    for f in range(len(fr)):
                        if f == len(fr)-1:
                            endchar4 = ""
                        else:
                            endchar4 = "," 
                        outfile.write(' '*16 + '"Eff%d" : { "odd" :  %.8f, "even" : %.8f }%s\n'%(fr[f], dphis[st][m][n][f][0], dphis[st][m][n][f][1], endchar4))
                    outfile.write('            }%s\n'%(endchar3))                        
                outfile.write('        }%s\n'%(endchar2))
        outfile.write('    }%s\n'%(endchar1))
 
    outfile.write('}\n')

    ## close the output file
    outfile.close()
    
    """
    ## print the dphi library for these samples
    outfile = open("%s"%(outFileName),"w")
    outfile.write("dphi_lct_pad = {\n")
    for f in range(len(fr)):
        outfile.write('    "%d" : {\n'%(fr[f]))
        if f == len(fr)-1:
            endchar1 = ""
        else:
            endchar1 = ","
        for n in range(len(pt)):
            pt_string = ("%s"%(pt[n])).ljust(4)
            if n == len(pt)-1:
                endchar = ""
            else:
                endchar = ","
            outfile.write("        '%s' : { 'odd' :  %.8f, 'even' : %.8f }%s\n"%(pt_string, dphis[n][f][0], dphis[n][f][1], endchar))
        outfile.write('        }%s\n'%(endchar1))

    outfile.write('}\n')

    ## close the output file
    outfile.close()
    """

    return
    
    ## print some additional information
    print "dPhi library written to:", outfile.name
    outfile = open("%s"%(outFileName),"r")
    print outfile.read()
    outfile.close()


if __name__ == "__main__":
    
    stations = ['ME1b', 'ME21', 'ME31', 'ME41']
    stations = ['ME1b', 'ME21']
    pt = ["Pt3","Pt5","Pt7","Pt10","Pt15","Pt20","Pt30","Pt40"]
    fr = [95,98,99]
    
    produceDphiDict("/uscms/home/dildick/nobackup/work/cscTriggerUpgradeGEMRPC/CMSSW_6_2_0_SLHC12/src/", "GEMCSCdPhiDict.py")
    
