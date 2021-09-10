import ROOT
import TurnOnPlot_DATA as TurnOnPlot


plots = []
plots.append(TurnOnPlot.TurnOnPlot())
plots[-1].name = "TurnOn_SingleEG_vsNvtx_2018ABCD"
plots[-1].xRange = (0,99.9)
#plots[-1].legendPosition = (0.6,0.2,0.9,0.4)
plots[-1].legendPosition = (0.6,0.2,0.9,0.5)


inputFile = ROOT.TFile.Open("turnon_rootfiles/Efficiency_turnons_Data2018_SingleTightIsoEG32_vs_nvtx_offlEt40.root")

histo = inputFile.Get("histo_SingleTightIsoEG32_vs_nvtx_offlEt40")
histo.__class__ = ROOT.RooHist

#fit   = inputFile.Get("fit_SingleEG40")        
#fit.__class__ = ROOT.RooCurve


turnon = TurnOnPlot.TurnOn(Name="Stage2", Histo=histo,
                                    MarkerColor=ROOT.kBlue+2,MarkerStyle=22,LineColor=ROOT.kBlue+2,LineStyle=1,
                                    Legend="#splitline{L1 E_{T} > 32 GeV}{& Tight Iso}")



inputFile = ROOT.TFile.Open("turnon_rootfiles/Efficiency_turnons_Data2018_SingleNoIsoEG32_vs_nvtx_offlEt40.root")

histo1 = inputFile.Get("histo_SingleNoIsoEG32_vs_nvtx_offlEt40")
histo.__class__ = ROOT.RooHist


turnon1 = TurnOnPlot.TurnOn(Name="Stage2", Histo=histo1,
                                    MarkerColor=ROOT.kRed+2,MarkerStyle=22,LineColor=ROOT.kRed+2,LineStyle=1,
                                    Legend="#splitline{L1 E_{T} > 32 GeV}{& Inclusive Iso}")

plots[0].addTurnOn(turnon)
plots[0].addTurnOn(turnon1)


canvas = []
for plot in plots:
    canvas.append(plot.plot())


inputFile.Close()

