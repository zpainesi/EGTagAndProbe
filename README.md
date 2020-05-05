# EGTagAndProbe
Set of tools to evaluate L1EG trigger performance on T&amp;P

Based on TauTagAndProbe package developed by L. Cadamuro & O. Davignon

### Install instructions
```
cmsrel CMSSW_X_Y_Z
cd CMSSW_X_Y_Z/src
cmsenv
git clone https://github.com/ckoraka/EGTagAndProbe.git
scram b -j4
```

### Producing TagAndProbe ntuples with unpacked L1EG (no re-emulation)
Set flag isMC and isMINIAOD according to sample in test/test.py
HLT path used specified in python/MCAnalysis_cff.py (MC) or python/tagAndProbe_cff.py (data)
Launch test.py

### Producing TagAndProbe ntuples with emulated L1EG
Set flag isMC and isMINIAOD according to sample in test/reEmulL1.py
HLT path used specified in python/MCAnalysis_cff.py (MC) or python/tagAndProbe_cff.py (data)
Launch reEmulL1.py


### Submit job on the Grid
Modify crab3_config.py: change requestName, inputDataSet, outLFNDirBase, outputDatasetTag, storageSite
```
cd ${CMSSW_BASE}/src/EGTagAndProbe/EGTagAndProbe/test
source /cvmfs/cms.cern.ch/crab3/crab.sh
voms-proxy-init -voms cms
crab submit -c crab3_config.py
```

### Producing turn-on plots
Create configuration file base on test/fitter/run/file.par
```
cd ${CMSSW_BASE}/src/EGTagAndProbe/EGTagAndProbe/test/fitter
make clean; make
./fit.exe run/file.par
```
