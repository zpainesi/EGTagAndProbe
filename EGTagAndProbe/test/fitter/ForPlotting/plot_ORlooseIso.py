import ROOT
import TurnOnPlot_DATA as TurnOnPlot


plots = []
plots.append(TurnOnPlot.TurnOnPlot())
plots[-1].name = "TurnOn_SingleEG_ORLooseIso_Run2018ABCD"
plots[-1].xRange = (0,99.9)
#plots[-1].legendPosition = (0.6,0.2,0.9,0.4)
plots[-1].legendPosition = (0.42,0.2,0.93,0.4)


inputFile = ROOT.TFile.Open("turnon_rootfiles/Efficiency_turnons_Data2018_SingleLooseIsoEG22er_OR_SingleTightIsoEG32.root")

histo = inputFile.Get("histo_SingleLooseIsoEG22er_OR_SingleTightIsoEG32")
histo.__class__ = ROOT.RooHist


turnon = TurnOnPlot.TurnOn(Name="Stage2", Histo=histo,
                                    MarkerColor=ROOT.kBlue+2, MarkerStyle=22, LineColor=ROOT.kBlue+2,LineStyle=1,
                                    Legend="#splitline{L1 E_{T} > 22 GeV && Loose Isolation }{OR L1 E_{T} > 30 GeV && Tight Isolation }")

inputFile = ROOT.TFile.Open("turnon_rootfiles/Efficiency_turnons_Data2018_SingleTightIsoEG22er.root")

histo1 = inputFile.Get("histo_SingleTightIsoEG22er")
histo1.__class__ = ROOT.RooHist

turnon1 = TurnOnPlot.TurnOn(Name="Stage2", Histo=histo1,
                                    MarkerColor=ROOT.kRed+2, MarkerStyle=22, LineColor=ROOT.kRed+2,LineStyle=1,
                                    Legend="L1 E_{T} > 22 GeV && Loose Isolation")

plots[0].addTurnOn(turnon)
plots[0].addTurnOn(turnon1)


canvas = []
for plot in plots:
    canvas.append(plot.plot())


inputFile.Close()

