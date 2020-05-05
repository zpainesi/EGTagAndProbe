from ROOT import *

# global style for nice plots!
# histoType = 0: histos with errors via sumw2() ==> set markers, not lines!
def SetLucaStyle ():
    #global LS
    LS = TStyle (gStyle) #copy some of the basics of defualt style...
    LS.SetName("LucaStyle")
    LS.SetTitle("Luca Style")

    # pad
    LS.SetOptStat(0)
    LS.SetTickLength(0.02,"X")
    LS.SetTickLength(0.02,"Y")
    LS.SetPadTickY(1)
    LS.SetPadTickX(1)
    LS.SetPadGridY(1);
    LS.SetPadGridX(1);
    #LS.SetPadTopMargin(0.05)
    #LS.SetPadBottomMargin(0.13)
    LS.SetPadLeftMargin(0.16)
    
    LS.SetCanvasDefH(600)
    LS.SetCanvasDefW(600)

    # axis
    LS.SetTitleYOffset(1.4)
    LS.SetTitleXOffset(0.9)
    LS.SetLabelOffset(0.009, "XYZ")
    LS.SetTitleSize(0.050, "XYZ")

    # legend
    LS.SetLegendBorderSize(0)
    LS.SetLegendFont(62)
    #LS.SetLegendFillStyle(0) #NOT IMPLEMENTED in root

    # if histoType == 0:
    #     LS.SetMarkerStyle(8);
    #     LS.SetMarkerSize(0.8);

    LS.cd() 
    return LS

gROOT.SetBatch(True)

styl = SetLucaStyle()

fMC = TFile.Open("outputForCalibration_MC_24_05_16.root")
fData = TFile.Open("outputForCalibration_2016B-HCollisions_MuTauSkim_02_06_16_TagAndProbe_Type1Mt.root")

tMC = fMC.Get("outTreeForCalibration")
tData = fData.Get("outTreeForCalibration")

hEta_MC = TH1F ("hEta_MC", "hEta_MC", 50, -0.3, 0.3)
hEta_Data = TH1F ("hEta_Data", "hEta_Data", 50, -0.3, 0.3)

hEt_MC = TH1F ("hEt_MC", "hEt_MC", 60, 0, 3)
hEt_Data = TH1F ("hEt_Data", "hEt_Data", 60, 0, 3)

hPhi_MC = TH1F ("hPhi_MC", "hPhi_MC", 50, -0.3, 0.3)
hPhi_Data = TH1F ("hPhi_Data", "hPhi_Data", 50, -0.3, 0.3)

tMC.Draw("L1Tau_eta - OfflineTau_eta >> hEta_MC", "OfflineTau_isMatched == 1")
tData.Draw("L1Tau_eta - OfflineTau_eta >> hEta_Data", "OfflineTau_isMatched == 1")

tMC.Draw("L1Tau_pt / OfflineTau_pt >> hEt_MC", "OfflineTau_isMatched == 1")
tData.Draw("L1Tau_pt / OfflineTau_pt >> hEt_Data", "OfflineTau_isMatched == 1")

tMC.Draw("L1Tau_phi - OfflineTau_phi >> hPhi_MC", "OfflineTau_isMatched == 1")
tData.Draw("L1Tau_phi - OfflineTau_phi >> hPhi_Data", "OfflineTau_isMatched == 1")

leg = TLegend (0.6, 0.6, 0.8, 0.8)
leg.AddEntry(hEta_MC, "MC", "l")
leg.AddEntry(hEta_Data, "2016B-H Data", "l")

c1 = TCanvas("c1", "c1", 600, 600)

hEta_MC.SetLineWidth(2)
hEta_MC.SetLineColor(kBlack)
hEta_Data.SetLineColor(kRed)
hEta_MC.SetTitle(";#eta(L1) - #eta(offline); a.u.")
# hEta_MC.DrawNormalized()
# hEta_Data.DrawNormalized("same")
hEta_Data.DrawNormalized()
leg.Draw()
c1.Print("eta.pdf", "pdf")

hEt_MC.SetLineWidth(2)
hEt_Data.SetLineWidth(2)
hEt_MC.SetLineColor(kBlack)
hEt_Data.SetLineColor(kRed)
hEt_MC.SetTitle(";E_{T}(L1) / E_{T}(offline); a.u.")
hEt_Data.SetTitle(";E_{T}(L1) / E_{T}(offline); a.u.")
hEt_Data.DrawNormalized()
# hEt_MC.DrawNormalized("same")
leg.Draw()
c1.Print("et.pdf", "pdf")

print "MC"   , hEt_MC.GetRMS() / hEt_MC.GetMean()
print "Data" , hEt_Data.GetRMS() / hEt_Data.GetMean()


hPhi_MC.SetLineWidth(2)
hPhi_Data.SetLineWidth(2)
hPhi_MC.SetLineColor(kBlack)
hPhi_Data.SetLineColor(kRed)
hPhi_MC.SetTitle(";#varphi(L1) - #varphi(offline); a.u.")
# hPhi_MC.DrawNormalized()
# hPhi_Data.DrawNormalized("same")
hPhi_Data.DrawNormalized()
leg.Draw()
c1.Print("phi.pdf", "pdf")

c1.Update()
raw_input()
