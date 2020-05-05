OutputFile: /afs/cern.ch/user/c/ckoraka/CMSSW_10_2_5_patch1/src/EGTagAndProbe/EGTagAndProbe/test/fitter/results2018/turnon_rootfiles/Efficiency_turnons_Data2017_EG40.root

NCPU: 64
Turnon.N: 1
NoFit: true

#SingleEG40
Turnon.1.Name: SingleEG40
#Turnon.1.File: /eos/cms/store/group/dpg_trigger/comm_trigger/L1Trigger/ckoraka/NTuplesFor2018PerformancePlots/data_2018/reducedTrees/EGamma_Run2018ABCD_L140reduced.root
#Turnon.1.File: /eos/cms/store/group/dpg_trigger/comm_trigger/L1Trigger/ckoraka/test_red/trees_reduced/EGamma_Run2018ABCD_L1turnon12345.root
#Turnon.1.File: /eos/cms/store/group/dpg_trigger/comm_trigger/L1Trigger/ckoraka/NTuplesFor2018PerformancePlots/data_2018/reducedTrees/Run2018A-PromptReco-v3_316700.root
Turnon.1.File: /afs/cern.ch/work/c/ckoraka/NTuple_total_2017_reduced.root
Turnon.1.Tree: Ntuplizer/TagAndProbe
Turnon.1.XVar: eleProbeSclEt
Turnon.1.Cut: (hasL1_40)
Turnon.1.SelectionVars: eleProbeSclEt
Turnon.1.Selection: (abs(eleProbeEta)<2.5 && isProbeLoose && sqrt(pow(eleProbeEta-eleTagEta,2)+pow(acos(cos(eleProbePhi-eleTagPhi)),2))>0.6)
Turnon.1.Binning: 1., 3., 5., 7., 9.,  10., 12., 15., 18., 20., 22., 24., 26., 28., 29., 30., 31., 32., 33., 34., 35., 36., 37., 38., 39., 40., 41., 42., 43., 45., 50., 60., 70., 100., 200., 300., 400., 600., 1000.
Turnon.1.FitRange: 0 1000.
#Turnon.1.CB.Max: 1. 0.6 1.
#Turnon.1.CB.Alpha: 7.4 0.01 80.
#Turnon.1.CB.N: 1.6 1.001 1000.
#Turnon.1.CB.Mean: 39. 0. 100.
#Turnon.1.CB.Sigma: 3.3 0.01 100
