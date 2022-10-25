import ROOT
import TurnOnPlot_DATA as TurnOnPlot


plots = []
inputFile2018   = ROOT.TFile.Open("/home/aravind/cernbox/work/l1egamma/histogramFiles/data2018_turnonHists.root")

plots.append(TurnOnPlot.TurnOnPlot())
plots[-1].name = "2018crossEfficienciy34"
plots[-1].xRange = (1.0,99.9)
plots[-1].desc=["L1 EG, 2018","#||{#eta^{offl}}<2.5 "]
plots[-1].legendPosition = (0.40,0.25,0.94,0.40)
plots[-1].descPosition =(0.6,0.58)
plots[-1].logx = False

histo0 = inputFile2018.Get("hist_L1_34Efficiency")
histo0.__class__ = ROOT.TH1D
turnon0 = TurnOnPlot.TurnOn(Name="Stage2", Histo=histo0,
                            MarkerColor=ROOT.kBlue, MarkerStyle=22, LineColor=ROOT.kBlue+2,LineStyle=1,
                            Legend="L1 E_{T} > 34")

histo1 = inputFile2018.Get("hist_L1_tightiso_28Efficiency")
histo1.__class__ = ROOT.TH1D
turnon1 = TurnOnPlot.TurnOn(Name="Stage2", Histo=histo1,
                            MarkerColor=ROOT.kRed, MarkerStyle=22, LineColor=ROOT.kRed+2,LineStyle=1,
                            Legend="L1 E_{T} > 28 & Tight Iso.")

histo2 = inputFile2018.Get("hist_L1_tightiso_28_OR_L1_34Efficiency")
histo2.__class__ = ROOT.TH1D
turnon2 = TurnOnPlot.TurnOn(Name="Stage2", Histo=histo2,
                            MarkerColor=ROOT.kGreen+2, MarkerStyle=22, LineColor=ROOT.kGreen+2,LineStyle=1,
                            Legend="L1 E_{T} > 34 OR (E_{T} > 34 and Tight Iso.)")

plots[-1].addTurnOn(turnon0)
plots[-1].addTurnOn(turnon1)
plots[-1].addTurnOn(turnon2)


canvas = []
for plot in plots:
    canvas.append(plot.plot())


inputFile2018.Close()

