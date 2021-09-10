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
//#include "secondary_peak.h"
//#include "/afs/cern.ch/work/c/ckoraka/codes/setTDRStyle_teliko.C"

void produceTurnOns(string infile,string ofileName,string prefix="",Long64_t maxEvents=100000);
void turn_on_production(string infile,string ofileName,string prefix="",Long64_t maxEvents=100000)
{

    produceTurnOns(infile,ofileName,prefix,maxEvents);
 //   produceTurnOns("/grid_mnt/t3storage3/athachay/l1egamma/triggerPerformance/CMSSW_10_6_25/src/EGTagAndProbe/EGTagAndProbe/test/fitter/Run3MC_TagAndProbeNtuple.root",
 //                   "turnon.root",
 //                   "run3MC_",
 //                  -1);
}

class efficiencyMeasurement
{

    public :
        string name;
        TH1D* passHist;
        TH1D* failHist;
        TH1D* effiHist;
        
        efficiencyMeasurement(std::string namep,Int_t XBINS,Double_t *xEdges): passHist(nullptr),failHist(nullptr),effiHist(nullptr)
        {
            name=namep;
            init(XBINS,xEdges);
        }   

        void init(Int_t XBINS,Double_t * xEdges)
        {   
           
            freeMem();
 //           std::cout<<" Allocation for "<<name<<"\n";
            passHist = new TH1D(("hist_"+name+"Pass").c_str(),("hist_"+name+"Pass").c_str(),XBINS,xEdges);
            failHist = new TH1D(("hist_"+name+"Fail").c_str(),("hist_"+name+"Fail").c_str(),XBINS,xEdges);
            effiHist = new TH1D(("hist_"+name+"Efficiency").c_str(),("hist_"+name+"Efficiency").c_str(),XBINS,xEdges);
            
            effiHist->SetDirectory(0); 
            passHist->SetDirectory(0); 
            failHist->SetDirectory(0); 
            
            effiHist->SetLineColor(kBlue);
            effiHist->SetMarkerColor(kBlue);
            effiHist->SetMarkerStyle(22);
        }
        void fill(bool rslt,Double_t x)
        {
            if (rslt == 0 ) failHist->Fill(x);
            if (rslt == 1 ) passHist->Fill(x);

        }
        void Write()
        {
            passHist->Write();
            failHist->Write();
            effiHist->Write();
        }

        void  freeMem()
        {
   //        std::cout<<" De-Allocation for "<<name<<"\n";
          if(passHist) delete passHist;
          if(failHist) delete failHist;
          if(effiHist) delete effiHist;

        }

        ~efficiencyMeasurement()
        {
            freeMem();
        }

};

double BetaInverse(double x,double p, double q)
{
	double result(0.0);
	double dy = 0.001;  
	double eMin = 100;
	for(int i=0;i<1000;i++){
		double y = i*dy;
	        double e = fabs(TMath::BetaIncomplete(y,p,q)-x);
      		if (e<eMin)  
        	{
         		eMin = e;
          		result = y;
        	}
    	}
  	return result;
}

