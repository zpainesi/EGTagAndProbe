#include "TH1D.h"
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
#include <TColor.h>
#include <TF1.h>
#include <TH1F.h>
#include <TFile.h>
#include "TH1.h"
#include "TAxis.h"
#include "RooCategory.h"
#include "RooEfficiency.h"
#include "RooDataSet.h"
#include "RooBinning.h"
#include "RooChi2Var.h"
#include "RooHist.h"
#include "RooRealVar.h"
#include "RooDataSet.h"
#include "TCanvas.h"
#include "TAxis.h"
#include "TLatex.h"
#include "RooPlot.h"
#include "/afs/cern.ch/work/c/ckoraka/codes/setTDRStyle_teliko.C"

using namespace std;
using namespace RooFit;
TCanvas* CreateCanvas(TString CanvasName = "myPlot", bool LogY = false, bool Grid = true)
{
  TCanvas* c = new TCanvas(CanvasName.Data(),CanvasName.Data(),800,800);
  c->SetLeftMargin(0.11);
  if(Grid)
    {
      c->SetGrid();
    }
  if(LogY)
    {
      c->SetLogy();
    }
  return c;
}

void DrawPrelimLabel(TCanvas* c)
{
  c->cd();
        
  TLatex tex;
  tex.SetTextSize(0.03);
  tex.DrawLatexNDC(0.11,0.91,"#scale[1.5]{CMS Preliminary}");
  tex.Draw("same");
        
  return;
}
        
void DrawLumiLabel(TCanvas* c, TString Lumi = "35.9")
{
  c->cd();
        
  TLatex tex;
  tex.SetTextSize(0.035);
  TString toDisplay = Lumi + " (13 TeV)";
  tex.DrawLatexNDC(0.55,0.91,toDisplay.Data());
  tex.Draw("same");
        
  return;
}
     
void SaveCanvas(TCanvas* c, TString PlotName = "myPlotName")   
{
  c->cd();
  c->SaveAs(PlotName + ".png");
  c->SaveAs(PlotName + ".pdf");
  c->SaveAs(PlotName + ".root");
        
  return;
}

