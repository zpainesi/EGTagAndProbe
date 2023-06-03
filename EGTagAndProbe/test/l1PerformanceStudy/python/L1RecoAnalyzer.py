from __future__ import print_function
from collections import OrderedDict
import ROOT 
import json
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
 
allFnames=getListOfStringsFromConfigs(cfgTxt,"#FNAMES_BEG","#FNAMES_END")
foutName=getValueFromConfigs(cfgTxt,"OutpuFileName","fggHists.root")
treeName=getValueFromConfigs(cfgTxt,"treeName",default="tagsDumper/trees/Data_13TeV_TrippleHTag_0")
emuTreeName=getValueFromConfigs(cfgTxt,"emuTreeName",default="tagsDumper/trees/Data_13TeV_TrippleHTag_0")
outTreeName=getValueFromConfigs(cfgTxt,"outTreeName",default="Data_13TeV_TrippleHTag_0")
processID=getValueFromConfigs(cfgTxt,"processID",default="DATA")
certificationJson=getValueFromConfigs(cfgTxt,"certificationJson",default=None)
allFriendTrees=getListOfStringsFromConfigs(cfgTxt,"#FTREE_BEG","#FTREE_END")
minPU=int(getValueFromConfigs(cfgTxt,"MinPU",default="-1"))

certificationData=None
if certificationJson:
    with open(certificationJson,'r') as f:
        certificationData=json.load(f)

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


nNoHtoGammaGamma=0
nNoHHto4B=0
totalEvents=0
branchesToFill=[]
branchesToFill.append("nonResonantMVA_v0")
branchesToFill.append("peakingMVA_v0")
branchesToFill.append("nonResonantMVA_v1")
branchesToFill.append("nonResonantMVA_v2")
branchesToFill.append("peakingMVA_v1")

branches=np.unique(branchesToFill)
filemode="RECREATE"
fout = ROOT.TFile(foutName, filemode)
fout.cd()

branches_skimmed=['M1jj','M2jj','CMS_hgg_mass','peakingMVA_v1','nonResonantMVA_v1','dZ','r_HH','weight','nonResonantMVA_v2']

ntuple={}
tofill={}
#ntuple['all'] = ROOT.TNtuple(outTreeName+'_NOTAG', outTreeName, ':'.join(branches_skimmed))
#tofill = OrderedDict(zip(branches, [np.nan]*len(branches)))

skimmedDataDict = OrderedDict(zip(branches_skimmed, [np.nan]*len(branches_skimmed)))
kyList = [ ky for ky in tofill ]

print("len(branches) : " , len(branches))

def getCMVAScore(x,edges):
    if x > edges[-1]:
        return 1.0
    n=edges.searchsorted(x)
    return (n-1.0*( edges[n] - x)/(edges[n] - edges[n-1])  )/edges.shape[0]

sumWeights=ROOT.TH1F("sumEvts","sumEvts",1,0.0,1.0)
sumWeights.SetCanExtend(ROOT.TH1.kAllAxes)

th1Store={}
th1Store["nTriggerTower"]= ROOT.TH1F("nTriggerTower","",2000,-0.5,1999.5)
th1Store["nECALTP"]= ROOT.TH1F("nECALTP","",2000,-0.5,1999.5)
th1Store["nHCALTP"]= ROOT.TH1F("nHCALTP","",2000,-0.5,1999.5)

th1Store["nTriggerTowerOverNVtx"]= ROOT.TH1F("nTriggerTowerOverNVtx","",400,0.0,49.5)
th1Store["nECALTPOverNVtx"]      = ROOT.TH1F("nECALTPOverNVtx","",120,-0.5,29.5)
th1Store["nHCALTPOverNVtx"]      = ROOT.TH1F("nHCALTPOverNVtx","",400,-0.5,49.5)

th1Store["TriggerTowerEnergy"]= ROOT.TH1F("TriggerTowerEnergy","",80,-0.25,39.75)
th1Store["ECALTPEnergy"]      = ROOT.TH1F("ECALTPEnergy","",80,-0.25,39.75)
th1Store["HCALTPEnergy"]      = ROOT.TH1F("HCALTPEnergy","",80,-0.25,39.75)

th1Store["isoEtPU45To50"]      = ROOT.TH1F("isoEtPU45To50","",50,0.0,50.0)
th1Store["isoEtPU55To70"]      = ROOT.TH1F("isoEtPU55To70","",50,0.0,50.0)
th1Store["isoEtPU50To60"]      = ROOT.TH1F("isoEtPU50To60","",50,0.0,50.0)
th1Store["isoEt"]      = ROOT.TH1F("isoEt","",50,0.0,50.0)

th1Store["nVtxVsNTT"]        = ROOT.TH2F("nVtxVsNTT","",80,-0.5,79.5,220,-0.5,219.5)
th1Store["nVtxVsIsoEt"]      = ROOT.TH2F("nVtxVsIsoEt","",80,-0.5,79.5,60,0.0,60.0)
th1Store["nTTVsIsoEt"]       = ROOT.TH2F("nTTVsIsoEt","",220,-0.5,219.5,60,0.0,60.0)

