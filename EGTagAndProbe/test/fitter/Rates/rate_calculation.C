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

//using namespace std;
TCanvas* CreateCanvas(TString CanvasName = "myPlot", bool LogY = false, bool Grid = true)
{
	TCanvas* c = new TCanvas(CanvasName.Data(),CanvasName.Data(),800,800);
	c->SetLeftMargin(0.11);
	if(Grid)
		c->SetGrid();
	if(LogY)
		c->SetLogy();
	return c;
}

void DrawPrelimLabel(TCanvas* c)
{
	c->cd();

	TLatex tex;
	tex.SetTextSize(0.03);
	tex.DrawLatexNDC(0.11,0.91,"#scale[1.5]{CMS}");
	tex.Draw("same");

	return;
}

void DrawLumiLabel(TCanvas* c, TString Lumi = "35.9")
{
	c->cd();

	TLatex tex;
	tex.SetTextSize(0.035);
	TString toDisplay = Lumi + " pb^{-1} (13 TeV)";
	tex.DrawLatexNDC(0.66,0.91,toDisplay.Data());
	tex.Draw("same");

	return;
}

void SaveCanvas(TCanvas* c, TString PlotName = "myPlotName")
{
	c->cd();
	c->SaveAs(PlotName + ".pdf");
	c->SaveAs(PlotName + ".root");

	return;
}


void rate_calculation()
{

	TPaveText *paveCMS = new TPaveText(0.15,0.96,0.93,0.99,"NDC");
	paveCMS->AddText("#bf{CMS      154 pb^{-1} (13 TeV)");
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
	TH1D *rate=new TH1D(" "," ",50,0.,50.);
	TH1D *rate_tight_Iso=new TH1D("rate_tight_Iso","rate_tight_Iso",50,0.,50.);
	// Double EG
	TH1D *rate_double =new TH1D("rate_double_Iso","rate_double_Iso",50,0.,50.);
	TH1D *rate_double_loose_Iso =new TH1D("rate_double_loose_Iso","rate_double_loose_Iso",50,0.,50.);

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

	rate->Sumw2(); 
	rate_tight_Iso->Sumw2();
	rate_double->Sumw2();
	rate_double_loose_Iso->Sumw2();

	TH1D *rate_double1 =new TH1D("rate_double_Iso1","rate_double_Iso1",50,0.,50.);
	TH1D *rate_double2 =new TH1D("rate_double_Iso2","rate_double_Iso2",50,0.,50.);
	TH1D *rate_double3 =new TH1D("rate_double_Iso3","rate_double_Iso3",50,0.,50.);


	int Et_Threshold=0; 
	int Bunches = 2544;     

	for(Et_Threshold=0;Et_Threshold<=50;Et_Threshold++)
	{    

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
				
		for(Long64_t i=0;i<total_entries/1;i++)
		{
			int passed_EGs=0;
			int passed_double_EGs=0;
			int passed_loose_EGs=0;         
			int passed_tight_EGs=0;
			int flag_double_trigger=0;
			int flag_double_trigger_loose=0;

			// Unpacked
			number_EGsBranch->GetEntry(i);
			EG_ETBranch->GetEntry(i);
			EG_ETABranch->GetEntry(i);
			EG_BXBranch->GetEntry(i);
			EG_ISOBranch->GetEntry(i);

			for (Long64_t j=0;j<number_EGs;++j)
			{
				if (EG_ET[j]>=Et_Threshold && egBX[j]==0)
				{
					passed_EGs+=1;
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
    
		// Rates 
		double Scale = 11.245*Bunches;
		Rate=Scale*fraction;
		Rate_double=Scale*fraction_double;
		Rate_loose_Iso=Scale*fraction_loose_Iso;
		Rate_tight_Iso=Scale*fraction_tight_Iso;
  
		rate->Fill(Et_Threshold,Rate);
		rate_double->Fill(Et_Threshold,Rate_double);
		rate_double_loose_Iso->Fill(Et_Threshold,Rate_loose_Iso);
		rate_tight_Iso->Fill(Et_Threshold,Rate_tight_Iso);

		std::cout<<"End of Et Threshold: " <<Et_Threshold<<"/50"<<std::endl;
	}

  	gStyle->SetOptStat(000000);

  	TString PlotName = "Rate";
  	bool LogY = true;
 	bool DisplayGrid = true;
  	TString Lumi = "154";
 
	TLegend *leg = new TLegend(0.4,0.65,0.9,0.9);
	leg->SetHeader("L1 e/#gamma Rate ");
	leg->SetTextSize(0.033);
	leg->AddEntry(rate,"Single EG - Inclusive","l");
	leg->AddEntry(rate_tight_Iso,"Single EG - Tight Isolation","l");
	leg->AddEntry(rate_double,"Double EG - Inclusive","l");
	leg->AddEntry(rate_double_loose_Iso,"Double EG - Loose Isolation","l");

	TCanvas* myCanvas = CreateCanvas(PlotName, LogY, DisplayGrid);
	rate->GetYaxis()->SetRangeUser(0.1,40000);
	rate->Draw("HIST");
	rate_tight_Iso->Draw("HIST same");	
	rate_double->Draw("HIST same");	
	rate_double_loose_Iso->Draw("HIST same");	
	DrawPrelimLabel(myCanvas);
	DrawLumiLabel(myCanvas, Lumi);
	leg->Draw("same"); 
	SaveCanvas(myCanvas, PlotName);
      
	/*     	
	TCanvas *c1= new TCanvas("c1","c1",800,800);
	c1->cd();
	c1->SetGrid();
	c1->SetLogy();
	rate->GetYaxis()->SetRangeUser(0.1,40000);
	rate->Draw("HIST");
	rate_tight_Iso->Draw("HIST same");	
	rate_double->Draw("HIST same");	
	rate_double_loose_Iso->Draw("HIST same");	
	leg->Draw("same");
	paveCMS->Draw("same");
	c1->SaveAs("L1EG_rate_2018.png");
	c1->SaveAs("L1EG_rate_2018.pdf");
	*/

	TFile *file =new TFile("L1EG_Rate_2018.root","RECREATE");
	file->cd();

	rate->SetName("SingleEG_rate_Inclusive");
	rate->Write();

	rate_tight_Iso->SetName("SingleEG_rate_TightIso");
	rate_tight_Iso->Write();

	rate_double->SetName("DoubleEG_rate_Inclusive");
	rate_double->Write();

	rate_double_loose_Iso->SetName("DoubleEG_rate_LooseIso");
	rate_double_loose_Iso->Write();

	leg->Write();
	myCanvas->Write();

}


