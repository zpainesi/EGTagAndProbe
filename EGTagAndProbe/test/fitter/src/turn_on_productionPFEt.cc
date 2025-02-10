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
#include "TDirectory.h"
#include "TGraphAsymmErrors.h"
#include <chrono>

#define BARREL_BEG -0.01
#define BARREL_END  1.305
#define ENDCAP_BEG    1.479
#define ENDCAP_END    2.400

using namespace std;
struct dataConfig{
    
    std::string infile;
    std::vector<Int_t> RunNumbers;
    std::string ofileName;
    std::string prefix;
    TString treeName;
    bool doEmulationBranches;
    bool doAllRuns;
    Int_t reportEvery;
    Long64_t maxEvents;
    
    dataConfig(){
        infile ="";
        ofileName="turnons.root";
        prefix="";
        treeName="Ntuplizer/TagAndProbe";
        doEmulationBranches=false;
        doAllRuns=false;
        Int_t reportEvery(500);
        Long64_t maxEvents(-1);
    }
};

void produceTurnOns(string infile,string ofileName,TString treeName,string prefix="",bool doEmulationBranches=false,
          bool doAllRuns=false,std::vector<Int_t> RunNumbers = std::vector<Int_t>(),Long64_t maxEvents=1000,Int_t reportEvery=100);

void turn_on_productionv2(int mode=0)
{
    int doFull = 1;
 //   produceTurnOns(infile,ofileName,prefix,maxEvents);
 /* 
    if(mode==0)
    produceTurnOns("/grid_mnt/t3storage3/athachay/l1egamma/data/run3Data/TagAndProbe_DataRun3_2022C_unpacked_jul29.root",
                    "turnon.root",
                    "TagAndProbe_DataRun3_2022C_unpacked_jul29_",
                    false,
                   -5*1e4
                   );
    if(mode==1)
    produceTurnOns("/eos/cms/store/group/dpg_trigger/comm_trigger/L1Trigger/athachay/store/Run3_EGamma_13p6TeV/TagAndProbe_DataRun3_2022C_unpacked_aug16.root",
                    "turnon.root",
                    "TagAndProbe_DataRun3_2022C_unpacked_aug16_f8120xx",
                    false,
                    357401,357401,
                    -5*1e4
                   );
*/
}


void tokenize(const std::string& str, std::vector<std::string>& tokens, const std::string& delimiters) {
  std::string::size_type lastPos = str.find_first_not_of(delimiters, 0);
  std::string::size_type pos = str.find_first_of(delimiters, lastPos);
  while (std::string::npos != pos || std::string::npos != lastPos)  {
    tokens.push_back(str.substr(lastPos, pos - lastPos));
    lastPos = str.find_first_not_of(delimiters, pos);
    pos = str.find_first_of(delimiters, lastPos);
  }
}



