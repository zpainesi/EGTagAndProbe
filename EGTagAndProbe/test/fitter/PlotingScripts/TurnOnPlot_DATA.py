import ROOT 
import operator 
import array


#ROOT.gSystem.Load('libRooFit')


class TurnOn:
    def __init__(self, **args):
        self.name        = args.get("Name", "turnon")
        #self.legend      = args.get("Legend","")
        self.legend      = args.get("Legend","Turn-on")
        self.histo       = args.get("Histo", None)
        #self.fit         = args.get("Fit", None)
        self.markerColor = args.get("MarkerColor", ROOT.kBlack)
        self.markerStyle = args.get("MarkerStyle", 20)
        self.lineColor   = args.get("LineColor", ROOT.kBlack)
        self.lineStyle   = args.get("LineStyle", 1)
        if self.histo : self.histo.SetName(self.name+"_histo")
        #self.fit.SetName(self.name+"_fit")



class TurnOnPlot:
    def __init__(self, **args):
        self.canvas = None
        self.name  = ""
        self.turnons = []
        self.plotDir = "plots"
        #self.xRange = (10, 120)
        self.yRange = (0.0, 1.0)
        self.xRange = (0, 100)
        #self.xTitle = "p_{T}^{offl} [GeV]"
        self.xTitle = args.get("xTitle","E_{T}^{e offl} [GeV]")
        self.yTitle = args.get("yTitle","Efficiency")
        #self.xTitle = "Number of Vertices"
	    #self.legendPosition = (0.6,0.2,0.9,0.4)
        self.legendPosition = (0.4,0.2,0.9,0.6)
        self.descPosition = (0.6,0.58)
        self.setPlotStyle()
        #self.triggerName = args.get("TriggerName", "Turn-On")
        self.triggerName = args.get("TriggerName", "")
        self.desc = args.get("desc",["L1 Efficiency"])
        self.logx = args.get("logx",True)
        self.logy = args.get("logx",False)

    def addTurnOn(self, turnon):
        self.turnons.append(turnon)
    def clearTurnOns(self):
        self.turnons=[]

    def setupPlotCanvas(self):
        if self.canvas==None:
            self.canvas = ROOT.TCanvas("c_"+self.name, self.name, 800, 800)
            print("@setupPlotCanvas Making new canvas : ",self.canvas.GetName())
            self.canvas.SetGrid()
        else:
            print("@setupPlotCanvas using passed one ",canvas.GetName())
            self.canvas.cd()
       
        if self.logx : self.canvas.SetLogx()
        if self.logy : self.canvas.SetLogy()

        cmsTextFont     = 61  # font of the "CMS" label
        cmsTextSize     = 0.05  # font size of the "CMS" label
        extraTextFont   = 52     # for the "preliminary"
        extraTextSize   = 0.76 * cmsTextSize # for the "preliminary"
        #xpos  = 0.2
        #ypos  = 0.9 
        #xpos  = 0.2
        #ypos  = 0.8
        xpos  = 0.15   
        ypos  = 0.98

        self.CMSbox       = ROOT.TLatex  (xpos, ypos+0.004, "CMS")
        #self.self.extraTextBox = ROOT.TLatex  (xpos, ypos - 0.05 , "#splitline{preliminary}{2018}")
        self.extraTextBox = ROOT.TLatex  (xpos + 0.12, ypos-0.002, "preliminary 2018")
        self.CMSbox.SetNDC()
        self.extraTextBox.SetNDC()
        self.CMSbox.SetTextSize(cmsTextSize)
        self.CMSbox.SetTextFont(cmsTextFont)
        self.CMSbox.SetTextColor(ROOT.kBlack)
        self.CMSbox.SetTextAlign(13)
        self.extraTextBox.SetTextSize(extraTextSize)
        self.extraTextBox.SetTextFont(extraTextFont)
        self.extraTextBox.SetTextColor(ROOT.kBlack)
        self.extraTextBox.SetTextAlign(13)
        self.selbox=[]
        for n in range(len(self.desc)):
            self.selbox.append(ROOT.TLatex  (self.descPosition[0], self.descPosition[1] -n*0.04, self.desc[n]))
            self.selbox[-1].SetNDC()
            self.selbox[-1].SetTextSize(0.035)
            self.selbox[-1].SetTextFont(12)
            self.selbox[-1].SetTextAlign(13)

        self.triggerNameBox = ROOT.TLatex(0.15, 0.95, self.triggerName)
        self.triggerNameBox.SetNDC()
        self.triggerNameBox.SetTextFont(42)
        self.triggerNameBox.SetTextSize(extraTextSize)
        self.triggerNameBox.SetTextColor(ROOT.kBlack)
        self.triggerNameBox.SetTextAlign(11)

        # lumi_num = float(cfg.readOption ("general::lumi"))
        # lumi_num = lumi_num/1000. # from pb-1 to fb-1
        # lumi = "%.1f fb^{-1} (13 TeV)" % lumi_num
        lumi = "58.8 fb^{-1} (13 TeV)"
        #lumi = "RunA : RunNumber > 316700,   2.6 fb^{-1} (13 TeV)"
        #lumi = ""
        self.lumibox = ROOT.TLatex  (0.953, 0.95, lumi)
        self.lumibox.SetNDC()
        self.lumibox.SetTextAlign(31)
        self.lumibox.SetTextSize(extraTextSize)
        self.lumibox.SetTextFont(42)
        self.lumibox.SetTextColor(ROOT.kBlack)
        #Line legend
        self.legend = ROOT.TLegend(self.legendPosition[0],self.legendPosition[1],self.legendPosition[2],self.legendPosition[3])
        self.legend.SetTextFont(42)
        self.legend.SetFillColor(0)
        self.canvas.Modified()
        self.canvas.Update()

    def savePlot(self,prefix=""):
        self.canvas.Print(self.plotDir+"/"+prefix+self.name+".png", "png")

    def plot(self,savePic=True,canvas=False):
        if self.canvas==None:
            self.setupPlotCanvas()
            self.canvas.cd()
            canvas=self.canvas
        else:
            print("using this canvas ",canvas.GetName())
            #self.setupPlotCanvas(canvas)
        canvas.cd()

        hDummy = ROOT.TH1F("hDummy_"+self.name, self.name, 1, self.xRange[0], self.xRange[1])
        hDummy.SetAxisRange(self.yRange[0], self.yRange[1], "Y")
        hDummy.SetXTitle(self.xTitle)
        hDummy.SetYTitle(self.yTitle)
        #hDummy.SetTitleSize(0,"X")
        hDummy.Draw()
       
        for turnon in self.turnons:
            print("printing : ",turnon.histo.GetName())
            histo = turnon.histo
            histo.SetMarkerStyle(turnon.markerStyle)
            histo.SetMarkerColor(turnon.markerColor)
            histo.SetLineColor(turnon.markerColor)
            histo.SetAxisRange(self.xRange[0],self.xRange[1],'X')
            histo.SetTitleSize(0,"X")
         #   histo.SetYTitle(self.yTitle)
            #fit = turnon.fit
            #fit.SetLineStyle(turnon.lineStyle)
            #fit.SetLineColor(turnon.lineColor)
            #fit.SetLineWidth(2)
            histo.Draw("ep sames")
            #fit.Draw("l same")
            #legends
            self.legend.AddEntry(histo, turnon.legend, "pe")
            self.legend.SetTextSize(0.025) #charis
            #if self.name=="turnon_Stage1_Stage2_EB":
       
        self.legend.Draw()
        self.triggerNameBox.Draw()
        self.CMSbox.Draw()
        self.extraTextBox.Draw()
        self.lumibox.Draw()
        
        
        for selb in self.selbox:
            selb.Draw()
        canvas.Modified()
        self.canvas.Update()
        if savePic:
            self.savePlot()
        
        return canvas


    def setPlotStyle(self):
        ROOT.gROOT.SetStyle("Plain")
        ROOT.gStyle.SetOptStat()
        ROOT.gStyle.SetOptFit(0)
        ROOT.gStyle.SetOptTitle(0)
        ROOT.gStyle.SetFrameLineWidth(1)
        ROOT.gStyle.SetPadBottomMargin(0.13)
        ROOT.gStyle.SetPadLeftMargin(0.15)
        ROOT.gStyle.SetPadTopMargin(0.06)
        ROOT.gStyle.SetPadRightMargin(0.05)

        ROOT.gStyle.SetLabelFont(42,"Y")
        ROOT.gStyle.SetLabelSize(0.04,"X")
        ROOT.gStyle.SetLabelSize(0.04,"Y")
        ROOT.gStyle.SetLabelOffset(0.01,"Y")
        ROOT.gStyle.SetTickLength(0.02,"X")
        ROOT.gStyle.SetTickLength(0.02,"Y")
        ROOT.gStyle.SetLineWidth(1)
        ROOT.gStyle.SetTickLength(0.02 ,"Z")

        ROOT.gStyle.SetTitleSize(0.1)
        ROOT.gStyle.SetTitleFont(42,"X")
        ROOT.gStyle.SetTitleFont(42,"Y")
        ROOT.gStyle.SetTitleSize(0.05,"X")
        ROOT.gStyle.SetTitleSize(0.05,"Y")
        ROOT.gStyle.SetTitleOffset(1.1,"X")
        ROOT.gStyle.SetTitleOffset(1.4,"Y")
        ROOT.gStyle.SetOptStat(0)
        ROOT.gStyle.SetPalette(1)
        ROOT.gStyle.SetPaintTextFormat("3.2f")
        ROOT.gROOT.ForceStyle()

