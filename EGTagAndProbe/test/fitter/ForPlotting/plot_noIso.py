import ROOT
import TurnOnPlot_DATA as TurnOnPlot


plots = []
plots.append(TurnOnPlot.TurnOnPlot())
plots[-1].name = "TurnOn_SingleEG_NoIso_2018ABCD"
plots[-1].xRange = (0,99.9)
#plots[-1].legendPosition = (0.6,0.2,0.9,0.4)
plots[-1].legendPosition = (0.6,0.2,0.9,0.45)

inputFile = ROOT.TFile.Open("turnon_rootfiles/eff_turnons_all_2018/Efficiency_turnons_Data2018_EG40.root")

histo = inputFile.Get("histo_SingleEG40")
histo.__class__ = ROOT.RooHist

#fit   = inputFile.Get("fit_SingleEG40")        
#fit.__class__ = ROOT.RooCurve


turnon = TurnOnPlot.TurnOn(Name="Stage2", Histo=histo,
                                    MarkerColor=ROOT.kBlue+2,MarkerStyle=22,LineColor=ROOT.kBlue+2,LineStyle=1,
                                    Legend="L1 E_{T} > 40 GeV")

inputFile = ROOT.TFile.Open("turnon_rootfiles/eff_turnons_all_2018/Efficiency_turnons_Data2018_EG30.root")

histo1 = inputFile.Get("histo_SingleEG30")
histo1.__class__ = ROOT.RooHist

#fit1   = inputFile.Get("fit_SingleEG30")
#fit1.__class__ = ROOT.RooCurve

turnon1 = TurnOnPlot.TurnOn(Name="Stage2", Histo=histo1,
                                    MarkerColor=ROOT.kRed+2, MarkerStyle=22, LineColor=ROOT.kRed+2,LineStyle=1,
                                    Legend="L1 E_{T} > 30 GeV")


plots[0].addTurnOn(turnon)
plots[0].addTurnOn(turnon1)



canvas = []
for plot in plots:
    canvas.append(plot.plot())


inputFile.Close()

