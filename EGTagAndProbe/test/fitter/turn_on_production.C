#include "TH1D.h"
#include "TH1F.h"
#include "TH2D.h"
#include "TH2F.h"
#include "TTree.h"
#include "TFile.h"
#include "TDirectory.h"
#include "TPaveText.h"
#include "TLegend.h"
#include "TCanvas.h"
#include "TMath.h"
#include "TStyle.h"
#include "TChain.h"
#include <iostream>
#include <fstream>
#include "TSystem.h"
#include "TROOT.h"
#include <vector>
#include "Efficiency.h"
//#include "secondary_peak.h"
//#include "/afs/cern.ch/work/c/ckoraka/codes/setTDRStyle_teliko.C"

void produceTurnOns(string infile,string ofileName,string prefix="",bool doEmulationBranches=false,Long64_t maxEvents=1000);
void turn_on_production(int mode=0)
{
    int doFull = -1;
 //   produceTurnOns(infile,ofileName,prefix,maxEvents);
    if(mode==0)
    produceTurnOns("/grid_mnt/t3storage3/athachay/l1egamma/data/run3MC/TandPFromMiniAOD_120X_mcRun3_2021_realistic_v5-v2.root",
                    "turnon.root",
                    "run3MC_12_0_2_",
                    false,
                   -1*doFull*1e5
                   );
    if(mode==1)
    produceTurnOns("/grid_mnt/t3storage3/athachay/l1egamma/data/2018/dataEGamma_120X_Run2018A-PromptReco-v3_TP.root",
                    "turnon.root",
                    "data2018_default_",
                    false,
                   -1*doFull*1e6
                   );
    if(mode==3)
    produceTurnOns("/eos/cms/store/group/dpg_trigger/comm_trigger/L1Trigger/athachay/store/mcDYToLL_120X_mcRun3_2021_realistic_v5-v2/TagAndProbe_EtCaliberatedReEmulationV2p1_120Xsample_CaloParams_2022_v0_1.root",
                    "turnon.root",
                    "run3MC_12_3_0_pre3RecalibV2_",
                    true,
                   -1*doFull*5e5
                   );
}

