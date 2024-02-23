import uproot,json,argparse
import ROOT,os
import numpy as np
import matplotlib.pyplot as plt
import mplhep as hep

def getEff(passCounts,failCounts):
    return passCounts/(failCounts +1e-9)

def main():

    parser = argparse.ArgumentParser()
    parser.add_argument('-i',"--inputFile", help="Input File",default='/home/aravind/Documents/dumpX/export_file_v6QuadWithOffset4.root')
    parser.add_argument("-o","--dest", help="destination To Use", default='workarea/results/' )
    parser.add_argument("--noAug", help="dont augment dataset",default=False,action='store_true')
    parser.add_argument("--export", help="export the augmented dataset",default=False,action='store_true')
    parser.add_argument("--exportOnlyBase", help="export the base dataset",default=False,action='store_true')
    parser.add_argument("--opt",help="Option to process",default='ISO_LUT_Progression_v3_582_10p0_0p7_40p0')
    parser.add_argument("--tag",help="tag for the  plotts",default='')
    parser.add_argument("--labelName"   ,help="label name for the legends ",default="Opt : [ 10.0, 0.7, 40.0]")
    args = parser.parse_args()
    
    print("Processing file  : ",args.inputFile)
    file=uproot.open(args.inputFile)
    data=file['TagAndProbe']
     
    optionList=[]
    print("Available Options ")
    for i in data.keys():
        if i.startswith('ISO'):
            print(i,end=" | " )
            optionList.append(i)
    print()
    
    # In[72]:
    
    dataStore=data.arrays(['l1tEmuPt','l1tEmuNTT','l1tEmuRawEt','l1tEmuTowerIEta',
                           'l1tPt','l1tEta','l1tIso','Nvtx',
                           'eleProbeSclEt','isProbeLoose','eleProbePt',
                           'eleProbeEta','eleProbePhi',
                           'eleTagEta','eleTagPhi']+optionList)
     
    dataStore['dR']= np.sqrt( 
                             ( dataStore['eleProbeEta'] - dataStore['eleTagEta'] )**2 + 
                             ( dataStore['eleProbePhi'] - dataStore['eleTagPhi'] )**2 
                            ) 
    
    base_mask = abs(dataStore['eleProbeEta']) < 2.5
    base_mask = np.logical_and( base_mask , dataStore['dR'] > 0.6 )
    base_mask = np.logical_and( base_mask , dataStore['isProbeLoose'] > 0.5 ) 
    
    isLoose =  np.logical_or(dataStore['l1tIso']==2 , dataStore['l1tIso']==3)
    isTight =  np.logical_or(dataStore['l1tIso']==1 , dataStore['l1tIso']==3)
    

    opt      =args.opt       
    labelName=args.labelName 
    print("processing option : ",opt)
    print("            with  option label : ",labelName)
    foutName=opt+'/'+args.tag
    
    puWindows=[0.0,15.0,25.0,35.0,42.0,50.0,58.0,64.0]
    
    # ## Efficiency in PU Bins
    
    pTBins= np.arange(0,60,3)
    pTBins=np.concatenate([pTBins , np.arange(60,98,8)])
    pTBinCenters=0.5*(pTBins[:-1]+pTBins[1:])
     
    dirname=f'{args.dest}/{foutName}/'
    if not os.path.exists(dirname):
        os.system('mkdir -p '+dirname)
    
    pu1=0
    pu2=20
    l1Et=30
    
    f,ax=plt.subplots(1,3,figsize=(15,5))
    for i in range(len(puWindows)-1):
        pu1=puWindows[i]
        pu2=puWindows[i+1]
        mask = np.logical_and(base_mask, dataStore['Nvtx']> pu1)
        mask = np.logical_and(mask, dataStore['Nvtx']<pu2)
    
        pass_mask  = np.logical_and(mask,dataStore['l1tPt'] > l1Et )
        pT_all = dataStore['eleProbePt'][mask]
        pT_pass= dataStore['eleProbePt'][pass_mask]
        count_all ,_=np.histogram(pT_all ,bins=pTBins)
        count_pass,_=np.histogram(pT_pass,bins=pTBins)
        eff=getEff(count_pass,count_all)
        effErr=eff*(1.0/np.sqrt(1e-9 + count_all) + 1.0/np.sqrt(1e-9 + count_pass))
        ax[0].errorbar(pTBinCenters,eff,effErr,fmt='.',label=f"{pu1} < PU < {pu2}")
    
        pass_mask  = np.logical_and(mask,dataStore['l1tPt'] > l1Et )
        pass_mask  = np.logical_and(pass_mask, isLoose )
        pT_all = dataStore['eleProbePt'][mask]
        pT_pass= dataStore['eleProbePt'][pass_mask]
        count_all ,_=np.histogram(pT_all ,bins=pTBins)
        count_pass,_=np.histogram(pT_pass,bins=pTBins)
        eff=getEff(count_pass,count_all)
        effErr=eff*(1.0/np.sqrt(1e-9 + count_all) + 1.0/np.sqrt(1e-9 + count_pass))
        ax[1].errorbar(pTBinCenters,eff,effErr,fmt='.',label=f"{pu1} < PU < {pu2}")
    
    
        pass_mask  = np.logical_and(mask,dataStore['l1tPt'] > l1Et )
        pass_mask  = np.logical_and(pass_mask, isTight)
        pT_all = dataStore['eleProbePt'][mask]
        pT_pass= dataStore['eleProbePt'][pass_mask]
        count_all ,_=np.histogram(pT_all ,bins=pTBins)
        count_pass,_=np.histogram(pT_pass,bins=pTBins)
        eff=getEff(count_pass,count_all)
        effErr=eff*(1.0/np.sqrt(1e-9 + count_all) + 1.0/np.sqrt(1e-9 + count_pass))
        ax[2].errorbar(pTBinCenters,eff,effErr,fmt='.',label=f"{pu1} < PU < {pu2}")
    
    for i in range(3):
        ax[i].set_xlabel('Offline Electron E$_{T}$ ',loc='right',fontsize=10)
        ax[i].set_ylabel("$\epsilon$",loc='top',fontsize=10)
        hep.cms.label('internal',data=True,com=13.6,year='2023 C',lumi=9.7,ax=ax[i],fontsize=12)
        ax[i].set_ylim([0.0,1.01])
        ax[i].set_xlim([0.0,100.01])
        ax[i].legend(fontsize=12)
        ax[i].tick_params(axis='both', which='major', labelsize=8)
        ax[i].grid(which='both',alpha=0.4)
    
    ax[0].text(90.0,0.65,"L1 E$_{T}$ > "+str(l1Et)+" GeV" ,fontsize=12,horizontalalignment='right')
    ax[1].text(90.0,0.6,"L1 E$_{T}$ > "+str(l1Et)+" GeV\nLoose Isolation" ,fontsize=12,horizontalalignment='right')
    ax[2].text(90.0,0.6,"L1 E$_{T}$ > "+str(l1Et)+f" GeV\n{labelName}" ,fontsize=12,horizontalalignment='right')
    fname=f'{dirname}/UnpackedAndOption_vsPU.png'
    f.savefig(fname,bbox_inches='tight')
    print(f"Unpacked Vs Option plot made at {fname}")    

    pu1=0
    pu2=20
    l1Et=12
    BarrelEndEtaVal=0
    EndcapEndEtaVal=16
    f,ax=plt.subplots(1,4,figsize=(20.0,6))
    ax=np.ndarray.flatten(ax)
    doLoose=False
    doOpt=True
    doInclusive=False
    
    minX=[0,10,17,24]
    maxX=[9,16,23,31]
    for i in range(len(puWindows)-1):
        pu1=puWindows[i]
        pu2=puWindows[i+1]
    
        if doOpt: 
            for j in range(4):
                mask = np.logical_and(base_mask, dataStore['Nvtx']>= pu1)
                mask = np.logical_and(mask, dataStore['Nvtx']<pu2)
                mask = np.logical_and(mask, abs(dataStore['l1tEmuTowerIEta']) <= maxX[j])
                mask = np.logical_and(mask, abs(dataStore['l1tEmuTowerIEta']) >= minX[j])
    
                pass_mask  = np.logical_and(mask,dataStore['l1tPt'] >= l1Et )
                pass_mask  = np.logical_and(pass_mask,dataStore[opt] >= 0.5 )
    
                pT_all = dataStore['eleProbePt'][mask]
                pT_pass= dataStore['eleProbePt'][pass_mask]
                count_all ,_=np.histogram(pT_all ,bins=pTBins)
                count_pass,_=np.histogram(pT_pass,bins=pTBins)
                eff=getEff(count_pass,count_all)
                effErr=eff*(1.0/np.sqrt(1e-9 + count_all) + 1.0/np.sqrt(1e-9 + count_pass))
                ax[j].errorbar(pTBinCenters,eff,effErr,fmt='.',label=f"{pu1} < PU < {pu2}")
            
    
        if doLoose:
            for j in range(4):
                mask = np.logical_and(base_mask, dataStore['Nvtx']>= pu1)
                mask = np.logical_and(mask, dataStore['Nvtx']<pu2)
                mask = np.logical_and(mask, abs(dataStore['l1tEmuTowerIEta']) <= maxX[j])
                mask = np.logical_and(mask, abs(dataStore['l1tEmuTowerIEta']) >= minX[j])
    
                pass_mask  = np.logical_and(mask,dataStore['l1tPt'] >= l1Et )
                pass_mask  = np.logical_and(pass_mask,isLoose )
    
                pT_all = dataStore['eleProbePt'][mask]
                pT_pass= dataStore['eleProbePt'][pass_mask]
                count_all ,_=np.histogram(pT_all ,bins=pTBins)
                count_pass,_=np.histogram(pT_pass,bins=pTBins)
                eff=getEff(count_pass,count_all)
                effErr=eff*(1.0/np.sqrt(1e-9 + count_all) + 1.0/np.sqrt(1e-9 + count_pass))
                ax[j].errorbar(pTBinCenters,eff,effErr,fmt='.',label=f"{pu1} < PU < {pu2}")
            
    
        if doInclusive: 
            for j in range(4):
                mask = np.logical_and(base_mask, dataStore['Nvtx']>= pu1)
                mask = np.logical_and(mask, dataStore['Nvtx']<pu2)
                mask = np.logical_and(mask, abs(dataStore['l1tEmuTowerIEta']) <= maxX[j])
                mask = np.logical_and(mask, abs(dataStore['l1tEmuTowerIEta']) >= minX[j])
    
                pass_mask  = np.logical_and(mask,dataStore['l1tPt'] >= l1Et )
    
                pT_all = dataStore['eleProbePt'][mask]
                pT_pass= dataStore['eleProbePt'][pass_mask]
                count_all ,_=np.histogram(pT_all ,bins=pTBins)
                count_pass,_=np.histogram(pT_pass,bins=pTBins)
                eff=getEff(count_pass,count_all)
                effErr=eff*(1.0/np.sqrt(1e-9 + count_all) + 1.0/np.sqrt(1e-9 + count_pass))
                ax[j].errorbar(pTBinCenters,eff,effErr,fmt='.',label=f"{pu1} < PU < {pu2}")
    
    for i in range(4):
        ax[i].set_xlabel('Offline Electron E$_{T}$ ',loc='right',fontsize=10)
        ax[i].set_ylabel("$\epsilon$",loc='top',fontsize=10)
        hep.cms.label('internal',data=True,com=13.6,year='2023 C',lumi=9.7,ax=ax[i],fontsize=12)
        ax[i].set_ylim([0.0,1.01])
        ax[i].set_xlim([0.0,100.01])
        ax[i].legend(fontsize=12,loc='lower right')
        ax[i].tick_params(axis='both', which='major', labelsize=8)
        ax[i].grid(which='both',alpha=0.4)
        textStr ="L1 E$_{T}$ >= "+str(l1Et)+" GeV"
        if doLoose:
            textStr+="\nLoose Unpacked"
        if doOpt:
            textStr+=f"\n{labelName}"
        textStr+=f"\n{minX[i]} < $|I\eta$|<{maxX[i]}" 
        ax[i].text(90.0,0.55,textStr,fontsize=12,horizontalalignment='right')
        
    fname=f'{dirname}/tunonsInSuperIEtaRings.png'
    f.savefig(fname,bbox_inches='tight')    
    print(f"Turnons in Super IEta Rings saved at {fname}")
 
if __name__=='__main__':
    main( )

