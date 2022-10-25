import ROOT
import TurnOnPlot_DATA as TurnOnPlot


plots = []
inputFileRun3MC = ROOT.TFile.Open("/home/aravind/cernbox/work/l1egamma/histogramFiles/mc2022_turnonHists.root")
inputFile2018   = ROOT.TFile.Open("/home/aravind/cernbox/work/l1egamma/histogramFiles/data2018_turnonHists.root")

plots.append(TurnOnPlot.TurnOnPlot())
plots[-1].name = "TurnOnRun3MC_SingleEG34_TightIso"
plots[-1].xRange = (10.0,999.9)
plots[-1].desc=["L1 EG","#||{#eta^{offl}}<2.5 ","L1 E_{T} > 30 GeV"]
plots[-1].legendPosition = (0.60,0.25,0.92,0.40)
plots[-1].descPosition =(0.6,0.58)

histo0 = inputFileRun3MC.Get("hist_L1_tightiso_34Efficiency")
histo0.__class__ = ROOT.TH1D
turnon0 = TurnOnPlot.TurnOn(Name="Stage2", Histo=histo0,
                            MarkerColor=ROOT.kBlue+2, MarkerStyle=22, LineColor=ROOT.kBlue+2,LineStyle=1,
                            Legend="MC : Run3")

histo1 = inputFile2018.Get("hist_L1_tightiso_34Efficiency")
histo1.__class__ = ROOT.TH1D
turnon1 = TurnOnPlot.TurnOn(Name="Stage2", Histo=histo1,
                            MarkerColor=ROOT.kRed+2, MarkerStyle=22, LineColor=ROOT.kRed+2,LineStyle=1,
                            Legend="2018 Data")


plots[0].addTurnOn(turnon0)
plots[0].addTurnOn(turnon1)


canvas = []
for plot in plots:
    canvas.append(plot.plot())


inputFileRun3MC.Close()
inputFile2018.Close()

