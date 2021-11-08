#include "Resolution.h"

using namespace std;

void produceResolution(string infile,string ofileName,string prefix="",bool doEmulation=false,Long64_t maxEvents=100000);

void resolution_production(string infile="",string ofileName="",string prefix="",Long64_t maxEvents=100000)
{

    //produceResolution(infile,ofileName,prefix,maxEvents);


 int doFull=1;
//produceResolution(
//    "/grid_mnt/t3storage3/athachay/l1egamma/triggerPerformance/CMSSW_10_6_25/src/EGTagAndProbe/EGTagAndProbe/test/fitter/EGamma_Run2018ABCD_reduced.root",
//                    "resolution.root",
//                    "data2018_",
//                    false,
//                   doFull*20000);
//
//produceResolution(
//    "/grid_mnt/t3storage3/athachay/l1egamma/triggerPerformance/CMSSW_10_6_25/src/EGTagAndProbe/EGTagAndProbe/test/fitter/DYToLL_M-50_TuneCP5_14TeV-pythia8.root",
//                    "resolution.root",
//                    "run3MC_",
//                    false,
//                   doFull*20000);

produceResolution("/grid_mnt/t3storage3/athachay/l1egamma/data/run3MC/NTuple_crab_4841files_fromSweta.root",
 //   "/grid_mnt/t3storage3/athachay/l1egamma/triggerPerformance/CMSSW_10_6_25/src/EGTagAndProbe/EGTagAndProbe/test/fitter/TAndP_ReEmulatedRun3MC_caloParams_2018_v1_2_V2.root",
                    "resolution.root",
                    "run3MCReEmulatedV2_",
                    true,
                   doFull*2000);
}


