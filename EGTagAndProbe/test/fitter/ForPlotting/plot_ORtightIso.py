import ROOT
import TurnOnPlot_DATA as TurnOnPlot


plots = []
plots.append(TurnOnPlot.TurnOnPlot())
plots[-1].name = "TurnOn_SingleEG_OR_tightIso_2018ABCD"
plots[-1].xRange = (0,99.9)
#plots[-1].legendPosition = (0.6,0.2,0.9,0.4)
plots[-1].legendPosition = (0.46,0.2,0.93,0.4)


inputFile = ROOT.TFile.Open("turnon_rootfiles/eff_turnons_all_2018/Efficiency_turnons_Data2018_SingleEG40_OR_SingleEGTightIso32_OR_SingleEGTightIso30er.root")

histo = inputFile.Get("histo_SingleEG40_OR_SingleEGTightIso32_OR_SingleEGTightIso30er")
histo.__class__ = ROOT.RooHist


turnon = TurnOnPlot.TurnOn(Name="Stage2", Histo=histo,
                                    MarkerColor=ROOT.kBlue+2,MarkerStyle=22,LineColor=ROOT.kBlue+2,LineStyle=1,
                                    Legend="#splitline{L1 E_{T} > 40GeV OR }{#splitline{L1 E_{T} > 32GeV && Tight Iso OR }{L1 E_{T} > 30GeV && Tight Iso && #||{#eta^{offl}}<2.1}}")


plots[0].addTurnOn(turnon)



canvas = []
for plot in plots:
    canvas.append(plot.plot())


inputFile.Close()

