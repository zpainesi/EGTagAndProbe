
version=v1p0

./misc/condorJobMakerGeneric.py \
    --exe                /grid_mnt/t3storage3/athachay/l1egamma/triggerPerformance/CMSSW_12_3_5/src/EGTagAndProbe/EGTagAndProbe/test/l1PerformanceStudy/python/eventScanner.py \
    --fsrc               misc/lists/rawRecoEraG_v1.fls \
    --runScript          misc/runPython.tpl.sh \
    --cfg                misc/cfg/eventScan.cfg \
    --dest               results/l1RunNumbberScan/$version/ \
    --jn                 10000 \
    --fn                 1 \
    --maxEvt             -1 \
    --tag                runScanG_$version \
    --maxMeterialize     300

./misc/condorJobMakerGeneric.py \
    --exe                /grid_mnt/t3storage3/athachay/l1egamma/triggerPerformance/CMSSW_12_3_5/src/EGTagAndProbe/EGTagAndProbe/test/l1PerformanceStudy/python/eventScanner.py \
    --fsrc               misc/lists/rawRecoEraF_v1.fls \
    --runScript          misc/runPython.tpl.sh \
    --cfg                misc/cfg/eventScan.cfg \
    --dest               results/l1RunNumbberScan/$version/ \
    --jn                 10000 \
    --fn                 1 \
    --maxEvt             -1 \
    --tag                runScanF_$version \
    --maxMeterialize     300