class RatioContainer:
    def __init__(self, **args):
        self.name        = args.get("Name", "ratio")
        #self.legend      = args.get("Legend","")
        self.legend      = args.get("Legend","Turn-on")
        self.turnOnN       = args.get("TurnOnNum", None)
        self.turnOnD       = args.get("TurnOnDen", None)
        self.ratioH       = args.get("HistRatio", None)
        #self.fit         = args.get("Fit", None)
        self.markerColor = args.get("MarkerColor", ROOT.kBlack)
        self.markerStyle = args.get("MarkerStyle", 20)
        self.lineColor   = args.get("LineColor", ROOT.kBlack)
        self.lineStyle   = args.get("LineStyle", 1)
        #self.fit.SetName(self.name+"_fit")


class RatioPlot(TurnOnPlot):
    def __init__(self,**args):
        self.ratioContainers=[]
        self.yRatioRange = (0.50,1.50)
        self.rlegendPosition = (0.7,0.7,0.84,0.9)
        TurnOnPlot.__init__(self,**args)
    
    def addRatioPlot(self,ratioContainer,addTurnons=True):
        if addTurnons:
            self.addTurnOn(ratioContainer.turnOnN)
            self.addTurnOn(ratioContainer.turnOnD)
        print(ratioContainer.turnOnN,ratioContainer.turnOnD)
        h1=ratioContainer.turnOnN.histo
        h2=ratioContainer.turnOnD.histo
        
        rHist=h1.Clone()
        rHist.SetName(ratioContainer.name)
        for i in range(1,rHist.GetNbinsX() + 1 ):
            numera=h1.GetBinContent(i)            
            denomi=h2.GetBinContent(i)
            ratio=0.0
            if denomi!=0:
                ratio=numera/denomi
            else :
                ratio = numera/(denomi + 1e-6)
                print("for ",h2.GetBinCenter(i),"deno = ",h2.GetBinContent(i),"  setting ratio as : ",ratio)
            rHist.SetBinContent(i,ratio)
        ratioContainer.ratioH=rHist
        ratioContainer.ratioH.SetName(self.name+"_histo")

        self.ratioContainers.append(ratioContainer)
    
    def plot(self,savePic=True):
        self.setupPlotCanvas()
        self.canvas.cd()
        pad2 = ROOT.TPad("pad2", "pad2", 0, 0.0, 1, 0.3)
        pad2.SetTopMargin(0)
        pad2.SetBottomMargin(0.2)
        pad2.SetGrid() #// vertical grid
        if self.logx : pad2.SetLogx()
        if self.logy : pad2.SetLogy()
        pad2.Draw()
        pad2.cd()
        
        ##self.savePlot()
        ##return self.canvas
        
        #Line legend
        self.rlegend = ROOT.TLegend(self.rlegendPosition[0],self.rlegendPosition[1],self.rlegendPosition[2],self.rlegendPosition[3])
        self.rlegend.SetTextFont(42)
        self.rlegend.SetFillColor(0)

        hDummy = ROOT.TH1F("hDummyR_"+self.name, self.name, 1, self.xRange[0], self.xRange[1])
        hDummy.SetAxisRange(self.yRatioRange[0], self.yRatioRange[1], "Y")
        hDummy.SetXTitle(self.xTitle)
        hDummy.SetTitleOffset(0.5,"X")
        hDummy.SetTitleSize(0.1)
        hDummy.SetLabelFont(42,"X")
        hDummy.SetLabelSize(0.1,"X")
        hDummy.SetNdivisions(5,"Y")
        #hDummy.SetYTitle(self.yTitle)
        hDummy.Draw()
        for rC in self.ratioContainers:
            histo= rC.ratioH
            print("Printing ratio : ",histo.GetName())
            histo.SetMarkerStyle(rC.markerStyle)
            histo.SetAxisRange(self.yRatioRange[0], self.yRatioRange[1], "Y")
            histo.SetMarkerColor(rC.markerColor)
            histo.SetLineColor(rC.markerColor)
            histo.SetAxisRange(self.xRange[0],self.xRange[1],'X')
            histo.Draw("epl sames")
            self.rlegend.AddEntry(histo, rC.legend, "pe")
            self.rlegend.SetTextSize(0.08) #charis
        self.rlegend.Draw()
        pad2.Modified()
        self.canvas.cd()
        self.canvas.Update()
        self.canvas.cd()
        self.pad1 = ROOT.TPad("pad1", "pad1", 0, 0.3, 1, 1.0)
        self.pad1.SetBottomMargin(0)

        if self.logx : self.pad1.SetLogx()
        if self.logy : self.pad1.SetLogy()
        self.pad1.Draw()             
        self.pad1.cd()
        self.pad1.SetGrid()  
        self.pad1.Modified()       
        #self.pad1=TurnOnPlot.plot(self,True,self.pad1)
        TurnOnPlot.plot(self,True,self.pad1)
        self.pad1.SetGrid()  
        self.pad1.Modified()
        self.pad1.Draw()
        self.canvas.SetGrid()
        self.canvas.Update()

        # lower plot will be in pad
        self.canvas.cd()

        return self.canvas

