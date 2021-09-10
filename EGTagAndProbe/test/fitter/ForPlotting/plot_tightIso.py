import ROOT
import TurnOnPlot_DATA as TurnOnPlot


plots = []
plots.append(TurnOnPlot.TurnOnPlot())
plots[-1].name = "TurnOn_SingleEG_TightIso_2018ABCD"
plots[-1].xRange = (0,99.9)
#plots[-1].legendPosition = (0.6,0.2,0.9,0.4)
plots[-1].legendPosition = (0.6,0.2,0.9,0.4)


inputFile = ROOT.TFile.Open("turnon_rootfiles/eff_turnons_all_2018/Efficiency_turnons_Data2018_TightIsoEG34.root")

histo = inputFile.Get("histo_SingleTightIsoEG34")
histo.__class__ = ROOT.RooHist


turnon = TurnOnPlot.TurnOn(Name="Stage2", Histo=histo,
                                    MarkerColor=ROOT.kBlue+2, MarkerStyle=22, LineColor=ROOT.kBlue+2,LineStyle=1,
                                    Legend="L1 E_{T} > 34 GeV")

inputFile = ROOT.TFile.Open("turnon_rootfiles/eff_turnons_all_2018/Efficiency_turnons_Data2018_TightIsoEG32.root")

histo1 = inputFile.Get("histo_SingleTightIsoEG32")
histo1.__class__ = ROOT.RooHist



turnon1 = TurnOnPlot.TurnOn(Name="Stage2", Histo=histo1,
                                    MarkerColor=ROOT.kRed+2, MarkerStyle=22, LineColor=ROOT.kRed+2,LineStyle=1,
                                    Legend="L1 E_{T} > 32 GeV")


inputFile = ROOT.TFile.Open("turnon_rootfiles/eff_turnons_all_2018/Efficiency_turnons_Data2018_TightIsoEG30.root")

histo2 = inputFile.Get("histo_SingleTightIsoEG30")     
histo2.__class__ = ROOT.RooHist

turnon2 = TurnOnPlot.TurnOn(Name="Stage2", Histo=histo2,
                                    MarkerColor=ROOT.kGreen+2, MarkerStyle=22, LineColor=ROOT.kGreen+2,LineStyle=1,
                                    Legend="L1 E_{T} > 30 GeV")     



plots[0].addTurnOn(turnon)
plots[0].addTurnOn(turnon1)
plots[0].addTurnOn(turnon2)


canvas = []
for plot in plots:
    canvas.append(plot.plot())


inputFile.Close()

