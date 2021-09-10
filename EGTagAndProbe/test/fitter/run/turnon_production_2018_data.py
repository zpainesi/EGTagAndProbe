OutputFile: /afs/cern.ch/user/c/ckoraka/CMSSW_10_2_5_patch1/src/EGTagAndProbe/EGTagAndProbe/test/fitter/results2018/turnon_rootfiles/Efficiency_turnons_Data2018.root

NCPU: 4
Turnon.N: 2
NoFit: true

#SingleEG30
Turnon.1.Name: SingleEG30
Turnon.1.File: /eos/cms/store/group/dpg_trigger/comm_trigger/L1Trigger/ckoraka/NTuplesFor2018PerformancePlots/data_2018/reducedTrees/EGamma_Run2018ABCD_reduced.root
#Turnon.1.File: /eos/cms/store/group/dpg_trigger/comm_trigger/L1Trigger/ckoraka/NTuplesFor2018PerformancePlots/data_2018/reducedTrees/EGamma_Run2018ABCD_reduced.root
Turnon.1.Tree: TagAndProbe
Turnon.1.XVar: eleProbeSclEt
Turnon.1.Cut: (hasL1_30) 
Turnon.1.SelectionVars: eleProbeSclEt
Turnon.1.Selection: (abs(eleProbeEta)<2.5 && isProbeLoose && sqrt(pow(eleProbeEta-eleTagEta,2)+pow(acos(cos(eleProbePhi-eleTagPhi)),2))>0.6)
Turnon.1.Binning: 1., 3., 5., 7., 9.,  10., 12., 15., 18., 20., 22., 24., 26., 28., 29., 30., 31., 32., 33., 34., 35., 36., 37., 38., 39., 40., 41., 42., 43., 45., 50., 60., 70., 100., 200., 300., 400., 600., 1000.
Turnon.1.FitRange: 0 200
Turnon.1.CB.Max: 1. 0.6 1.
Turnon.1.CB.Alpha: 7.4 0.01 80.
Turnon.1.CB.N: 1.6 1.001 1000.
Turnon.1.CB.Mean: 39. 0. 100.
Turnon.1.CB.Sigma: 3.3 0.01 100

#SingleEG40
Turnon.2.Name: SingleEG40
Turnon.2.File: /eos/cms/store/group/dpg_trigger/comm_trigger/L1Trigger/ckoraka/NTuplesFor2018PerformancePlots/data_2018/reducedTrees/EGamma_Run2018ABCD_reduced.root
#Turnon.2.File: /eos/cms/store/group/dpg_trigger/comm_trigger/L1Trigger/ckoraka/NTuplesFor2018PerformancePlots/data_2018/reducedTrees/EGamma_Run2018ABCD_reduced.root
Turnon.2.Tree: TagAndProbe
Turnon.2.XVar: eleProbeSclEt
Turnon.2.Cut: (hasL1_40)
Turnon.2.SelectionVars: eleProbeSclEt
Turnon.2.Selection: (abs(eleProbeEta)<2.5 && isProbeLoose && sqrt(pow(eleProbeEta-eleTagEta,2)+pow(acos(cos(eleProbePhi-eleTagPhi)),2))>0.6)
Turnon.2.Binning: 1., 3., 5., 7., 9.,  10., 12., 15., 18., 20., 22., 24., 26., 28., 29., 30., 31., 32., 33., 34., 35., 36., 37., 38., 39., 40., 41., 42., 43., 45., 50., 60., 70., 100., 200., 300., 400., 600., 1000.
Turnon.2.FitRange: 0 200
Turnon.2.CB.Max: 1. 0.6 1.
Turnon.2.CB.Alpha: 7.4 0.01 80.
Turnon.2.CB.N: 1.6 1.001 1000.
Turnon.2.CB.Mean: 39. 0. 100.
Turnon.2.CB.Sigma: 3.3 0.01 100

