import ROOT
import copy
import TurnOnPlot_DATA as TurnOnPlot


plots = []
inputFile2018   = ROOT.TFile.Open("/home/aravind/cernbox/work/l1egamma/histogramFiles/data2018_resolutionHists.root")
inputFile2017   = ROOT.TFile.Open("/home/aravind/cernbox/work/l1egamma/histogramFiles/data2017_resolutionHists.root")

## Inclusive

plots.append(TurnOnPlot.TurnOnPlot())
plots[-1].name = "Resolution2018_EtFWHMSvsEta"
plots[-1].yRange = (0.0,0.30)
plots[-1].xRange = (0.0,2.4)
plots[-1].yTitle = "FWHM / (E_{T}^{eg,L1}/E_{T}^{eg,offl})"
plots[-1].xTitle = "|#eta^{eg,offl}|"
plots[-1].desc=["L1 EG","E_{T} Resolution"]
plots[-1].legendPosition = (0.25,0.55,0.38,0.66)
plots[-1].descPosition   = (0.25,0.80)
plots[-1].logx = False

histo0 = inputFile2018.Get("EtaVsdEtResInclusive")
histo0.__class__ = ROOT.TH1D
turnon0 = TurnOnPlot.TurnOn(Name="Stage2", Histo=histo0,
                            MarkerColor=ROOT.kBlue, MarkerStyle=22, LineColor=ROOT.kBlue+2,LineStyle=1,
                            Legend="2018")


histo1 = inputFile2017.Get("EtaVsdEtResInclusive")
histo1.__class__ = ROOT.TH1D
turnon1 = TurnOnPlot.TurnOn(Name="Stage2", Histo=histo1,
                            MarkerColor=ROOT.kRed+2, MarkerStyle=21, LineColor=ROOT.kRed+2,LineStyle=1,
                            Legend="2017")
plots[-1].addTurnOn(turnon0)
plots[-1].addTurnOn(turnon1)

canvas = []
for plot in plots:
    canvas.append(plot.plot())


inputFile2017.Close()
inputFile2018.Close()

