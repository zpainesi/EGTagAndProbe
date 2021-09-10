OutputFile: /afs/cern.ch/user/c/ckoraka/CMSSW_10_2_5_patch1/src/EGTagAndProbe/EGTagAndProbe/test/fitter/results2018/turnon_rootfiles/Efficiency_turnons_Data2018_TightEG28.root

NCPU: 64
Turnon.N: 1
NoFit: true

#SingleEG30
Turnon.1.Name: TightEG28
Turnon.1.File: /afs/cern.ch/user/c/ckoraka/CMSSW_10_2_5_patch1/src/EGTagAndProbe/EGTagAndProbe/test/EGamma_Run2018ABCD_L1turnon17_2.root
Turnon.1.Tree: Ntuplizer/TagAndProbe
Turnon.1.XVar: eleProbeSclEt
#Turnon.1.Cut: (hasL1_34)
#Turnon.1.Cut: (hasL1_tightiso_28 || hasL1_34)
Turnon.1.Cut: (hasL1_tightiso_28)
Turnon.1.SelectionVars: eleProbeSclEt
Turnon.1.Selection: (abs(eleProbeEta)<2.5 && isProbeLoose && sqrt(pow(eleProbeEta-eleTagEta,2)+pow(acos(cos(eleProbePhi-eleTagPhi)),2))>0.6)
Turnon.1.Binning: 1., 3., 5., 7., 9.,  10., 12., 15., 18., 20., 22., 24., 26., 28., 29., 30., 31., 32., 33., 34., 35., 36., 37., 38., 39., 40., 41., 42., 43., 45., 50., 60., 70., 100., 200., 300., 400., 600., 1000.
Turnon.1.FitRange: 0 1000.

