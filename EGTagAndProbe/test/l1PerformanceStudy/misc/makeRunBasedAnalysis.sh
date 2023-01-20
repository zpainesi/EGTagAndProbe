
version=2p4

./misc/condorJobMakerGenericRunBased.py \
    --exe                /grid_mnt/t3storage3/athachay/l1egamma/triggerPerformance/CMSSW_12_3_5/src/EGTagAndProbe/EGTagAndProbe/test/l1PerformanceStudy/python/L1RecoAnalyzer.py \
    --runFileMap         misc/runFileMap.json  \
    --runs               `cat misc/runList2` \
    --runScript          misc/runPython.tpl.sh \
    --cfg                misc/cfg/l1PerformanceV1.cfg \
    --dest               results/l1HealthAnalysis/$version/ \
    --jn                 10000 \
    --fn                 1 \
    --maxEvt             20000 \
    --tag                l1Health_$version \
    --maxMeterialize     300
