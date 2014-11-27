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

c1 = ROOT.TCanvas()
c1.SetGridx()
c1.SetGridy()
c1.SetTickx()
c1.SetTicky()
#____________________________________________________________________________
def getEff(file,dir,den,num):
    f = ROOT.TFile(file)
    t = f.Get(dir)
    h1 = ROOT.TH1F("h1","h1",50,1.5,2.5)
    t.Draw("(-eta) >> h1",den)
    h2 = ROOT.TH1F("h2","h2",50,1.5,2.5)
    t.Draw("(-eta) >> h2",num)
    e = ROOT.TEfficiency(h2,h1)
    return e


#_______________________________________________________________________________
def getHist(file_name, histo_name, dir_name = "L1TAnalyser"):
    """Get the ROOT directory from the GEMCSCTriggerEfficiency analyzer. Normally,
    the directory should be called GEMCSCTriggerEfficiency. You should check it anyway"""

    ### get the ROOT file
    file = ROOT.TFile.Open(file_name)
    if not file:
        sys.exit('Input ROOT file %s is missing.' %(file_name))

    ## get the ROOT directory
    dir = file.Get(dir_name)
    if not dir:
        sys.exit('Directory %s does not exist.' %(dir_name))

    """Get the histogram from a directory"""
    histo = dir.Get("%s;1"%(histo_name))
    if not histo:
       print "No such histogram: ", histo_name
    return histo


b1 = ROOT.TH1F("b1","b1",40,1.5,2.5)
b1.GetYaxis().SetRangeUser(0.60,1.02)
b1.GetYaxis().SetNdivisions(520)
b1.GetYaxis().SetTitle("Efficiency")
b1.GetXaxis().SetTitle("Simulated muon #eta")
b1.SetTitle(" "*12 +"TFTrack reconstruction"+" "*10 + "CMS Simulation Preliminary")
b1.SetStats(0)

treename = "GEMCSCAnalyzer/trk_eff_ALL"
den = "  pt >10"
num = " (has_tfTrack)>0 && nstubs>=3 && pt>10"

e1 = getEff("PU140_200k_Pt2-50_2023_correction_GEMCSC.root",treename,den,num)
#e2 = getEff("PU140_100k_2019withoutGEM_GEMCSCAna.root",treename,den,num)
e4 = getEff("PU140_200k_Pt2-50_2023_lct_GEMCSC.root",treename,den,num)
#e3 = getEff("PU140_Jason_100k_2023_delta_fixdt_GEMCSCAna.root",treename,den,num)
e3 = getEff("PU140_10k_Pt20_2023_update_GEMCSC.root",treename,den,num)
e2 = getHist("histo_SingleMu_SLHC16_PU140_Pt2-50_jason.root","L1cscTrack_stub3_eta")

#e5 = getEff("GSA_GEMCSC_Step2_Com_PU140.root",treename,den,num)
#e6 = getEff("GSA_GEMCSC_Step3_Com_PU140.root",treename,den,num)
#e7 = getEff("GSA_GEMCSC_Step4_Com_PU140.root",treename,den,num)

e1.SetFillColor(ROOT.kMagenta)
#e1.SetLineWidth(2)
e2.SetFillColor(ROOT.kBlack)
#e2.SetLineWidth(2)
e3.SetFillColor(ROOT.kRed)
#e3.SetLineWidth(2)
e4.SetFillColor(ROOT.kAzure-1)
#e4.SetLineWidth(2)
b1.Draw("e3")
e3.Draw("e3same")
e4.Draw("e3same")
e2.Draw("e3same")
e1.Draw("e3same")
#e7.Draw("same")

legend = ROOT.TLegend(0.20,0.23,.75,0.5, "", "brNDC")
legend.SetBorderSize(0)
legend.SetFillStyle(0)
legend.SetTextSize(0.05)
legend.SetHeader("PU140,  P_{T} > 10 GeV, at least 3stubs")
legend.AddEntry(e1,"simeta, propagated phi","f")
legend.AddEntry(e2,"Jason's ana","f")
legend.AddEntry(e3,"simeta, simphi","f")
legend.AddEntry(e4,"eta,phi from stubs","f")
legend.Draw("same")

#tex = ROOT.TLatex(0.25,0.5,"PU140, P\_T > 10Gev")
#tex.SetTextSize(0.05)
#tex.SetNDC()
#tex.Draw("same")

c1.SaveAs("TFTrack_200k_reco_eff_3stubs_com_3_PU140.pdf")
c1.SaveAs("TFTrack_200k_reco_eff_3stubs_com_3_PU140.png")
