import ROOT
import copy
import TurnOnPlot_DATA as TurnOnPlot


plots = []
inputFile2018   = ROOT.TFile.Open("/home/aravind/cernbox/work/l1egamma/histogramFiles/data2018_resolutionHists.root")

## Barrel

plots.append(TurnOnPlot.TurnOnPlot())
plots[-1].name = "Resolution2018_Eta"
plots[-1].yRange = (0.0,0.18)
plots[-1].xRange = (-0.3,0.3)
plots[-1].yTitle = "a.u"
plots[-1].xTitle = "#eta^{eg,L1}-#eta^{eg,offl}"
plots[-1].desc=["L1 EG","#eta Resolution"]
plots[-1].legendPosition = (0.65,0.55,0.92,0.65)
plots[-1].descPosition   = (0.65,0.80)
plots[-1].logx = False

histo0 = inputFile2018.Get("EtaVsdEtaBareResBarrel")
histo0.__class__ = ROOT.TH1D
turnon0 = TurnOnPlot.TurnOn(Name="Stage2", Histo=histo0,
                            MarkerColor=ROOT.kBlue+2, MarkerStyle=22, LineColor=ROOT.kBlue+2,LineStyle=1,
                            Legend="Barrel")

histo1 = inputFile2018.Get("EtaVsdEtaBareResECap")
histo1.__class__ = ROOT.TH1D
turnon1 = TurnOnPlot.TurnOn(Name="Stage2", Histo=histo1,
                            MarkerColor=ROOT.kRed+2, MarkerStyle=22, LineColor=ROOT.kRed+2,LineStyle=1,
                            Legend="Endcap")
plots[-1].addTurnOn(turnon0)
plots[-1].addTurnOn(turnon1)
canvas = []
for plot in plots:
    canvas.append(plot.plot())


inputFile2018.Close()

