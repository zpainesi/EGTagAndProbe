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
#include "secondary_peak.h"
#include "/afs/cern.ch/work/c/ckoraka/codes/setTDRStyle_teliko.C"

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


void turn_on_production()
{

	//////		For plotting 		////////////////////////////////////////////////////
	gROOT->LoadMacro("/afs/cern.ch/work/c/ckoraka/codes/setTDRStyle_teliko.C");
	setTDRStyle_teliko();


	double den1,den2,num1,num2; 	

	TPaveText *paveCMS = new TPaveText(0.21,0.96,0.93,0.99,"NDC");
        paveCMS->AddText("#bf{CMS #it{Preliminary}}     (13 TeV)");
        paveCMS->SetFillColor(0);
        paveCMS->SetBorderSize(0);
        paveCMS->SetTextSize(0.04);
        paveCMS->SetTextFont(42);


	const Int_t XBINS = 38; 

	Double_t xEdges[XBINS + 1] = {1., 3., 5., 7., 9.,  10., 12., 15., 18., 20., 22., 24., 26., 28., 29., 30., 31., 32., 33., 34., 35., 36., 37., 38., 39., 40., 41., 42., 43., 45., 50., 60., 70., 100., 200., 300., 400., 600., 1000.};
	
        TH1D *TT1 = new TH1D("TT1","TT1",XBINS,xEdges);
        TH1D *TP1 = new TH1D("TP1","TP1",XBINS,xEdges);
        TH1D *TF1 = new TH1D("TF1","TF1",XBINS,xEdges);

        TH1D *TT2 = new TH1D("TT2","TT2",XBINS,xEdges);
        TH1D *TP2 = new TH1D("TP2","TP2",XBINS,xEdges);
        TH1D *TF2 = new TH1D("TF2","TF2",XBINS,xEdges);



        TH1D *TT1_num = new TH1D("TT1num","TT1num",XBINS,xEdges);
        TH1D *TP1_num = new TH1D("TP1num","TP1num",XBINS,xEdges);
        TH1D *TF1_num = new TH1D("TF1num","TF1num",XBINS,xEdges);
        TT1_num->SetLineColor(kBlack);
        TP1_num->SetLineColor(kBlack);
        TF1_num->SetLineColor(kBlack);        

        TH1D *TT2_num = new TH1D("TT2num","TT2num",XBINS,xEdges);
        TH1D *TP2_num = new TH1D("TP2num","TP2num",XBINS,xEdges);
        TH1D *TF2_num = new TH1D("TF2num","TF2num",XBINS,xEdges);
        TT2_num->SetLineColor(kBlue);
        TP2_num->SetLineColor(kBlue);
        TF2_num->SetLineColor(kBlue);


        TH1D *eff1 = new TH1D("eff1","eff1",XBINS,xEdges);
        eff1->SetLineColor(kBlue);
        eff1->SetMarkerColor(kBlue);
        eff1->SetMarkerStyle(22);
        TH1D *eff2 = new TH1D("eff2","eff2",XBINS,xEdges);
        eff2->SetLineColor(kRed+1);
        eff2->SetMarkerColor(kRed+1);
        eff2->SetMarkerStyle(22);

	//////////////////////////////////////////////////////////////////////////////////////////////

	double dR,dphi,phi1,phi2;	

        TFile *f=new TFile("/eos/cms/store/group/dpg_trigger/comm_trigger/L1Trigger/ckoraka/NTuplesFor2018PerformancePlots/data_2018/reducedTrees/EGamma_Run2018ABCD_reduced.root","READ");
	TTree *t1=(TTree*)(f->Get("Ntuplizer/TagAndProbe"));
	
        t1->SetBranchAddress("eleProbeEta",&eleProbeEta);
        t1->SetBranchAddress("eleProbePhi",&eleProbePhi);
        t1->SetBranchAddress("eleTagEta",&eleTagEta);
        t1->SetBranchAddress("eleTagPhi",&eleTagPhi);
        t1->SetBranchAddress("l1tEta",&l1tEta);
        t1->SetBranchAddress("l1tPhi",&l1tPhi);
        t1->SetBranchAddress("Nvtx",&Nvtx);
        t1->SetBranchAddress("isProbeLoose",&isProbeLoose);
        t1->SetBranchAddress("eleProbeSclEt",&eleProbeSclEt);
        t1->SetBranchAddress("hasL1_40",&hasL1_40);
        t1->SetBranchAddress("hasL1_30",&hasL1_30);

		
	int nentries = t1->GetEntries();	

	for (int jentry=0; jentry<nentries; jentry++){
	
		t1->GetEntry(jentry);  		
	
               if( isProbeLoose==1 && fabs(eleProbeEta) < 2.5  && sqrt(pow(eleProbeEta-eleTagEta,2)+pow(eleProbePhi-eleTagPhi,2))>0.6) {
                	if(hasL1_40==1) TP1->Fill(eleProbeSclEt,1);
                	if(hasL1_40==0) TF1->Fill(eleProbeSclEt,1);

                	if(hasL1_30==1) TP2->Fill(eleProbeSclEt,1);
                	if(hasL1_30==0) TF2->Fill(eleProbeSclEt,1);

		}
	}
 	
	f->Close();

	double error1,error2;
     
	for (int k=1; k<=(TP1->GetNbinsX()); ++k){
		
                num2 = TP2->GetBinContent(k);
                den2 = TP2->GetBinContent(k) + TF2->GetBinContent(k);
		num1 = TP1->GetBinContent(k);
                den1 = TP1->GetBinContent(k) + TF1->GetBinContent(k); 
	

		eff1->SetBinErrorOption(TH1::kPoisson);
		eff2->SetBinErrorOption(TH1::kPoisson);

		if(den1!=0){
			error1 = (1./pow(den1,2))*sqrt(pow(TP1->GetBinError(k)*TF1->GetBinContent(k),2) + pow(TF1->GetBinError(k)*TP1->GetBinContent(k),2));
			eff1->SetBinContent(k,num1/den1);
			//eff1->SetBinError(k,error1);

		}
		else{
	 		eff1->SetBinContent(k,0.);
			//eff1->SetBinError(k,0);                  
		}
		if(den2!=0){
			error2 = (1./pow(den2,2))*sqrt(pow(TP2->GetBinError(k)*TF2->GetBinContent(k),2) + pow(TF2->GetBinError(k)*TP2->GetBinContent(k),2));
	       		eff2->SetBinContent(k,num2/den2);      
	       		//eff2->SetBinError(k,error2);      
		}
		else{
	      		eff2->SetBinContent(k,0.);
                  	//eff2->SetBinError(k,0);      
		}

	}
	double a  = 0.3173;
  	for(int k=1; k<=(TP1->GetNbinsX()); ++k){
		double N2 = TP1->GetBinContent(k);
		double N1 = TP1->GetBinContent(k) + TF1->GetBinContent(k);
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
		eff1->SetBinError(k,eL);
	}


  	for(int k=1; k<=(TP1->GetNbinsX()); ++k){
		double N2 = TP2->GetBinContent(k);
		double N1 = TP2->GetBinContent(k) + TF2->GetBinContent(k);
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
		eff2->SetBinError(k,eL);
	}


	TLegend *l = new TLegend(.6, .5, .9, .7);
	l->AddEntry(eff1,"L1 E_{T} > 30","l");
	l->AddEntry(eff2,"L1 E_{T} > 40","l");  
	l->SetTextSize(0.03);


        TCanvas *c4 = new TCanvas("c4","TT");
        eff1->GetXaxis()->SetTitle("E_{T} probe [GeV]");
        eff1->Draw("ep");
        eff2->Draw("ep same");
        l->Draw("same");
        paveCMS->Draw("same");
        c4->SaveAs("c4.png");




}

