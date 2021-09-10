from ROOT import *

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
    LS.SetPadBottomMargin(0.13)
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

fIn = TFile ("plotOutData_vsEtaPt_2017C.root")

c1 = TCanvas()

######################################## general for plotiting
leg = TLegend(0.6, 0.65, 0.8, 0.85)
leg.SetFillColor(kWhite)
leg.SetBorderSize(0)
leg.SetTextFont(43)
leg.SetTextSize(20)

cmsTextFont     = 61  # font of the "CMS" label
cmsTextSize   = 0.05  # font size of the "CMS" label
extraTextFont   = 52     # for the "preliminary"
extraTextSize   = 0.76 * cmsTextSize # for the "preliminary"
xpos  = 0.177
ypos  = 0.89

CMSbox       = TLatex  (xpos, ypos         , "CMS")       
extraTextBox = TLatex  (xpos, ypos - 0.05 , "#splitline{preliminary}{2017}")
CMSbox.SetNDC()
extraTextBox.SetNDC()
CMSbox.SetTextSize(cmsTextSize)
CMSbox.SetTextFont(cmsTextFont)
CMSbox.SetTextColor(kBlack)
CMSbox.SetTextAlign(13)
extraTextBox.SetTextSize(extraTextSize)
extraTextBox.SetTextFont(extraTextFont)
extraTextBox.SetTextColor(kBlack)
extraTextBox.SetTextAlign(13)


# lumi_num = float(cfg.readOption ("general::lumi"))
# lumi_num = lumi_num/1000. # from pb-1 to fb-1
# lumi = "%.1f fb^{-1} (13 TeV)" % lumi_num
lumi = "XX.X fb^{-1} (13 TeV)"
#lumi = "12.9 fb^{-1} (13 TeV)"
lumibox = TLatex  (0.9, 0.91, lumi)       
lumibox.SetNDC()
lumibox.SetTextAlign(31)
lumibox.SetTextSize(extraTextSize)
lumibox.SetTextFont(42)
lumibox.SetTextColor(kBlack)


title = "L1T e#gamma"
titlebox = TLatex  (0.26, 0.91, title)
titlebox.SetNDC()
titlebox.SetTextAlign(31)
titlebox.SetTextSize(extraTextSize)
titlebox.SetTextFont(42)
titlebox.SetTextColor(kBlack)


bar = "Barrel"
barbox = TLatex  (0.5, 0.7, bar)
barbox.SetNDC()
barbox.SetTextAlign(31)
barbox.SetTextSize(extraTextSize)
barbox.SetTextFont(42)
barbox.SetTextColor(kBlack)


end = "Endcaps"
endbox = TLatex  (0.83, .7, end)
endbox.SetNDC()
endbox.SetTextAlign(31)
endbox.SetTextSize(extraTextSize)
endbox.SetTextFont(42)
endbox.SetTextColor(kBlack)


######################################## general for plotiting


######## eta plot

hResol_eta = fIn.Get("hResol_eta")
hResol_eta.SetLineWidth(2)
hResol_eta.SetLineColor(kBlue+2)
hResol_eta.SetMarkerColor(kBlue+2)
hResol_eta.SetMarkerStyle(33)
hResol_eta.SetMarkerSize(1.3)
# c1.SetLogx()
# hResol_eta.SetTitle(";|#eta^{offline}|;online #tau E_{T} resolution")
hResol_eta.SetTitle(";|#eta^{e#gamma, offline}|;RMS / <E_{T}^{e#gamma, L1}/p_{T}^{e#gamma, offline}>")
hResol_eta.SetMinimum(0.05)
hResol_eta.SetMaximum(0.25)
hResol_eta.GetXaxis().SetTitleOffset(0.9)
hResol_eta.Draw("pe")

line = TLine(1.479,.05,1.479,.248)
line.SetLineWidth(4)
line.SetLineStyle(7)

line.Draw()
titlebox.Draw()
lumibox.Draw()
CMSbox.Draw()
extraTextBox.Draw()
barbox.Draw()
endbox.Draw()


# hResol_eta.GetXaxis().SetMoreLogLabels(True)
# hResol_eta.GetXaxis().SetNoExponent(True)
c1.Print("resol_vs_eta_2017C.pdf", "pdf")
c1.Print("resol_vs_eta_2017C.png", "png")