void compare_ET()
{

        gStyle->SetOptStat(000000);

        TString PlotName = "L1EG_nvtx_comparison_Data_vs_MC_UL2016_barrel_pre";
        bool LogY = false;
        bool DisplayGrid = true;
        TString Lumi = "UL 2016 - RunB";
        //TString Lumi = "UL 2016 - Simulation";

	 //gROOT->LoadMacro("/afs/cern.ch/work/c/ckoraka/codes/setTDRStyle_teliko.C");
	 //setTDRStyle_teliko();


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
	int     Nvtx;
        std::vector<float> *allEl_Pt;
        std::vector<float> *allEl_Eta;
        std::vector<float> *allEl_Phi;
        std::vector<float> *allEl_SclEt;
        std::vector<float> *allEl_Charge;

        std::vector<float> *allL1tPt;
        std::vector<float> *allL1tEta;
        std::vector<float> *allL1tIso;
        std::vector<float> *allL1tPhi;

        TBranch         *b_allEl_Pt;
        TBranch         *b_allEl_Eta;
        TBranch         *b_allEl_Phi;
        TBranch         *b_allEl_SclEt;
        TBranch         *b_allEl_Charge;
        TBranch         *b_allL1tPt;
        TBranch         *b_allL1tEta;
        TBranch         *b_allL1tPhi;
        TBranch         *b_allL1tIso;

	/////////////////////////////////////////////////////////////////////////////////////////////////////////	


	const Int_t PtBINS = 14; 


        TH1D *et_barrel = new TH1D(" "," ",60,0.,60.);
        TH1D *et_endcap = new TH1D("  ","  ",60,0.,60.);

 
	et_barrel->SetLineColor(kRed);
        et_barrel->SetMarkerColor(kRed);
        et_barrel->SetMarkerSize(3);
        et_barrel->SetMarkerStyle(1);

	et_endcap->SetLineColor(kGreen+2);
        et_endcap->SetMarkerColor(kGreen+2);
        et_endcap->SetMarkerSize(3);   
        et_endcap->SetMarkerStyle(1);


        et_barrel->GetXaxis()->SetTitle("Number of vertices");
        et_endcap->GetXaxis()->SetTitle("Number of vertices");

	//et_barrel->GetXaxis()->SetTitle("E_{T}^{e#gamma, L1}");
	//et_endcap->GetXaxis()->SetTitle("E_{T}^{e#gamma, L1}");
	et_barrel->GetYaxis()->SetTitle("a.u.");
	et_endcap->GetYaxis()->SetTitle("a.u.");


	/////////////////////////////////////////////////////////////////////////////////////////////////////

       // TFile *f1=new TFile("/afs/cern.ch/user/c/ckoraka/L1EG_for_UL2016/CMSSW_10_6_8/src/EGTagAndProbe/EGTagAndProbe/test/ZEE_CMSSW_10_6_8-PU25ns_106X_mcRun2_asymptotic_preVFP_v3_UL16_CP5.root","READ");
	TFile *f1=new TFile("/afs/cern.ch/user/c/ckoraka/L1EG_for_UL2016/CMSSW_10_6_8/src/EGTagAndProbe/EGTagAndProbe/test/DoubleEG_TagAndProbe_Run2016B-ForValUL2016_HIPM-v1.root","READ");
	//TFile *f1=new TFile("/afs/cern.ch/user/c/ckoraka/L1EG_for_UL2016/CMSSW_10_6_8/src/EGTagAndProbe/EGTagAndProbe/test/DoubleEG_TagAndProbe_DoubleEG_Run2016G-ForValUL2016-v1.root","READ");
	TTree *t1=(TTree*)(f1->Get("Ntuplizer/TagAndProbe"));

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
        t1->SetBranchAddress("l1tPt",&l1tPt);
        t1->SetBranchAddress("l1tEta",&l1tEta);
        t1->SetBranchAddress("l1tPhi",&l1tPhi);
        t1->SetBranchAddress("isProbeLoose",&isProbeLoose);
        t1->SetBranchAddress("Nvtx",&Nvtx);


        int nentries = t1->GetEntries();

	int bar=0;
	int end=0;
	int incl=0;

        for (int jentry=0; jentry<nentries; jentry++){   
                t1->GetEntry(jentry);
		if(!isProbeLoose) continue;
                        if(fabs(eleProbeEta)<1.305) {
                        //if(fabs(eleProbeEta)>1.479 && fabs(eleProbeEta)<2.5){  
                              	et_barrel->Fill(Nvtx,1);
   	                     	//et_barrel->Fill(l1tPt,1);
	              		//et_barrel->Fill(eleProbeSclEt,1); 
			}
	}

        TFile *f2=new TFile("/afs/cern.ch/user/c/ckoraka/L1EG_for_UL2016/CMSSW_10_6_8/src/EGTagAndProbe/EGTagAndProbe/test/ZEE_CMSSW_10_6_8-PU25ns_106X_mcRun2_asymptotic_preVFP_v3_UL16_CP5.root","READ");
	//TFile *f2=new TFile("/afs/cern.ch/user/c/ckoraka/L1EG_for_UL2016/CMSSW_10_6_8/src/EGTagAndProbe/EGTagAndProbe/test/ZEE_CMSSW_10_6_8-PU25ns_106X_mcRun2_asymptotic_v9_UL16_CP5_postVFP-v1.root","READ");
        TTree *t2=(TTree*)(f2->Get("Ntuplizer/TagAndProbe"));
        
        //t2->SetBranchAddress("eleProbePt",&eleProbePt);
        t2->SetBranchAddress("eleProbeEta",&eleProbeEta);
        t2->SetBranchAddress("eleProbePhi",&eleProbePhi);
        t2->SetBranchAddress("eleProbeSclEt",&eleProbeSclEt);
        //t2->SetBranchAddress("eleTagPt",&eleTagPt);
        t2->SetBranchAddress("eleTagEta",&eleTagEta);
        t2->SetBranchAddress("eleTagPhi",&eleTagPhi);
        //t2->SetBranchAddress("hltPt",&hltPt);
        //t2->SetBranchAddress("hltEta",&hltEta);
        //t2->SetBranchAddress("hltPhi",&hltPhi);
        t2->SetBranchAddress("l1tPt",&l1tPt);
        t2->SetBranchAddress("l1tEta",&l1tEta);
        t2->SetBranchAddress("l1tPhi",&l1tPhi);
        t2->SetBranchAddress("isProbeLoose",&isProbeLoose);
        t2->SetBranchAddress("Nvtx",&Nvtx);

        
        nentries = t2->GetEntries();
        
        for (int jentry=0; jentry<nentries; jentry++){
                t2->GetEntry(jentry);
		if(!isProbeLoose) continue;
   			if(fabs(eleProbeEta)<1.305) {
                        //if(fabs(eleProbeEta)>1.479 && fabs(eleProbeEta)<2.5){        
                                //et_endcap->Fill(l1tPt,1);
	                        et_endcap->Fill(Nvtx,1);
        			//et_endcap->Fill(eleProbeSclEt,1);
			}
	}




	// Plotting		////////////////////////////

        TLegend *l = new TLegend(.55, .5, .89, .6);
        l->SetTextSize(0.021);
        l->AddEntry(et_barrel,"Data - RunB UL16","lp");
        //l->AddEntry(et_barrel,"Simulation - UL16 preVFP ","lp");
        //l->AddEntry(et_barrel,"Data - RunG UL16","lp");
        l->AddEntry(et_endcap,"Simulation - UL16 preVFP ","lp");
        //l->AddEntry(et_endcap,"Simulation - UL16 postVFP ","lp");
	l->SetBorderSize(1);

        TPaveText *l1eg = new TPaveText(.6,.65,.85,.80,"NDC");
        l1eg->AddText("L1 EG - Barrel");
        //l1eg->AddText("L1 EG - Endcaps");
        //l1eg->AddText("Barrel");
        //l1eg->AddText("|#eta^{offl}|<2.4");
        l1eg->SetTextSize(0.04);
        l1eg->SetTextAlign(13);
        l1eg->SetFillColor(0);
        l1eg->SetFillStyle(0);
        l1eg->SetBorderSize(0);
        l1eg->SetTextFont(12);
        l1eg->SetTextColor(kBlack);

        TCanvas* myCanvas = CreateCanvas(PlotName, LogY, DisplayGrid);
	//TCanvas *c1 = new TCanvas("c1","c1",800,800);
	et_barrel->DrawNormalized("ep");
	et_endcap->DrawNormalized("ep same");
	l->Draw("same");
        l1eg->Draw("same");
        DrawPrelimLabel(myCanvas);
        DrawLumiLabel(myCanvas, Lumi);
        SaveCanvas(myCanvas, PlotName);
 
        //TFile *file =new TFile("NVTX_2018MC_vsUL_comparison_barrel.root","RECREATE");
        //file->cd();
        //et_barrel->SetName("NVTX_distribution_2016");
        //et_barrel->Write();   
        //et_endcap->SetName("NVTX_distribution_MC_UL_2018");
        //et_endcap->Write();


}

