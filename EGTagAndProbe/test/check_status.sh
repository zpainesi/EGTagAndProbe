#!/bin/bash

# Load CRAB and CMSSW environment once
source /cvmfs/cms.cern.ch/crab3/crab.sh 
eval `scramv1 runtime -sh` # equivalent to cmsenv

for EGamma in {0..3}; do
  for run in C D F; do

    # For C and F, loop over both v1 and v2
    if [[ "$run" == "C" ]]; then # can add: || "$run" == 'XXX'
      for ver in v1 v2; do
        crab resubmit workarea/crab_EGamma${EGamma}_Run2025${run}-ZElectron-PromptReco-${ver}__211025__2
      done
    else
      # For B, D, E use only v1
      crab resubmit workarea/crab_EGamma${EGamma}_Run2025${run}-ZElectron-PromptReco-v1__211025__2
    fi

  done
done
