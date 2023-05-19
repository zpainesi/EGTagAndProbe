
version=3p0

./misc/condorJobMakerGenericRunBased.py \
    --exe                /grid_mnt/t3storage3/athachay/l1egamma/triggerPerformance/CMSSW_12_3_5/src/EGTagAndProbe/EGTagAndProbe/test/l1PerformanceStudy/python/L1RecoAnalyzer.py \
    --runFileMap         misc/lists/runFileMap_eraFG_v1.json  \
    --runScript          misc/runPython.tpl.sh \
    --cfg                misc/cfg/l1PerformanceV1.cfg \
    --dest               results/l1HealthAnalysis/$version/ \
    --jn                 10000 \
    --fn                 1 \
    --maxEvt             20000 \
    --tag                l1Health_v1_$version \
    --maxMeterialize     300
#    --runs               `cat misc/runList2` \