void produceTurnOns(string infile,string ofileName,TString treeName,string prefix,bool doEmulationBranches, bool doAllRuns,std::vector<Int_t> RunNumbers,Long64_t maxEvents,Int_t reportEvery)
{

	const Int_t XBINS = 38; 
	Double_t xEdges[XBINS + 1] = {1., 3., 5., 7., 9.,  10., 12., 15., 18., 20., 22., 24., 26., 28., 29., 30., 31., 32., 33., 34., 35., 36., 37., 38., 39., 40., 41., 42., 43., 45., 50., 60., 70., 100., 200., 300., 400., 600., 1000.};
	
    const Int_t XBINSfine = 100; 
	Double_t xEdges_fine[XBINSfine + 1];
    for(int i=0;i<=XBINSfine;i++)
    {
        xEdges_fine[i]=0.0+1.0*i;
    }

     std::vector<TH1D*> efficHists;
     std::map<string,efficiencyMeasurement * > triggers;
    

    ///////////////////////////////////////////////////////////////////////////////////////////

	double dR,dphi,phi1,phi2;	
    Float_t eleProbeEta	;
    Float_t eleProbePhi	;
    Float_t eleTagEta		;
    Float_t eleTagPhi		;
    Float_t eleProbePt ;
    Float_t eleProbeSclEt ;
    Float_t   l1tPt       ;
    Int_t   l1tEmuRawEt(1)      ;
    
    Int_t Nvtx			;
    Int_t isProbeLoose	;
    Int_t RunNumber     ;

    TFile *f=new TFile(infile.c_str(),"READ");
    TTree *t1=(TTree*)(f->Get(treeName));
	
    if(doEmulationBranches)
    {
        ofileName= "reEmulated_"+ofileName;
    }
    else{
        ofileName= "unpacked_"+ofileName;
    }
    auto ofile=new TFile((prefix+ofileName).c_str(),"RECREATE");
    ofile->cd();

    t1->SetBranchAddress("eleProbeEta"  ,&eleProbeEta			);
    t1->SetBranchAddress("eleProbePhi"  ,&eleProbePhi			);
    t1->SetBranchAddress("eleTagEta"    ,&eleTagEta		    	);
    t1->SetBranchAddress("eleTagPhi"    ,&eleTagPhi			    );
    t1->SetBranchAddress("Nvtx"         ,&Nvtx			        );
    t1->SetBranchAddress("isProbeLoose" ,&isProbeLoose			);
    t1->SetBranchAddress("eleProbeSclEt",&eleProbeSclEt			);
    t1->SetBranchAddress("eleProbePt",&eleProbePt	     		);
    t1->SetBranchAddress("RunNumber",&RunNumber	);
    
    TString emulationSelection="";
    TString emulationSelection2="_";
   
    if( doEmulationBranches )
    {
        std::cout<<"Doing Emulation Branches !\n";
        emulationSelection="Emu";
        emulationSelection2="";
        t1->SetBranchAddress("l1tEmuPt", &l1tPt);
        t1->SetBranchAddress("l1tEmuRawEt", &l1tEmuRawEt);
        std::cout<<"setting emu \n";
    }
    else
    {
        t1->SetBranchAddress("l1tPt", &l1tPt);
        std::cout<<"setting non emu \n";
    }
    
    const Int_t ET_MAX(100);
    std::map<Int_t,TString> triggerPrefix;
    std::map<TString,int> hasL1,hasL1LooseIso,hasL1TightIso;
    std::map<Int_t , std::map<TString,efficiencyMeasurement*> > triggerStore;


    TString trPrefixA="L1Et",prefixL1;
    for(Int_t ii=0;ii<ET_MAX;ii++)
    {
           triggerPrefix[ii]=trPrefixA+ii; 
           hasL1[triggerPrefix[ii]]=0;
           hasL1LooseIso[triggerPrefix[ii]]=0;
           hasL1TightIso[triggerPrefix[ii]]=0;
           t1->SetBranchAddress("hasL1"+emulationSelection +"_"+ii    ,  &hasL1[triggerPrefix[ii]]    );
           t1->SetBranchAddress("hasL1"+emulationSelection+"_looseiso"+emulationSelection2+ii,&hasL1LooseIso[triggerPrefix[ii]+"_looseiso"]);
           t1->SetBranchAddress("hasL1"+emulationSelection+"_tightiso"+emulationSelection2+ii,&hasL1TightIso[triggerPrefix[ii]+"_tightiso"]);
    }

    std::cout<<"  ============   Defining the Efficiency for Run Numbers============ \n";
    if (RunNumbers.empty())
    {
        RunNumbers.push_back(0);
    }
    for( auto idx : RunNumbers) {
        
        if(doAllRuns)
            idx=0;
        TString rnStr("");
        rnStr+=idx;
        ofile->mkdir( rnStr );

        triggerStore[idx] =  std::map<TString,efficiencyMeasurement*>() ;
        
        std::cout<<"Making efficiencies for Run Number "<<idx<<"\n"; 
        
        
        for(Int_t ii=0;ii<ET_MAX;ii++)
        {

               triggerPrefix[ii]=trPrefixA+ii; 
               //hasL1[triggerPrefix[ii]]=0;
               //hasL1LooseIso[triggerPrefix[ii]]=0;
               //hasL1TightIso[triggerPrefix[ii]]=0;
               //t1->SetBranchAddress("hasL1"+emulationSelection +emulationSelection2+ii    ,  &hasL1[triggerPrefix[ii]]    );
               //t1->SetBranchAddress("hasL1"+emulationSelection+"_looseiso"+emulationSelection2+ii,&hasL1LooseIso[triggerPrefix[ii]+"_looseiso"]);
               //t1->SetBranchAddress("hasL1"+emulationSelection+"_tightiso"+emulationSelection2+ii,&hasL1TightIso[triggerPrefix[ii]+"_tightiso"]);
               
               triggerStore[idx][triggerPrefix[ii]+"_default"] = new efficiencyMeasurement(string(triggerPrefix[ii])+"_default",XBINS,xEdges);
               triggerStore[idx][triggerPrefix[ii]+"_looseiso_def"] = new efficiencyMeasurement(string(triggerPrefix[ii]+"_looseiso_def"),XBINS,xEdges);
               triggerStore[idx][triggerPrefix[ii]+"_tightiso_def"] = new efficiencyMeasurement(string(triggerPrefix[ii]+"_tightiso_def"),XBINS,xEdges);
               
               triggerStore[idx][triggerPrefix[ii]] = new efficiencyMeasurement(string(triggerPrefix[ii]),XBINSfine,xEdges_fine);
               triggerStore[idx][triggerPrefix[ii]+"_looseiso"] = new efficiencyMeasurement(string(triggerPrefix[ii]+"_looseiso"),XBINSfine,xEdges_fine);
               triggerStore[idx][triggerPrefix[ii]+"_tightiso"] = new efficiencyMeasurement(string(triggerPrefix[ii]+"_tightiso"),XBINSfine,xEdges_fine);
               
               triggerStore[idx][triggerPrefix[ii]+"_default_Barrel"] = new efficiencyMeasurement(string(triggerPrefix[ii])+"_default_Barrel",XBINS,xEdges);
               triggerStore[idx][triggerPrefix[ii]+"_looseiso_def_Barrel"] = new efficiencyMeasurement(string(triggerPrefix[ii]+"_looseiso_def_Barrel"),XBINS,xEdges);
               triggerStore[idx][triggerPrefix[ii]+"_tightiso_def_Barrel"] = new efficiencyMeasurement(string(triggerPrefix[ii]+"_tightiso_def_Barrel"),XBINS,xEdges);
               
               triggerStore[idx][triggerPrefix[ii]+"_Barrel"] = new efficiencyMeasurement(string(triggerPrefix[ii]),XBINSfine,xEdges_fine);
               triggerStore[idx][triggerPrefix[ii]+"_looseiso_Barrel"] = new efficiencyMeasurement(string(triggerPrefix[ii]+"_looseiso_Barrel"),XBINSfine,xEdges_fine);
               triggerStore[idx][triggerPrefix[ii]+"_tightiso_Barrel"] = new efficiencyMeasurement(string(triggerPrefix[ii]+"_tightiso_Barrel"),XBINSfine,xEdges_fine);
               
               triggerStore[idx][triggerPrefix[ii]+"_default_ECap"] = new efficiencyMeasurement(string(triggerPrefix[ii])+"_default_ECap",XBINS,xEdges);
               triggerStore[idx][triggerPrefix[ii]+"_looseiso_def_ECap"] = new efficiencyMeasurement(string(triggerPrefix[ii]+"_looseiso_def_ECap"),XBINS,xEdges);
               triggerStore[idx][triggerPrefix[ii]+"_tightiso_def_ECap"] = new efficiencyMeasurement(string(triggerPrefix[ii]+"_tightiso_def_ECap"),XBINS,xEdges);
               
               triggerStore[idx][triggerPrefix[ii]+"_ECap"] = new efficiencyMeasurement(string(triggerPrefix[ii]),XBINSfine,xEdges_fine);
               triggerStore[idx][triggerPrefix[ii]+"_looseiso_ECap"] = new efficiencyMeasurement(string(triggerPrefix[ii]+"_looseiso_ECap"),XBINSfine,xEdges_fine);
               triggerStore[idx][triggerPrefix[ii]+"_tightiso_ECap"] = new efficiencyMeasurement(string(triggerPrefix[ii]+"_tightiso_ECap"),XBINSfine,xEdges_fine);
        }
      if(doAllRuns)
        break;
    }
    std::cout<<"  ============ ********* ============ \n";

   
    triggers["L1_tightiso_28_OR_L1_34"] = new efficiencyMeasurement("L1_tightiso_28_OR_L1_34",XBINS,xEdges);
 
    const int XBINS_forPU=100;
    Double_t xEdges_forPU[XBINS_forPU];
    for(int i=0;i<XBINS_forPU;i++) xEdges_forPU[i]= -0.5 + i*1.0;
    triggers["L1_32vsPUoffline40GeV"] = new efficiencyMeasurement("L1_32vsPUoffline40GeV",XBINS,xEdges);
    triggers["L1_32vsPUoffline50GeV"] = new efficiencyMeasurement("L1_32vsPUoffline50GeV",XBINS,xEdges);
    triggers["L1_32vsPUoffline35GeV"] = new efficiencyMeasurement("L1_32vsPUoffline25GeV",XBINS,xEdges);
    triggers["L1_28TightvsPUoffline32GeV"] = new efficiencyMeasurement("L1_28TightvsPUoffline32GeV",XBINS,xEdges);
    triggers["L1_28LoosevsPUoffline32GeV"] = new efficiencyMeasurement("L1_28LoosevsPUoffline32GeV",XBINS,xEdges);
    triggers["L1_15LoosevsPUoffline18GeV"] = new efficiencyMeasurement("L1_15LoosevsPUoffline18GeV",XBINS,xEdges);
    triggers["L1_10LoosevsPUoffline12GeV"] = new efficiencyMeasurement("L1_10LoosevsPUoffline12GeV",XBINS,xEdges);
    

	Long64_t nentries = t1->GetEntries();
    std::cout<<" Available total "<<nentries<<" \n";
    if (maxEvents >0 ) nentries = nentries > maxEvents ? maxEvents : nentries;
    

    std::cout<<" Processing total "<<nentries<<" \n";
    
    auto t_start = std::chrono::high_resolution_clock::now();
    auto t_end = std::chrono::high_resolution_clock::now();
	int k=0;
    int nEventsProcessed=0;
    for (Long64_t jentry=0; jentry<nentries; jentry++){
       
       t1->GetEntry(jentry); 
       eleProbeSclEt=eleProbePt;

       //if(l1tEmuRawEt < 0.0 ){   k++;   continue ;  }
       if(jentry%reportEvery ==0 )
       {
            t_end = std::chrono::high_resolution_clock::now();
             cout<<" Run : "<<RunNumber<<"  Processing Entry "<<jentry<<" / "<<nentries<<"  [ "<<100.0*jentry/nentries<<"  % ]  "
             << " Elapsed time : "<< std::chrono::duration<double, std::milli>(t_end-t_start).count()/1000.0
             <<"  Estimated time left : "<< std::chrono::duration<double, std::milli>(t_end-t_start).count()*( nentries - jentry)/jentry * 0.001
             <<endl;
       }
        

       if(doAllRuns)
       {
            RunNumber=0;
       }    
       else if( std::find(RunNumbers.begin(),RunNumbers.end(),RunNumber)  == RunNumbers.end() )
       {
            continue;
       }
       
       nEventsProcessed++;
       if( isProbeLoose==1 && fabs(eleProbeEta) < 2.5  && sqrt(pow(eleProbeEta-eleTagEta,2)+pow(eleProbePhi-eleTagPhi,2))>0.6 ) 
          {
           	
               for(Int_t ii=0; ii< ET_MAX;ii++)
               {
                   triggerStore[RunNumber][triggerPrefix[ii]+"_default"]->fill(hasL1[triggerPrefix[ii]],eleProbeSclEt);
                   triggerStore[RunNumber][triggerPrefix[ii]+"_looseiso_def"]->fill(hasL1LooseIso[triggerPrefix[ii]+"_looseiso"],eleProbeSclEt);
                   triggerStore[RunNumber][triggerPrefix[ii]+"_tightiso_def"]->fill(hasL1TightIso[triggerPrefix[ii]+"_tightiso"],eleProbeSclEt);
                   
                   triggerStore[RunNumber][triggerPrefix[ii]]->fill(hasL1[triggerPrefix[ii]],eleProbeSclEt);
                   triggerStore[RunNumber][triggerPrefix[ii]+"_looseiso"]->fill(hasL1LooseIso[triggerPrefix[ii]+"_looseiso"],eleProbeSclEt);
                   triggerStore[RunNumber][triggerPrefix[ii]+"_tightiso"]->fill(hasL1TightIso[triggerPrefix[ii]+"_tightiso"],eleProbeSclEt);

                   if(abs(eleProbeEta) < BARREL_END)
                   {

                       triggerStore[RunNumber][triggerPrefix[ii]+"_default_Barrel"]->fill(hasL1[triggerPrefix[ii]],eleProbeSclEt);
                       triggerStore[RunNumber][triggerPrefix[ii]+"_looseiso_def_Barrel"]->fill(hasL1LooseIso[triggerPrefix[ii]+"_looseiso"],eleProbeSclEt);
                       triggerStore[RunNumber][triggerPrefix[ii]+"_tightiso_def_Barrel"]->fill(hasL1TightIso[triggerPrefix[ii]+"_tightiso"],eleProbeSclEt);
                       
                       triggerStore[RunNumber][triggerPrefix[ii]+"_Barrel"]->fill(hasL1[triggerPrefix[ii]],eleProbeSclEt);
                       triggerStore[RunNumber][triggerPrefix[ii]+"_looseiso_Barrel"]->fill(hasL1LooseIso[triggerPrefix[ii]+"_looseiso"],eleProbeSclEt);
                       triggerStore[RunNumber][triggerPrefix[ii]+"_tightiso_Barrel"]->fill(hasL1TightIso[triggerPrefix[ii]+"_tightiso"],eleProbeSclEt);
                   }

                  if(abs(eleProbeEta) > ENDCAP_BEG )
                   {
                       triggerStore[RunNumber][triggerPrefix[ii]+"_default_ECap"]->fill(hasL1[triggerPrefix[ii]],eleProbeSclEt);
                       triggerStore[RunNumber][triggerPrefix[ii]+"_looseiso_def_ECap"]->fill(hasL1LooseIso[triggerPrefix[ii]+"_looseiso"],eleProbeSclEt);
                       triggerStore[RunNumber][triggerPrefix[ii]+"_tightiso_def_ECap"]->fill(hasL1TightIso[triggerPrefix[ii]+"_tightiso"],eleProbeSclEt);
                       
                       triggerStore[RunNumber][triggerPrefix[ii]+"_ECap"]->fill(hasL1[triggerPrefix[ii]],eleProbeSclEt);
                       triggerStore[RunNumber][triggerPrefix[ii]+"_looseiso_ECap"]->fill(hasL1LooseIso[triggerPrefix[ii]+"_looseiso"],eleProbeSclEt);
                       triggerStore[RunNumber][triggerPrefix[ii]+"_tightiso_ECap"]->fill(hasL1TightIso[triggerPrefix[ii]+"_tightiso"],eleProbeSclEt);
                   }
               }
               
           }
          
           

          if( isProbeLoose==1 && fabs(eleProbeEta) < 2.5 && eleProbeSclEt > 50.0  && sqrt(pow(eleProbeEta-eleTagEta,2)+pow(eleProbePhi-eleTagPhi,2))>0.6 ) 
          {
               triggers["L1_32vsPUoffline50GeV"]->fill(hasL1["L1Et32"],Nvtx);
          }
          if( isProbeLoose==1 && fabs(eleProbeEta) < 2.5 && eleProbeSclEt > 40.0  && sqrt(pow(eleProbeEta-eleTagEta,2)+pow(eleProbePhi-eleTagPhi,2))>0.6 ) 
          {
               triggers["L1_32vsPUoffline40GeV"]->fill(hasL1["L1Et32"],Nvtx);
          }
          if( isProbeLoose==1 && fabs(eleProbeEta) < 2.5 && eleProbeSclEt > 32.0  && sqrt(pow(eleProbeEta-eleTagEta,2)+pow(eleProbePhi-eleTagPhi,2))>0.6 ) 
          {
               triggers["L1_28TightvsPUoffline32GeV"]->fill(hasL1LooseIso[triggerPrefix[32]+"_looseiso"],Nvtx);
               triggers["L1_28LoosevsPUoffline32GeV"]->fill(hasL1LooseIso[triggerPrefix[32]+"_looseiso"],Nvtx);
          }
          if( isProbeLoose==1 && fabs(eleProbeEta) < 2.5 && eleProbeSclEt > 18.0  && sqrt(pow(eleProbeEta-eleTagEta,2)+pow(eleProbePhi-eleTagPhi,2))>0.6 ) 
          {
               triggers["L1_15LoosevsPUoffline18GeV"]->fill(hasL1LooseIso[triggerPrefix[15]+"_looseiso"],Nvtx);
          }
          if( isProbeLoose==1 && fabs(eleProbeEta) < 2.5 && eleProbeSclEt > 12.0  && sqrt(pow(eleProbeEta-eleTagEta,2)+pow(eleProbePhi-eleTagPhi,2))>0.6 ) 
          {
               triggers["L1_10LoosevsPUoffline12GeV"]->fill(hasL1LooseIso[triggerPrefix[10]+"_looseiso"],Nvtx);
          }
          
	}
 	
	f->Close();
    
    std::cout<<"Nevents processed : "<<nEventsProcessed<<"\n";
   

   
    for( std::map<string,efficiencyMeasurement * >::iterator it=triggers.begin() ; it!=triggers.end(); ++it)
    {
        std::cout<<"Processing "<<it->first<<"  \n";
        auto &curTrigger = *(it->second); 
        curTrigger.calculateEfficiency();
    }
    
    for( auto it=triggerStore.begin() ; it!=triggerStore.end(); ++it  ) {
        
        auto rn = it->first;
        TString rnStr("");
        rnStr+=rn;
        ofile->cd( rnStr );
        std::cout<<"\n Making the directory : "<<it->first << " | "<<rn<<" rnStr : "<<rnStr<<"\n";
        //ofile-cidls();
        //dir->cd();
        for(Int_t ii=0; ii< ET_MAX;ii++)
        {
            if(ii%10==0)
                std::cout<<"Processing "<<ii<<"GeV  \n";

            triggerStore[rn][triggerPrefix[ii]+"_default"]->calculateEfficiency();
            triggerStore[rn][triggerPrefix[ii]+"_looseiso_def"]->calculateEfficiency();
            triggerStore[rn][triggerPrefix[ii]+"_tightiso_def"]->calculateEfficiency();
            triggerStore[rn][triggerPrefix[ii]]->calculateEfficiency();
            triggerStore[rn][triggerPrefix[ii]+"_looseiso"]->calculateEfficiency();
            triggerStore[rn][triggerPrefix[ii]+"_tightiso"]->calculateEfficiency();
            
            triggerStore[rn][triggerPrefix[ii]+"_default_Barrel"]->calculateEfficiency();
            triggerStore[rn][triggerPrefix[ii]+"_looseiso_def_Barrel"]->calculateEfficiency();
            triggerStore[rn][triggerPrefix[ii]+"_tightiso_def_Barrel"]->calculateEfficiency();
            triggerStore[rn][triggerPrefix[ii]+"_Barrel"]->calculateEfficiency();
            triggerStore[rn][triggerPrefix[ii]+"_looseiso_Barrel"]->calculateEfficiency();
            triggerStore[rn][triggerPrefix[ii]+"_tightiso_Barrel"]->calculateEfficiency();
            
            triggerStore[rn][triggerPrefix[ii]+"_default_ECap"]->calculateEfficiency();
            triggerStore[rn][triggerPrefix[ii]+"_looseiso_def_ECap"]->calculateEfficiency();
            triggerStore[rn][triggerPrefix[ii]+"_tightiso_def_ECap"]->calculateEfficiency();
            triggerStore[rn][triggerPrefix[ii]+"_ECap"]->calculateEfficiency();
            triggerStore[rn][triggerPrefix[ii]+"_looseiso_ECap"]->calculateEfficiency();
            triggerStore[rn][triggerPrefix[ii]+"_tightiso_ECap"]->calculateEfficiency();
            
            triggerStore[rn][triggerPrefix[ii]+"_default"]->Write();
            triggerStore[rn][triggerPrefix[ii]+"_looseiso_def"]->Write();
            triggerStore[rn][triggerPrefix[ii]+"_tightiso_def"]->Write();
            triggerStore[rn][triggerPrefix[ii]]->Write();
            triggerStore[rn][triggerPrefix[ii]+"_looseiso"]->Write();
            triggerStore[rn][triggerPrefix[ii]+"_tightiso"]->Write();

            triggerStore[rn][triggerPrefix[ii]+"_default_Barrel"]->Write();
            triggerStore[rn][triggerPrefix[ii]+"_looseiso_def_Barrel"]->Write();
            triggerStore[rn][triggerPrefix[ii]+"_tightiso_def_Barrel"]->Write();
            triggerStore[rn][triggerPrefix[ii]+"_Barrel"]->Write();
            triggerStore[rn][triggerPrefix[ii]+"_looseiso_Barrel"]->Write();
            triggerStore[rn][triggerPrefix[ii]+"_tightiso_Barrel"]->Write();
            
            triggerStore[rn][triggerPrefix[ii]+"_default_ECap"]->Write();
            triggerStore[rn][triggerPrefix[ii]+"_looseiso_def_ECap"]->Write();
            triggerStore[rn][triggerPrefix[ii]+"_tightiso_def_ECap"]->Write();
            triggerStore[rn][triggerPrefix[ii]+"_ECap"]->Write();
            triggerStore[rn][triggerPrefix[ii]+"_looseiso_ECap"]->Write();
            triggerStore[rn][triggerPrefix[ii]+"_tightiso_ECap"]->Write();
        }
        //ofile->ls();
        std::cout<<"\n\n\n";
        //ofile->ls(TString(rn));
        std::cout<<"\n\n\n";
    }

    int i=0;
    for (std::map<string,efficiencyMeasurement*>::iterator it=triggers.begin() ; it!=triggers.end(); ++it)
    {
        auto trigName=it->first;
        auto &curTrigger = *(it->second); 

        curTrigger.effiHist->GetXaxis()->SetTitle("E_{T} probe [GeV]");
        i++;
        curTrigger.Write();
   }     

    
   ofile->Purge();
   ofile->Close();
}

