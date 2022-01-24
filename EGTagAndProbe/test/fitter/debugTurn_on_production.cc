#include <vector>
#include "Efficiency.h"
#include "Resolution.h"
#include <chrono>

using namespace std;
#define ECAL_BARREL_ETAMAX 1.4

//#include "secondary_peak.h"
//#include "/afs/cern.ch/work/c/ckoraka/codes/setTDRStyle_teliko.C"

void produceTurnOns(string infile,string ofileName,string prefix="", bool doEmulationBranches=false,Long64_t maxEvents=100000);
void debugTurn_on_production(string infile="",string ofileName="",string prefix="",Long64_t maxEvents=100000)
{
    int doFull=-1;
    
    //  Low pT Customization
    
     produceTurnOns("/grid_mnt/t3storage3/athachay/l1egamma/data/run3MC/lowPtReg/V0/lowPtRegV0_TandPNtuple.root",
                    "turnon.root",
                    "run3MCLowPtRegressionV0DebugEmul_",
                    true,
                   -1*doFull*0.5e6);
     return;
     produceTurnOns("/grid_mnt/t3storage3/athachay/l1egamma/data/run3MC/lowPtReg/V1/lowPtRegV1_TandPNtuple.root",
                    "turnon.root",
                    "run3MCLowPtRegressionV1DebugEmul_",
                    true,
                   -1*doFull*0.5e6);
 
     produceTurnOns("/grid_mnt/t3storage3/athachay/l1egamma/data/run3MC/lowPtReg/V2/lowPtRegV2_TandPNtuple.root",
                    "turnon.root",
                    "run3MCLowPtRegressionV2DebugEmul_",
                    true,
                   -1*doFull*0.5e6);
 
     produceTurnOns("/grid_mnt/t3storage3/athachay/l1egamma/data/run3MC/lowPtReg/V3/lowPtRegV3_TandPNtuple.root",
                    "turnon.root",
                    "run3MCLowPtRegressionV3DebugEmul_",
                    true,
                   -1*doFull*0.5e6);

     produceTurnOns("/grid_mnt/t3storage3/athachay/l1egamma/data/run3MC/lowPtReg/V4/lowPtRegV4_TandPNtuple.root",
                    "turnon.root",
                    "run3MCLowPtRegressionV4DebugEmul_",
                    true,
                   -1*doFull*0.5e6);


    
    return ;

    produceTurnOns("/grid_mnt/t3storage3/athachay/l1egamma/triggerPerformance/CMSSW_10_6_25/src/EGTagAndProbe/EGTagAndProbe/test/fitter/haddTmp/MergedFile0.root",
                    "turnon.root",
                    "run3MCLowPtDebugEmul_",
                    true,
                   -1*doFull*0.5e6);


    produceTurnOns("/grid_mnt/t3storage3/athachay/l1egamma/data/run3MC/NTuple_crab_4841files_fromSweta.root",
                    "turnon.root",
                    "run3MCDebugEmul_",
                    true,
                   -1*doFull*0.5e6);
    return;
    produceTurnOns("/grid_mnt/t3storage3/athachay/l1egamma/data/run3MC/NTuple_crab_4841files_fromSweta.root",
                    "turnon.root",
                    "run3MCDebug_",
                    false,
                   -1*doFull*1e6);
}

