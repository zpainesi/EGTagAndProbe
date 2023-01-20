
version=1p1

./misc/condorJobMakerGeneric.py \
    --exe                /grid_mnt/t3storage3/athachay/l1egamma/triggerPerformance/CMSSW_12_3_5/src/EGTagAndProbe/EGTagAndProbe/test/l1PerformanceStudy/python/eventScanner.py \
    --fsrc               fileList/eraG.fls \
    --runScript          misc/runPython.tpl.sh \
    --cfg                misc/cfg/eventScan.cfg \
    --dest               results/l1RunNumbberScan/$version/ \
    --jn                 10000 \
    --fn                 1 \
    --maxEvt             -1 \
    --tag                runScan_$version \
    --maxMeterialize     300