void readParameters(const std::string jfile , struct dataConfig & cfg) {
  std::cout<<"Reading Config file  : " << jfile << std::endl;
  std::ifstream jobcardFile(jfile.c_str());
  if (!jobcardFile) {
    std::cerr << "Input Config File: " << jfile << " could not be opened!" << std::endl;
    return;
  }

  std::string line;

  if(jobcardFile.is_open()) {

    while(std::getline(jobcardFile,line)) {

      if (line.substr(0,1) == "#" || line.substr(0,2) == "//") continue;

      std::vector<std::string> tokens;
      tokenize(line,tokens,"=");
      std::cout << tokens[0] << ":" << tokens[1] << std::endl;
      std::string key   = tokens.at(0);
      std::string value = tokens.at(1); 
      if(key=="infile"){
        cfg.infile= value;
      }
      //else if (key=="ofileName")	    {
      //          cfg.ofileName= value;
      //}
      else if (key=="treeName")	    {
                cfg.treeName= value;
      }
      else if (key=="prefix")	    {
                cfg.prefix= value;
      }
      else if (key=="RunNumbers")
	  {
	    std::string tmp_string = value;
	    std::vector<std::string> tmp_vec;
	    tokenize(tmp_string,tmp_vec,",");
	    for (auto it : tmp_vec) {
	      cfg.RunNumbers.push_back(std::atoi(it.c_str()));
	    }	  
	  }         
      else if (key=="doEmulationBranches")	    {
                cfg.doEmulationBranches= bool(atoi(value.c_str()));
      }
      else if (key=="doAllRuns")	    {
                cfg.doAllRuns= bool(atoi(value.c_str()));
      }
      else if (key=="reportEvery")	    {
                cfg.reportEvery= atoi(value.c_str());
      }
      else if (key=="MaxEntries")	    {
                cfg.maxEvents= atoi(value.c_str());
      }
      else 
	std::cout << " unknown option " << " key " << key << std::endl;
    }

  }
  jobcardFile.close();
}  


int  main(int argc , char **argv )
{
    dataConfig cfg;
    readParameters( argv[1], cfg);

    std::cout<<" infile      : "<<cfg.infile<<"\n";
    std::cout<<" ofile       : "<<cfg.ofileName<<"\n";
    std::cout<<" treeName    : "<<cfg.treeName<<"\n";
    std::cout<<" doEmulationBranches : "<<cfg.doEmulationBranches<<"\n";
    std::cout<<" doAllRuns   : "<<cfg.doAllRuns<<"\n";
    std::cout<<" maxEvents   : "<<cfg.maxEvents<<"\n";
    std::cout<<" reportEvery : "<<cfg.reportEvery<<"\n";
    std::cout<<" RunNumbers  : ";
        for( auto i : cfg.RunNumbers)
        {
            std::cout<<i<<",";
        }
    produceTurnOns(
                    cfg.infile,
                    cfg.ofileName,
                    cfg.treeName,
                    cfg.prefix,
                    cfg.doEmulationBranches,
                    cfg.doAllRuns,
                    cfg.RunNumbers,
                    cfg.maxEvents,
                    cfg.reportEvery
                  );

}
