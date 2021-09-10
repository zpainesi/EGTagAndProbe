#!/usr/bin/env python
import os
import json
from subprocess import Popen, PIPE
import sys 

    #print "usage ./condorJobFileMaker_reEmuL1.py PrimaryFileList.txt  <PREFIX>" 
    #print "Enter the file with primary file list"


def splitInChunks(l, n):
    """Yield successive n-sized chunks from l."""
    blocks=[]
    for i in range(0, len(l), n):
        blocks.append( l[i:i+n] )
    return blocks

def splitInBlocks (l, n):
    """split the list l in n blocks of equal size"""
    k = len(l) / n
    r = len(l) % n
    i = 0
    blocks = []
    while i < len(l):
        if len(blocks)<r:
            blocks.append(l[i:i+k+1])
            i += k+1
        else:
            blocks.append(l[i:i+k])
            i += k

    return blocks

###########
njobs = 200
nPrimaryFIlesPerJob=1
maxEvents=-1
fname="file.txt"
prefix=''
destination = '/eos/home-a/athachay/workarea/data/l1egamma/emulatedTandPNtuples/DYToLL_M-50_TuneCP5_14TeV-pythia8/FlatPU30to80FEVT_112X_mcRun3_2021_realistic_v16-v2/caloParams_2018_v1_2_cfi'
if len(sys.argv) > 1 :
    fname=sys.argv[1]
if len(sys.argv) > 2 :
    njobs=int(sys.argv[2])
if len(sys.argv) > 3 :
    nPrimaryFIlesPerJob=int(sys.argv[3])
if len(sys.argv) > 4 :
    prefix=sys.argv[4]


pwd=os.environ['PWD']
proxy_path=os.environ['X509_USER_PROXY']
home=os.environ['HOME']

#filelist = open("Data_SingleMu_2016RunB_PromptRecov2_1Luglio.txt")

condorScriptString="\
executable = $(filename)\n\
output = $Fp(filename)cdr.stdout\n\
error = $Fp(filename)cdr.stderr\n\
log = $Fp(filename)cdr.log\n\
+JobFlavour = \"workday\"\
"

condorScript=open(prefix+'JobSubmit.sub','w')
condorScript.write(condorScriptString)

runScriptTxt="\
#!/bin/bash\n\
set -x\n\
source /cvmfs/cms.cern.ch/cmsset_default.sh \n\
export HOME="+home+"\n\
export X509_USER_PROXY="+proxy_path+"\n\
cd "+pwd+" \n\
eval `scramv1 runtime -sh`\n\
@@CMD\n\
if [ $? -eq 0 ]; then \n\
    echo OK\n\
    mv *.root "+destination+"\n\
    mv @@RUNSCRIPTNAME @@RUNSCRIPTNAME.sucess\n\
else\n\
    echo FAIL\n\
fi\n\
"



JSONfile = "/home/llr/cms/davignon/json_DCSONLY.txt"

###########

#os.system ('source /opt/exp_soft/cms/t3/t3setup')

filelist = open(fname,'r')
files = [f.strip() for f in filelist]
fileblocks=[]
#fileblocks = splitInBlocks (files, njobs)
fileblocks = splitInChunks(files, nPrimaryFIlesPerJob)
filelist.close()
print "Input has" , len(files) , "files" 

if njobs > len(files) : njobs = len(files)
print("Making ",njobs," ",prefix+"Jobs ")
baseFolder = prefix + "Jobs"
if not os.path.exists(baseFolder):
    os.system('mkdir -p ' + baseFolder )

for idx, block in enumerate(fileblocks):
    if not idx< njobs:
        break
    print "Making ",idx+1
    folder= baseFolder +'/'+ 'Jobs_'+str(idx)
    if not os.path.exists( folder ):
        os.system('mkdir -p ' + folder)
    secondaryFileName = folder+"/secondaryFileList_split_"+str(idx)+".txt"
    f = open(secondaryFileName, 'w')
    myList = list()

    for currentFile in block:
        #print "finding parent of: ",currentFile
        command_das = "dasgoclient --query=\"parent file="+currentFile+"\" --limit=0"
        #print "    > command is: " , command_das

        pipe = Popen(command_das, shell=True, stdout=PIPE)        
        pCount=0
        for line in pipe.stdout:
            line = line.rstrip("\n")
            myList.append(line)
            pCount+=1
            #print >> f, line
        print " \t ",pCount," parent files found !"

    unique = []
    [unique.append(item) for item in myList if item not in unique]
    #print unique

    for line in unique:
        print >> f, line
    
    targetFolder = folder
    outRootName = targetFolder + '/ReEmulNtuple_' + str(idx) + '.root'
    outJobName  = folder + '/job_' + str(idx) + '.sh'
    outListName = folder + "/filelist_" + str(idx) + ".txt"
    outLogName  = os.getcwd() + "/" + folder + "/log_" + str(idx) + ".txt"

    jobfilelist = open(outListName, 'w')
    for f in block: jobfilelist.write(f+"\n")
    jobfilelist.close()

    cmsRun = "cmsRun reEmulL1.py maxEvents="+str(maxEvents)+" inputFiles_load="+outListName + " secondaryFilesList="+secondaryFileName+" " + " outputFile="+outRootName
    
    # For data
    #cmsRun = "cmsRun reEmulL1.py maxEvents=-1 inputFiles_load="+outListName + " secondaryFilesList="+secondaryFileName+" " + " outputFile="+outRootName + " JSONfile="+JSONfile + " >& " + outLogName
    
    rTxt=runScriptTxt.replace("@@CMD",cmsRun)
    rTxt=rTxt.replace("@@DIRNAME",folder)
    runScriptNameFull=pwd+'/'+outJobName
    rTxt=rTxt.replace("@@RUNSCRIPTNAME",runScriptNameFull)
    skimjob = open (outJobName, 'w')
    skimjob.write (rTxt)
    skimjob.close ()

    os.system ('chmod +x ' + outJobName)
    condorScript.write("queue filename matching ("+outJobName+")\n")    