#SingleTightIsoEG34
Turnon.3.Name: SingleTightIsoEG34
Turnon.3.File: /eos/cms/store/group/dpg_trigger/comm_trigger/L1Trigger/ckoraka/NTuplesFor2018PerformancePlots/data_2018/reducedTrees/EGamma_Run2018ABCD_reduced.root
Turnon.3.Tree: Ntuplizer/TagAndProbe
Turnon.3.XVar: eleProbeSclEt
Turnon.3.Cut: (hasL1_tightiso_34)
Turnon.3.SelectionVars: eleProbeSclEt
Turnon.3.Selection: (abs(eleProbeEta)<2.5 && isProbeLoose && sqrt(pow(eleProbeEta-eleTagEta,2)+pow(acos(cos(eleProbePhi-eleTagPhi)),2))>0.6)
Turnon.3.Binning: 1., 3., 5., 7., 9.,  10., 12., 15., 18., 20., 22., 24., 26., 28., 29., 30., 31., 32., 33., 34., 35., 36., 37., 38., 39., 40., 41., 42., 43., 45., 50., 60., 70., 100., 200., 300., 400., 600., 1000.
Turnon.3.FitRange: 0 200
Turnon.3.CB.Max: 1. 0.6 1.
Turnon.3.CB.Alpha: 7.4 0.01 80.
Turnon.3.CB.N: 1.6 1.001 1000.
Turnon.3.CB.Mean: 39. 0. 100.
Turnon.3.CB.Sigma: 3.3 0.01 100

#SingleTightIsoEG32
Turnon.4.Name: SingleTightIsoEG32
Turnon.4.File: /eos/cms/store/group/dpg_trigger/comm_trigger/L1Trigger/ckoraka/NTuplesFor2018PerformancePlots/data_2018/reducedTrees/EGamma_Run2018ABCD_reduced.root
Turnon.4.Tree: Ntuplizer/TagAndProbe
Turnon.4.XVar: eleProbeSclEt
Turnon.4.Cut: (hasL1_tightiso_32)
Turnon.4.SelectionVars: eleProbeSclEt
Turnon.4.Selection: (abs(eleProbeEta)<2.5 && isProbeLoose && sqrt(pow(eleProbeEta-eleTagEta,2)+pow(acos(cos(eleProbePhi-eleTagPhi)),2))>0.6)
Turnon.4.Binning: 1., 3., 5., 7., 9.,  10., 12., 15., 18., 20., 22., 24., 26., 28., 29., 30., 31., 32., 33., 34., 35., 36., 37., 38., 39., 40., 41., 42., 43., 45., 50., 60., 70., 100., 200., 300., 400., 600., 1000.
Turnon.4.FitRange: 0 200
Turnon.4.CB.Max: 1. 0.6 1.
Turnon.4.CB.Alpha: 7.4 0.01 80.
Turnon.4.CB.N: 1.6 1.001 1000.
Turnon.4.CB.Mean: 39. 0. 100.
Turnon.4.CB.Sigma: 3.3 0.01 100

#SingleTightIsoEG30
Turnon.5.Name: SingleTightIsoEG30
Turnon.5.File: /eos/cms/store/group/dpg_trigger/comm_trigger/L1Trigger/ckoraka/NTuplesFor2018PerformancePlots/data_2018/reducedTrees/EGamma_Run2018ABCD_reduced.root
Turnon.5.Tree: Ntuplizer/TagAndProbe
Turnon.5.XVar: eleProbeSclEt
Turnon.5.Cut: (hasL1_tightiso_30)
Turnon.5.SelectionVars: eleProbeSclEt
Turnon.5.Selection: (abs(eleProbeEta)<2.5 && isProbeLoose && sqrt(pow(eleProbeEta-eleTagEta,2)+pow(acos(cos(eleProbePhi-eleTagPhi)),2))>0.6)
Turnon.5.Binning: 1., 3., 5., 7., 9.,  10., 12., 15., 18., 20., 22., 24., 26., 28., 29., 30., 31., 32., 33., 34., 35., 36., 37., 38., 39., 40., 41., 42., 43., 45., 50., 60., 70., 100., 200., 300., 400., 600., 1000.
Turnon.5.FitRange: 0 200
Turnon.5.CB.Max: 1. 0.6 1.
Turnon.5.CB.Alpha: 7.4 0.01 80.
Turnon.5.CB.N: 1.6 1.001 1000.
Turnon.5.CB.Mean: 39. 0. 100.
Turnon.5.CB.Sigma: 3.3 0.01 100