######## pt plot -- all
hResol_pt = fIn.Get("hResol_pt")
hResol_pt.SetLineWidth(2)
hResol_pt.SetLineColor(kRed+2)
hResol_pt.SetMarkerColor(kRed+2)
hResol_pt.SetMarkerStyle(33)
hResol_pt.SetMarkerSize(1.3)
c1.SetLogx()
# hResol_pt.SetTitle(";p_{T}^{offline} [GeV];online #tau E_{T} resolution")
hResol_pt.SetTitle(";p_{T}^{e#gamma, offline} [GeV];RMS / <E_{T}^{e#gamma, L1}/p_{T}^{e#gamma, offline}>")
hResol_pt.SetMinimum(0.05)
hResol_pt.SetMaximum(0.40)

#hResol_pt.GetXaxis().SetRange(0.,100.)


hResol_pt.GetXaxis().SetTitleOffset(0.9)
hResol_pt.Draw("pe")
hResol_pt.GetXaxis().SetMoreLogLabels(True)
hResol_pt.GetXaxis().SetNoExponent(True)
lumibox.Draw()
CMSbox.Draw()
extraTextBox.Draw()

c1.Print("resol_vs_pt_2017C.pdf", "pdf")
c1.Print("resol_vs_pt_2017C.png", "png")


######## pt plot -- barr/endc
hResol_pt_barrel = fIn.Get("hResol_pt_barrel")
hResol_pt_endcap = fIn.Get("hResol_pt_endcap")

####### profile endcap #########
prof = fIn.Get("hResol_pt_endcap_2d").ProfileX()
prof.GetXaxis().SetMoreLogLabels(True)
prof.GetXaxis().SetNoExponent(True)
prof.SetLineWidth(2)
prof.SetLineColor(kRed)
prof.SetMarkerColor(kRed)
prof.SetMarkerStyle(8)
prof.SetMarkerSize(0.8)
prof.SetTitle(";p_{T}^{e#gamma, offline} [GeV];scale")

leg = TLegend(0.4, 0.25, 0.45, 0.45)
leg.SetFillColor(kWhite)
leg.SetBorderSize(0)
leg.SetTextFont(43)
leg.SetTextSize(20)
leg.AddEntry(prof, "Endcaps", "lp")
prof.Draw()
leg.Draw()
c1.Print("prof.pdf","pdf")
##############################


hResol_pt_barrel.SetLineWidth(2)
hResol_pt_barrel.SetLineColor(kBlack)
hResol_pt_barrel.SetMarkerColor(kBlack)
hResol_pt_barrel.SetMarkerStyle(8)
hResol_pt_barrel.SetMarkerSize(0.8)

hResol_pt_endcap.SetLineWidth(2)
hResol_pt_endcap.SetLineColor(kRed)
hResol_pt_endcap.SetMarkerColor(kRed)
hResol_pt_endcap.SetMarkerStyle(8)
hResol_pt_endcap.SetMarkerSize(0.8)

c1.SetLogx()
hResol_pt_barrel.SetTitle(";p_{T}^{e#gamma, offline} [GeV];online e#gamma E_{T} resolution")
hResol_pt_barrel.SetTitle(";p_{T}^{e#gamma, offline} [GeV];RMS / <E_{T}^{e#gamma, L1}/p_{T}^{e#gamma, offl}>")
hResol_pt_barrel.SetMinimum(0.05)
hResol_pt_barrel.SetMaximum(0.40)
miin = min (hResol_pt_barrel.GetMinimum(), hResol_pt_endcap.GetMinimum())
maax = max (hResol_pt_barrel.GetMaximum(), hResol_pt_endcap.GetMaximum())
#hResol_pt_barrel.SetMinimum(0.5*miin)
#hResol_pt_barrel.SetMaximum(1.1*maax)
hResol_pt_barrel.GetXaxis().SetTitleOffset(0.9)
hResol_pt_barrel.Draw("pe")
hResol_pt_barrel.GetXaxis().SetMoreLogLabels(True)
hResol_pt_barrel.GetXaxis().SetNoExponent(True)
hResol_pt_endcap.Draw("pe same")
leg = TLegend(0.6, 0.65, 0.8, 0.85)
leg.SetFillColor(kWhite)
leg.SetBorderSize(0)
leg.SetTextFont(43)
leg.SetTextSize(20)
leg.AddEntry(hResol_pt_barrel, "Barrel", "lp")
leg.AddEntry(hResol_pt_endcap, "Endcaps", "lp")
leg.Draw()
titlebox.Draw()
lumibox.Draw()
CMSbox.Draw()
extraTextBox.Draw()
c1.Print("resol_vs_pt_barrendc_2017C.pdf", "pdf")
c1.Print("resol_vs_pt_barrendc_2017C.png", "png")
