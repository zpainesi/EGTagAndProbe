./misc/condorJobMakerGeneric.py \
      --exe /grid_mnt/t3storage3/athachay/l1egamma/triggerPerformance/CMSSW_12_3_5/src/EGTagAndProbe/EGTagAndProbe/test/l1Remulation/cmsCfgs/TAandPReEmulated_run3.py  \
      --fsrc parentmap.json \
      --runScript misc/scripts/runCmd.tpl.sh \
      --dest results/eraG/reEmul/v0/ \
      --jn 10 \
      --fn 1 \
      --maxEvt -1 \
      --tag recalibReEmul \
      --jobType l1ReEmmul \
      --maxMeterialize 250

