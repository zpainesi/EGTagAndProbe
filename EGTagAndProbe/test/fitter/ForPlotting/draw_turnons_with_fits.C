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

double ApproxErf(Double_t arg)
{
  double erflim = 5.0;
  if( arg > erflim )
    return 1.0;
  if( arg < -erflim )
    return -1.0;

  return TMath::Erf(arg);
}


double turnon_func3(Double_t *x, Double_t *par){

	double m 		= x[0];
	double norm 	= par[0];
	double m0 		= par[1];
	double alpha 	= par[2];
	double sigma 	= par[3];
	double n 		= par[4];
	double mturn 	= par[5];
	double p 		= par[6];
	double width 	= par[7];


	double sqrtPiOver2 = 1.2533141373; 
	double sqrt2 = 1.4142135624;

	double sig = fabs(sigma);

	double t = (m - m0)/sig ;

	if (alpha < 0)
		t = -t;

	double absAlpha = fabs(alpha / sig);
	double a = TMath::Power(n/absAlpha,n)*exp(-0.5*absAlpha*absAlpha);
	double b = absAlpha - n/absAlpha;


	double aireGauche = (1 + ApproxErf( absAlpha / sqrt2 )) * sqrtPiOver2 ;
	double aireDroite = ( a * 1/TMath::Power(absAlpha - b,n-1)) / (n - 1);
	double aire = aireGauche + aireDroite;	
 

	//Arctan part
	Double_t Linear = 0.;
	if(m<mturn) Linear = p;
	if(m>=mturn) Linear = pow(ApproxErf((m-mturn)/5.),2)*2.*(1.-p)/3.14159*TMath::ATan(3.14159/80.*width*(m-mturn))+p;

	if ( t <= absAlpha ){
		//return norm * (1 + ApproxErf( t / sqrt2 )) * sqrtPiOver2 / aire ;
		return norm * (1 + ApproxErf( t / sqrt2 )) * sqrtPiOver2 / aire * Linear ;
	}
	else{
		//return norm * (aireGauche +  a * (1/TMath::Power(t-b,n-1) - 1/TMath::Power(absAlpha - b,n-1)) / (1 - n)) / aire ;
		return norm * (aireGauche +  a * (1/TMath::Power(t-b,n-1) - 1/TMath::Power(absAlpha - b,n-1)) / (1 - n)) / aire * Linear ;
	}

}

double Fit_tail_low(Double_t *x, Double_t *par, double lim){
  //double lim = 28.; //l130:28 //l140:38 //l134:32 //l1tightOR:26
	if( x[0] > lim )
		return (par[0]+par[1]*x[0])*exp(lim-x[0]);
	else
		return par[0]+par[1]*x[0];	
}

double Fit_tail_high(Double_t *x, Double_t *par){
  	double lim = 50.; //l130:28 //l140:38 //l134:32
  	if( x[0] < lim )
		return 0.;	
	else
		return par[0]+par[1]*x[0]+par[2]/x[0];
}

double fit4_1(Double_t *x, Double_t *par) {

      return  turnon_func3(x,&par[2]) + Fit_tail_low(x,par,34);

}

double fit4_2(Double_t *x, Double_t *par) {
 
      return  turnon_func3(x,&par[2]) + Fit_tail_low(x,par,30);

}

double fit5(Double_t *x, Double_t *par) {

      return  turnon_func3(x,&par[5]) + Fit_tail_low(x,&par[0],26.5) + Fit_tail_high(x,&par[2]);

}