void produceResolution(string infile,string ofileName,string prefix="",bool doEmulation=false,Long64_t maxEvents=100000)
{


	// gROOT->LoadMacro("/afs/cern.ch/work/c/ckoraka/codes/setTDRStyle_teliko.C");
	// setTDRStyle_teliko();
    
    TFile *f2=new TFile(infile.c_str());
	TTree *t1=(TTree*)(f2->Get("Ntuplizer/TagAndProbe"));

        float   eleProbePt;
        float   eleProbeEta;
        float   eleProbePhi;
        float   eleProbeSclEt;
        float   eleTagPt;
        float   eleTagEta;
        float   eleTagPhi;
        float   hltPt;
        float   hltEta;
        float   hltPhi;
        float   l1tPt;
        float   l1tEta;
        float   l1tPhi;
        int     eleProbeCharge;
        int     eleTagCharge;
        int     allEl_Size;
        int     isProbeLoose;


	/////////////////////////////////////////////////////////////////////////////////////////////////////////	
    
    std::map<string,resolutionMeasurement * > resolutionMap;

    const Int_t PtBINS = 14; 
	Double_t PtEdges[PtBINS + 1] = {5., 10., 14., 18., 20., 23., 26., 30., 40., 50., 60., 70., 80., 90., 100.};
    const Int_t dPtBINS = 4000; 
	Double_t dPtEdges[dPtBINS + 1];
    for(int i=0;i<=dPtBINS;i++) dPtEdges[i] = -0.0005 + 0.001*i ;
	
    const Int_t EtaBINS = 23; 
	Double_t EtaEdges[EtaBINS + 1] = {0.,0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0, 1.1, 1.2, 1.305, 1.479, 1.6, 1.7, 1.8, 1.9, 2.0, 2.1, 2.2, 2.3, 2.4};
    const Int_t dEtaBINS = 200; 
	Double_t dEtaEdges[dEtaBINS + 1];
    for(int i=0;i<=dEtaBINS;i++) dEtaEdges[i] = -1.005 + 0.01*i ;
	
    const Int_t PhiBINS = 68; 
	Double_t PhiEdges[PhiBINS + 1] ;
    for(int i=0;i<=PhiBINS;i++) PhiEdges[i] = -3.4 + 0.1*i ;
    const Int_t dPhiBINS = 200; 
	Double_t dPhiEdges[dPhiBINS + 1];
    for(int i=0;i<=dPhiBINS;i++) dPhiEdges[i] = -1.005 + 0.01*i ;
	
    
    resolutionMap["EtaVsdEta"] = new resolutionMeasurement("EtaVsdEta",EtaBINS,EtaEdges,dEtaBINS,dEtaEdges);
    resolutionMap["PhiVsdPhi"] = new resolutionMeasurement("PhiVsdPhi",PhiBINS,PhiEdges,dPhiBINS,dPhiEdges);
    resolutionMap["EtVsdEt"  ] = new resolutionMeasurement("EtVsdEt",PtBINS,PtEdges,dPtBINS,dPtEdges);
    resolutionMap["EtaVsdEt" ] = new resolutionMeasurement("EtaVsdEt",EtaBINS,EtaEdges,dPtBINS,dPtEdges);

    //res_vs_et_barrel->GetYaxis()->SetTitle("FWHM / (E_{T}^{e#gamma, L1}/E_{T}^{e#gamma, offline})_{at maximum}");
	//res_vs_et_barrel->GetXaxis()->SetTitle("E_{T}^{e#gamma, offline} [GeV]");
    //res_vs_eta_inclusive->GetXaxis()->SetTitle("|#eta^{e#gamma, offline}|");
	//res_vs_eta_inclusive->GetYaxis()->SetTitle("FWHM / (E_{T}^{e#gamma, L1}/E_{T}^{e#gamma, offline})_{at maximum}");

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////

    //t1->SetBranchAddress("eleProbePt",&eleProbePt);
    t1->SetBranchAddress("eleProbeEta",&eleProbeEta);
    t1->SetBranchAddress("eleProbePhi",&eleProbePhi);
    t1->SetBranchAddress("eleProbeSclEt",&eleProbeSclEt);
    //t1->SetBranchAddress("eleTagPt",&eleTagPt);
    t1->SetBranchAddress("eleTagEta",&eleTagEta);
    t1->SetBranchAddress("eleTagPhi",&eleTagPhi);
    //t1->SetBranchAddress("hltPt",&hltPt);
    //t1->SetBranchAddress("hltEta",&hltEta);
    //t1->SetBranchAddress("hltPhi",&hltPhi);
    if( doEmulation)
    {
        t1->SetBranchAddress("l1tEmuPt",&l1tPt);
        t1->SetBranchAddress("l1tEmuEta",&l1tEta);
        t1->SetBranchAddress("l1tEmuPhi",&l1tPhi);
    }
    else
    {
        t1->SetBranchAddress("l1tPt",&l1tPt);
        t1->SetBranchAddress("l1tEta",&l1tEta);
        t1->SetBranchAddress("l1tPhi",&l1tPhi);
    }
    t1->SetBranchAddress("isProbeLoose",&isProbeLoose);
    
    int nentries = t1->GetEntries();

    std::cout<<" Total number events available for processing : "<<nentries<<"\n";
    if(maxEvents>0) nentries= nentries > maxEvents ? maxEvents : nentries;
    std::cout<<" Total number events scheduled for processing : "<<nentries<<"\n";
	int bar=0;
    int end=0;
	int incl=0;

    for (int jentry=0; jentry<nentries; jentry++){
        
        t1->GetEntry(jentry);
		
        if(jentry%10000==0) cout<<"Processing Entry "<<jentry<<" / "<<nentries<<"  [ "<<100.0*jentry/nentries<<"  % ]  "<<endl;

		//if(!(eleProbeSclEt>60. && eleProbeSclEt<70.)) continue;
		if(isProbeLoose==1){

            resolutionMap["EtaVsdEta"] ->Fill(eleProbeEta,abs(eleProbeEta),l1tEta -  eleProbeEta);
            resolutionMap["EtaVsdEt"]  ->Fill(eleProbeEta,abs(eleProbeEta),l1tPt/eleProbeSclEt);
            resolutionMap["EtVsdEt"]   ->Fill(eleProbeEta,eleProbeSclEt,l1tPt/eleProbeSclEt);
            resolutionMap["PhiVsdPhi"] ->Fill(eleProbeEta,eleProbePhi,l1tPhi-eleProbePhi);
			

		}

     }

    
    for (std::map<string,resolutionMeasurement * >::iterator it=resolutionMap.begin() ; it!=resolutionMap.end(); ++it)
	{
        
        std::cout<<"Processing "<<it->first<<"  \n";
        auto &resolutionItem = *(it->second); 
        resolutionItem.scaleBareVarHists();
        resolutionItem.calculateResolutions();
    }



    TFile *file =new TFile((prefix+ofileName).c_str(),"RECREATE");

    for (std::map<string,resolutionMeasurement * >::iterator it=resolutionMap.begin() ; it!=resolutionMap.end(); ++it)
	{
        
        std::cout<<"Writing to file !! [  "<<it->first<<" ] \n";
        auto &resolutionItem = *(it->second); 
        resolutionItem.Write();
    }

    

    file->Purge();
    file->Close();


}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	/*	

	*/

