#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <sstream>
#include <string>
#include <ctime>
#include <cstdlib>

#include <TROOT.h>
#include "TH1.h"
#include "TH2.h"
#include "TF1.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TGraph.h"
#include "TLine.h"
#include "TString.h"
#include "TMath.h"
#include "TString.h"
#include "TPaveText.h"
#include "TLegend.h"
#include "TTree.h"
#include "TFile.h"
#include "TLatex.h"
#include "TMarker.h"
#include "TAxis.h"

void rate_calculation()
{
	TPaveText *paveCMS = new TPaveText(0.15,0.96,0.93,0.99,"NDC");
	paveCMS->AddText("#bf{CMS #it{Preliminary}} 2018      154 pb^{-1} (13 TeV)");
	paveCMS->SetFillColor(0);
	paveCMS->SetBorderSize(0);
	paveCMS->SetTextSize(0.04);
	paveCMS->SetTextFont(42);

	TFile *f=TFile::Open("/eos/cms/store/group/dpg_trigger/comm_trigger/L1Trigger/ckoraka/NtuplesForCMSWeek/For_2018_Rate_Plot/ZeroBias/ZeroBias_RateCalculation/merged_ZeroBias_L1UpgradeTree.root");
	f->cd();
	TTree *tree1=(TTree*)f->Get("L1UpgradeTree");

	TFile *f1=TFile::Open("/eos/cms/store/group/dpg_trigger/comm_trigger/L1Trigger/ckoraka/NtuplesForCMSWeek/For_2018_Rate_Plot/ZeroBias/ZeroBias_RateCalculation/merged_ZeroBias_L1UpgradeEmuTree.root");
	f1->cd();
	TTree *tree2=(TTree*)f1->Get("L1UpgradeTree");


	TBranch *number_EGsBranch=0;
	TBranch *EG_ETBranch=0;
	TBranch *EG_ETABranch=0;
	TBranch *EG_BXBranch=0;
	TBranch *EG_ISOBranch=0;

	TBranch *number_emu_EGsBranch=0;
	TBranch *emu_EG_ETBranch=0;
	TBranch *emu_EG_ETABranch=0;

	TBranch *emu_EG_ISOBranch=0;


	Int_t number_EGs=0;
	std::vector<float> EG_ET;
	std::vector<float> EG_ETA;
	std::vector<short> egBX;
	std::vector<short> egISO;

	Int_t number_emu_EGs=0;
	std::vector<float> emu_EG_ET;
	std::vector<float> emu_EG_ETA;
	std::vector<short> emu_egISO;


	tree1->SetMakeClass(1);
	tree1->SetBranchAddress("nEGs",&number_EGs,&number_EGsBranch);
	tree1->SetBranchAddress("egEt",&EG_ET,&EG_ETBranch);
	tree1->SetBranchAddress("egEta",&EG_ETA,&EG_ETABranch);
	tree1->SetBranchAddress("egBx",&egBX,&EG_BXBranch);
	tree1->SetBranchAddress("egIso",&egISO,&EG_ISOBranch);

	tree2->SetMakeClass(1);
	tree2->SetBranchAddress("nEGs",&number_emu_EGs,&number_emu_EGsBranch);
	tree2->SetBranchAddress("egEt",&emu_EG_ET,&emu_EG_ETBranch);
	tree2->SetBranchAddress("egEta",&emu_EG_ETA,&emu_EG_ETABranch);
	tree2->SetBranchAddress("egIso",&emu_egISO,&emu_EG_ISOBranch);    
      
	Double_t total_entries=tree1->GetEntries();
 	std::cout<<"The total entries are: "<< total_entries << endl;
   
	Double_t total_emu_entries=tree2->GetEntries();
	std::cout<<"The total emulated entries are: "<< total_emu_entries << endl;

	// Histograms
	// Single EG
	TH1D *rate=new TH1D(" "," ",100,0.,100.);
	TH1D *rate_tight_Iso=new TH1D("rate_tight_Iso","rate_tight_Iso",100,0.,100.);
	//Double EG
	TH1D *rate_double =new TH1D("rate_double_Iso","rate_double_Iso",100,0.,100.);
	TH1D *rate_double_loose_Iso =new TH1D("rate_double_loose_Iso","rate_double_loose_Iso",100,0.,100.);

	TH2D *rate_inclusive_vs_eta =new TH2D("rate_inclusive","rate_inclusive",6,0.,2.4,70,0.,70.);
	rate_inclusive_vs_eta->GetZaxis()->SetTitle("Rate [kHz]");
	rate_inclusive_vs_eta->GetXaxis()->SetTitle("|#eta|");
	rate_inclusive_vs_eta->GetYaxis()->SetTitle("E_{T}^{L1} Threshold [GeV]");

        TH1D *rate_double_la =new TH1D("la","la",6,0.,2.4);
        rate_double_la->GetYaxis()->SetTitle("Rate [kHz]");
        rate_double_la->GetXaxis()->SetTitle("|#eta|");


	rate->GetYaxis()->SetTitle("Rate [kHz]");

	rate->GetXaxis()->SetTitle("E_{T}^{L1} Threshold [GeV]");
	rate->GetXaxis()->SetTitleOffset(1.3);

	rate_double->GetXaxis()->SetTitle("E_{T}^{L1} Threshold [GeV]");
	rate_double->GetXaxis()->SetTitleOffset(1.3);

	rate_double_loose_Iso->GetXaxis()->SetTitle("E_{T}^{L1} Threshold [GeV]");
	rate_double_loose_Iso->GetXaxis()->SetTitleOffset(1.3);

	rate_tight_Iso->GetXaxis()->SetTitle("E_{T}^{L1} Threshold [GeV]");
	rate_tight_Iso->GetXaxis()->SetTitleOffset(1.3);

	rate->SetLineColor(kBlack);
	rate_double->SetLineColor(kBlue-2);
	rate_double_loose_Iso->SetLineColor(46);
	rate_tight_Iso->SetLineColor(kGreen-1);

	rate->SetMarkerColor(kBlack);
	rate_double->SetMarkerColor(kBlue-2);
	rate_double_loose_Iso->SetMarkerColor(46);
	rate_tight_Iso->SetMarkerColor(kGreen-1);


	rate->SetLineWidth(2);
	rate_double->SetLineWidth(2);
	rate_double_loose_Iso->SetLineWidth(2);
	rate_tight_Iso->SetLineWidth(2);

     	/////////////////////////
	TH1D *rate_double1 =new TH1D("rate_double_Iso1","rate_double_Iso1",100,0.,100.);
	TH1D *rate_double2 =new TH1D("rate_double_Iso2","rate_double_Iso2",100,0.,100.);
	TH1D *rate_double3 =new TH1D("rate_double_Iso3","rate_double_Iso3",100,0.,100.);

	//////////////////////////

	int Et_Threshold=0; 
	int Bunches = 2544;   
	double EGs_per_eta[30],fraction_per_eta[30],rate_per_eta[30],passed_EGs_per_eta[30];  

	for(Et_Threshold=0;Et_Threshold<=70;Et_Threshold++)
	{    
		//	Unpacked
		Double_t fraction;
		Double_t passed_entries=0;
		Double_t Rate;

		Double_t fraction_double;
		Double_t passed_double_entries=0;
		Double_t Rate_double;

		Double_t fraction_loose_Iso;
		Double_t passed_entries_loose_Iso=0;
		Double_t Rate_loose_Iso=0;

		Double_t fraction_tight_Iso;
		Double_t passed_entries_tight_Iso=0;
		Double_t Rate_tight_Iso=0;

		for(int k=0;k<12;++k)
		{
			EGs_per_eta[k]=0;
			fraction_per_eta[k]=0;
			rate_per_eta[k]=0;
		}
				
		for(Long64_t i=0;i<total_entries/1;i++)
		{
		
			int passed_EGs=0;
			int passed_double_EGs=0;
			int passed_loose_EGs=0;         
			int passed_tight_EGs=0;
			int flag_double_trigger=0;
			int flag_double_trigger_loose=0;

			for(int k=0;k<12;++k)  
				passed_EGs_per_eta[k]=0;
			// Unpacked
			number_EGsBranch->GetEntry(i);
			EG_ETBranch->GetEntry(i);
			EG_ETABranch->GetEntry(i);
			EG_BXBranch->GetEntry(i);
			EG_ISOBranch->GetEntry(i);

			for (Long64_t j=0;j<number_EGs;j++)
			{
				if (EG_ET[j]>=Et_Threshold && egBX[j]==0)
				{
					passed_EGs+=1;				
			
					float ind = (fabs(EG_ETA[j]*10.));
					int index = ind;
					if (index<12) 
						passed_EGs_per_eta[index]+=1; //For 2D eg threshold vs eta rate
					if(egISO[j]==1 || egISO[j]==3)
						passed_tight_EGs+=1;
				}
				if (EG_ET[j]>=Et_Threshold && egBX[j]==0)
				{
					if(EG_ET[j]>=Et_Threshold+10) 
						flag_double_trigger=1;
					passed_double_EGs+=1;
					if(egISO[j]==2 || egISO[j]==3)
					{
						if(EG_ET[j]>=Et_Threshold+10 ) 
							flag_double_trigger_loose=1;
						passed_loose_EGs+=1;
					}       
				}

			}

			for(int k=0;k<12;++k){
				if(passed_EGs_per_eta[k]>=1)  
					EGs_per_eta[k]+=1;
			}

			if (passed_EGs>=1)
				passed_entries+=1;
			
			if (passed_double_EGs>=2 && flag_double_trigger==1)
				passed_double_entries+=1;

			//if (passed_loose_EGs>=2 && flag_double_trigger_loose==1){ //Both legs are loose isolated
			if (passed_double_EGs>=2 && flag_double_trigger_loose==1){  // Only leading leg is loose isolated
				passed_entries_loose_Iso+=1;
			}

			if (passed_tight_EGs>=1)
				passed_entries_tight_Iso+=1;
		}
		// Fractions 
		fraction=passed_entries/total_entries;
		fraction_double=passed_double_entries/total_entries;
		fraction_loose_Iso=passed_entries_loose_Iso/total_entries;
		fraction_tight_Iso=passed_entries_tight_Iso/total_entries;
		for(int k=0;k<12;++k) 
			fraction_per_eta[k]= EGs_per_eta[k]/total_entries;

		// Rates 
		double Scale = 11.245*Bunches;
		Rate=Scale*fraction;
		Rate_double=Scale*fraction_double;
		Rate_loose_Iso=Scale*fraction_loose_Iso;
		Rate_tight_Iso=Scale*fraction_tight_Iso;
		for(int k=0;k<12;++k) 
			rate_per_eta[k] = Scale*fraction_per_eta[k];
		rate->Fill(Et_Threshold,Rate);
		rate_double->Fill(Et_Threshold,Rate_double);
		rate_double_loose_Iso->Fill(Et_Threshold,Rate_loose_Iso);
		rate_tight_Iso->Fill(Et_Threshold,Rate_tight_Iso);
		for(int k=0;k<12;++k) 
			rate_inclusive_vs_eta->Fill(k*0.2,Et_Threshold,rate_per_eta[k]);
		std::cout<<"End of Et Threshold: " <<Et_Threshold<<"/100"<<std::endl;
		if(Et_Threshold==0)
		{ 
			for(int k=0;k<12;++k) 
				rate_double_la->Fill(k*0.4,rate_per_eta[k]);
		}
	}

	rate->Sumw2(); 
	rate_tight_Iso->Sumw2();
	rate_double->Sumw2();
	rate_double_loose_Iso->Sumw2();
	rate_double_la->Sumw2();

	TCanvas *c2= new TCanvas("c2","c2");
	c2->cd();
	c2->SetGrid();
	//c2->SetLogy();
	rate_double_la->SetLineColor(kBlue);
	rate_double_la->SetMarkerColor(kBlue);
	rate_double_la->Draw();
	c2->SaveAs("rate_vs_eta.png");

}