iEtaBoundaries = [  -33,-28,-22,-17,-14, -9, -2,
                    3, 10 , 15, 18, 23, 29, 34]

def GetBoundaries(x,vals):
    for j,i in enumerate(vals[:-1]):
        if i <= x  and x < vals[j+1]:
            return i,vals[j+1]

def GetL1EGIdx(eTree):
    # Many Hardcoded cuts !!

    rslt={'isValid':False}

    if eTree.Electron.nElectrons != 2:
        return rslt
    rslt['idxs']=[]
    for i in [0,1]:
        phi_SC=eTree.Electron.phi_SC[i]
        eta_SC=eTree.Electron.eta_SC[i]
        dr=0.3
        idx=-1
        for i in range( eTree.L1Upgrade.nEGs):
            if eTree.L1Upgrade.egBx[i]!=0:
                continue
            dr_= deltaR( eta_SC , phi_SC , eTree.L1Upgrade.egEta[i] , eTree.L1Upgrade.egPhi[i]  )
            if dr_ < dr:
                idx=i
        if idx==-1:
            continue
        if idx not in rslt['idxs'] :
            rslt['idxs'].append(idx)
    
    if len(rslt['idxs'])==2:
        rslt['isValid']=True
    return rslt

for j in range(1,len(iEtaBoundaries)):
    i=j-1
    nme= "nECALTP_inIEtaRings_"+str(iEtaBoundaries[i])+"_"+str(iEtaBoundaries[j]) ; th1Store[nme] = ROOT.TH1F(nme,"",200,-0.5,199.5)
    nme= "nHCALTP_inIEtaRings_"+str(iEtaBoundaries[i])+"_"+str(iEtaBoundaries[j]) ; th1Store[nme] = ROOT.TH1F(nme,"",200,-0.5,199.5)
    nme= "nTT_inIEtaRings_"    +str(iEtaBoundaries[i])+"_"+str(iEtaBoundaries[j]) ; th1Store[nme] = ROOT.TH1F(nme,"",200,-0.5,199.5)
    
    nme= "nECALTPoverNVTX_inIEtaRings_"+str(iEtaBoundaries[i])+"_"+str(iEtaBoundaries[j]) ; th1Store[nme] = ROOT.TH1F(nme,"", 50,0.0, 5.0)
    nme= "nHCALTPoverNVTX_inIEtaRings_"+str(iEtaBoundaries[i])+"_"+str(iEtaBoundaries[j]) ; th1Store[nme] = ROOT.TH1F(nme,"", 50,0.0, 5.0)
    nme= "nTToverNVTX_inIEtaRings_"    +str(iEtaBoundaries[i])+"_"+str(iEtaBoundaries[j]) ; th1Store[nme] = ROOT.TH1F(nme,"", 50,0.0, 5.0)

