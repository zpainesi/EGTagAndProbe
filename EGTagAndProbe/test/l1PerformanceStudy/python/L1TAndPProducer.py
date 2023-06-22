from __future__ import print_function
from collections import OrderedDict
import ROOT 
from Util  import *
from array import array

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
 
allFnames=getListOfStringsFromConfigs(cfgTxt,"#FNAMES_BEG","#FNAMES_END")
foutName=getValueFromConfigs(cfgTxt,"OutpuFileName","fggHists.root")
treeName=getValueFromConfigs(cfgTxt,"treeName",default="tagsDumper/trees/Data_13TeV_TrippleHTag_0")
tagAndProbeFName=getValueFromConfigs(cfgTxt,"tagAndProbeFName",default="")
emuTreeName=getValueFromConfigs(cfgTxt,"emuTreeName",default="tagsDumper/trees/Data_13TeV_TrippleHTag_0")
outTreeName=getValueFromConfigs(cfgTxt,"outTreeName",default="Data_13TeV_TrippleHTag_0")
processID=getValueFromConfigs(cfgTxt,"processID",default="DATA")
allFriendTrees=getListOfStringsFromConfigs(cfgTxt,"#FTREE_BEG","#FTREE_END")
minPU=int(getValueFromConfigs(cfgTxt,"MinPU",default="-1"))

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

branchesToFill=[
        'Mee','Nvtx',
        'eleTagPt','eleTagEta','eleTagPhi','eleTagCharge',
        'eleProbePt','eleProbeEta','eleProbePhi','eleProbeCharge',
        'eleProbeSclEt','isProbeLoose','isProbeMedium','isProbeTight',
        'l1tPt','l1tEta','l1tPhi','l1tQual','l1tIso',
        'l1tEmuPt','l1tEmuEta','l1tEmuPhi','l1tEmuQual','l1tEmuIso',
        'l1tEmuIsoEt','l1tEmuTowerIEta','l1tEmuTowerIPhi','l1tEmuRawEt','l1tEmuNTT'
]

branches=np.unique(branchesToFill)
filemode="RECREATE"
fout = ROOT.TFile(foutName, filemode)
fout.cd()


ntuple={}
tofill={}
ntuple['all'] = ROOT.TNtuple('TagAndProbe', 'TagAndProbe', ':'.join(branches))
tofill = OrderedDict(zip(branches, [np.nan]*len(branches)))

print("len(branches) : " , len(branches))
th1Store={}
th1Store["nTriggerTower"]= ROOT.TH1F("nTriggerTower","",2000,-0.5,1999.5)
th1Store["nECALTP"]= ROOT.TH1F("nECALTP","",2000,-0.5,1999.5)
th1Store["nHCALTP"]= ROOT.TH1F("nHCALTP","",2000,-0.5,1999.5)
th1Store["TriggerTowerEnergy"]= ROOT.TH1F("TriggerTowerEnergy","",80,-0.25,39.75)
th1Store["ECALTPEnergy"]      = ROOT.TH1F("ECALTPEnergy","",80,-0.25,39.75)
th1Store["HCALTPEnergy"]      = ROOT.TH1F("HCALTPEnergy","",80,-0.25,39.75)
th1Store["isoEt"]      = ROOT.TH1F("isoEt","",50,0.0,50.0)
th1Store["nVtxVsNTT"]        = ROOT.TH2F("nVtxVsNTT","",80,-0.5,79.5,220,-0.5,219.5)
th1Store["nVtxVsIsoEt"]      = ROOT.TH2F("nVtxVsIsoEt","",80,-0.5,79.5,60,0.0,60.0)
th1Store["nTTVsIsoEt"]       = ROOT.TH2F("nTTVsIsoEt","",220,-0.5,219.5,60,0.0,60.0)

