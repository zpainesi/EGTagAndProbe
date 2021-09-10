import ROOT
import TurnOnPlot_DATA as TurnOnPlot


plots = []
inputFile2017   = ROOT.TFile.Open("/home/aravind/cernbox/work/l1egamma/histogramFiles/data2017_turnonHists.root")
inputFile2018   = ROOT.TFile.Open("/home/aravind/cernbox/work/l1egamma/histogramFiles/data2018_turnonHists.root")

plots.append(TurnOnPlot.RatioPlot())
plots[-1].name = "TurnOn_SingleEGNoIso3040Ratio"
plots[-1].xRange = (5.0,999.9)
plots[-1].desc=["L1 EG","#||{#eta^{offl}}<2.5 ","L1 E_{T} > 30 GeV"]
plots[-1].legendPosition = (0.60,0.25,0.92,0.40)
plots[-1].descPosition =(0.6,0.58)

histo0 = inputFile2017.Get("hist_L1_30Efficiency")
histo0.__class__ = ROOT.TH1D
turnon0 = TurnOnPlot.TurnOn(Name="Stage2", Histo=histo0,
                            MarkerColor=ROOT.kRed, MarkerStyle=21, LineColor=ROOT.kRed,LineStyle=1,
                            Legend="2017 , L1 E_{T} > 30")

histo1 = inputFile2018.Get("hist_L1_30Efficiency")
histo1.__class__ = ROOT.TH1D
turnon1 = TurnOnPlot.TurnOn(Name="Stage2", Histo=histo1,
                            MarkerColor=ROOT.kRed+2, MarkerStyle=22, LineColor=ROOT.kRed+2,LineStyle=1,
                            Legend="2018 , L1 E_{T} > 30")

rp = TurnOnPlot.RatioContainer(Name="rp1",TurnOnNum=turnon1 ,TurnOnDen= turnon0,
                               MarkerColor=ROOT.kRed, MarkerStyle=22, LineColor=ROOT.kRed,LineStyle=1,
                               Legend="L1 E_{T} > 30")
plots[0].addRatioPlot(rp,True)


histo0 = inputFile2017.Get("hist_L1_40Efficiency")
histo0.__class__ = ROOT.TH1D
turnon0 = TurnOnPlot.TurnOn(Name="Stage2", Histo=histo0,
                            MarkerColor=ROOT.kBlue, MarkerStyle=21, LineColor=ROOT.kBlue+2,LineStyle=1,
                            Legend="2017 , L1 E_{T} > 40 ")

histo1 = inputFile2018.Get("hist_L1_40Efficiency")
histo1.__class__ = ROOT.TH1D
turnon1 = TurnOnPlot.TurnOn(Name="Stage2", Histo=histo1,
                            MarkerColor=ROOT.kBlue+2, MarkerStyle=22, LineColor=ROOT.kRed+2,LineStyle=1,
                            Legend="2018 , L1 E_{T} > 40")

rp = TurnOnPlot.RatioContainer(Name="rp1",TurnOnNum=turnon1 ,TurnOnDen= turnon0,
                               MarkerColor=ROOT.kBlue, MarkerStyle=22, LineColor=ROOT.kBlue,LineStyle=1,
                               Legend="L1 E_{T} > 40")
plots[0].addRatioPlot(rp,True)

canvas = []
for plot in plots:
    canvas.append(plot.plot())


inputFile2017.Close()
inputFile2018.Close()