nProcessed=0
maxEvents_=1e4
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
    if not eTree:
        eTree=simFile.Get('tagsDumper/trees/Data_13TeV_TrippleHTag_0')
    for fTree in allFriendTrees:
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

        print(  "eid : ",eid," / ",maxEvents_,"/ ",maxEvents," | ", 
                " event : "  ,eTree.Event.run ,
                " nHCALTP : ",eTree.CaloTP.nHCALTP,
                " C nTT " , eEmuTree.L1CaloTower.nTower,
                " isGoodRunLumi : ", isGoodRunLumi( certificationData , eTree.Event.run , eTree.Event.lumi)
             )
        if not isGoodRunLumi( certificationData , eTree.Event.run , eTree.Event.lumi):
            continue

        if(eid%500==0):
            print("   Doing i = ",eid," / ",maxEvents_,
                 )
        if(eTree.L1Upgrade.nEGs > 0):
           th1Store["nVtxVsNTT"].Fill( eTree.Vertex.nVtx , eTree.L1Upgrade.egNTT[0]  )

        rslt=GetL1EGIdx(eTree)
        if rslt['isValid']:
            a,b=rslt['idxs'][0],rslt['idxs'][1]
            th1Store["isoEt"].Fill( eTree.L1Upgrade.egIsoEt[a] )                
            th1Store["isoEt"].Fill( eTree.L1Upgrade.egIsoEt[b] )                
            
            if np.random.randint(0,2) < 1:
                th1Store["nVtxVsIsoEt"].Fill( eTree.Vertex.nVtx , eTree.L1Upgrade.egIsoEt[a] )                
                if(eTree.L1Upgrade.nEGs > 0):
                    th1Store["nTTVsIsoEt"].Fill( eTree.L1Upgrade.egNTT[0] , eTree.L1Upgrade.egIsoEt[a] )                
            else:
                th1Store["nVtxVsIsoEt"].Fill( eTree.Vertex.nVtx , eTree.L1Upgrade.egIsoEt[b] )                
                if(eTree.L1Upgrade.nEGs > 0):
                    th1Store["nTTVsIsoEt"].Fill( eTree.L1Upgrade.egNTT[0] , eTree.L1Upgrade.egIsoEt[b] )                
            if eTree.Vertex.nVtx >=45  and eTree.Vertex.nVtx <=50: 
                th1Store["isoEtPU45To50"].Fill( eTree.L1Upgrade.egIsoEt[a] )                
                th1Store["isoEtPU45To50"].Fill( eTree.L1Upgrade.egIsoEt[b] )                
            if eTree.Vertex.nVtx >=50  and eTree.Vertex.nVtx <=60: 
                th1Store["isoEtPU50To60"].Fill( eTree.L1Upgrade.egIsoEt[a] )                
                th1Store["isoEtPU50To60"].Fill( eTree.L1Upgrade.egIsoEt[b] )                
            if eTree.Vertex.nVtx >=55  and eTree.Vertex.nVtx <=70: 
                th1Store["isoEtPU55To70"].Fill( eTree.L1Upgrade.egIsoEt[a] )                
                th1Store["isoEtPU55To70"].Fill( eTree.L1Upgrade.egIsoEt[b] )                
 
        if eTree.Vertex.nVtx < minPU:
            continue
         
        th1Store["nTriggerTower"].Fill(eEmuTree.L1CaloTower.nTower)
        th1Store["nHCALTP"].Fill(eTree.CaloTP.nHCALTP)
        th1Store["nECALTP"].Fill(eTree.CaloTP.nECALTP)
        
        th1Store["nTriggerTowerOverNVtx"].Fill(eEmuTree.L1CaloTower.nTower/eTree.Vertex.nVtx)
        th1Store["nHCALTPOverNVtx"].Fill(eTree.CaloTP.nHCALTP/eTree.Vertex.nVtx)
        th1Store["nECALTPOverNVtx"].Fill(eTree.CaloTP.nECALTP/eTree.Vertex.nVtx)

        x=np.array(eTree.L1CaloTower.iet)
        for e in x:
            th1Store["TriggerTowerEnergy"].Fill(e)
        x=np.array(eTree.CaloTP.hcalTPet)
        for e in x:
            th1Store["HCALTPEnergy"].Fill(e)
        x=np.array(eTree.CaloTP.ecalTPet)
        for e in x:
            th1Store["ECALTPEnergy"].Fill(e)

        nProcessed+=1      
        x=np.array(eTree.CaloTP.hcalTPieta)
        counts,edges=np.histogram(x,bins=iEtaBoundaries)
        for j in range(1,len(iEtaBoundaries)):
            i=j-1
            ct=counts[i]
            nme= "nHCALTP_inIEtaRings_"+str(iEtaBoundaries[i])+"_"+str(iEtaBoundaries[j]) ;
            th1Store[nme].Fill(ct)
            nme= "nHCALTPoverNVTX_inIEtaRings_"+str(iEtaBoundaries[i])+"_"+str(iEtaBoundaries[j]) ;
            th1Store[nme].Fill(ct/eTree.Vertex.nVtx)
        
        x=np.array(eTree.CaloTP.ecalTPieta)
        counts,edges=np.histogram(x,bins=iEtaBoundaries)
        for j in range(1,len(iEtaBoundaries)):
            i=j-1
            ct=counts[i]
            nme= "nECALTP_inIEtaRings_"+str(iEtaBoundaries[i])+"_"+str(iEtaBoundaries[j]) ;
            th1Store[nme].Fill(ct)
            nme= "nECALTPoverNVTX_inIEtaRings_"+str(iEtaBoundaries[i])+"_"+str(iEtaBoundaries[j]) ;
            th1Store[nme].Fill(ct/eTree.Vertex.nVtx)

        x=np.array(eEmuTree.L1CaloTower.ieta)
        counts,edges=np.histogram(x,bins=iEtaBoundaries)
        for j in range(1,len(iEtaBoundaries)):
            i=j-1
            ct=counts[i]
            nme= "nTT_inIEtaRings_"+str(iEtaBoundaries[i])+"_"+str(iEtaBoundaries[j]) ;
            th1Store[nme].Fill(ct)
            nme= "nTToverNVTX_inIEtaRings_"+str(iEtaBoundaries[i])+"_"+str(iEtaBoundaries[j]) ;
            th1Store[nme].Fill(ct/eTree.Vertex.nVtx)
 
#        tdata_={}
#        for i in range( eTree.L1CaloTower.nTower ) :
#            x,y = GetBoundaries(abs(eTree.L1CaloTower.ieta[i]) , iEtaBoundaries )
#            if (x,y) not in tdata_:
#                tdata_[(x,y)]=0
#            tdata_[(x,y)]   += 1 
#           print("filling x,y" ,x,y,1)
#        print(tdata_)

#        for keys in tdata_:
#            x,y=keys
            #th1Store["nTToverNVTX_inIEtaRings_"+str(x)+"_"+str(y)].Fill( tdata_[keys] /eTree.Vertex.nVtx )
#          print("filling x,y --> " ,x,y,tdata_[keys])
           

    simFile.Close()           
    print("Closing file : ",fname)

fout.cd()
for ky in th1Store:
    th1Store[ky].Write()
for cat in ntuple:
    ntuple[cat].Write()
fout.Close()
print(" Processed  ",nProcessed)
print(" File written out  : ",foutName)

