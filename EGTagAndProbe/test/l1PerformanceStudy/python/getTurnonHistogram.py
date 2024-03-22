import uproot
import ROOT
import numpy as np
import matplotlib.pyplot as plt
import mplhep as hep
import argparse
import Util as utl

def getEff(passCounts,failCounts):
    return passCounts/(failCounts +1e-9)



def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('-t',"--tag", help="Tag for file",default='')
    parser.add_argument('-i',"--input", help="input file")
    parser.add_argument('-d',"--dest", help="output destination prefix ",default='./')
    parser.add_argument('-e',"--eT", help="Et ",default=32,type=int)
    parser.add_argument('--loose', help="Do Tight", action='store_true')
    parser.add_argument('--tight', help="Do Loose", action='store_true')
    parser.add_argument('--doEmu', help="Do Emulation Branches", action='store_true')
    parser.add_argument("--opt", help="Option name ",default=None)
    args = parser.parse_args()
    
    output_fname=args.tag

    fileIn=uproot.open(args.input)
    data=fileIn['TagAndProbe']
    arraysToGet=['l1tEmuPt','l1tEmuIso',
                 'eleProbeSclEt','isProbeLoose',
                 'eleProbeEta','eleProbePhi',
                 'eleTagEta','eleTagPhi']
    l1ETVar='l1tEmuPt'
    l1IsoVar='l1tEmuIso'
    if args.opt:
        if not args.doEmu:
            print("Please use re-emulated vars when making isolation turons for specific options")
            exit(1)
        arraysToGet+=[args.opt]
    if not args.doEmu:
        for i in range(len(arraysToGet)):
            arraysToGet[i]=arraysToGet[i].replace('Emu','')
        l1ETVar=l1ETVar.replace("Emu","")
        l1IsoVar=l1IsoVar.replace("Emu","")
        print("Doing Unpacked TurnOns")
    else:
        print("Doing Re-Emulated TurnOns")
    dataStore=data.arrays(arraysToGet)

    dataStore['dR']= np.sqrt( 
                             ( dataStore['eleProbeEta'] - dataStore['eleTagEta'] )**2 + 
                             ( dataStore['eleProbePhi'] - dataStore['eleTagPhi'] )**2 
                            ) 
    
    preselection_mask = dataStore['eleProbeEta'] < 2.5
    preselection_mask = np.logical_and( preselection_mask , dataStore['dR'] > 0.6 )
    preselection_mask = np.logical_and( preselection_mask , dataStore['isProbeLoose'] > 0.5 ) 
    
    print("Et Threshold for the trigger set to : ",args.eT)
    output_fname+=f'_L1Et{args.eT}GeV'
    pass_mask= np.logical_and(preselection_mask , dataStore[l1ETVar] > args.eT )
    

    isoCondition = np.ones(len(pass_mask),dtype=bool)
    if args.loose:
        print("Isolation condition set to Loose")
        output_fname+=f'_loose'
        isoCondition = np.logical_or( dataStore[l1IsoVar]==2 ,  dataStore[l1IsoVar]==3 )
    elif args.tight:
        print("Isolation condition set to Tight")
        output_fname+=f'_tight'
        isoCondition = np.logical_or( dataStore[l1IsoVar]==1 ,  dataStore[l1IsoVar]==3 )
    elif args.opt:
        print(f"Isolation condition set to {args.opt}")
        output_fname+=f'_{args.opt}'
        isoCondition = dataStore[args.opt] > 0.5
    else:
        print("No isolation selection ")
    pass_mask= np.logical_and(pass_mask         , isoCondition  )
    
    
    pT_all = dataStore['eleProbeSclEt'][preselection_mask]
    pT_pass= dataStore['eleProbeSclEt'][pass_mask]
    
    pTBins= np.arange(0,100,1)
    pTBinCenters=0.5*(pTBins[:-1]+pTBins[1:])
    #count_all ,_=np.histogram(pT_all ,bins=pTBins)
    #count_pass,_=np.histogram(pT_pass,bins=pTBins)
    #eff=getEff(count_pass,count_all)
    
    h_all =np.histogram(pT_all ,bins=pTBins)
    h_pass=np.histogram(pT_pass,bins=pTBins)
    th1_all=utl.getTH1FromNumpHist(h_all)
    th1_pass=utl.getTH1FromNumpHist(h_pass)
    eff_x,eff_y,eff_y_errL,eff_y_errH=utl.getRatiosAndErrors(th1_pass,th1_all)
    
    f=plt.figure(figsize=(6.5,8))
    
    #plt.scatter(pTBinCenters,eff,c='b',s=6)
    
    plt.errorbar(eff_x,eff_y,yerr=[eff_y_errL,eff_y_errH])
    

    hep.cms.label('internal',data=True,year='2022, EraG',com=13.6,fontsize=18)
    plt.ylim([-0.01,1.02])
    plt.ylabel('$\epsilon$')
    plt.xlabel('E$_{T}$')
    plt.grid(which='both')
    TextLabel="L1 e$/\gamma$"
    if not args.doEmu:
        TextLabel+=", Unpacked"
        output_fname+=f'_upkd'
    else:
        TextLabel+=", Re-Emulated"
        output_fname+=f'_reemuld'

    TextLabel+="\nLoose Offline Probe Electrons\nL1 E$_{T}$ >"+f" {args.eT}"
    if args.loose:
        TextLabel+="\nLoose Isolation"
    elif args.tight:
        TextLabel+="\nTight Isolation"
    elif args.opt:
        TextLabel+=f"\n{args.opt}"
    plt.text(90,0.1,TextLabel,horizontalalignment='right',verticalalignment='baseline',fontsize=12)
    plt.yticks(np.arange(0,1.0,0.05),minor=True)
    plt.xticks(np.arange(0,100,5),minor=True)
    plt.grid(which='both',color='g',alpha=0.3)
    fname=(args.dest+'/effVsPt_'+output_fname+'.png').replace("__","_")
    print("Output plot saved as  ",fname)
    plt.savefig(fname,bbox_inches='tight')


if __name__=='__main__':
    main()