#SingleTightIsoEG32er
Turnon.6.Name: SingleTightIsoEG32er
Turnon.6.File: /eos/cms/store/group/dpg_trigger/comm_trigger/L1Trigger/ckoraka/NTuplesFor2018PerformancePlots/data_2018/reducedTrees/EGamma_Run2018ABCD_reduced.root
Turnon.6.Tree: Ntuplizer/TagAndProbe
Turnon.6.XVar: eleProbeSclEt
Turnon.6.Cut: ((hasL1_tightiso_32) && (abs(l1tEta)<2.1))
Turnon.6.SelectionVars: eleProbeSclEt
Turnon.6.Selection: (abs(eleProbeEta)<2.1 && isProbeLoose && sqrt(pow(eleProbeEta-eleTagEta,2)+pow(acos(cos(eleProbePhi-eleTagPhi)),2))>0.6)
Turnon.6.Binning: 1., 3., 5., 7., 9.,  10., 12., 15., 18., 20., 22., 24., 26., 28., 29., 30., 31., 32., 33., 34., 35., 36., 37., 38., 39., 40., 41., 42., 43., 45., 50., 60., 70., 100., 200., 300., 400., 600., 1000.
Turnon.6.FitRange: 0 200  
Turnon.6.CB.Max: 1. 0.6 1.
Turnon.6.CB.Alpha: 7.4 0.01 80.
Turnon.6.CB.N: 1.6 1.001 1000. 
Turnon.6.CB.Mean: 39. 0. 100.  
Turnon.6.CB.Sigma: 3.3 0.01 100

#SingleTightIsoEG30er
Turnon.7.Name: SingleTightIsoEG30er
Turnon.7.File: /eos/cms/store/group/dpg_trigger/comm_trigger/L1Trigger/ckoraka/NTuplesFor2018PerformancePlots/data_2018/reducedTrees/EGamma_Run2018ABCD_reduced.root
Turnon.7.Tree: Ntuplizer/TagAndProbe
Turnon.7.XVar: eleProbeSclEt
Turnon.7.Cut: ((hasL1_tightiso_30) && (abs(l1tEta)<2.1))
Turnon.7.SelectionVars: eleProbeSclEt
Turnon.7.Selection: (abs(eleProbeEta)<2.1 && isProbeLoose && sqrt(pow(eleProbeEta-eleTagEta,2)+pow(acos(cos(eleProbePhi-eleTagPhi)),2))>0.6)
Turnon.7.Binning: 1., 3., 5., 7., 9.,  10., 12., 15., 18., 20., 22., 24., 26., 28., 29., 30., 31., 32., 33., 34., 35., 36., 37., 38., 39., 40., 41., 42., 43., 45., 50., 60., 70., 100., 200., 300., 400., 600., 1000.
Turnon.7.FitRange: 0 200
Turnon.7.CB.Max: 1. 0.6 1.
Turnon.7.CB.Alpha: 7.4 0.01 80.
Turnon.7.CB.N: 1.6 1.001 1000.
Turnon.7.CB.Mean: 39. 0. 100.
Turnon.7.CB.Sigma: 3.3 0.01 100

#SingleTightIsoEG28er
Turnon.8.Name: SingleTightIsoEG28er
Turnon.8.File: /eos/cms/store/group/dpg_trigger/comm_trigger/L1Trigger/ckoraka/NTuplesFor2018PerformancePlots/data_2018/reducedTrees/EGamma_Run2018ABCD_reduced.root
Turnon.8.Tree: Ntuplizer/TagAndProbe
Turnon.8.XVar: eleProbeSclEt
Turnon.8.Cut: ((hasL1_tightiso_28) && (abs(l1tEta)<2.1))
Turnon.8.SelectionVars: eleProbeSclEt
Turnon.8.Selection: (abs(eleProbeEta)<2.1 && isProbeLoose && sqrt(pow(eleProbeEta-eleTagEta,2)+pow(acos(cos(eleProbePhi-eleTagPhi)),2))>0.6)
Turnon.8.Binning: 1., 3., 5., 7., 9.,  10., 12., 15., 18., 20., 22., 24., 26., 28., 29., 30., 31., 32., 33., 34., 35., 36., 37., 38., 39., 40., 41., 42., 43., 45., 50., 60., 70., 100., 200., 300., 400., 600., 1000.
Turnon.8.FitRange: 0 200
Turnon.8.CB.Max: 1. 0.6 1.
Turnon.8.CB.Alpha: 7.4 0.01 80.
Turnon.8.CB.N: 1.6 1.001 1000.
Turnon.8.CB.Mean: 39. 0. 100.
Turnon.8.CB.Sigma: 3.3 0.01 100

