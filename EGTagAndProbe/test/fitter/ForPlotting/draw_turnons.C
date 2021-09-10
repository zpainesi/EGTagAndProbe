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

TCanvas* CreateCanvas(TString CanvasName = "myPlot",bool LogX = false, bool LogY = false, bool Grid = true)
{
	TCanvas* c = new TCanvas(CanvasName.Data(),CanvasName.Data(),800,800);
  	c->SetLeftMargin(0.11);
  	if(Grid){
      		c->SetGrid();
    	}
  	if(LogY){
      		c->SetLogy();
    	}
  	if(LogX){
      		c->SetLogx();
    	}
 	return c;
}

void DrawPrelimLabel(TCanvas* c)
{
  	c->cd();

  	TLatex tex;
  	tex.SetTextSize(0.03);
  	tex.DrawLatexNDC(0.11,0.91,"#scale[1.5]{CMS} Preliminary");
  	tex.Draw("same");

  	return;
}

void DrawLumiLabel(TCanvas* c, TString Lumi = ""){
	c->cd();

  	TLatex tex;
  	tex.SetTextSize(0.035);
  	//TString toDisplay = Lumi + " fb^{-1} (13 TeV)";
  	TString toDisplay = Lumi + "(13 TeV)";
  	tex.DrawLatexNDC(0.5,0.91,toDisplay.Data());
  	tex.Draw("same");

  	return;
}

void SaveCanvas(TCanvas* c, TString PlotName = ""){
	c->cd();
  	c->SaveAs(PlotName + ".pdf");
  	c->SaveAs(PlotName + ".root");

  	return;
}

using namespace std;
using namespace RooFit;


void draw_turnons(char PlotName[1000],char Lumi[100], char turnoOn_Data[1000],char turnoOn_MC[1000],char turnoOn_hist[1000],int numb)
{

  	gStyle->SetOptStat(000000);
 	bool LogY = false;
 	bool LogX = false;
	bool DisplayGrid = true;


        TFile *f1  = new TFile(turnoOn_Data,"READ");
        f1->cd();
        RooHist* l130_noiso_1 = (RooHist*) f1->Get(turnoOn_hist);
        l130_noiso_1->SetTitle("");
	///////////////////

        TFile *f2  = new TFile(turnoOn_MC,"READ");
        f2->cd();
        RooHist* l130_noiso_2 = (RooHist*) f2->Get(turnoOn_hist);
        l130_noiso_2->SetTitle("");

    
	l130_noiso_1->SetLineColor(kBlue+2);
	l130_noiso_1->SetMarkerColor(kBlue+2);
	l130_noiso_1->SetMarkerStyle(22);
	l130_noiso_1->SetLineStyle(0);
	l130_noiso_2->SetLineColor(kRed+2);	
	l130_noiso_2->SetMarkerColor(kRed+2);	
	l130_noiso_2->SetMarkerStyle(22);
	l130_noiso_2->SetLineStyle(0);


        TLegend *ll = new TLegend(0.45,0.2,0.85,0.45);
        ll->SetTextSize(0.027);
        //ll->AddEntry(l130_noiso_1,"Data","lp");
        //ll->AddEntry(l130_noiso_2,"Simulation","lp");
        ll->AddEntry(l130_noiso_1,"RunB","lp");
        ll->AddEntry(l130_noiso_2,"RunG","lp");
        //ll->AddEntry(l130_noiso_1," GT: preVFP_v3 ","lp");
        //ll->AddEntry(l130_noiso_2," GT: v9 ","lp");
 	//ll->AddEntry(l130_noiso_1,"L1 E_{T} > 34 GeV","lp");
        //ll->AddEntry(l130_noiso_2,"L1 E_{T} > 28 GeV & Tight Iso","lp");
        //ll->AddEntry(l130_noiso_3,"#splitline{L1 E_{T} > 34 GeV OR}{L1 E_{T} > 28 GeV & Tight Iso}","lp");
        //ll->AddEntry(l130_noiso_1,"L1 E_{T} > 32 GeV & Tight Iso","lp");
        //ll->AddEntry(l130_noiso_2,"#splitline{L1 E_{T} > 32 GeV}{& Inclusive Iso}","lp");

        TPaveText *l1eg = new TPaveText(.45,.47,.85,.6,"NDC");
       	//if( numb == 0 || numb == 1) l1eg->AddText("#splitline{L1 E_{T} > 30 GeV}{|#eta^{offl}|<2.5}");
       	//else if( numb == 2 || numb == 3) l1eg->AddText("#splitline{L1 E_{T} > 40 GeV}{|#eta^{offl}|<2.5}");
       	//else l1eg->AddText("#splitline{L1 E_{T} > 32 GeV  - Tight Isolation}{|#eta^{offl}|<2.5}");
        //l1eg->AddText("#splitline{L1 E_{T} > 30 GeV}{|#eta^{offl}|<2.5}");
        //l1eg->AddText("#splitline{L1 EG}{|#eta^{offl}|<2.5}");
        l1eg->AddText("#splitline{L1 E_{T} > 32 GeV}{|#eta^{offl}|< 2.5  &  E_{T}^{offl}> 40 GeV}");
	l1eg->SetTextSize(0.033);
        l1eg->SetFillColor(kWhite);
        l1eg->SetFillStyle(1001);
        l1eg->SetBorderSize(0);
        l1eg->SetTextAlign(21);
        l1eg->SetTextFont(12);
        l1eg->SetTextColor(kBlack);
	

	//l130_noiso_1->GetXaxis()->SetTitle("E_{T}^{offl} [GeV]");
	//l130_noiso_2->GetXaxis()->SetTitle("E_{T}^{offl} [GeV]");
	l130_noiso_1->GetXaxis()->SetTitle("Number of Vertices");
	l130_noiso_2->GetXaxis()->SetTitle("Number of Vertices");
	l130_noiso_1->GetYaxis()->SetTitle("Efficiency");
	l130_noiso_2->GetYaxis()->SetTitle("Efficiency");

 	TAxis *axisx = l130_noiso_1->GetXaxis();
   	axisx->SetLimits(0.,60.); 
 	TAxis *axisx2 = l130_noiso_2->GetXaxis();
   	axisx2->SetLimits(0.,60.); 


 	l130_noiso_1->GetHistogram()->SetMaximum(1.05);             
 	l130_noiso_2->GetHistogram()->SetMaximum(1.05);            

 	l130_noiso_1->GetHistogram()->SetMinimum(0.);             
 	l130_noiso_2->GetHistogram()->SetMinimum(0.);            

  	TCanvas* myCanvas = CreateCanvas(PlotName, LogX, LogY, DisplayGrid);
	l130_noiso_1->Draw("AP");
	l130_noiso_2->Draw("P");

	ll->Draw("same");
	l1eg->Draw("same");
  	DrawPrelimLabel(myCanvas);
  	DrawLumiLabel(myCanvas, Lumi);
  	SaveCanvas(myCanvas, PlotName);

	

}

