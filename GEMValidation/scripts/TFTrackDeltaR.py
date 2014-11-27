import ROOT
ROOT.gROOT.SetBatch(1)
ROOT.gStyle.SetStatW(0.07)
ROOT.gStyle.SetStatH(0.06)

ROOT.gStyle.SetOptStat(0)



ROOT.gStyle.SetTitleStyle(0)
ROOT.gStyle.SetTitleAlign(13) ## coord in top left
ROOT.gStyle.SetTitleX(0.)
ROOT.gStyle.SetTitleY(1.)
ROOT.gStyle.SetTitleW(1)
ROOT.gStyle.SetTitleH(0.058)
ROOT.gStyle.SetTitleBorderSize(0)

ROOT.gStyle.SetPadLeftMargin(0.126)
ROOT.gStyle.SetPadRightMargin(0.04)
ROOT.gStyle.SetPadTopMargin(0.06)
ROOT.gStyle.SetPadBottomMargin(0.13)

ROOT.gStyle.SetMarkerStyle(1)

#____________________________________________________________________________
def getHistInTree(file,dir,cut):
    f = ROOT.TFile(file)
    t = f.Get(dir)
    h = ROOT.TH1F("h","h",100,0,0.5)
#t.Draw("deltaR >> h",cut)
#    print "file: ",file
#    print "h type ", type(h)," t type ", type(t)
#    ROOT.SetOwnership(h, False)
#    c_temp = ROOT.TCanvas()
#    h.Draw()
#    c_temp.SaveAs("temp.png")
    return t

if __name__ == "__main__":
    c1 = ROOT.TCanvas()
    c1.SetGridx()
    c1.SetGridy()
    c1.SetTickx()
    c1.SetTicky()
    b1 = ROOT.TH1F("b1","b1",100,0,0.5)
#b1.GetYaxis().SetNdivisions(520)
    b1.GetYaxis().SetTitle("counts")
    b1.GetXaxis().SetTitle("deltaR")
    b1.SetTitle(" "*36 + "CMS Simulation Preliminary")
    b1.SetStats(0)

    treename = "GEMCSCAnalyzer/trk_eff_ALL"
    num = " (has_tfTrack)>0 && pt>10"
#print "treename ",treename,"  type ",type(treename)
#    print "function return type ", type(getHistInTree("PU140_200k_Pt2-50_2023_correction_GEMCSC.root",treename,num))
    f1 = ROOT.TFile("PU140_200k_Pt2-50_2023_correction_GEMCSC.root")
    t1 = f1.Get(treename)
    h1 = ROOT.TH1F("h1","h1",100,0,0.5)
    t1.Draw("deltaR>>h1",num)
    f2 = ROOT.TFile("PU140_200k_Pt2-50_2023_lct_GEMCSC.root")
    t2 = f2.Get(treename)
    h2 = ROOT.TH1F("h2","h2",100,0,0.5)
    t2.Draw("deltaR>>h2",num)
    t2 = getHistInTree("PU140_10k_Pt20_2023_update_GEMCSC.root",treename,num)
    f3 = ROOT.TFile("PU140_10k_Pt20_2023_update_GEMCSC.root")
    t3 = f3.Get(treename)
    h3 = ROOT.TH1F("h3","h3",100,0,0.5)
    t3.Draw("deltaR>>h3",num)

#    print "t1", type(t1)
    ROOT.gPad.SetLogy()
    b1.SetMaximum(20000)
    h1.SetLineColor(ROOT.kRed)
    h2.SetLineColor(ROOT.kGreen)
    h3.SetLineColor(ROOT.kBlue)
    b1.Draw()
    h1.Draw("same")
    h2.Draw("same")
    h3.Draw("same")
#e7.Draw("same")

    legend = ROOT.TLegend(0.20,0.23,.75,0.5, "", "brNDC")
    legend.SetBorderSize(0)
    legend.SetFillStyle(0)
    legend.SetTextSize(0.05)
#legend.SetHeader("PU140,  P_{T} > 10 GeV")
    legend.AddEntry(h1,"simeta, propagated phi","l")
    legend.AddEntry(h3,"Jason's ana","l")
    #legend.AddEntry(e3,"simeta, simphi","f")
    legend.AddEntry(h2,"eta,phi from stubs","l")
#legend.AddEntry(e4,"Upgrade TMB:CSC-RPC local trigger","f")
#legend.AddEntry(e5,"GEM-CSC Algorithm (step 2)","l")
#legend.AddEntry(e6,"GEM-CSC Algorithm (step 3)","l")
#legend.AddEntry(e7,"GEM-CSC Algorithm (step 4)","l")
    legend.Draw("same")

#tex = ROOT.TLatex(0.25,0.5,"PU140, P\_T > 10Gev")
#tex.SetTextSize(0.05)
#tex.SetNDC()
#tex.Draw("same")

    c1.SaveAs("TFTrack_200k_deltaR_PU140.pdf")
    c1.SaveAs("TFTrack_200k_deltaR_PU140.png")