#SingleEG40_OR_SingleEGTightIso32_OR_SingleEGTightIso30er
Turnon.9.Name: SingleEG40_OR_SingleEGTightIso32_OR_SingleEGTightIso30er
Turnon.9.File: /eos/cms/store/group/dpg_trigger/comm_trigger/L1Trigger/ckoraka/NTuplesFor2018PerformancePlots/data_2018/reducedTrees/EGamma_Run2018ABCD_reduced.root
Turnon.9.Tree: Ntuplizer/TagAndProbe
Turnon.9.XVar: eleProbeSclEt
Turnon.9.Cut: ((hasL1_40) || (hasL1_tightiso_32) || ( (hasL1_tightiso_30)  && (abs(l1tEta)<2.1)) )
Turnon.9.SelectionVars: eleProbeSclEt
Turnon.9.Selection: (abs(eleProbeEta)<2.5 && isProbeLoose && sqrt(pow(eleProbeEta-eleTagEta,2)+pow(acos(cos(eleProbePhi-eleTagPhi)),2))>0.6)
Turnon.9.Binning: 1., 3., 5., 7., 9.,  10., 12., 15., 18., 20., 22., 24., 26., 28., 29., 30., 31., 32., 33., 34., 35., 36., 37., 38., 39., 40., 41., 42., 43., 45., 50., 60., 70., 100., 200., 300., 400., 600., 1000.
Turnon.9.FitRange: 0 200
Turnon.9.CB.Max: 1. 0.6 1.
Turnon.9.CB.Alpha: 7.4 0.01 80.
Turnon.9.CB.N: 1.6 1.001 1000.
Turnon.9.CB.Mean: 39. 0. 100.
Turnon.9.CB.Sigma: 3.3 0.01 100


#SingleLooseIsoEG22er_OR_SingleTightIsoEG32
Turnon.10.Name: SingleLooseIsoEG22er_OR_SingleTightIsoEG32
Turnon.10.File: /eos/cms/store/group/dpg_trigger/comm_trigger/L1Trigger/ckoraka/NTuplesFor2018PerformancePlots/data_2018/reducedTrees/EGamma_Run2018ABCD_reduced.root
Turnon.10.Tree: Ntuplizer/TagAndProbe
Turnon.10.XVar: eleProbeSclEt
Turnon.10.Cut: ( ((hasL1_looseiso_22) && (abs(l1tEta)<2.1) ) || ((hasL1_tightiso_30) && (abs(l1tEta)<2.1)) )
Turnon.10.SelectionVars: eleProbeSclEt
Turnon.10.Selection: (abs(eleProbeEta)<2.1 && isProbeLoose && sqrt(pow(eleProbeEta-eleTagEta,2)+pow(acos(cos(eleProbePhi-eleTagPhi)),2))>0.6)
Turnon.10.Binning: 1., 3., 5., 7., 9.,  10., 12., 15., 18., 20., 22., 24., 26., 28., 29., 30., 31., 32., 33., 34., 35., 36., 37., 38., 39., 40., 41., 42., 43., 45., 50., 60., 70., 100., 200., 300., 400., 600., 1000.
Turnon.10.FitRange: 0 200
Turnon.10.CB.Max: 1. 0.6 1.
Turnon.10.CB.Alpha: 7.4 0.01 80.
Turnon.10.CB.N: 1.6 1.001 1000.
Turnon.10.CB.Mean: 39. 0. 100.
Turnon.10.CB.Sigma: 3.3 0.01 100

#SingleTightIsoEG22er
Turnon.11.Name: SingleTightIsoEG22er
Turnon.11.File: /eos/cms/store/group/dpg_trigger/comm_trigger/L1Trigger/ckoraka/NTuplesFor2018PerformancePlots/data_2018/reducedTrees/EGamma_Run2018ABCD_reduced.root
Turnon.11.Tree: Ntuplizer/TagAndProbe
Turnon.11.XVar: eleProbeSclEt
Turnon.11.Cut: ((hasL1_tightiso_22) && (abs(l1tEta)<2.1)) 
Turnon.11.SelectionVars: eleProbeSclEt
Turnon.11.Selection: (abs(eleProbeEta)<2.1 && isProbeLoose && sqrt(pow(eleProbeEta-eleTagEta,2)+pow(acos(cos(eleProbePhi-eleTagPhi)),2))>0.6)
Turnon.11.Binning: 1., 3., 5., 7., 9.,  10., 12., 15., 18., 20., 22., 24., 26., 28., 29., 30., 31., 32., 33., 34., 35., 36., 37., 38., 39., 40., 41., 42., 43., 45., 50., 60., 70., 100., 200., 300., 400., 600., 1000.
Turnon.11.FitRange: 0 200
Turnon.11.CB.Max: 1. 0.6 1.
Turnon.11.CB.Alpha: 7.4 0.01 80.
Turnon.11.CB.N: 1.6 1.001 1000. 
Turnon.11.CB.Mean: 39. 0. 100.
Turnon.11.CB.Sigma: 3.3 0.01 100

