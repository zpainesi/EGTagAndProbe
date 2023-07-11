from __future__ import print_function
from collections import OrderedDict
import ROOT 
from Util  import *

import os,sys

cfgFileName=''
if len(sys.argv) <2:
    print("Usage\n\t ~$python recoAnalyzer.py <configFile>\n")
    exit(1)
else:
    cfgFileName=sys.argv[1]

maxEvtsSuperSeeder=-1
if len(sys.argv) >2:
    maxEvtsSuperSeeder=int(sys.argv[2])
print("Loading cfg file ",cfgFileName)

f=open(cfgFileName,'r')
cfgTxt=f.readlines()
f.close()

headers=getListOfStringsFromConfigs(cfgTxt,"#HEADER_BEG","#HEADER_END")
for header in headers:
    print("Loading cfg file ",header)
    f=open(header,'r')
    tmp=f.readlines()
    f.close()
    for l in tmp:
        cfgTxt.append(l)
 
allFnames = getListOfStringsFromConfigs(cfgTxt,"#FNAMES_BEG","#FNAMES_END")
foutName  = getValueFromConfigs(cfgTxt,"OutpuFileName","runs.txt")
treeName  = getValueFromConfigs(cfgTxt,"treeName",default="tagsDumper/trees/Data_13TeV_TrippleHTag_0")
processID = getValueFromConfigs(cfgTxt,"processID",default="DATA")

print("allFnames   :  ",              allFnames)
print("foutName   :  ",               foutName)
print("processID   :  ",              processID)
print("treeName   :  ",               treeName)

maxEvents=-1
tmp_=getValueFromConfigs(cfgTxt,"MaxEvents")
if tmp_!='':
    maxEvents=int(tmp_)

for i in allFnames:
    print(" file : ",i)
if(maxEvtsSuperSeeder > 0):
    maxEvents=maxEvtsSuperSeeder
print("maxevents : ",maxEvents)


totalEvents=0
nProcessed=0
allRuns={}
for fname in allFnames:
    print("Opening file : |"+fname+"|")
    simFile = ROOT.TFile.Open(fname,'READ')
    eTree=simFile.Get(treeName)
    print(" NEntries = ", eTree.GetEntries())
    if not eTree:
        eTree=simFile.Get('tagsDumper/trees/Data_13TeV_TrippleHTag_0')
    maxEvents_ = eTree.GetEntries()
    if(maxEvents >0  and (totalEvents+maxEvents_) > maxEvents):
        maxEvents_= (maxEvents - totalEvents)
    totalEvents+=maxEvents_
    allRuns[fname]=set()
    for eid in range(maxEvents_):
        eTree.GetEntry(eid)
        if(eid%50==0):
            print("   Doing i = ",eid," / ",maxEvents_,
                 )
        allRuns[fname].add(eTree.Event.run)    
        nProcessed+=1

    simFile.Close()           
    print("Closing file : ",fname)


fout=open(foutName , 'w')

for fname in allRuns:
    if len(allRuns[fname]) < 1:
        continue
    fout.write(fname+",")
    for r in allRuns[fname]:
        fout.write(str(r)+",")
    fout.write("\n")
fout.close()

print(" Processed  ",nProcessed)
print(" File written out  : ",foutName)