def doClosure(unpackedTree,eTree,evtNumber):
    tagAndProbes={'isValid':False}
    evtIdx = unpackedTree.getEventIdx(evtNumber)
    if  evtIdx < 0:
        return  False, tagAndProbes

    unpackedTree.TandPTree.GetEntry(evtIdx)
    tagAndProbes['isValid']=True
    tagAndProbes['tag']={'drMin':0.2,'isMatched':False}
    tagAndProbes['probe']={'drMin':0.2,'isMatched':False}
    tagIdx=-1
    for i in range(eTree.Electron.nElectrons):
        phi_SC=eTree.Electron.phi[i]
        eta_SC=eTree.Electron.eta[i]
        dr_= deltaR( eta_SC , phi_SC ,    unpackedTree.TandPTree.eleTagEta,   unpackedTree.TandPTree.eleTagPhi   )
        if dr_ < tagAndProbes['tag']['drMin']:
            tagAndProbes['tag']['drMin']=dr_
            tagAndProbes['tag']['isMatched']=True
            tagIdx=i    
    for i in range(eTree.Electron.nElectrons):
        if i==tagIdx:
            continue
        phi_SC=eTree.Electron.phi[i]
        eta_SC=eTree.Electron.eta[i]
        dr_= deltaR( eta_SC , phi_SC , unpackedTree.TandPTree.eleProbeEta ,unpackedTree.TandPTree.eleProbePhi   )
        if dr_ < tagAndProbes['probe']['drMin']:
            tagAndProbes['probe']['drMin']=dr_
            tagAndProbes['probe']['isMatched']=True
            probeIdx=i  


    if not ( tagAndProbes['probe']['isMatched']  and  tagAndProbes['tag']['isMatched']) : 
        print("\nPROBLEM ! [ for evt ",evtNumber," entry idx in TandP lookup tree : ",evtIdx,"]",
                "\n\t  Evt. / run / lumi  in TandP    : " ,unpackedTree.TandPTree.EventNumber, " / ", unpackedTree.TandPTree.RunNumber , " / ",unpackedTree.TandPTree.lumi,
                "\n\t  Evt. / run / lumi  in L1Ntuple : " ,eTree.Event.event," / ", eTree.Event.run," / " , eTree.Event.lumi)
        for i in range(eTree.Electron.nElectrons):
            print("\tl1Ntup | ele eta/phi/et/pt",i,eTree.Electron.eta[i],eTree.Electron.phi[i] , eTree.Electron.eta[i] , eTree.Electron.et[i] , eTree.Electron.pt[i] )
    #else:
    #    print("Succesful  !!   [ got evt ",evtNumber," ret val : ",evtIdx,"]",unpackedTree.TandPTree.EventNumber,eTree.Event.event)
    if not tagAndProbes['probe']['isMatched'] :
           print("\t",evtNumber," probe eta/phi/pt : ",unpackedTree.TandPTree.eleProbeEta,   unpackedTree.TandPTree.eleProbePhi , unpackedTree.TandPTree.eleProbePt)
    if not tagAndProbes['tag']['isMatched'] :
        print("\t",evtNumber," tag eta/phi/pt : ",unpackedTree.TandPTree.eleTagEta,   unpackedTree.TandPTree.eleTagPhi , unpackedTree.TandPTree.eleTagPt)
    return tagAndProbes['probe']['isMatched'] and tagAndProbes['tag']['isMatched'], tagAndProbes

