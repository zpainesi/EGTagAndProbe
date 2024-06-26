#include "Resolution.h"

using namespace std;

void produceResolution(string infile,string ofileName,string prefix="",bool doEmulation=false,Long64_t maxEvents=100000);

void resolution_production(int mode=0)
{

    //produceResolution(infile,ofileName,prefix,maxEvents);
  if(mode==0)
  produceResolution("/grid_mnt/t3storage3/athachay/l1egamma/data/run3MC/TandPNTuples_UnpackedMiniAOD_12XSamples.root",
                    "resolution.root",
                    "run3MC_12_0_2_",
                    false,
                   -1e5);

  if(mode==1)
  produceResolution("/grid_mnt/t3storage3/athachay/l1egamma/data/2018/dataEGamma_120X_Run2018A-PromptReco-v3_TP.root",
                    "resolution.root",
                    "data2018_default_",
                    false,
                   -1e6);

  if(mode==2)
  produceResolution("/grid_mnt/t3storage3/athachay/l1egamma/data/run3MC/TagAndProbe_DefaultReemulationV0_120Xsample_CaloParams_2022_v0_1.root",
                    "resolution.root",
                    "run3MC_12_0_2_defaultRecoCloParams2022__",
                    true,
                   4e6);
  if(mode==3)
  produceResolution("/eos/cms/store/group/dpg_trigger/comm_trigger/L1Trigger/athachay/store/mcDYToLL_120X_mcRun3_2021_realistic_v5-v2/TagAndProbe_EtCaliberatedReEmulationV2p1_120Xsample_CaloParams_2022_v0_1.root",
                    "resolution.root",
                    "run3MC_12_3_0_pre3RecalibV2_",
                    true,
                   5e5);
  if(mode==4)
  produceResolution("/grid_mnt/t3storage3/athachay/l1egamma/data/run3MC/TagAndProbe_EtCaliberatedReEmulationV3p2_120Xsample_CaloParams_2022_v0_1_part1.root",
                    "resolution.root",
                    "run3MC_12_0_2_RecalibV3_",
                    true,
                    -1e6);


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
    const Int_t dPtBINS = 400; 
	Double_t dPtEdges[dPtBINS + 1];
    for(int i=0;i<=dPtBINS;i++) dPtEdges[i] = -0.005 + 0.01*i ;
	
    const Int_t dPtHighResBINS = 4000; 
	Double_t dPtHighResEdges[dPtHighResBINS + 1];
    for(int i=0;i<=dPtHighResBINS;i++) dPtHighResEdges[i] = ( 0.0 - 2.0/dPtHighResBINS ) + (4.0/dPtHighResBINS)*i ;
	
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
    
    auto mass= new TH1F("mass","mass",200.0,0.5,199.5);
    auto pT  = new TH1F("pT","pT",200.0,0.5,199.5);
    TLorentzVector eA,eB;

    resolutionMap["EtaVsdEta"] = new resolutionMeasurement("EtaVsdEta",EtaBINS,EtaEdges,dEtaBINS,dEtaEdges);
    resolutionMap["PhiVsdPhi"] = new resolutionMeasurement("PhiVsdPhi",PhiBINS,PhiEdges,dPhiBINS,dPhiEdges);
    
    resolutionMap["EtVsdEt"  ] = new resolutionMeasurement("EtVsdEt",PtBINS,PtEdges,dPtBINS,dPtEdges);
    resolutionMap["EtaVsdEt"  ] = new resolutionMeasurement("EtaVsdEt",EtaBINS,EtaEdges,dPtBINS,dPtEdges);
    resolutionMap["EtVsdEt_highReso"  ] = new resolutionMeasurement("EtVsdEt_highReso",PtBINS,PtEdges,dPtHighResBINS,dPtHighResEdges);
    resolutionMap["EtaVsdEt_highReso"  ] = new resolutionMeasurement("EtaVsdEt_highReso",EtaBINS,EtaEdges,dPtHighResBINS,dPtHighResEdges);

    resolutionMap["Pt5To15EtVsdEt"  ] = new resolutionMeasurement("Pt5To15EtVsdEt",PtBINS,PtEdges,dPtBINS,dPtEdges);
    resolutionMap["Pt5To15EtaVsdEt"  ] = new resolutionMeasurement("Pt5To15VsdEt",EtaBINS,EtaEdges,dPtBINS,dPtEdges);
    resolutionMap["Pt5To15EtVsdEt_highReso"  ] = new resolutionMeasurement("Pt5To15EtVsdEt_highReso",PtBINS,PtEdges,dPtHighResBINS,dPtHighResEdges);
    resolutionMap["Pt5To15EtaVsdEt_highReso"  ] = new resolutionMeasurement("Pt5To15EtaVsdEt_highReso",EtaBINS,EtaEdges,dPtHighResBINS,dPtHighResEdges);
    
    resolutionMap["Pt0To20EtVsdEt"  ] = new resolutionMeasurement("Pt0To20EtVsdEt",PtBINS,PtEdges,dPtBINS,dPtEdges);
    resolutionMap["Pt0To20EtaVsdEt"  ] = new resolutionMeasurement("Pt0To20EtaVsdEt",EtaBINS,EtaEdges,dPtBINS,dPtEdges);
    resolutionMap["Pt0To20EtVsdEt_highReso"  ] = new resolutionMeasurement("Pt0To20EtVsdEt_highReso",PtBINS,PtEdges,dPtHighResBINS,dPtHighResEdges);
    resolutionMap["Pt0To20EtaVsdEt_highReso"  ] = new resolutionMeasurement("Pt0To20EtaVsdEt_highReso",EtaBINS,EtaEdges,dPtHighResBINS,dPtHighResEdges);

    resolutionMap["Pt20To40EtVsdEt"  ] = new resolutionMeasurement("Pt20To40EtVsdEt",PtBINS,PtEdges,dPtBINS,dPtEdges);
    resolutionMap["Pt20To40EtaVsdEt"  ] = new resolutionMeasurement("Pt20To40EtaVsdEt",EtaBINS,EtaEdges,dPtBINS,dPtEdges);
    resolutionMap["Pt20To40EtVsdEt_highReso"  ] = new resolutionMeasurement("Pt20To40EtVsdEt_highReso",PtBINS,PtEdges,dPtHighResBINS,dPtHighResEdges);
    resolutionMap["Pt20To40EtaVsdEt_highReso"  ] = new resolutionMeasurement("Pt20To40EtaVsdEt_highReso",EtaBINS,EtaEdges,dPtHighResBINS,dPtHighResEdges);

    resolutionMap["Pt40To60EtVsdEt"  ] = new resolutionMeasurement("Pt40To60EtVsdEt",PtBINS,PtEdges,dPtBINS,dPtEdges);
    resolutionMap["Pt40To60EtaVsdEt"  ] = new resolutionMeasurement("Pt40To60EtaVsdEt",EtaBINS,EtaEdges,dPtBINS,dPtEdges);
    resolutionMap["Pt40To60EtVsdEt_highReso"  ] = new resolutionMeasurement("Pt40To60EtVsdEt_highReso",PtBINS,PtEdges,dPtHighResBINS,dPtHighResEdges);
    resolutionMap["Pt40To60EtaVsdEt_highReso"  ] = new resolutionMeasurement("Pt40To60EtaVsdEt_highReso",EtaBINS,EtaEdges,dPtHighResBINS,dPtHighResEdges);

    resolutionMap["Pt60To80EtVsdEt"  ] = new resolutionMeasurement("Pt60To80EtVsdEt",PtBINS,PtEdges,dPtBINS,dPtEdges);
    resolutionMap["Pt60To80EtaVsdEt"  ] = new resolutionMeasurement("Pt60To80EtaVsdEt",EtaBINS,EtaEdges,dPtBINS,dPtEdges);
    resolutionMap["Pt60To80EtVsdEt_highReso"  ] = new resolutionMeasurement("Pt60To80EtVsdEt_highReso",PtBINS,PtEdges,dPtHighResBINS,dPtHighResEdges);
    resolutionMap["Pt60To80EtaVsdEt_highReso"  ] = new resolutionMeasurement("Pt60To80EtaVsdEt_highReso",EtaBINS,EtaEdges,dPtHighResBINS,dPtHighResEdges);

    resolutionMap["Pt80To120EtVsdEt"  ] = new resolutionMeasurement("Pt80To120EtVsdEt",PtBINS,PtEdges,dPtBINS,dPtEdges);
    resolutionMap["Pt80To120EtaVsdEt"  ] = new resolutionMeasurement("Pt80To120EtaVsdEt",EtaBINS,EtaEdges,dPtBINS,dPtEdges);
    resolutionMap["Pt80To120EtVsdEt_highReso"  ] = new resolutionMeasurement("Pt80To120EtVsdEt_highReso",PtBINS,PtEdges,dPtHighResBINS,dPtHighResEdges);
    resolutionMap["Pt80To120EtaVsdEt_highReso"  ] = new resolutionMeasurement("Pt80To120EtaVsdEt_highReso",EtaBINS,EtaEdges,dPtHighResBINS,dPtHighResEdges);

    resolutionMap["Pt120To200EtVsdEt"  ] = new resolutionMeasurement("Pt120To200EtVsdEt",PtBINS,PtEdges,dPtBINS,dPtEdges);
    resolutionMap["Pt120To200EtaVsdEt"  ] = new resolutionMeasurement("Pt120To200EtaVsdEt",EtaBINS,EtaEdges,dPtBINS,dPtEdges);
    resolutionMap["Pt120To200EtVsdEt_highReso"  ] = new resolutionMeasurement("Pt120To200EtVsdEt_highReso",PtBINS,PtEdges,dPtHighResBINS,dPtHighResEdges);
    resolutionMap["Pt120To200EtaVsdEt_highReso"  ] = new resolutionMeasurement("Pt120To200EtaVsdEt_highReso",EtaBINS,EtaEdges,dPtHighResBINS,dPtHighResEdges);

    resolutionMap["Pt200ToXEtVsdEt"  ] = new resolutionMeasurement("Pt200ToXEtVsdEt",PtBINS,PtEdges,dPtBINS,dPtEdges);
    resolutionMap["Pt200ToXEtaVsdEt"  ] = new resolutionMeasurement("Pt200ToXEtaVsdEt",EtaBINS,EtaEdges,dPtBINS,dPtEdges);
    resolutionMap["Pt200ToXEtVsdEt_highReso"  ] = new resolutionMeasurement("Pt200ToXEtVsdEt_highReso",PtBINS,PtEdges,dPtHighResBINS,dPtHighResEdges);
    resolutionMap["Pt200ToXEtaVsdEt_highReso"  ] = new resolutionMeasurement("Pt200ToXEtaVsdEt_highReso",EtaBINS,EtaEdges,dPtHighResBINS,dPtHighResEdges);

    //res_vs_et_barrel->GetYaxis()->SetTitle("FWHM / (E_{T}^{e#gamma, L1}/E_{T}^{e#gamma, offline})_{at maximum}");
	//res_vs_et_barrel->GetXaxis()->SetTitle("E_{T}^{e#gamma, offline} [GeV]");
    //res_vs_eta_inclusive->GetXaxis()->SetTitle("|#eta^{e#gamma, offline}|");
	//res_vs_eta_inclusive->GetYaxis()->SetTitle("FWHM / (E_{T}^{e#gamma, L1}/E_{T}^{e#gamma, offline})_{at maximum}");

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    t1->SetBranchAddress("eleProbePt",&eleProbePt);
    t1->SetBranchAddress("eleProbeEta",&eleProbeEta);
    t1->SetBranchAddress("eleProbePhi",&eleProbePhi);
    t1->SetBranchAddress("eleProbeSclEt",&eleProbeSclEt);
    t1->SetBranchAddress("eleTagPt",&eleTagPt);
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
    
    auto t_start = std::chrono::high_resolution_clock::now();
    auto t_end = std::chrono::high_resolution_clock::now();

    for (int jentry=0; jentry<nentries; jentry++){
        
        t1->GetEntry(jentry);
		
        if(jentry%10000==0)
        {
             t_end = std::chrono::high_resolution_clock::now();
             cout<<"Processing Entry "<<jentry<<" / "<<nentries<<"  [ "<<100.0*jentry/nentries<<"  % ]  "
             << " Elapsed time : "<< std::chrono::duration<double, std::milli>(t_end-t_start).count()/1000.0
             <<"  Estimated time left : "<< std::chrono::duration<double, std::milli>(t_end-t_start).count()*( nentries - jentry)/jentry * 0.001
             <<endl;
        }



        pT->Fill(eleProbeSclEt);
        eA.SetPtEtaPhiM(eleProbePt,eleProbeEta,eleProbePhi,0.5e-3);
        eB.SetPtEtaPhiM(eleTagPt,eleTagEta,eleTagPhi,0.5e-3);
        //std::cout<<"Ea , eb "<<eA.Pt()<<" , "<<eB.Pt()<<" Mass  : "<<(eA+eB).M()<<"\n";
        mass->Fill((eA+eB).M());
		if(isProbeLoose==1){

            resolutionMap["EtaVsdEta"] ->Fill(eleProbeEta,abs(eleProbeEta),l1tEta -  eleProbeEta);
            resolutionMap["PhiVsdPhi"] ->Fill(eleProbeEta,eleProbePhi,l1tPhi-eleProbePhi);
            resolutionMap["EtaVsdEt"]  ->Fill(eleProbeEta,abs(eleProbeEta),l1tPt/eleProbeSclEt);
            resolutionMap["EtVsdEt"]   ->Fill(eleProbeEta,eleProbeSclEt,l1tPt/eleProbeSclEt);
            resolutionMap["EtaVsdEt_highReso"]  ->Fill(eleProbeEta,abs(eleProbeEta),l1tPt/eleProbeSclEt);
            resolutionMap["EtVsdEt_highReso"]   ->Fill(eleProbeEta,eleProbeSclEt,l1tPt/eleProbeSclEt);
		 if( ( eleProbeSclEt < 15.0 ) and ( eleProbeSclEt > 5.0 ) )
          {
            resolutionMap["Pt5To15EtaVsdEt"]  ->Fill(eleProbeEta,abs(eleProbeEta),l1tPt/eleProbeSclEt);
            resolutionMap["Pt5To15EtVsdEt"]   ->Fill(eleProbeEta,eleProbeSclEt,l1tPt/eleProbeSclEt);
            resolutionMap["Pt5To15EtaVsdEt_highReso"]  ->Fill(eleProbeEta,abs(eleProbeEta),l1tPt/eleProbeSclEt);
            resolutionMap["Pt5To15EtVsdEt_highReso"]   ->Fill(eleProbeEta,eleProbeSclEt,l1tPt/eleProbeSclEt);
          } 
          if( eleProbeSclEt < 20.0 )
          {
            resolutionMap["Pt0To20EtaVsdEt"]  ->Fill(eleProbeEta,abs(eleProbeEta),l1tPt/eleProbeSclEt);
            resolutionMap["Pt0To20EtVsdEt"]   ->Fill(eleProbeEta,eleProbeSclEt,l1tPt/eleProbeSclEt);
            resolutionMap["Pt0To20EtaVsdEt_highReso"]  ->Fill(eleProbeEta,abs(eleProbeEta),l1tPt/eleProbeSclEt);
            resolutionMap["Pt0To20EtVsdEt_highReso"]   ->Fill(eleProbeEta,eleProbeSclEt,l1tPt/eleProbeSclEt);
          }
          else if ( eleProbeSclEt < 40.0 )
          {
            resolutionMap["Pt20To40EtaVsdEt"]  ->Fill(eleProbeEta,abs(eleProbeEta),l1tPt/eleProbeSclEt);
            resolutionMap["Pt20To40EtVsdEt"]   ->Fill(eleProbeEta,eleProbeSclEt,l1tPt/eleProbeSclEt);
            resolutionMap["Pt20To40EtaVsdEt_highReso"]  ->Fill(eleProbeEta,abs(eleProbeEta),l1tPt/eleProbeSclEt);
            resolutionMap["Pt20To40EtVsdEt_highReso"]   ->Fill(eleProbeEta,eleProbeSclEt,l1tPt/eleProbeSclEt);
          }
          else if ( eleProbeSclEt < 60.0 )
          {
            resolutionMap["Pt40To60EtaVsdEt"]  ->Fill(eleProbeEta,abs(eleProbeEta),l1tPt/eleProbeSclEt);
            resolutionMap["Pt40To60EtVsdEt"]   ->Fill(eleProbeEta,eleProbeSclEt,l1tPt/eleProbeSclEt);
            resolutionMap["Pt40To60EtaVsdEt_highReso"]  ->Fill(eleProbeEta,abs(eleProbeEta),l1tPt/eleProbeSclEt);
            resolutionMap["Pt40To60EtVsdEt_highReso"]   ->Fill(eleProbeEta,eleProbeSclEt,l1tPt/eleProbeSclEt);
          }
          else if( eleProbeSclEt < 80.0 )
          {
            resolutionMap["Pt60To80EtaVsdEt"]  ->Fill(eleProbeEta,abs(eleProbeEta),l1tPt/eleProbeSclEt);
            resolutionMap["Pt60To80EtVsdEt"]   ->Fill(eleProbeEta,eleProbeSclEt,l1tPt/eleProbeSclEt);
            resolutionMap["Pt60To80EtaVsdEt_highReso"]  ->Fill(eleProbeEta,abs(eleProbeEta),l1tPt/eleProbeSclEt);
            resolutionMap["Pt60To80EtVsdEt_highReso"]   ->Fill(eleProbeEta,eleProbeSclEt,l1tPt/eleProbeSclEt);
          }
          else if( eleProbeSclEt < 120.0 )
          {
            resolutionMap["Pt80To120EtaVsdEt"]  ->Fill(eleProbeEta,abs(eleProbeEta),l1tPt/eleProbeSclEt);
            resolutionMap["Pt80To120EtVsdEt"]   ->Fill(eleProbeEta,eleProbeSclEt,l1tPt/eleProbeSclEt);
            resolutionMap["Pt80To120EtaVsdEt_highReso"]  ->Fill(eleProbeEta,abs(eleProbeEta),l1tPt/eleProbeSclEt);
            resolutionMap["Pt80To120EtVsdEt_highReso"]   ->Fill(eleProbeEta,eleProbeSclEt,l1tPt/eleProbeSclEt);
          }
          else if( eleProbeSclEt < 200.0 )
          {
            resolutionMap["Pt120To200EtaVsdEt"]  ->Fill(eleProbeEta,abs(eleProbeEta),l1tPt/eleProbeSclEt);
            resolutionMap["Pt120To200EtVsdEt"]   ->Fill(eleProbeEta,eleProbeSclEt,l1tPt/eleProbeSclEt);
            resolutionMap["Pt120To200EtaVsdEt_highReso"]  ->Fill(eleProbeEta,abs(eleProbeEta),l1tPt/eleProbeSclEt);
            resolutionMap["Pt120To200EtVsdEt_highReso"]   ->Fill(eleProbeEta,eleProbeSclEt,l1tPt/eleProbeSclEt);
          }
          else
          {
            resolutionMap["Pt200ToXEtaVsdEt"]  ->Fill(eleProbeEta,abs(eleProbeEta),l1tPt/eleProbeSclEt);
            resolutionMap["Pt200ToXEtVsdEt"]   ->Fill(eleProbeEta,eleProbeSclEt,l1tPt/eleProbeSclEt);
            resolutionMap["Pt200ToXEtaVsdEt_highReso"]  ->Fill(eleProbeEta,abs(eleProbeEta),l1tPt/eleProbeSclEt);
            resolutionMap["Pt200ToXEtVsdEt_highReso"]   ->Fill(eleProbeEta,eleProbeSclEt,l1tPt/eleProbeSclEt);
          }
			

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
    pT->Write();
    mass->Write();
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