void produceTurnOns(string infile,string ofileName,string prefix="",Long64_t maxEvents=100000)
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
    float l1tEta		;
    float l1tPhi		;
    Int_t Nvtx			;
    Int_t isProbeLoose	;
    float eleProbeSclEt	;


    TFile *f=new TFile(infile.c_str(),"READ");
    TTree *t1=(TTree*)(f->Get("Ntuplizer/TagAndProbe"));
	
    t1->SetBranchAddress("eleProbeEta"  ,&eleProbeEta			);
    t1->SetBranchAddress("eleProbePhi"  ,&eleProbePhi			);
    t1->SetBranchAddress("eleTagEta"    ,&eleTagEta		    	);
    t1->SetBranchAddress("eleTagPhi"    ,&eleTagPhi			    );
    t1->SetBranchAddress("l1tEta"       ,&l1tEta			    );
    t1->SetBranchAddress("l1tPhi"       ,&l1tPhi			    );
    t1->SetBranchAddress("Nvtx"         ,&Nvtx			        );
    t1->SetBranchAddress("isProbeLoose" ,&isProbeLoose			);
    t1->SetBranchAddress("eleProbeSclEt",&eleProbeSclEt			);


    int   hasL1_24, hasL1_looseiso_24, hasL1_tightiso_24 ;
    t1->SetBranchAddress("hasL1_24"     ,&hasL1_24			    );
    t1->SetBranchAddress("hasL1_tightiso_24",&hasL1_tightiso_24 );
    t1->SetBranchAddress("hasL1_looseiso_24",&hasL1_tightiso_24 );
    triggers["L1_24"] = new efficiencyMeasurement("L1_24",XBINS,xEdges);
    triggers["L1_looseiso_24"] = new efficiencyMeasurement("L1_looseiso_24",XBINS,xEdges);
    triggers["L1_tightiso_24"] = new efficiencyMeasurement("L1_tightiso_24",XBINS,xEdges);
    
    int   hasL1_26, hasL1_looseiso_26, hasL1_tightiso_26 ;
    t1->SetBranchAddress("hasL1_26"     ,&hasL1_26			    );
    t1->SetBranchAddress("hasL1_tightiso_26",&hasL1_tightiso_26 );
    t1->SetBranchAddress("hasL1_looseiso_26",&hasL1_tightiso_26 );
    triggers["L1_26"] = new efficiencyMeasurement("L1_26",XBINS,xEdges);
    triggers["L1_looseiso_26"] = new efficiencyMeasurement("L1_looseiso_26",XBINS,xEdges);
    triggers["L1_tightiso_26"] = new efficiencyMeasurement("L1_tightiso_26",XBINS,xEdges);
    
    int   hasL1_28, hasL1_looseiso_28, hasL1_tightiso_28 ;
    t1->SetBranchAddress("hasL1_28"     ,&hasL1_28			    );
    t1->SetBranchAddress("hasL1_tightiso_28",&hasL1_tightiso_28 );
    t1->SetBranchAddress("hasL1_looseiso_28",&hasL1_tightiso_28 );
    triggers["L1_28"] = new efficiencyMeasurement("L1_28",XBINS,xEdges);
    triggers["L1_looseiso_28"] = new efficiencyMeasurement("L1_looseiso_28",XBINS,xEdges);
    triggers["L1_tightiso_28"] = new efficiencyMeasurement("L1_tightiso_28",XBINS,xEdges);
    
    int   hasL1_30, hasL1_looseiso_30, hasL1_tightiso_30 ;
    t1->SetBranchAddress("hasL1_30"     ,&hasL1_30			    );
    t1->SetBranchAddress("hasL1_tightiso_30",&hasL1_tightiso_30 );
    t1->SetBranchAddress("hasL1_looseiso_30",&hasL1_tightiso_30 );
    triggers["L1_30"] = new efficiencyMeasurement("L1_30",XBINS,xEdges);
    triggers["L1_looseiso_30"] = new efficiencyMeasurement("L1_looseiso_30",XBINS,xEdges);
    triggers["L1_tightiso_30"] = new efficiencyMeasurement("L1_tightiso_30",XBINS,xEdges);
    
    int   hasL1_32, hasL1_looseiso_32, hasL1_tightiso_32 ;
    t1->SetBranchAddress("hasL1_32"     ,&hasL1_32			    );
    t1->SetBranchAddress("hasL1_tightiso_32",&hasL1_tightiso_32 );
    t1->SetBranchAddress("hasL1_looseiso_32",&hasL1_tightiso_32 );
    triggers["L1_32"] = new efficiencyMeasurement("L1_32",XBINS,xEdges);
    triggers["L1_looseiso_32"] = new efficiencyMeasurement("L1_looseiso_32",XBINS,xEdges);
    triggers["L1_tightiso_32"] = new efficiencyMeasurement("L1_tightiso_32",XBINS,xEdges);
    
    int   hasL1_34, hasL1_looseiso_34, hasL1_tightiso_34 ;
    t1->SetBranchAddress("hasL1_34"     ,&hasL1_34			    );
    t1->SetBranchAddress("hasL1_tightiso_34",&hasL1_tightiso_34 );
    t1->SetBranchAddress("hasL1_looseiso_34",&hasL1_tightiso_34 );
    triggers["L1_34"] = new efficiencyMeasurement("L1_34",XBINS,xEdges);
    triggers["L1_looseiso_34"] = new efficiencyMeasurement("L1_looseiso_34",XBINS,xEdges);
    triggers["L1_tightiso_34"] = new efficiencyMeasurement("L1_tightiso_34",XBINS,xEdges);
 
    int   hasL1_40, hasL1_looseiso_40, hasL1_tightiso_40 ;
    t1->SetBranchAddress("hasL1_40"     ,&hasL1_40			    );
    t1->SetBranchAddress("hasL1_tightiso_40",&hasL1_tightiso_40 );
    t1->SetBranchAddress("hasL1_looseiso_40",&hasL1_tightiso_40 );
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
	for (Long64_t jentry=0; jentry<nentries; jentry++){

       if(jentry%10000 ==0 )
       {
            std::cout<<"Processing jentry : "<<jentry<<"\n";
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
        
		curTrigger.effiHist->SetBinErrorOption(TH1::kPoisson);
        
	    for (int k=1; k<=(curTrigger.passHist->GetNbinsX()); ++k){
		
                num = curTrigger.passHist->GetBinContent(k);
                den = curTrigger.passHist->GetBinContent(k) + curTrigger.failHist->GetBinContent(k);


		if(den!=0){
			curTrigger.effiHist->SetBinContent(k,num/den);
	//		error1 = (1./pow(den,2))*\
                    sqrt(\
                        pow(curTrigger.passHist->GetBinError(k)*curTrigger.failHist->GetBinContent(k),2) + \
                        pow(hist_hasL130Fail->GetBinError(k)*hist_hasL130Pass->GetBinContent(k),2)\
                      );
			//hist_effL130->SetBinError(k,error1);

		}
		else{
	 		curTrigger.effiHist->SetBinContent(k,0.);
			//hist_effL130->SetBinError(k,0);                  
		}
		
        double N2 = num;
		double N1 = den;
		double p  = 0;
		double eU = 0;
      		double eL = 0;
      		double aeff = (1-a)/2;
      		double n,w;
      		if (N1 > 0)
        	{
          		p = N2/N1;
          		n = N1+N2;
          		w = N2/n;
        		if (N1*p>100 || N1*(1-p)>100){ //cout<< " pass " <<endl;
				eU = sqrt(p*(1-p)/N1);
				eL = sqrt(p*(1-p)/N1);
                	}
              		else
                	{   //cout<<" pass2 "<<endl;
                  		eU = (1-BetaInverse(aeff,N1-N2,N2+1))-p;
                  		eL = p-(1-BetaInverse(1-aeff,N1-N2+1,N2));
                	}
        	}
		curTrigger.effiHist->SetBinError(k,eL);
	   }
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

