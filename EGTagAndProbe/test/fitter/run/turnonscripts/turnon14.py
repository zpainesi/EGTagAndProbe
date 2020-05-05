OutputFile: /afs/cern.ch/user/c/ckoraka/CMSSW_10_2_5_patch1/src/EGTagAndProbe/EGTagAndProbe/test/fitter/results2018/turnon_rootfiles/Efficiency_turnons_Data2018_SingleNoIsoEG32_vs_nvtx_offlEt40.root

NCPU: 64
Turnon.N: 1
NoFit: true


#SingleTightIsoEG32_vs_nvtx  
Turnon.1.Name: SingleNoIsoEG32_vs_nvtx_offlEt40  
Turnon.1.File: /eos/cms/store/group/dpg_trigger/comm_trigger/L1Trigger/ckoraka/NTuplesFor2018PerformancePlots/data_2018/reducedTrees/EGamma_Run2018ABCD_reduced_turnon14v2.root
Turnon.1.Tree: Ntuplizer/TagAndProbe
Turnon.1.XVar: Nvtx
Turnon.1.Cut: (hasL1_32)
#Turnon.1.Cut: (hasL1_tightiso_32)                     
Turnon.1.SelectionVars: eleProbeSclEt
Turnon.1.Selection: ((eleProbeSclEt > 40.) && isProbeLoose && sqrt(pow(eleProbeEta-eleTagEta,2)+pow(acos(cos(eleProbePhi-eleTagPhi)),2))>0.6)
Turnon.1.Binning: 1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12., 13., 14., 15., 16., 17., 18., 19., 20., 21., 22., 23., 24., 25., 26. , 27., 28., 29., 30., 31., 32., 33., 34., 35., 36., 37., 38., 39., 40., 41., 42., 43., 44., 45., 46., 47., 48., 49., 50., 51., 52., 53., 54., 55., 56., 57., 58., 59., 60.
Turnon.1.FitRange: 0 60
