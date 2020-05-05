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
    
    LS.SetCanvasDefH(800)
    LS.SetCanvasDefW(800)

    # axis
    LS.SetTitleYOffset(1.4)
    LS.SetTitleXOffset(0.9)
    LS.SetLabelOffset(0.009, "XYZ")
    LS.SetTitleSize(0.050, "XYZ")

    # legend
    LS.SetLegendBorderSize(0)
    LS.SetLegendFont(62)
    #LS.SetLegendFillStyle(0) #NOT IMPLEMENTED in root

    LS.cd() 
    return LS


#############################################

SetLucaStyle()

fIn = TFile ("plotOutData_charis_fit.root")
#varNames = ["Et"]
varNames = ["Et", "Eta", "Phi"]

titles = {
    "Et" : ";E_{T}^{e#gamma, L1} / E_{T}^{e#gamma, offline}; a.u.",
    "Eta" : ";#eta^{e#gamma, L1} - #eta^{e#gamma, offline}; a.u.",
    "Phi" : ";#varphi^{e#gamma, L1} - #varphi^{e#gamma, offline}; a.u.",
}

c1 = TCanvas ("c1", "c1", 800, 800)

leg = TLegend(0.6, 0.65, 0.8, 0.8)
leg.SetFillColor(kWhite)
leg.SetBorderSize(0)
leg.SetTextFont(43)
leg.SetTextSize(20)

cmsTextFont     = 61  # font of the "CMS" label
cmsTextSize   = 0.05  # font size of the "CMS" label
extraTextFont   = 52     # for the "preliminary"
extraTextSize   = 0.76 * cmsTextSize # for the "preliminary"
#xpos  = 0.177
#ypos  = 0.89
xpos  = 0.18
ypos  = 0.94

#CMSbox       = TLatex  (xpos, ypos         , "CMS")       
#extraTextBox = TLatex  (xpos, ypos - 0.05 , "#splitline{preliminary}{2018}")

CMSbox       = TLatex  (xpos, ypos, "CMS")
extraTextBox = TLatex  (xpos + 0.12, ypos, "preliminary 2018")


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
lumi = "58.8 fb^{-1} (13 TeV)"
#lumi = "XX.X fb^{-1} (13 TeV)"
lumibox = TLatex  (0.9, 0.91, lumi)       
lumibox.SetNDC()
lumibox.SetTextAlign(31)
lumibox.SetTextSize(extraTextSize)
lumibox.SetTextFont(42)
lumibox.SetTextColor(kBlack)

#title = "L1T e#gamma"
#titlebox = TLatex  (0.245, 0.91, title)       
#titlebox.SetNDC()
#titlebox.SetTextAlign(31)
#titlebox.SetTextSize(extraTextSize)
#titlebox.SetTextFont(42)
#titlebox.SetTextColor(kBlack)


first = True
for varName in varNames:

    hbarr = fIn.Get("h%s_Data_barrel" % varName)
    hendc = fIn.Get("h%s_Data_endcap" % varName)

    hbarr.Sumw2()
    hendc.Sumw2()

    fitbart = None
    firendc = None
    if varName == "Eta" or varName == "Phi":
        fitbarr = hbarr.GetFunction("CBFunc");
        fitendc = hendc.GetFunction("CBFunc");
    if varName == "Et":
        fitbarr = hbarr.GetFunction("CBFuncAsymm");
        fitendc = hendc.GetFunction("CBFuncAsymm");
    
    hbarr.SetLineColor(kBlue)
    hbarr.SetMarkerColor(kBlue)
    hbarr.SetMarkerSize(0.8)
    hbarr.SetMarkerStyle(21)
    fitbarr.SetLineColor(kBlue)
    fitbarr.SetLineWidth(2)
    fitbarr.SetNpx(1000)
    fitendc.SetNpx(1000)

    fitbarr.SetBit(TF1.kNotDraw)
    fitendc.SetBit(TF1.kNotDraw)

    # get histos of the function to scale at the same integral
    htempbarr = fitbarr.GetHistogram().Clone("tempbarr_%s" % varName);
    htempendc = fitendc.GetHistogram().Clone("tempendc_%s" % varName);
    binscale = htempbarr.GetNbinsX()/hbarr.GetNbinsX()
    # print binscale

    htempbarr.Scale(1.*binscale/htempbarr.Integral())
    htempendc.Scale(1.*binscale/htempendc.Integral())

    hendc.SetLineColor(kGreen+3)
    hendc.SetMarkerColor(kGreen+3)
    hendc.SetMarkerSize(0.8)
    hendc.SetMarkerStyle(22)
    fitendc.SetLineColor(kGreen+3)
    fitendc.SetLineWidth(2)

    mm = max (hbarr.GetMaximum(), hendc.GetMaximum())
    hbarr.SetMaximum(1.15*mm)
    hbarr.SetMinimum(0)
    if varName in titles:
        hbarr.SetTitle(titles[varName])

    if first:
        first = False
        leg.AddEntry(hbarr, "Barrel",  "lp")        
        leg.AddEntry(hendc, "Endcaps", "lp")        

    #line = TLine(1,0,1,1.15*mm)
    line = TLine(0,0,0,1.15*mm)
    line.SetLineWidth(4)
    line.SetLineStyle(7)
    hbarr.GetXaxis().SetTitleOffset(1.1)
    #print hendc.GetBinError(10)
    hbarr.Draw("hist p")
    #line.Draw("same")
    hendc.Draw("hist p same")
    #htempbarr.Draw("l same")#was here
    #htempendc.Draw("l same")#was here

    fitbarr.Draw("l same")
    fitendc.Draw("l same")

    leg.Draw()
    lumibox.Draw()
    CMSbox.Draw()
    extraTextBox.Draw()
    #titlebox.Draw()    

    c1.Print("plot_" + varName + "_2018.pdf", "pdf")
    c1.Print("plot_" + varName + "_2018.png", "png")
    c1.Print("plot_" + varName + "_2018.root", "root")

raw_input()

