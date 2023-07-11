NJOBS=${1-5}
FILES_PER_JOB=${2-1}
VER=4p0
RUNS_PER_JOB=1
echo NJOBS : $NJOBS
echo FILES_PER_JOB : $FILES_PER_JOB
echo ""

declare -a SourceFiles=(\
"fileList/runScanEraC_miniAODv4_haclZS_v2p0.json" \
)

declare -a tagArr=(\
"eraCMiniAODv4_hcalZS_$VER" \
)

for i in "${!tagArr[@]}"; do 
    echo $i : ${jobArr[$i]}
    src=${SourceFiles[$i]}
    TAG=${tagArr[$i]}
    DEST=${Destination[$i]}
#    set -x
    ./misc/condorJobMakerGenericRunBased.py \
        --exe                /grid_mnt/t3storage3/athachay/l1egamma/triggerPerformance/CMSSW_12_3_5/src/EGTagAndProbe/EGTagAndProbe/test/l1PerformanceStudy/python/L1TAndPProducer.py \
        --runFileMap         $src  \
        --runScript          misc/runPython.tpl.sh \
        --cfg                misc/cfg/l1TagAndP.cfg \
        --dest               results/l1TagAndPMaker/$TAG/ \
        --jn                 $NJOBS \
        --rn                 $RUNS_PER_JOB \
        --fn                 $FILES_PER_JOB \
        --maxEvt             -1 \
        --tag                l1TagAndP_$TAG \
        --maxMeterialize     300

#    set +x

done    

