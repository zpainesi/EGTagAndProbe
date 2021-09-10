import ROOT
import TurnOnPlot_DATA as TurnOnPlot


plots = []
inputFile2018   = ROOT.TFile.Open("/home/aravind/cernbox/work/l1egamma/histogramFiles/data2018_turnonHists.root")

plots.append(TurnOnPlot.TurnOnPlot())
plots[-1].name = "TurnOn_SingleEG30_32_34TightIso2018"
plots[-1].xRange = (4.0,999.9)
plots[-1].desc=["L1 EG, 2018","#||{#eta^{offl}}<2.5 ","Tight Isolation"]
plots[-1].legendPosition = (0.60,0.25,0.92,0.40)
plots[-1].descPosition =(0.6,0.58)

histo0 = inputFile2018.Get("hist_L1_tightiso_30Efficiency")
histo0.__class__ = ROOT.TH1D
turnon0 = TurnOnPlot.TurnOn(Name="Stage2", Histo=histo0,
                            MarkerColor=ROOT.kGreen, MarkerStyle=22, LineColor=ROOT.kGreen+2,LineStyle=1,
                            Legend="L1 E_{T} > 30")

histo1 = inputFile2018.Get("hist_L1_tightiso_32Efficiency")
histo1.__class__ = ROOT.TH1D
turnon1 = TurnOnPlot.TurnOn(Name="Stage2", Histo=histo1,
                            MarkerColor=ROOT.kRed, MarkerStyle=22, LineColor=ROOT.kRed+2,LineStyle=1,
                            Legend="L1 E_{T} > 32")

histo2 = inputFile2018.Get("hist_L1_tightiso_34Efficiency")
histo2.__class__ = ROOT.TH1D
turnon2 = TurnOnPlot.TurnOn(Name="Stage2", Histo=histo2,
                            MarkerColor=ROOT.kBlue+2, MarkerStyle=22, LineColor=ROOT.kBlue+2,LineStyle=1,
                            Legend="L1 E_{T} > 34 ")

plots[-1].addTurnOn(turnon0)
plots[-1].addTurnOn(turnon1)
plots[-1].addTurnOn(turnon2)


canvas = []
for plot in plots:
    canvas.append(plot.plot())


inputFile2018.Close()

