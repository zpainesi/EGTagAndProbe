import ROOT
import copy
import TurnOnPlot_DATA as TurnOnPlot


plots = []
inputFileRun3MC = ROOT.TFile.Open("/home/aravind/cernbox/work/l1egamma/histogramFiles/mc2022_resolutionHists.root")
inputFile2018   = ROOT.TFile.Open("/home/aravind/cernbox/work/l1egamma/histogramFiles/data2018_resolutionHists.root")

## Barrel

plots.append(TurnOnPlot.TurnOnPlot())
plots[-1].name = "Resolution_PhiBarrel"
plots[-1].yRange = (0.0,0.14)
plots[-1].xRange = (-0.2,0.2)
plots[-1].yTitle = "a.u"
plots[-1].xTitle = "#varphi^{eg,L1} - #varphi^{eg,offl}"
plots[-1].desc=["L1 EG","#varphi Resolution Barrel"]
plots[-1].legendPosition = (0.65,0.55,0.92,0.65)
plots[-1].descPosition   = (0.65,0.80)
plots[-1].logx = False

histo0 = inputFileRun3MC.Get("PhiVsdPhiBareResBarrel")
histo0.__class__ = ROOT.TH1D
turnon0 = TurnOnPlot.TurnOn(Name="Stage2", Histo=histo0,
                            MarkerColor=ROOT.kBlue+2, MarkerStyle=22, LineColor=ROOT.kBlue+2,LineStyle=1,
                            Legend="MC Run3")

histo1 = inputFile2018.Get("PhiVsdPhiBareResBarrel")
histo1.__class__ = ROOT.TH1D
turnon1 = TurnOnPlot.TurnOn(Name="Stage2", Histo=histo1,
                            MarkerColor=ROOT.kRed+2, MarkerStyle=22, LineColor=ROOT.kRed+2,LineStyle=1,
                            Legend="Data 2018")
plots[-1].addTurnOn(turnon0)
plots[-1].addTurnOn(turnon1)

##  EndCap

plots.append(copy.deepcopy(plots[-1]))
plots[-1].clearTurnOns()
plots[-1].name = "Resolution_PhiEndcap"
plots[-1].yRange = (0.0,0.14)
plots[-1].desc=["L1 EG","#varphi Resolution Endcap"]
plots[-1].legendPosition = (0.65,0.55,0.92,0.65)
plots[-1].descPosition   = (0.65,0.80)

histo0 = inputFileRun3MC.Get("PhiVsdPhiBareResECap")
histo0.__class__ = ROOT.TH1D
turnon0 = TurnOnPlot.TurnOn(Name="Stage2", Histo=histo0,
                            MarkerColor=ROOT.kBlue+2, MarkerStyle=22, LineColor=ROOT.kBlue+2,LineStyle=1,
                            Legend="MC Run3")

histo1 = inputFile2018.Get("PhiVsdPhiBareResECap")
histo1.__class__ = ROOT.TH1D
turnon1 = TurnOnPlot.TurnOn(Name="Stage2", Histo=histo1,
                            MarkerColor=ROOT.kRed+2, MarkerStyle=22, LineColor=ROOT.kRed+2,LineStyle=1,
                            Legend="Data 2018")
plots[-1].addTurnOn(turnon0)
plots[-1].addTurnOn(turnon1)


## Inclusive
plots.append(copy.deepcopy(plots[-1]))
plots[-1].clearTurnOns()
plots[-1].name = "Resolution_PhiInclusive"
plots[-1].yRange = (0.0,0.14)
plots[-1].desc=["L1 EG","#varphi Resolution"]
plots[-1].legendPosition = (0.65,0.55,0.92,0.65)
plots[-1].descPosition   = (0.65,0.80)

histo0 = inputFileRun3MC.Get("PhiVsdPhiBareResInclusive")
histo0.__class__ = ROOT.TH1D
turnon0 = TurnOnPlot.TurnOn(Name="Stage2", Histo=histo0,
                            MarkerColor=ROOT.kBlue+2, MarkerStyle=22, LineColor=ROOT.kBlue+2,LineStyle=1,
                            Legend="MC Run3")

histo1 = inputFile2018.Get("PhiVsdPhiBareResInclusive")
histo1.__class__ = ROOT.TH1D
turnon1 = TurnOnPlot.TurnOn(Name="Stage2", Histo=histo1,
                            MarkerColor=ROOT.kRed+2, MarkerStyle=22, LineColor=ROOT.kRed+2,LineStyle=1,
                            Legend="Data 2018")
plots[-1].addTurnOn(turnon0)
plots[-1].addTurnOn(turnon1)




canvas = []
for plot in plots:
    canvas.append(plot.plot())


inputFileRun3MC.Close()
inputFile2018.Close()