void produceTurnOns(string infile,string ofileName,string prefix="",bool doEmulationBranches=false,Long64_t maxEvents=1000)
{

	//////		For plotting 		////////////////////////////////////////////////////
	//gROOT->LoadMacro("/afs/cern.ch/work/c/ckoraka/codes/setTDRStyle_teliko.C");
	//setTDRStyle_teliko();




	const Int_t XBINS = 38; 

	Double_t xEdges[XBINS + 1] = {1., 3., 5., 7., 9.,  10., 12., 15., 18., 20., 22., 24., 26., 28., 29., 30., 31., 32., 33., 34., 35., 36., 37., 38., 39., 40., 41., 42., 43., 45., 50., 60., 70., 100., 200., 300., 400., 600., 1000.};
     std::vector<TH1D*> efficHists;
	


     std::map<string,efficiencyMeasurement * > triggers;
    
    ///////////////////////////////////////////////////////////////////////////////////////////

	double dR,dphi,phi1,phi2;	
    float eleProbeEta	;
    float eleProbePhi	;
    float eleTagEta		;
    float eleTagPhi		;
    Int_t Nvtx			;
    Int_t isProbeLoose	;
    float eleProbeSclEt	;


    TFile *f=new TFile(infile.c_str(),"READ");
    TTree *t1=(TTree*)(f->Get("Ntuplizer/TagAndProbe"));
	
    t1->SetBranchAddress("eleProbeEta"  ,&eleProbeEta			);
    t1->SetBranchAddress("eleProbePhi"  ,&eleProbePhi			);
    t1->SetBranchAddress("eleTagEta"    ,&eleTagEta		    	);
    t1->SetBranchAddress("eleTagPhi"    ,&eleTagPhi			    );
    t1->SetBranchAddress("Nvtx"         ,&Nvtx			        );
    t1->SetBranchAddress("isProbeLoose" ,&isProbeLoose			);
    t1->SetBranchAddress("eleProbeSclEt",&eleProbeSclEt			);
    
    TString emulationSelection="";
    TString emulationSelection2="_";
    if( doEmulationBranches)
    {
        emulationSelection="Emu";
        emulationSelection2="";
    }
    

    int   hasL1_24, hasL1_looseiso_24, hasL1_tightiso_24 ;
    //hasL1Emu_98
    t1->SetBranchAddress("hasL1"+emulationSelection+"_24"     ,&hasL1_24			    );
    t1->SetBranchAddress("hasL1"+emulationSelection+"_tightiso"+emulationSelection2+"24",&hasL1_tightiso_24 );
    t1->SetBranchAddress("hasL1"+emulationSelection+"_looseiso"+emulationSelection2+"24",&hasL1_tightiso_24 );
    triggers["L1_24"] = new efficiencyMeasurement("L1_24",XBINS,xEdges);
    triggers["L1_looseiso_24"] = new efficiencyMeasurement("L1_looseiso_24",XBINS,xEdges);
    triggers["L1_tightiso_24"] = new efficiencyMeasurement("L1_tightiso_24",XBINS,xEdges);
    
    int   hasL1_26, hasL1_looseiso_26, hasL1_tightiso_26 ;
    t1->SetBranchAddress("hasL1"+emulationSelection+"_26"     ,&hasL1_26			    );
    t1->SetBranchAddress("hasL1"+emulationSelection+"_tightiso"+emulationSelection2+"26",&hasL1_tightiso_26 );
    t1->SetBranchAddress("hasL1"+emulationSelection+"_looseiso"+emulationSelection2+"26",&hasL1_tightiso_26 );
    triggers["L1_26"] = new efficiencyMeasurement("L1_26",XBINS,xEdges);
    triggers["L1_looseiso_26"] = new efficiencyMeasurement("L1_looseiso_26",XBINS,xEdges);
    triggers["L1_tightiso_26"] = new efficiencyMeasurement("L1_tightiso_26",XBINS,xEdges);
    
    int   hasL1_28, hasL1_looseiso_28, hasL1_tightiso_28 ;
    t1->SetBranchAddress("hasL1"+emulationSelection+"_28"     ,&hasL1_28			    );
    t1->SetBranchAddress("hasL1"+emulationSelection+"_tightiso"+emulationSelection2+"28",&hasL1_tightiso_28 );
    t1->SetBranchAddress("hasL1"+emulationSelection+"_looseiso"+emulationSelection2+"28",&hasL1_tightiso_28 );
    triggers["L1_28"] = new efficiencyMeasurement("L1_28",XBINS,xEdges);
    triggers["L1_looseiso_28"] = new efficiencyMeasurement("L1_looseiso_28",XBINS,xEdges);
    triggers["L1_tightiso_28"] = new efficiencyMeasurement("L1_tightiso_28",XBINS,xEdges);
    
    int   hasL1_30, hasL1_looseiso_30, hasL1_tightiso_30 ;
    t1->SetBranchAddress("hasL1"+emulationSelection+"_30"     ,&hasL1_30			    );
    t1->SetBranchAddress("hasL1"+emulationSelection+"_tightiso"+emulationSelection2+"30",&hasL1_tightiso_30 );
    t1->SetBranchAddress("hasL1"+emulationSelection+"_looseiso"+emulationSelection2+"30",&hasL1_tightiso_30 );
    triggers["L1_30"] = new efficiencyMeasurement("L1_30",XBINS,xEdges);
    triggers["L1_looseiso_30"] = new efficiencyMeasurement("L1_looseiso_30",XBINS,xEdges);
    triggers["L1_tightiso_30"] = new efficiencyMeasurement("L1_tightiso_30",XBINS,xEdges);
    
    int   hasL1_32, hasL1_looseiso_32, hasL1_tightiso_32 ;
    t1->SetBranchAddress("hasL1"+emulationSelection+"_32"     ,&hasL1_32			    );
    t1->SetBranchAddress("hasL1"+emulationSelection+"_tightiso"+emulationSelection2+"32",&hasL1_tightiso_32 );
    t1->SetBranchAddress("hasL1"+emulationSelection+"_looseiso"+emulationSelection2+"32",&hasL1_tightiso_32 );
    triggers["L1_32"] = new efficiencyMeasurement("L1_32",XBINS,xEdges);
    triggers["L1_looseiso_32"] = new efficiencyMeasurement("L1_looseiso_32",XBINS,xEdges);
    triggers["L1_tightiso_32"] = new efficiencyMeasurement("L1_tightiso_32",XBINS,xEdges);
    
    int   hasL1_34, hasL1_looseiso_34, hasL1_tightiso_34 ;
    t1->SetBranchAddress("hasL1"+emulationSelection+"_34"     ,&hasL1_34			    );
    t1->SetBranchAddress("hasL1"+emulationSelection+"_tightiso"+emulationSelection2+"34",&hasL1_tightiso_34 );
    t1->SetBranchAddress("hasL1"+emulationSelection+"_looseiso"+emulationSelection2+"34",&hasL1_tightiso_34 );
    triggers["L1_34"] = new efficiencyMeasurement("L1_34",XBINS,xEdges);
    triggers["L1_looseiso_34"] = new efficiencyMeasurement("L1_looseiso_34",XBINS,xEdges);
    triggers["L1_tightiso_34"] = new efficiencyMeasurement("L1_tightiso_34",XBINS,xEdges);
 
    int   hasL1_40, hasL1_looseiso_40, hasL1_tightiso_40 ;
    t1->SetBranchAddress("hasL1"+emulationSelection+"_40"     ,&hasL1_40			    );
    t1->SetBranchAddress("hasL1"+emulationSelection+"_tightiso"+emulationSelection2+"40",&hasL1_tightiso_40 );
    t1->SetBranchAddress("hasL1"+emulationSelection+"_looseiso"+emulationSelection2+"40",&hasL1_tightiso_40 );
    triggers["L1_40"] = new efficiencyMeasurement("L1_40",XBINS,xEdges);
    triggers["L1_looseiso_40"] = new efficiencyMeasurement("L1_looseiso_40",XBINS,xEdges);
    triggers["L1_tightiso_40"] = new efficiencyMeasurement("L1_tightiso_40",XBINS,xEdges);
    
    triggers["L1_tightiso_28_OR_L1_34"] = new efficiencyMeasurement("L1_tightiso_28_OR_L1_34",XBINS,xEdges);
 
    const int XBINS_forPU=100;
    Double_t xEdges_forPU[XBINS_forPU];
    for(int i=0;i<XBINS_forPU;i++) xEdges_forPU[i]= -0.5 + i*1.0;
    triggers["L1_32vsPUoffline40GeV"] = new efficiencyMeasurement("L1_32vsPUoffline40GeV",XBINS,xEdges);
    triggers["L1_32vsPUoffline50GeV"] = new efficiencyMeasurement("L1_32vsPUoffline50GeV",XBINS,xEdges);
    

    


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
                	
                    triggers["L1_24"]->fill(hasL1_24,eleProbeSclEt);
                    triggers["L1_looseiso_24"]->fill(hasL1_24,eleProbeSclEt);
                    triggers["L1_tightiso_24"]->fill(hasL1_tightiso_24,eleProbeSclEt);

                    triggers["L1_26"]->fill(hasL1_26,eleProbeSclEt);
                    triggers["L1_looseiso_26"]->fill(hasL1_26,eleProbeSclEt);
                    triggers["L1_tightiso_26"]->fill(hasL1_tightiso_26,eleProbeSclEt);

                    triggers["L1_28"]->fill(hasL1_28,eleProbeSclEt);
                    triggers["L1_looseiso_28"]->fill(hasL1_28,eleProbeSclEt);
                    triggers["L1_tightiso_28"]->fill(hasL1_tightiso_28,eleProbeSclEt);

                    triggers["L1_32"]->fill(hasL1_32,eleProbeSclEt);
                    triggers["L1_looseiso_32"]->fill(hasL1_32,eleProbeSclEt);
                    triggers["L1_tightiso_32"]->fill(hasL1_tightiso_32,eleProbeSclEt);

                    triggers["L1_30"]->fill(hasL1_30,eleProbeSclEt);
                    triggers["L1_looseiso_30"]->fill(hasL1_30,eleProbeSclEt);
                    triggers["L1_tightiso_30"]->fill(hasL1_tightiso_30,eleProbeSclEt);

                    triggers["L1_34"]->fill(hasL1_34,eleProbeSclEt);
                    triggers["L1_looseiso_34"]->fill(hasL1_34,eleProbeSclEt);
                    triggers["L1_tightiso_34"]->fill(hasL1_tightiso_34,eleProbeSclEt);

                    triggers["L1_40"]->fill(hasL1_40,eleProbeSclEt);
                    triggers["L1_looseiso_40"]->fill(hasL1_40,eleProbeSclEt);
                    triggers["L1_tightiso_40"]->fill(hasL1_tightiso_40,eleProbeSclEt);
                    
                    int rslt = hasL1_tightiso_28 + hasL1_34;
                    if (rslt > 0 ) rslt=1;

                    triggers["L1_tightiso_28_OR_L1_34"]->fill(rslt,eleProbeSclEt);
		        
                }
               
                

               if( isProbeLoose==1 && fabs(eleProbeEta) < 2.5 && eleProbeSclEt > 50.0  && sqrt(pow(eleProbeEta-eleTagEta,2)+pow(eleProbePhi-eleTagPhi,2))>0.6 ) 
               {
                    triggers["L1_32vsPUoffline50GeV"]->fill(hasL1_32,Nvtx);
               }
               if( isProbeLoose==1 && fabs(eleProbeEta) < 2.5 && eleProbeSclEt > 40.0  && sqrt(pow(eleProbeEta-eleTagEta,2)+pow(eleProbePhi-eleTagPhi,2))>0.6 ) 
               {
                    triggers["L1_32vsPUoffline40GeV"]->fill(hasL1_30,Nvtx);
               }
	}
 	
	f->Close();

   
    double a  = 0.3173;
	double den,num;

    for (std::map<string,efficiencyMeasurement * >::iterator it=triggers.begin() ; it!=triggers.end(); ++it)
    {
        std::cout<<"Processing "<<it->first<<"  \n";
        auto &curTrigger = *(it->second); 
        curTrigger.calculateEfficiency();
    }

	TLegend *l = new TLegend(.6, .5, .9, .7);
    TCanvas *c4 = new TCanvas("c4","TT");
    auto ofile=new TFile((prefix+ofileName).c_str(),"RECREATE");
    ofile->cd();

    int i=0;
	l->SetTextSize(0.03);
    for (std::map<string,efficiencyMeasurement*>::iterator it=triggers.begin() ; it!=triggers.end(); ++it)
    {
        auto trigName=it->first;
        std::cout<<i<<" Drawing : "<<trigName<<"  \n";
        auto &curTrigger = *(it->second); 

        l->AddEntry(curTrigger.effiHist,trigName.c_str(),"l");
        curTrigger.effiHist->GetXaxis()->SetTitle("E_{T} probe [GeV]");
        if(i==0) curTrigger.effiHist->Draw("ep pmc plc");
        else     curTrigger.effiHist->Draw("ep pmc plc same");
        i++;
        curTrigger.Write();
   }     

        l->Draw("same");
        c4->SaveAs((prefix+"c4.png").c_str());
    
   ofile->Purge();
   ofile->Close();
}