void produceTurnOns(string infile,string ofileName,string prefix="",bool doEmulationBranches=false,Long64_t maxEvents=100000)
{

	//////		For plotting 		////////////////////////////////////////////////////
	//gROOT->LoadMacro("/afs/cern.ch/work/c/ckoraka/codes/setTDRStyle_teliko.C");
	//setTDRStyle_teliko();

	const Int_t XBINS = 38; 

	Double_t xEdges[XBINS + 1] = {1., 3., 5., 7., 9.,  10., 12., 15., 18., 20., 22., 24., 26., 28., 29., 30., 31., 32., 33., 34., 35., 36., 37., 38., 39., 40., 41., 42., 43., 45., 50., 60., 70., 100., 200., 300., 400., 600., 1000.};
     std::vector<TH1D*> efficHists;

     std::map<string,efficiencyMeasurement * > triggers;
     std::map<string,resolutionMeasurement * > resolutionMap;
    
    ///////////////////////////////////////////////////////////////////////////////////////////

	double dR,dphi,phi1,phi2;	
    float eleProbePt	;
    float eleProbeEta	;
    float eleProbePhi	;
    float eleTagEta		;
    float eleTagPhi		;
    float l1tPt 		;
    float l1tEta		;
    float l1tPhi		;
    Int_t Nvtx			;
    Int_t isProbeLoose	;
    float eleProbeSclEt	;


    TFile *f=new TFile(infile.c_str(),"READ");
    TTree *t1=(TTree*)(f->Get("Ntuplizer/TagAndProbe"));
	
    t1->SetBranchAddress("eleProbePt"  ,&eleProbePt			);
    t1->SetBranchAddress("eleProbeEta"  ,&eleProbeEta			);
    t1->SetBranchAddress("eleProbePhi"  ,&eleProbePhi			);
    t1->SetBranchAddress("eleTagEta"    ,&eleTagEta		    	);
    t1->SetBranchAddress("eleTagPhi"    ,&eleTagPhi			    );
    t1->SetBranchAddress("Nvtx"         ,&Nvtx			        );
    t1->SetBranchAddress("isProbeLoose" ,&isProbeLoose			);
    t1->SetBranchAddress("eleProbeSclEt",&eleProbeSclEt			);
    
    string emulationSelection="";
    string emulationSelection2="_";
    if( doEmulationBranches)
    {
        emulationSelection="Emu";
        emulationSelection2="";
        t1->SetBranchAddress("l1tEmuPt" ,&l1tPt);
        t1->SetBranchAddress("l1tEmuEta",&l1tEta);
        t1->SetBranchAddress("l1tEmuPhi",&l1tPhi);
    }
    else
    {
        t1->SetBranchAddress("l1tPt" ,&l1tPt);
        t1->SetBranchAddress("l1tEta",&l1tEta);
        t1->SetBranchAddress("l1tPhi",&l1tPhi);
    }


    int   hasL1_30, hasL1_looseiso_30, hasL1_tightiso_30 ;
    t1->SetBranchAddress(("hasL1"+emulationSelection+"_30"         ).c_str(),&hasL1_30			    );
    t1->SetBranchAddress(("hasL1"+emulationSelection+"_tightiso"+emulationSelection2+"30").c_str(),&hasL1_tightiso_30 );
    t1->SetBranchAddress(("hasL1"+emulationSelection+"_looseiso"+emulationSelection2+"30").c_str(),&hasL1_tightiso_30 );
    
    triggers["L1_barrel_30"] = new efficiencyMeasurement("L1_barrel_30",XBINS,xEdges);
    triggers["L1_barrel_looseiso_30"] = new efficiencyMeasurement("L1_barrel_looseiso_30",XBINS,xEdges);
    triggers["L1_barrel_tightiso_30"] = new efficiencyMeasurement("L1_barrel_tightiso_30",XBINS,xEdges);
    triggers["L1_ecap_30"] = new efficiencyMeasurement("L1_ecap_30",XBINS,xEdges);
    triggers["L1_ecap_looseiso_30"] = new efficiencyMeasurement("L1_ecap_looseiso_30",XBINS,xEdges);
    triggers["L1_ecap_tightiso_30"] = new efficiencyMeasurement("L1_ecap_tightiso_30",XBINS,xEdges);
    triggers["L1_30"] = new efficiencyMeasurement("L1_30",XBINS,xEdges);
    triggers["L1_looseiso_30"] = new efficiencyMeasurement("L1_looseiso_30",XBINS,xEdges);
    triggers["L1_tightiso_30"] = new efficiencyMeasurement("L1_tightiso_30",XBINS,xEdges);
    
    int   hasL1_40, hasL1_looseiso_40, hasL1_tightiso_40 ;
    t1->SetBranchAddress(("hasL1"+emulationSelection+"_40"         ).c_str(),&hasL1_40		    );
    t1->SetBranchAddress(("hasL1"+emulationSelection+"_tightiso"+emulationSelection2+"40").c_str(),&hasL1_tightiso_40 );
    t1->SetBranchAddress(("hasL1"+emulationSelection+"_looseiso"+emulationSelection2+"40").c_str(),&hasL1_tightiso_40 );

    triggers["L1_barrel_40"] = new efficiencyMeasurement("L1_barrel_40",XBINS,xEdges);
    triggers["L1_barrel_looseiso_40"] = new efficiencyMeasurement("L1_barrel_looseiso_40",XBINS,xEdges);
    triggers["L1_barrel_tightiso_40"] = new efficiencyMeasurement("L1_barrel_tightiso_40",XBINS,xEdges);
    triggers["L1_ecap_40"] = new efficiencyMeasurement("L1_ecap_40",XBINS,xEdges);
    triggers["L1_ecap_looseiso_40"] = new efficiencyMeasurement("L1_ecap_looseiso_40",XBINS,xEdges);
    triggers["L1_ecap_tightiso_40"] = new efficiencyMeasurement("L1_ecap_tightiso_40",XBINS,xEdges);
    triggers["L1_40"] = new efficiencyMeasurement("L1_40",XBINS,xEdges);
    triggers["L1_looseiso_40"] = new efficiencyMeasurement("L1_looseiso_40",XBINS,xEdges);
    triggers["L1_tightiso_40"] = new efficiencyMeasurement("L1_tightiso_40",XBINS,xEdges);
    

    const int XBINS_forPt=900;
    Double_t xEdges_forPt[XBINS_forPt+1];
    for(int i=0;i<=XBINS_forPt;i++) xEdges_forPt[i]= 0.0 + i*1.0;
    const int XBINS_forDPt=1000;
    Double_t xEdges_forDPt[XBINS_forDPt+1];
    for(int i=0;i<=XBINS_forDPt;i++) xEdges_forDPt[i]= 0.0 + i*0.05;
    const int XBINS_forInvDPt=512*5;
    Double_t xEdges_forInvDPt[XBINS_forInvDPt+1];
    for(int i=0;i<=XBINS_forInvDPt;i++) xEdges_forInvDPt[i]= 0.0 + i*2.0*5.0/XBINS_forInvDPt;
    std::cout<<" xEdges_forInvDPt "<<xEdges_forInvDPt[0]<<"  , "<<xEdges_forInvDPt[1]<<"\n";

    //xEdges_forDPt[1001] = 10.0;
    //xEdges_forDPt[1002] = 20.0;
    //xEdges_forDPt[1003] = 30.0;
    //xEdges_forDPt[1004] = 40.0;
    //xEdges_forDPt[1005] = 50.0;
    //xEdges_forDPt[1006] = 60.0;
    //xEdges_forDPt[1007] = 70.0;
    //xEdges_forDPt[1008] = 90.0;
    //xEdges_forDPt[1009] = 110.0;
    //xEdges_forDPt[1010] = 120.0;
    //xEdges_forDPt[1011] = 140.0;
    //xEdges_forDPt[1012] = 150.0;
    //xEdges_forDPt[1013] = 150.0;
    
    const int XBINS_forEta=60;
    Double_t xEdges_forEta[XBINS_forEta+1];
    for(int i=0;i<=XBINS_forEta;i++) xEdges_forEta[i]= -3.0 + i*0.1;
    const int XBINS_forPhi=60;
    Double_t xEdges_forPhi[XBINS_forPhi+1];
    for(int i=0;i<=XBINS_forPhi;i++) xEdges_forPhi[i]= -3.5 + i*0.1;
    const int XBINS_forMass=120;

    std::map<string, TH1D > histMap;

    histMap["recoL1_pt"]       = TH1D("recoL1_pt", "recoL1_pt",XBINS_forPt,xEdges_forPt);
    histMap["recoL1_eta"]      = TH1D("recoL1_eta","recoL1_eta",XBINS_forEta,xEdges_forEta);
    histMap["recoL1_phi"]      = TH1D("recoL1_phi","recoL1_phi",XBINS_forPhi,xEdges_forPhi);    
    
    histMap["recoOffline_pt"]  = TH1D("recoOffline_pt", "recoOffline_pt",XBINS_forPt,xEdges_forPt);
    histMap["recoOffline_scEt"]= TH1D("recoOffline_scEt", "recoOffline_scEt",XBINS_forPt,xEdges_forPt);
    histMap["recoOffline_eta"] = TH1D("recoOffline_eta","recoOffline_eta",XBINS_forEta,xEdges_forEta);
    histMap["recoOffline_phi"] = TH1D("recoOffline_phi","recoOffline_phi",XBINS_forPhi,xEdges_forPhi);    
 
    histMap["recoL1_has30_pt"]       = TH1D("recoL1_has30_pt", "recoL1_has30_pt",XBINS_forPt,xEdges_forPt);
    histMap["recoL1_has30_scEt"]     = TH1D("recoL1_has30_scEt", "recoL1_has30_scEt",XBINS_forPt,xEdges_forPt);
    histMap["recoL1_has30_eta"]      = TH1D("recoL1_has30_eta","recoL1_has30_eta",XBINS_forEta,xEdges_forEta);
    histMap["recoL1_has30_phi"]      = TH1D("recoL1_has30_phi","recoL1_has30_phi",XBINS_forPhi,xEdges_forPhi);    
    
    histMap["recoOffline_has30_pt"]  = TH1D("recoOffline_has30_pt", "recoOffline_has30_pt",XBINS_forPt,xEdges_forPt);
    histMap["recoOffline_has30_eta"] = TH1D("recoOffline_has30_eta","recoOffline_has30_eta",XBINS_forEta,xEdges_forEta);
    histMap["recoOffline_has30_phi"] = TH1D("recoOffline_has30_phi","recoOffline_has30_phi",XBINS_forPhi,xEdges_forPhi);    


    std::cout<<" making resolutions !\n";
    resolutionMap["PtVsL1Pt"]  = new resolutionMeasurement("PtVsL1Pt",XBINS_forPt,xEdges_forPt,XBINS_forPt,xEdges_forPt);
    resolutionMap["PtVsdL1Pt"]  = new resolutionMeasurement("PtVsdL1Pt",XBINS_forPt,xEdges_forPt,XBINS_forDPt,xEdges_forDPt);
    resolutionMap["PtVsInvdL1Pt"]  = new resolutionMeasurement("PtVsInvdL1Pt",XBINS_forPt,xEdges_forPt,XBINS_forInvDPt,xEdges_forInvDPt);
    
    resolutionMap["PtVsL1Pt_has30"]  = new resolutionMeasurement("PtVsL1Pt_has30",XBINS_forPt,xEdges_forPt,XBINS_forPt,xEdges_forPt);
    resolutionMap["PtVsdL1Pt_has30"]  = new resolutionMeasurement("PtVsdL1Pt_has30",XBINS_forPt,xEdges_forPt,XBINS_forDPt,xEdges_forDPt);
    resolutionMap["PtVsInvdL1Pt_has30"]  = new resolutionMeasurement("PtVsInvdL1Pt_has30",XBINS_forPt,xEdges_forPt,XBINS_forInvDPt,xEdges_forInvDPt);
    std::cout<<" made resolutions !\n";


	Long64_t nentries = t1->GetEntries();
    std::cout<<" Available total "<<nentries<<" \n";
    if (maxEvents >0 ) nentries = nentries > maxEvents ? maxEvents : nentries;
    std::cout<<" Processing total "<<nentries<<" \n";
	
    auto t_start = std::chrono::high_resolution_clock::now();
    auto t_end = std::chrono::high_resolution_clock::now();
    
    for (Long64_t jentry=0; jentry<nentries; jentry++){

       if(jentry%10000 ==0 )
       {
         t_end = std::chrono::high_resolution_clock::now();
         cout<<"Processing Entry "<<jentry<<" / "<<nentries<<"  [ "<<100.0*jentry/nentries<<"  % ]  "
             << " Elapsed time : "<< std::chrono::duration<double, std::milli>(t_end-t_start).count()/1000.0
             <<"  Estimated time left : "<< std::chrono::duration<double, std::milli>(t_end-t_start).count()*( nentries - jentry)/jentry * 0.001
             <<endl;
       }
	
		t1->GetEntry(jentry);  		
	
               if( isProbeLoose==1 && fabs(eleProbeEta) < 2.5  && sqrt(pow(eleProbeEta-eleTagEta,2)+pow(eleProbePhi-eleTagPhi,2))>0.6 ) 
               {
 
                 histMap["recoL1_pt"]       .Fill(l1tPt);    
                 histMap["recoL1_eta"]      .Fill(l1tEta);
                 histMap["recoL1_phi"]      .Fill(l1tPhi);

                 histMap["recoOffline_pt"]  .Fill(eleProbePt );
                 histMap["recoOffline_scEt"]  .Fill(eleProbeSclEt );
                 histMap["recoOffline_eta"] .Fill(eleProbeEta);                    
                 histMap["recoOffline_phi"] .Fill(eleProbePhi);

                resolutionMap["PtVsL1Pt"]    ->Fill(eleProbeEta , eleProbeSclEt,l1tPt);
                resolutionMap["PtVsdL1Pt"]   ->Fill(eleProbeEta , eleProbeSclEt,l1tPt/eleProbeSclEt);
                resolutionMap["PtVsInvdL1Pt"]   ->Fill(eleProbeEta , eleProbeSclEt,eleProbeSclEt/l1tPt);
                
                if(hasL1_30)
                    {

                        histMap["recoL1_has30_pt"]       .Fill(l1tPt);    
                        histMap["recoL1_has30_eta"]      .Fill(l1tEta);
                        histMap["recoL1_has30_phi"]      .Fill(l1tPhi);

                        histMap["recoOffline_has30_pt"]  .Fill(eleProbePt );
                        histMap["recoOffline_has30_scEt"]  .Fill(eleProbePt );
                        histMap["recoOffline_has30_eta"] .Fill(eleProbeEta);                    
                        histMap["recoOffline_has30_phi"] .Fill(eleProbePhi);
                
                        resolutionMap["PtVsL1Pt_has30"]    ->Fill(eleProbeEta , eleProbeSclEt,l1tPt);
                        resolutionMap["PtVsdL1Pt_has30"]   ->Fill(eleProbeEta , eleProbeSclEt,l1tPt/eleProbeSclEt);
                        resolutionMap["PtVsInvdL1Pt_has30"]   ->Fill(eleProbeEta , eleProbeSclEt,eleProbeSclEt/l1tPt);
                
                    }

                    triggers["L1_30"]->fill(hasL1_30,eleProbeSclEt);
                    triggers["L1_looseiso_30"]->fill(hasL1_30,eleProbeSclEt);
                    triggers["L1_tightiso_30"]->fill(hasL1_tightiso_30,eleProbeSclEt);

                    triggers["L1_40"]->fill(hasL1_40,eleProbeSclEt);
                    triggers["L1_looseiso_40"]->fill(hasL1_40,eleProbeSclEt);
                    triggers["L1_tightiso_40"]->fill(hasL1_tightiso_40,eleProbeSclEt);

                    if (  fabs(eleProbeEta) < ECAL_BARREL_ETAMAX )
                    {
                    triggers["L1_barrel_30"]->fill(hasL1_30,eleProbeSclEt);
                    triggers["L1_barrel_looseiso_30"]->fill(hasL1_30,eleProbeSclEt);
                    triggers["L1_barrel_tightiso_30"]->fill(hasL1_tightiso_30,eleProbeSclEt);

                    triggers["L1_barrel_40"]->fill(hasL1_40,eleProbeSclEt);
                    triggers["L1_barrel_looseiso_40"]->fill(hasL1_40,eleProbeSclEt);
                    triggers["L1_barrel_tightiso_40"]->fill(hasL1_tightiso_40,eleProbeSclEt);

                    }

                    if (  fabs(eleProbeEta) >= ECAL_BARREL_ETAMAX )
                    {
                        triggers["L1_ecap_30"]->fill(hasL1_30,eleProbeSclEt);
                        triggers["L1_ecap_looseiso_30"]->fill(hasL1_30,eleProbeSclEt);
                        triggers["L1_ecap_tightiso_30"]->fill(hasL1_tightiso_30,eleProbeSclEt);

                        triggers["L1_ecap_40"]->fill(hasL1_40,eleProbeSclEt);
                        triggers["L1_ecap_looseiso_40"]->fill(hasL1_40,eleProbeSclEt);
                        triggers["L1_ecap_tightiso_40"]->fill(hasL1_tightiso_40,eleProbeSclEt);
                    }
                    
                }
               
                
	}
 	

   
    double a  = 0.3173;
	double den,num;

    auto ofile=new TFile((prefix+ofileName).c_str(),"RECREATE");
    ofile->cd();
    
    for (std::map<string,efficiencyMeasurement * >::iterator it=triggers.begin() ; it!=triggers.end(); ++it)
    {
        std::cout<<"Processing Efficiency "<<it->first<<"  \n";
        auto &curTrigger = *(it->second); 
        curTrigger.calculateEfficiency();
        curTrigger.Write();
    }
    
    ofile->Write();
    ofile->cd();

    for (std::map<string,TH1D >::iterator it=histMap.begin() ; it!=histMap.end(); ++it)
    {
        cout<<"Processing Hist "<<it->first<<"  \n";
        auto curTrigger = it->second; 
        curTrigger.Write();
    }
    
    for (std::map<string,resolutionMeasurement * >::iterator it=resolutionMap.begin() ; it!=resolutionMap.end(); ++it)
	{
        std::cout<<"Processing Resolution "<<it->first<<"  \n";
        auto &resolutionItem = *(it->second); 
        resolutionItem.scaleBareVarHists();
        resolutionItem.calculateResolutions();
        resolutionItem.Write();
    }
 
   std::cout<<"\n";
   ofile->Purge();
   ofile->Close();
	f->Close();
}

