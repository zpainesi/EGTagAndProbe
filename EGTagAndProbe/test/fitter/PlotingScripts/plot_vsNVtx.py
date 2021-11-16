import ROOT
import TurnOnPlot_DATA as TurnOnPlot


plots = []
inputFileRun3MC = ROOT.TFile.Open("/home/aravind/cernbox/work/l1egamma/histogramFiles/mc2022_turnonHists.root")
inputFile2018   = ROOT.TFile.Open("/home/aravind/cernbox/work/l1egamma/histogramFiles/data2018_turnonHists.root")

plots.append(TurnOnPlot.TurnOnPlot())
plots[-1].name = "L1EG32TightEfficiencyVsPU"
plots[-1].xRange = (9.9,60.0)
plots[-1].yRange = (0.0,1.2)
plots[-1].desc=["L1 EG","Tight Isolation","#||{#eta^{offl}}<2.5 & E_{T}^{offl} > 50 GeV"]
plots[-1].legendPosition = (0.60,0.25,0.92,0.40)
plots[-1].descPosition =(0.6,0.58)
plots[-1].logx = False

histo0 = inputFileRun3MC.Get("hist_L1_32TightvsPUoffline50GeVEfficiency")
histo0.__class__ = ROOT.TH1D
turnon0 = TurnOnPlot.TurnOn(Name="Stage2", Histo=histo0,
                            MarkerColor=ROOT.kBlue+2, MarkerStyle=22, LineColor=ROOT.kBlue+2,LineStyle=1,
                            Legend="L1 E_{T} > 32")

print(histo0.Integral())

plots[0].addTurnOn(turnon0)


canvas = []
for plot in plots:
    canvas.append(plot.plot())


inputFileRun3MC.Close()
inputFile2018.Close()

