OutputFile: test.root

NCPU: 64
Turnon.N: 1
NoFit: true

#SingleEG30
Turnon.1.Name: SingleEG30
Turnon.1.File: /eos/cms/store/group/dpg_trigger/comm_trigger/L1Trigger/ckoraka/For_HLT_turnon_investigation/DYToLL_M-50_TuneCP5_14TeV-pythia8/TnP_DYJetsToLL_M-50__Run3Winter20DRMiniAOD-DRFlatPU30to80_110X_mcRun3_2021_realistic_v6-v2/200429_085829/0000/NTuple_1.root
Turnon.1.Tree: Ntuplizer/TagAndProbe
Turnon.1.XVar: eleProbeSclEt
Turnon.1.Cut: (hasL1_30)
#Turnon.1.SelectionVars: eleProbeSclEt
Turnon.1.Selection: (abs(eleProbeEta)<2.5 && isProbeLoose && sqrt(pow(eleProbeEta-eleTagEta,2)+pow(acos(cos(eleProbePhi-eleTagPhi)),2))>0.6)
Turnon.1.Binning: 1., 3., 5., 7., 9.,  10., 12., 15., 18., 20., 22., 24., 26., 28., 29., 30., 31., 32., 33., 34., 35., 36., 37., 38., 39., 40., 41., 42., 43., 45., 50., 60., 70., 100., 200., 300., 400., 600., 1000.
Turnon.1.FitRange: 0 1000.

