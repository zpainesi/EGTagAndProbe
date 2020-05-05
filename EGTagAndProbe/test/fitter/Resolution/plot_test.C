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
#include "RooPlot.h"

#include <TCanvas.h>
#include <TF1.h>
#include <TLatex.h>
#include <TLegend.h>
#include <TPaveText.h>
#include <TMarker.h>
#include <TLine.h>
#include <TAxis.h>
#include <TROOT.h>
#include <TStyle.h>
#include <TH1.h>
#include <TGraph.h>
#include <TMultiGraph.h>
#include <TGraphAsymmErrors.h>


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
  tex.DrawLatexNDC(0.11,0.91,"#scale[1.5]{CMS}");
  tex.Draw("same");

  return;
}

void DrawLumiLabel(TCanvas* c, TString Lumi = "35.9")
{
  c->cd();

  TLatex tex;
  tex.SetTextSize(0.035);
  TString toDisplay = Lumi + " fb^{-1} (13 TeV)";
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

using namespace std;
using namespace RooFit;

void plot_test()
{


  	gStyle->SetOptStat(000000);

  	TString PlotName = "L1EG_Resolution2018_vs_ET";
  	bool LogY = false;
  	bool DisplayGrid = true;
  	TString Lumi = "58.8";

        TFile *f1 = new TFile("old_scripts/plotOutData_charis_fit.root","READ");
	TFile *f2 = new TFile ("Resolution2018_plots_and_rootfiles/Resolution2018_vs_ET.root","READ");

	//Resolution vs ET

        TH1F *fhEt_Data_barrel = (TH1F*) f1->Get("hEt_Data_barrel");
        //TF1 *myfunc1 = fhEt_Data_barrel->GetFunction("CBFuncAsymm");
        //TF1 *myfunc1 = fhEt_Data_barrel->GetFunction("CBFunc");
        TF1 *myfunc1 = (TF1*) f2->Get("Fit_curve_Barrel");

        TH1F *fhEt_Data_endcap = (TH1F*) f1->Get("hEt_Data_endcap");
        TF1 *myfunc2 = fhEt_Data_endcap->GetFunction("CBFuncAsymm");
        //TF1 *myfunc2 = fhEt_Data_endcap->GetFunction("CBFunc");
        //TF1 *myfunc2 = (TF1*) f2->Get("Fit_curve_EndCap");

	//for(int i=0; i < fhEt_Data_barrel->GetNbinsX(); ++i){
	//	fhEt_Data_barrel->SetBinError(i,0);
	//	fhEt_Data_endcap->SetBinError(i,0);
	//}

	fhEt_Data_barrel->SetLineColor(kBlue);
	fhEt_Data_barrel->SetMarkerColor(kBlue);
	fhEt_Data_barrel->SetMarkerSize(0.8);
	fhEt_Data_barrel->SetMarkerStyle(21);
	myfunc1->SetLineColor(kBlue);
        fhEt_Data_barrel->SetTitle("");


	fhEt_Data_endcap->SetLineColor(kGreen+3);
	fhEt_Data_endcap->SetMarkerColor(kGreen+3);
	fhEt_Data_endcap->SetMarkerSize(0.8);
	fhEt_Data_endcap->SetMarkerStyle(22);
	myfunc2->SetLineColor(kGreen+3);
        fhEt_Data_endcap->SetTitle("");

	TPaveText *l1eg = new TPaveText(.65,.765,.85,.805,"NDC");
        l1eg->AddText("L1 EG");
	l1eg->SetTextSize(0.04);
        l1eg->SetTextAlign(13);
        l1eg->SetFillColor(0);
	l1eg->SetFillStyle(0);
	l1eg->SetBorderSize(0);
        l1eg->SetTextFont(12);
        l1eg->SetTextColor(kBlack);

	
        TLegend *l = new TLegend(.60, .6, .85, .75);
        l->SetTextSize(0.04);
        l->AddEntry(fhEt_Data_barrel,"Barrel","lp");
        l->AddEntry(fhEt_Data_endcap,"Endcap","lp");
	l->SetBorderSize(1);

        fhEt_Data_barrel->GetYaxis()->SetRangeUser(0.,0.14);
        fhEt_Data_endcap->GetYaxis()->SetRangeUser(0.,0.14);
        fhEt_Data_barrel->GetXaxis()->SetRangeUser(0.6,1.4);
        fhEt_Data_endcap->GetXaxis()->SetRangeUser(0.6,1.4);

        //fhEt_Data_barrel->GetYaxis()->SetRangeUser(0.,0.2);
        //fhEt_Data_endcap->GetYaxis()->SetRangeUser(0.,0.2);
        //fhEt_Data_barrel->GetXaxis()->SetRangeUser(-0.2,0.2);
        //fhEt_Data_endcap->GetXaxis()->SetRangeUser(-0.2,0.2);

  	fhEt_Data_barrel->GetXaxis()->SetTitleOffset(1.);
  	fhEt_Data_endcap->GetXaxis()->SetTitleOffset(1.);

  	TCanvas* myCanvas = CreateCanvas(PlotName, LogY, DisplayGrid);
        fhEt_Data_barrel->Draw("EP");
        fhEt_Data_endcap->Draw("EP same");
	l1eg->Draw("same");	
	l->Draw("same");
	DrawPrelimLabel(myCanvas);
  	DrawLumiLabel(myCanvas, Lumi);
  	SaveCanvas(myCanvas, PlotName);

        TFile *file =new TFile("Resolution2018_vs_ET_updated_fit_2.root","RECREATE");
        file->cd();
        
        fhEt_Data_barrel->SetName("ET_Resolution_Barrel");
        fhEt_Data_barrel->Write();
  
        fhEt_Data_endcap->SetName("ET_Resolution_EndCap");
        fhEt_Data_endcap->Write();

        myfunc1->SetName("Fit_curve_Barrel");
        myfunc1->Write();
  
        myfunc2->SetName("Fit_curve_EndCap");
        myfunc2->Write();

        l->SetName("leg1");
        l->Write();
        l1eg->SetName("leg2");
        l1eg->Write();   
        
        myCanvas->Write();


}