TCanvas* CreateCanvas(TString CanvasName = "myPlot",bool LogX = false, bool LogY = false, bool Grid = true)
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
  if(LogX)
    {
      c->SetLogx();
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

void SaveCanvas(TCanvas* c, TString PlotName = "L1EG_efficiency2018_OR_of_typical_algos_plus_fit")
//void SaveCanvas(TCanvas* c, TString PlotName = "L1EG_efficiency2018_No_Iso_plus_fit")
{
  c->cd();
  c->SaveAs(PlotName + ".pdf");
  c->SaveAs(PlotName + ".root");

  return;
}

using namespace std;
using namespace RooFit;


void draw_turnons_with_fits()
{

  gStyle->SetOptStat(000000);
      
 	//TString PlotName = "L1EG_Efficiency2018_vs_Nvtx";
    //TString PlotName = "L1EG_Efficiency2018";
 	TString PlotName = "L1EG_Efficiency2018_OR_of_typical_algos";
 	bool LogY = false;
 	bool LogX = false;
	bool DisplayGrid = true;
	TString Lumi = "58.8";

	////////////

	//TFile *f1  = new TFile("turnon_rootfiles/eff_turnons_all_2018/Efficiency_turnons_Data2018_EG40.root","READ");
	TFile *f1  = new TFile("turnon_rootfiles/Efficiency_turnons_Data2018_EG34.root","READ");
	//TFile *f1  = new TFile("turnon_rootfiles/Efficiency_turnons_Data2018_SingleTightIsoEG32_vs_nvtx_offlEt40.root","READ");

	f1->cd();
	//RooHist* l130_noiso_1 = (RooHist*) f1->Get("histo_SingleEG40");
	RooHist* l130_noiso_1 = (RooHist*) f1->Get("histo_SingleEG34");
	//RooHist* l130_noiso_1 = (RooHist*) f1->Get("histo_SingleTightIsoEG32_vs_nvtx_offlEt40");
	l130_noiso_1->SetTitle("");
	///////////////////

	//TFile *f2  = new TFile("turnon_rootfiles/eff_turnons_all_2018/Efficiency_turnons_Data2018_EG30.root","READ");
	TFile *f2  = new TFile("turnon_rootfiles/Efficiency_turnons_Data2018_TightEG28.root","READ");
	//TFile *f2  = new TFile("turnon_rootfiles/Efficiency_turnons_Data2018_SingleNoIsoEG32_vs_nvtx_offlEt40.root","READ");
	f2->cd();
	//RooHist* l130_noiso_2 = (RooHist*) f2->Get("histo_SingleEG30");
	RooHist* l130_noiso_2 = (RooHist*) f2->Get("histo_TightEG28");
	//RooHist* l130_noiso_2 = (RooHist*) f2->Get("histo_SingleNoIsoEG32_vs_nvtx_offlEt40");
	l130_noiso_2->SetTitle("");

	////////////////
	TFile *f3  = new TFile("turnon_rootfiles/Efficiency_turnons_Data2018_TightEG28_OR_EG34.root","READ");
	f3->cd();

	RooHist* l130_noiso_3 = (RooHist*) f3->Get("histo_TightEG28_OR_EG34");
	l130_noiso_3->SetTitle("");
       
    
	l130_noiso_1->SetLineColor(kBlue+2);
	l130_noiso_1->SetMarkerColor(kBlue+2);
	l130_noiso_1->SetMarkerStyle(22);
	l130_noiso_1->SetLineStyle(0);
	l130_noiso_2->SetLineColor(kRed+2);	
	l130_noiso_2->SetMarkerColor(kRed+2);	
	l130_noiso_2->SetMarkerStyle(22);
	l130_noiso_2->SetLineStyle(0);
	l130_noiso_3->SetLineColor(kGreen+2);	
	l130_noiso_3->SetMarkerColor(kGreen+2);	
	l130_noiso_3->SetMarkerStyle(22);
	l130_noiso_3->SetLineStyle(0);


	TLegend *ll = new TLegend(0.45,0.2,0.85,0.45);
	ll->SetTextSize(0.027);
	//ll->AddEntry(l130_noiso_1,"L1 E_{T} > 40 GeV","lp");
	//ll->AddEntry(l130_noiso_2,"L1 E_{T} > 30 GeV","lp");
 	ll->AddEntry(l130_noiso_1,"L1 E_{T} > 34 GeV","lp");
	ll->AddEntry(l130_noiso_2,"L1 E_{T} > 28 GeV & Tight Iso","lp");
	ll->AddEntry(l130_noiso_3,"#splitline{L1 E_{T} > 34 GeV OR}{L1 E_{T} > 28 GeV & Tight Iso}","lp");
	//ll->AddEntry(l130_noiso_1,"L1 E_{T} > 34 GeV & Tight Iso","lp");
	//ll->AddEntry(l130_noiso_2,"#splitline{L1 E_{T} > 34 GeV}{& Inclusive Iso}","lp");

	TPaveText *l1eg = new TPaveText(.45,.47,.85,.6,"NDC");
	l1eg->AddText("#splitline{L1 EG}{|#eta^{offl}|<2.5}");
	//l1eg->AddText("#splitline{L1 EG}{|#eta^{offl}|< 2.5  &  E_{T}^{offl}> 40 GeV}");
	l1eg->SetTextSize(0.033);
	//l1eg->SetFillColor(kWhite);
	//l1eg->SetFillStyle(1001);
	l1eg->SetBorderSize(0);
 	l1eg->SetTextAlign(21);
	l1eg->SetTextFont(12);
	l1eg->SetTextColor(kBlack);
	

	/////////////////////////////////////////////////////

	l130_noiso_1->GetXaxis()->SetTitle("E_{T}^{offl} [GeV]");
	l130_noiso_2->GetXaxis()->SetTitle("E_{T}^{offl} [GeV]");
	l130_noiso_1->GetYaxis()->SetTitle("Efficiency");
	l130_noiso_2->GetYaxis()->SetTitle("Efficiency");
	l130_noiso_3->GetXaxis()->SetTitle("E_{T}^{offl} [GeV]");
	l130_noiso_3->GetYaxis()->SetTitle("Efficiency");

 	TAxis *axisx = l130_noiso_1->GetXaxis();
   	axisx->SetLimits(0.,100.); 

 	TAxis *axisx2 = l130_noiso_2->GetXaxis();
   	axisx2->SetLimits(0.,100.); 

 	TAxis *axisx3 = l130_noiso_3->GetXaxis();
   	axisx3->SetLimits(0.,100.); 

 	l130_noiso_1->GetHistogram()->SetMaximum(1.05);             
 	l130_noiso_2->GetHistogram()->SetMaximum(1.05);            
 	l130_noiso_3->GetHistogram()->SetMaximum(1.05);              

 	l130_noiso_1->GetHistogram()->SetMinimum(0.);             
 	l130_noiso_2->GetHistogram()->SetMinimum(0.);            
 	l130_noiso_3->GetHistogram()->SetMinimum(0.); 

	//l130 Parameters
/*
	TF1 *fit1 = new TF1("fit1",fit4_1,5.,1000.,10);//lim is 28
        fit1->SetParameter(0,-0.00258787);
        fit1->SetParameter(1,0.00121335);
        fit1->SetParameter(2,1.00);
        fit1->SetParameter(3,30.1426);
        fit1->SetParameter(4,3.34536);
        fit1->SetParameter(5,1.88957);
        fit1->SetParameter(6,2.47891);	
        fit1->SetParameter(7,31.6194);	
        fit1->SetParameter(8,0.938761);	
        fit1->SetParameter(9,0.975232);	
	fit1->SetNpx(500);


	//l140 Parameters
		
	TF1 *fit2 = new TF1("fit2",fit4_2,5.,1000.,10); //lim is 38
        fit2->SetParameter(0,-0.000501409);
        fit2->SetParameter(1,0.000285493);
        fit2->SetParameter(2,1.0);
        fit2->SetParameter(3,40.1963);
        fit2->SetParameter(4,8.43073);
        fit2->SetParameter(5,1.61742);
        fit2->SetParameter(6,1.85126);
        fit2->SetParameter(7,41.9441);
        fit2->SetParameter(8,0.916551);
        fit2->SetParameter(9,1.43553);
	fit2->SetNpx(500);

*/

	//l134 Parameters	
	TF1 *fit1 = new TF1("fit1",fit4_1,5.,1000.,10); // lim is 32
        fit1->SetParameter(0,-7.31365e-04);
        fit1->SetParameter(1,7.37123e-04);
        fit1->SetParameter(2,1.0);
        fit1->SetParameter(3,3.41973e+01);
        fit1->SetParameter(4,2.97200e+00);
        fit1->SetParameter(5,2.02633e+00);
        fit1->SetParameter(6,9.99992e+01);
        fit1->SetParameter(7,3.74648e+01);
        fit1->SetParameter(8,9.34630e-01);
        fit1->SetParameter(9,1.05224e+00);
        fit1->SetNpx(5000);

	//l128Tight Parameters
	TF1 *fit2 = new TF1("fit2",fit4_2,0.,100.,10);
        fit2->SetParameter(0,-3.50352e-03);
        fit2->SetParameter(1,1.36452e-03);
        fit2->SetParameter(2,1.0);
        fit2->SetParameter(3,2.91329e+01);
        fit2->SetParameter(4,3.24544e+00);
        fit2->SetParameter(5,2.46883e+00);
        fit2->SetParameter(6,2.29389e+00);
        fit2->SetParameter(7,8.14420e+00);
        fit2->SetParameter(8,4.98903e-01);
        fit2->SetParameter(9,1.08045e+01);
        fit2->SetNpx(5000);

        //l128Tight OR l134 Parameters
        TF1 *fit3 = new TF1("fit3",fit4_2,0.,100.,10);
        fit3->SetParameter(0,-0.00346143);
        fit3->SetParameter(1,0.00138656);
        fit3->SetParameter(2,1.0);
        fit3->SetParameter(3,26.1259);
        fit3->SetParameter(4,1.57906);
        fit3->SetParameter(5,1.20537);   
        fit3->SetParameter(6,1.838093);  
        fit3->SetParameter(7,26.2714); 
        fit3->SetParameter(8,0.328318);
        fit3->SetParameter(9,35.7827);
        fit3->SetNpx(5000);

        fit1->SetLineColor(kBlue+2);
        fit1->SetLineWidth(2);
        fit2->SetLineColor(kRed+2);
        fit2->SetLineWidth(2);
        fit3->SetLineColor(kGreen+2);
        fit3->SetLineWidth(2);



  	TCanvas* myCanvas = CreateCanvas(PlotName, LogX, LogY, DisplayGrid);
	l130_noiso_1->Draw("AP");
	l130_noiso_2->Draw("P");
	l130_noiso_3->Draw("P");
	fit1->Draw("same");
	fit2->Draw("same");
    fit3->Draw("same");

	//ll->Draw("same");
	//l1eg->Draw("same");
  	DrawPrelimLabel(myCanvas);
  	DrawLumiLabel(myCanvas, Lumi);
  	SaveCanvas(myCanvas, PlotName);


	//TFile *file =new TFile("Efficiency2018_L1EG_NoIso.root","RECREATE");
	TFile *file =new TFile("Efficiency2018_L1EG_OR_of_typical_algos.root","RECREATE");
	//TFile *file =new TFile("Efficiency2018_L1EG_vs_Nvtx.root","RECREATE");

	file->cd();

	myCanvas->Write();

	//l130_noiso_1->SetName("L1SingleEG40_No_Iso");
	//l130_noiso_1->SetName("L1SingleEG32_Tight_Iso");
	l130_noiso_1->SetName("L1SingleEG34_No_Iso"); 
	l130_noiso_1->Write();
	//fit2->SetName("Fit_L1SingleEG40_No_Iso");
	fit1->SetName("Fit_L1SingleEG34_No_Iso");
	fit1->Write();

	//l130_noiso_2->SetName("L1SingleEG30_No_Iso");
	//l130_noiso_2->SetName("L1SingleEG32_No_Iso");
	l130_noiso_2->SetName("L1SingleEG28_Tight_Iso");
	l130_noiso_2->Write();
	//fit1->SetName("Fit_L1SingleEG30_No_Iso");
	fit2->SetName("Fit_L1SingleEG28_Tight_Iso");
	fit2->Write();

	l130_noiso_3->SetName("L1SingleEG34_no_Iso_OR_EG28_Tight_Iso");
	l130_noiso_3->Write();
	fit3->SetName("Fit_L1SingleEG34_no_Iso_OR_EG28_Tight_Iso");
	fit3->Write();

	ll->SetName("Names");	
	ll->Write();
	l1eg->SetName("Condition");		
	l1eg->Write();	

}

