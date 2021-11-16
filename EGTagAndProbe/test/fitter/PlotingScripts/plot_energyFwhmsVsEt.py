import ROOT
import copy
import TurnOnPlot_DATA as TurnOnPlot


plots = []
inputFile2018   = ROOT.TFile.Open("/home/aravind/cernbox/work/l1egamma/histogramFiles/data2018_resolutionHists.root")
inputFile2017   = ROOT.TFile.Open("/home/aravind/cernbox/work/l1egamma/histogramFiles/data2017_resolutionHists.root")

## Barrel

plots.append(TurnOnPlot.TurnOnPlot())
plots[-1].name = "Resolution2018_EtFWHMSvsEt"
plots[-1].yRange = (0.0,0.65)
plots[-1].xRange = (5.2,120.0)
plots[-1].yTitle = "FWHM / (E_{T}^{eg,L1}/E_{T}^{eg,offl})"
plots[-1].xTitle = "E_{T}^{eg,offl}"
plots[-1].desc=["L1 EG","E_{T} Resolution"]
plots[-1].legendPosition = (0.65,0.55,0.92,0.68)
plots[-1].descPosition   = (0.65,0.80)
plots[-1].logx = True

histo0 = inputFile2018.Get("EtVsdEtResBarrel")
histo0.__class__ = ROOT.TH1D
turnon0 = TurnOnPlot.TurnOn(Name="Stage2", Histo=histo0,
                            MarkerColor=ROOT.kBlue, MarkerStyle=22, LineColor=ROOT.kBlue+2,LineStyle=1,
                            Legend="2018-Barrel")

histo1 = inputFile2018.Get("EtVsdEtResECap")
histo1.__class__ = ROOT.TH1D
turnon1 = TurnOnPlot.TurnOn(Name="Stage2", Histo=histo1,
                            MarkerColor=ROOT.kGreen+2, MarkerStyle=21, LineColor=ROOT.kRed+2,LineStyle=1,
                            Legend="2018-Endcap")
plots[-1].addTurnOn(turnon0)
plots[-1].addTurnOn(turnon1)

histo0 = inputFile2017.Get("EtVsdEtResBarrel")
histo0.__class__ = ROOT.TH1D
turnon0 = TurnOnPlot.TurnOn(Name="Stage2", Histo=histo0,
                            MarkerColor=ROOT.kMagenta+2, MarkerStyle=22, LineColor=ROOT.kBlue+2,LineStyle=1,
                            Legend="2017-Barrel")

histo1 = inputFile2017.Get("EtVsdEtResECap")
histo1.__class__ = ROOT.TH1D
turnon1 = TurnOnPlot.TurnOn(Name="Stage2", Histo=histo1,
                            MarkerColor=ROOT.kRed+2, MarkerStyle=21, LineColor=ROOT.kRed+2,LineStyle=1,
                            Legend="2017-Endcap")
plots[-1].addTurnOn(turnon0)
plots[-1].addTurnOn(turnon1)

canvas = []
for plot in plots:
    canvas.append(plot.plot())


inputFile2017.Close()
inputFile2018.Close()

