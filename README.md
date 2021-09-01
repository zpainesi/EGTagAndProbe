# EGTagAndProbe
Set of tools to evaluate L1EG trigger performance on T&amp;P

Based on TauTagAndProbe package developed by L. Cadamuro & O. Davignon

### Install instructions
```
cmsrel CMSSW_X_Y_Z
cd CMSSW_X_Y_Z/src
cmsenv
git clone https://gitlab.cern.ch/ckoraka/EGTagAndProbe.git
scram b -j 4
```

### Producing TagAndProbe ntuples with unpacked L1EG (no re-emulation)
1. cd to relevant directory
```
${CMSSW_BASE}/src/EGTagAndProbe/EGTagAndProbe
```
2. Set flag isMC and isMINIAOD according to sample in test/test.py
3. HLT path used specified in python/MCAnalysis_cff.py (MC) or python/tagAndProbe_cff.py (data)
4. Launch test/test.py


```
cd test
cmsRun test.py
```


### Producing TagAndProbe ntuples with emulated L1EG
Follow the instructions from the following twiki to set-up the L1 Emulator:


https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideL1TStage2Instructions#Environment_Setup_with_Integrati


1. Compile and cd to relevant directory:
```
cd CMSSW_X_Y_Z/src
scram b -j4
${CMSSW_BASE}/src/EGTagAndProbe/EGTagAndProbe
```

2. Set flag isMC and isMINIAOD according to sample in test/reEmulL1.py
3. HLT path used specified in python/MCAnalysis_cff.py (MC) or python/tagAndProbe_cff.py (data)
4. Launch test/reEmulL1.py


```
cd test
cmsRun reEmulL1.py
```



### Submit job on the Grid
Modify crab3_config.py: change requestName, inputDataSet, outLFNDirBase, outputDatasetTag, storageSite
```
cd ${CMSSW_BASE}/src/EGTagAndProbe/EGTagAndProbe/test
source /cvmfs/cms.cern.ch/crab3/crab.sh
voms-proxy-init -voms cms
crab submit -c crab3_config.py
```

### Producing turn-on plots
Create configuration file base on test/fitter/run/turnon_production_2018_data.py. Some information regarding this file : 
1. Turnon.N : Number of turn-ons you want to produce in the same file
2. NoFit: false/true if you don't want/want your turn-on to be fitted 
3. Turnon.1.Cut: Trigger threshold (i.e. hasL1_40 or any other hasL1_<threshold> variable stored in the ntuples)
4. Turnon.1.XVar: The variable on the x-axis
5. Turnon.1.SelectionVars : Variable(s) used for selection criteria (these and the x-axis variable described in 4 must also be defined in src/TurnonFit.cpp macro similarly to line i.e. 130)
6. Turnon.1.Selection : Set of selection criteria 


Run with the following commands : 
```
cd ${CMSSW_BASE}/src/EGTagAndProbe/EGTagAndProbe/test/fitter
make clean; make
./fit.exe run/turnonscripts/turnon1.py
```


Plotting macros are stored in **fitter/ForPlotting** directory : 
1. Configure plot_<name>.py scripts accordingly changing the .root input file path and histogram name (defined and produced by the 
previous step). Uses class *TurnOnPlot_DATA.py* defined in for plotting purposes.
2. draw_turnons.C : standalone macro to draw the turn-ons directly from the input .root files. Can run for multiple files with *Run_plotting.C* macro
3. draw_turnons_with_fits.C : standalone macros that draws the turn-ons directly from the input .root files and performs fits to the turnons (useful when python script takes to long)
4. turn_on_production.C : standalone macro to produce the turn-ons directly from the T&P ntuples


In directory **fitter/Resolution** macros used to plot the resolution using the t&p ntuples as inputs can be found.


In directory **fitter/Rates** instructions and macros used to calculate the L1 e/g rate can be found.

