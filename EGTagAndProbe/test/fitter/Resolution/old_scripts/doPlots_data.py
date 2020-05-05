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

    LS.cd() 
    return LS

gROOT.SetBatch(True)
TH1.SetDefaultSumw2();

styl = SetLucaStyle()

fData = TFile.Open("/eos/cms/store/group/dpg_trigger/comm_trigger/L1Trigger/ckoraka/NTuplesFor2018PerformancePlots/data_2018/reducedTrees/EGamma_Run2018ABCD_reduced.root")
tData = fData.Get("Ntuplizer/TagAndProbe")

fOut = TFile("plotOutData_charis.root", "recreate")


# all
hEta_Data = TH1F ("hEta_Data", "hEta_Data", 49, -0.3, 0.3)
hEt_Data = TH1F ("hEt_Data", "hEt_Data", 160, 0.2, 1.8)
hPhi_Data = TH1F ("hPhi_Data", "hPhi_Data", 49, -0.3, 0.3)

# barrel
hEta_Data_barrel = TH1F ("hEta_Data_barrel", "hEta_Data_barrel", 49, -0.3, 0.3)
hEt_Data_barrel = TH1F ("hEt_Data_barrel", "hEt_Data_barrel", 100, 0.5, 1.5)
hPhi_Data_barrel = TH1F ("hPhi_Data_barrel", "hPhi_Data_barrel", 49, -0.3, 0.3)

#endcap
hEta_Data_endcap = TH1F ("hEta_Data_endcap", "hEta_Data_endcap", 49, -0.3, 0.3)
hEt_Data_endcap = TH1F ("hEt_Data_endcap", "hEt_Data_endcap", 100, 0.5, 1.5)
hPhi_Data_endcap = TH1F ("hPhi_Data_endcap", "hPhi_Data_endcap", 49, -0.3, 0.3)

tData.Draw("l1tEta - eleProbeEta >> hEta_Data", "TMath::Abs(eleProbeEta) < 2.5 && isProbeLoose")
tData.Draw("l1tPt / eleProbeSclEt >> hEt_Data", "TMath::Abs(eleProbeEta) < 2.5 && isProbeLoose")
tData.Draw("l1tPhi - eleProbePhi >> hPhi_Data", "TMath::Abs(eleProbeEta) < 2.5 && isProbeLoose")

tData.Draw("l1tEta - eleProbeEta >> hEta_Data_barrel", "TMath::Abs(eleProbeEta) < 1.305 && isProbeLoose")
tData.Draw("l1tPt / eleProbeSclEt >> hEt_Data_barrel", "TMath::Abs(eleProbeEta) < 1.305 && isProbeLoose")
tData.Draw("l1tPhi - eleProbePhi >> hPhi_Data_barrel", "TMath::Abs(eleProbeEta) < 1.305 && isProbeLoose")

tData.Draw("l1tPt / eleProbeSclEt >> hEt_Data_endcap"  , "(TMath::Abs(eleProbeEta) > 1.479) && (TMath::Abs(eleProbeEta) < 2.5) && isProbeLoose")
tData.Draw("l1tEta - eleProbeEta  >> hEta_Data_endcap" , "(TMath::Abs(eleProbeEta) > 1.479) && (TMath::Abs(eleProbeEta) < 2.5) && isProbeLoose")
tData.Draw("l1tPhi - eleProbePhi  >> hPhi_Data_endcap" , "(TMath::Abs(eleProbeEta) > 1.479) && (TMath::Abs(eleProbeEta) < 2.5) && isProbeLoose")


fOut.Write()