def getTagAndProbe(unpackedTree,evtNumber):
    tagAndProbes={'isValid':False,'tAndP':[]}
    evtIdx = unpackedTree.getEventIdx(evtNumber)
    if  evtIdx < 0:
        return evtIdx
    evtIdxs=[evtIdx]
    tagAndProbes['isValid']= True
    x=unpackedTree.TandPTree.GetEntry(evtIdx-1)
    if unpackedTree.TandPTree.EventNumber== evtNumber :
        evtIdxs.append(evtIdx-1)
    x=unpackedTree.TandPTree.GetEntry(evtIdx+1)
    if unpackedTree.TandPTree.EventNumber== evtNumber :
        evtIdxs.append(evtIdx+1)
    
    for idx in evtIdxs:
        x=unpackedTree.TandPTree.GetEntry(idx)
        tAndP = {
            'Mee' : unpackedTree.TandPTree.Mee,
            'eleTagPt' : unpackedTree.TandPTree.eleTagPt,
            'eleTagEta' : unpackedTree.TandPTree.eleTagEta,
            'eleTagPhi' : unpackedTree.TandPTree.eleTagPhi,
            'eleTagCharge' : unpackedTree.TandPTree.eleTagCharge,
            'eleProbePt' : unpackedTree.TandPTree.eleProbePt,
            'eleProbeEta' : unpackedTree.TandPTree.eleProbeEta,
            'eleProbePhi' : unpackedTree.TandPTree.eleProbePhi,
            'eleProbeCharge' : unpackedTree.TandPTree.eleProbeCharge,
            'eleProbeSclEt' : unpackedTree.TandPTree.eleProbeSclEt,
            'isProbeLoose' : unpackedTree.TandPTree.isProbeLoose,
            'isProbeMedium' : unpackedTree.TandPTree.isProbeMedium,
            'isProbeTight' : unpackedTree.TandPTree.isProbeTight,

            'l1tPt' : unpackedTree.TandPTree.l1tPt,
            'l1tEta' : unpackedTree.TandPTree.l1tEta,
            'l1tPhi' : unpackedTree.TandPTree.l1tPhi,
            'l1tQual' : unpackedTree.TandPTree.l1tQual,
            'l1tIso' : unpackedTree.TandPTree.l1tIso,
        
            'l1tEmuPt'         :  -1.0 ,
            'l1tEmuEta'        :  666.0 ,
            'l1tEmuPhi'        :  666.0 ,
            'l1tEmuQual'       :  -1.0 ,
            'l1tEmuIso'        :  8.0 ,
            'l1tEmuIsoEt'      :  -1.0 ,
            'l1tEmuTowerIEta'  :  -666.0 ,
            'l1tEmuTowerIPhi'  :  -666.0 ,
            'l1tEmuRawEt'      :  -1.0 ,
            'l1tEmuNTT'        :  -1.0 ,

        }
        
        tagAndProbes['tAndP'].append( tAndP )

    return tagAndProbes
        
def updateL1EmuBranches(emuTree,tAndP):
    idx=-1
    dr=0.3
    tAndP['l1tEmuPt']        =  -1.0 ,
    tAndP['l1tEmuEta']       =  666.0 ,
    tAndP['l1tEmuPhi']       =  666.0 ,
    tAndP['l1tEmuQual']      =  -1.0 ,
    tAndP['l1tEmuIso']       =  8.0 ,
    tAndP['l1tEmuIsoEt']     =  -1.0 ,
    tAndP['l1tEmuRawEt']     =  -666.0 ,
    tAndP['l1tEmuTowerIEta'] =  -666.0 ,
    tAndP['l1tEmuTowerIPhi'] =  -1.0 ,
    tAndP['l1tEmuNTT']       =  -1.0 ,

    for i in range( eTree.L1Upgrade.nEGs):
        if eTree.L1Upgrade.egBx[i]!=0:
            continue
        dr_= deltaR( tAndP['eleProbeEta'] , tAndP['eleProbePhi'] , eTree.L1Upgrade.egEta[i] , eTree.L1Upgrade.egPhi[i]  )
        if dr_ < dr:
            idx=i
            dr=dr_
    
    if idx>-1:
        tAndP['l1tEmuPt'] = eTree.L1Upgrade.egEt[idx]
        tAndP['l1tEmuEta'] = eTree.L1Upgrade.egEta[idx]
        tAndP['l1tEmuPhi'] = eTree.L1Upgrade.egPhi[idx]
        tAndP['l1tEmuQual'] = eTree.L1Upgrade.egHwQual[idx]
        tAndP['l1tEmuIso'] = eTree.L1Upgrade.egIso[idx]
        tAndP['l1tEmuIsoEt'] = eTree.L1Upgrade.egIsoEt[idx]
        tAndP['l1tEmuRawEt'] = float(eTree.L1Upgrade.egRawEt[idx])
        tAndP['l1tEmuTowerIEta'] = float(eTree.L1Upgrade.egTowerIEta[idx])
        tAndP['l1tEmuTowerIPhi'] = float(eTree.L1Upgrade.egTowerIPhi[idx])
        tAndP['l1tEmuNTT'] = float(eTree.L1Upgrade.egNTT[idx])
    return tAndP


