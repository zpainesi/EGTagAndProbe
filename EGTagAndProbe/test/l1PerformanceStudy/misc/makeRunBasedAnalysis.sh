NJOBS=${1-5}
FILES_PER_JOB=${2-1}
echo NJOBS : $NJOBS
echo FILES_PER_JOB : $FILES_PER_JOB
echo ""

declare -a SourceFiles=(\
"misc/lists/runScanEraB_hcalZS_v2p0.json" \
"misc/lists/runScanEraC_hcalZS_v2p0.json" \
"misc/lists/runScanEraB_caloV6_v2p0.json" \
"misc/lists/runScanEraC_caloV6_v2p0.json" \
)

declare -a tagArr=(\
"eraB_hcalZS_v2p0" \
"eraC_hcalZS_v2p0" \
"eraB_calo6_v2p0" \
"eraC_calo6_v2p0" \
)

for i in "${!tagArr[@]}"; do 
    echo $i : ${jobArr[$i]}
    src=${SourceFiles[$i]}
    TAG=${tagArr[$i]}
    DEST=${Destination[$i]}
#    set -x
    ./misc/condorJobMakerGenericRunBased.py \
        --exe                /grid_mnt/t3storage3/athachay/l1egamma/triggerPerformance/CMSSW_12_3_5/src/EGTagAndProbe/EGTagAndProbe/test/l1PerformanceStudy/python/L1RecoAnalyzer.py \
        --runFileMap         $src  \
        --runScript          misc/runPython.tpl.sh \
        --cfg                misc/cfg/l1PerformanceV1.cfg \
        --dest               results/l1HealthAnalysis/$TAG/ \
        --jn                 $NJOBS \
        --fn                 $FILES_PER_JOB \
        --maxEvt             -1 \
        --tag                l1Health_$TAG \
        --maxMeterialize     300

#    set +x

done    

