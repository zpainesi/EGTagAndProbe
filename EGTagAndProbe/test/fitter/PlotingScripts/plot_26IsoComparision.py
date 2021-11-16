import ROOT
import TurnOnPlot_DATA as TurnOnPlot


plots = []
inputFile2017   = ROOT.TFile.Open("/home/aravind/cernbox/work/l1egamma/histogramFiles/data2017_turnonHists.root")
inputFile2018   = ROOT.TFile.Open("/home/aravind/cernbox/work/l1egamma/histogramFiles/data2018_turnonHists.root")

plots.append(TurnOnPlot.TurnOnPlot())
plots[-1].name = "TurnOn_SingleEG26"
plots[-1].xRange = (10.0,999.9)
plots[-1].desc=["L1 EG","#||{#eta^{offl}}<2.5 ","L1 E_{T} > 30 GeV"]
plots[-1].legendPosition = (0.60,0.25,0.92,0.40)
plots[-1].descPosition =(0.6,0.58)

histo0 = inputFile2017.Get("hist_L1_looseiso_26Efficiency")
histo0.__class__ = ROOT.TH1D
turnon0 = TurnOnPlot.TurnOn(Name="Stage2", Histo=histo0,
                            MarkerColor=ROOT.kBlue+2, MarkerStyle=22, LineColor=ROOT.kBlue+2,LineStyle=1,
                            Legend="2017 , Loose Iso.")

histo1 = inputFile2018.Get("hist_L1_looseiso_26Efficiency")
histo1.__class__ = ROOT.TH1D
turnon1 = TurnOnPlot.TurnOn(Name="Stage2", Histo=histo1,
                            MarkerColor=ROOT.kRed+2, MarkerStyle=22, LineColor=ROOT.kRed+2,LineStyle=1,
                            Legend="2018 , Loose Iso.")


plots[-1].addTurnOn(turnon0)
plots[-1].addTurnOn(turnon1)


#plots.append(TurnOnPlot.TurnOnPlot())
#plots[-1].name = "TurnOn_SingleEG40_NoIsoRun3"
#plots[-1].xRange = (10,999.9)
#plots[-1].desc=["L1 EG","#||{#eta^{offl}}<2.5 ","L1 E_{T} > 40 GeV"]
#plots[-1].legendPosition = (0.60,0.25,0.92,0.40)
#plots[-1].descPosition =(0.6,0.58)

histo0 = inputFile2017.Get("hist_L1_tightiso_26Efficiency")
histo0.__class__ = ROOT.TH1D
turnon0 = TurnOnPlot.TurnOn(Name="Stage2", Histo=histo0,
                            MarkerColor=ROOT.kBlue+2, MarkerStyle=21, LineColor=ROOT.kBlue+2,LineStyle=1,
                            Legend="2017 , Tight Iso.")

histo1 = inputFile2018.Get("hist_L1_tightiso_26Efficiency")
histo1.__class__ = ROOT.TH1D
turnon1 = TurnOnPlot.TurnOn(Name="Stage2", Histo=histo1,
                            MarkerColor=ROOT.kRed+2, MarkerStyle=21, LineColor=ROOT.kRed+2,LineStyle=1,
                            Legend="2018 , Tight Iso.")


plots[-1].addTurnOn(turnon0)
plots[-1].addTurnOn(turnon1)



canvas = []
for plot in plots:
    canvas.append(plot.plot())


inputFile2017.Close()
inputFile2018.Close()