nProcessed=0
eventsLost=0
eventsFound=0
nTandPFound=0
maxEvents_=1e4
totalEvents=0
lostEvents=[]

tagAndProbeUnpackedFile = ROOT.TFile( tagAndProbeFName , "READ")
tagAndProbeUnpackedTree = tagAndProbeUnpackedFile.Get("Ntuplizer/TagAndProbe") 
unpackedTree= unpackedTAndPTree( tagAndProbeUnpackedTree)

for fname in allFnames:
    if maxEvents_<=0:
        break
    print("Opening file : ",fname)
    try:
        simFile = ROOT.TFile.Open(fname,'READ')
    except:
        print("Failed to open File : ",fname)
        continue
    eTree=simFile.Get(treeName)
    eEmuTree=simFile.Get(emuTreeName)
    print("eTree NEntries = ", eTree.GetEntries() , maxEvents )
    print("eEmuTree NEntries = ", eEmuTree.GetEntries() , maxEvents )
    for fTree in allFriendTrees:
        print("Addinf fried tree : ",fTree)
        eTree.AddFriend(fTree)
    maxEvents_ = eTree.GetEntries()
    if(maxEvents >0  and (totalEvents+maxEvents_) > maxEvents):
        maxEvents_= (maxEvents - totalEvents)
    totalEvents+=maxEvents_
    allBranches=[]
    for ky in eTree.GetListOfBranches():
        allBranches.append(ky.GetName())
    for eid in range(maxEvents_):
        eTree.GetEntry(eid)
        eEmuTree.GetEntry(eid)
        nProcessed+=1      
        if(eid%500==0):
            print(f"   Doing i = {nProcessed} [ {eid}] / ",maxEvents_      )
            print("\t Processed  ",nProcessed, " , eventsFound : ",eventsFound," , eventsLost : ",eventsLost , " total number of TAndP : ",nTandPFound)
        isClosed,rslt=doClosure(unpackedTree,eTree,eTree.Event.event)
        
        if not isClosed:
            eventsLost+=1
            continue
        
        tAndPs = getTagAndProbe(unpackedTree,eTree.Event.event  )
        if not tAndPs['isValid']:
            lostEvents.append((eTree.Event.run , eTree.Event.lumi , eTree.Event.event)  )
            continue
        eventsFound+=1
        tofill['RunNumber']  =eTree.Event.run
        tofill['lumi']       =eTree.Event.lumi
        tofill['EventNumber']=eTree.Event.event
        tofill['Nvtx']=eTree.Vertex.nVtx
        
        for tAndP in tAndPs['tAndP']:
            nTandPFound+=1
            tAndP=updateL1EmuBranches(eEmuTree,tAndP)
        
            for ky in tAndP:
                tofill[ky]=tAndP[ky]

            ntuple['all'].Fill(array('f', tofill.values()))

                   

    simFile.Close()           
    print("Closing file : ",fname)

fout.cd()
for ky in th1Store:
    th1Store[ky].Write()
dirN=fout.mkdir("Ntuplizer")
dirN.cd()
for cat in ntuple:
    ntuple[cat].Write()
fout.Purge()
fout.Close()
k=0
print()
for evt in lostEvents:
    print(k,evt[0],evt[1],evt[2])
    k+=1
print("FINAL STAT :  Processed  ",nProcessed, " , eventsFound : ",eventsFound," , eventsLost : ",eventsLost , " total number of TAndP : ",nTandPFound)
print(" File written out  : ",foutName)