#SingleLooseIsoEG26
Turnon.12.Name: SingleLooseIsoEG26
Turnon.12.File: /eos/cms/store/group/dpg_trigger/comm_trigger/L1Trigger/ckoraka/NTuplesFor2018PerformancePlots/data_2018/reducedTrees/EGamma_Run2018ABCD_reduced.root
Turnon.12.Tree: Ntuplizer/TagAndProbe
Turnon.12.XVar: eleProbeSclEt
Turnon.12.Cut: (hasL1_looseiso_26)
Turnon.12.SelectionVars: eleProbeSclEt
Turnon.12.Selection: (abs(eleProbeEta)<2.5 && isProbeLoose && sqrt(pow(eleProbeEta-eleTagEta,2)+pow(acos(cos(eleProbePhi-eleTagPhi)),2))>0.6)
Turnon.12.Binning: 1., 3., 5., 7., 9.,  10., 12., 15., 18., 20., 22., 24., 26., 28., 29., 30., 31., 32., 33., 34., 35., 36., 37., 38., 39., 40., 41., 42., 43., 45., 50., 60.$
Turnon.12.FitRange: 0 200
Turnon.12.CB.Max: 1. 0.6 1.  
Turnon.12.CB.Alpha: 7.4 0.01 80.
Turnon.12.CB.N: 1.6 1.001 1000.
Turnon.12.CB.Mean: 39. 0. 100.
Turnon.12.CB.Sigma: 3.3 0.01 100

#SingleLooseIsoEG24  
Turnon.13.Name: SingleLooseIsoEG24  
Turnon.13.File: /eos/cms/store/group/dpg_trigger/comm_trigger/L1Trigger/ckoraka/NTuplesFor2018PerformancePlots/data_2018/reducedTrees/EGamma_Run2018ABCD_reduced.root
Turnon.13.Tree: Ntuplizer/TagAndProbe
Turnon.13.XVar: eleProbeSclEt
Turnon.13.Cut: (hasL1_looseiso_24)                      
Turnon.13.SelectionVars: eleProbeSclEt
Turnon.13.Selection: (abs(eleProbeEta)<2.5 && isProbeLoose && sqrt(pow(eleProbeEta-eleTagEta,2)+pow(acos(cos(eleProbePhi-eleTagPhi)),2))>0.6)
Turnon.13.Binning: 1., 3., 5., 7., 9.,  10., 12., 15., 18., 20., 22., 24., 26., 28., 29., 30., 31., 32., 33., 34., 35., 36., 37., 38., 39., 40., 41., 42., 43., 45., 50., 60.$
Turnon.13.FitRange: 0 200
Turnon.13.CB.Max: 1. 0.6 1.
Turnon.13.CB.Alpha: 7.4 0.01 80.
Turnon.13.CB.N: 1.6 1.001 1000.
Turnon.13.CB.Mean: 39. 0. 100.
Turnon.13.CB.Sigma: 3.3 0.01 100

#SingleTightIsoEG32_vs_nvtx  
Turnon.14.Name: SingleTightIsoEG32_vs_nvtx  
Turnon.14.File: /eos/cms/store/group/dpg_trigger/comm_trigger/L1Trigger/ckoraka/NTuplesFor2018PerformancePlots/data_2018/reducedTrees/EGamma_Run2018ABCD_reduced.root
Turnon.14.Tree: Ntuplizer/TagAndProbe
Turnon.14.XVar: Nvtx
Turnon.14.Cut: (hasL1_tightiso_32)                      
Turnon.14.SelectionVars: eleProbeSclEt
Turnon.14.Selection: ((eleProbeSclEt > 50.) && isProbeLoose && sqrt(pow(eleProbeEta-eleTagEta,2)+pow(acos(cos(eleProbePhi-eleTagPhi)),2))>0.6)
Turnon.14.Binning: 1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12., 13., 14., 15., 16., 17., 18., 19., 20., 21., 22., 23., 24., 25., 26. , 27., 28., 29., 30., 31., 32., 33., 34., 35., 36., 37., 38., 39., 40., 41., 42., 43., 44., 45., 46., 47., 48., 49., 50., 51., 52., 53., 54., 55., 56., 57., 58., 59., 60.
Turnon.14.FitRange: 0 200
Turnon.14.CB.Max: 1. 0.6 1.
Turnon.14.CB.Alpha: 7.4 0.01 80.
Turnon.14.CB.N: 1.6 1.001 1000.
Turnon.14.CB.Mean: 39. 0. 100.
Turnon.14.CB.Sigma: 3.3 0.01 100



