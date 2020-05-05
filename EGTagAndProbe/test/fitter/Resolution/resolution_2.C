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
#include "/afs/cern.ch/work/c/ckoraka/codes/setTDRStyle_teliko.C"

using namespace std;
using namespace RooFit;

void resolution()
{


	 gROOT->LoadMacro("/afs/cern.ch/work/c/ckoraka/codes/setTDRStyle_teliko.C");
	 setTDRStyle_teliko();

         TPaveText *paveCMS = new TPaveText(0.15,0.96,0.93,0.99,"NDC");
         paveCMS->AddText("#bf{CMS} #it{Preliminary} 2018         58.8 fb^{-1}  (13 TeV)");
         //paveCMS->AddText("#bf{CMS} #it{Preliminary} 2017         41.3 fb^{-1}  (13 TeV)");
         paveCMS->SetFillColor(0);
         paveCMS->SetBorderSize(0);
         paveCMS->SetTextSize(0.04);
         paveCMS->SetTextFont(42);


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

        TH1D *et_barrel = new TH1D("et barrel","et barrel",80,0.6,1.4);
        TH1D *et_endcap = new TH1D("et endcap","et endcap",80,0.6,1.4);

        TH1D *eta_barrel = new TH1D("eta barrel","eta barrel",64,-0.2,0.2);	//47
        TH1D *eta_endcap = new TH1D("eta endcap","eta endcap",64,-0.2,0.2);

        TH1D *phi_barrel = new TH1D("phi barrel","phi barrel",98,-0.3,0.3);	//49
        TH1D *phi_endcap = new TH1D("phi endcap","phi endcap",98,-0.3,0.3);

	et_barrel->GetXaxis()->SetTitle("E_{T}^{e#gamma, L1} / E_{T}^{e#gamma, offline}");
	et_endcap->GetXaxis()->SetTitle("E_{T}^{e#gamma, L1} / E_{T}^{e#gamma, offline}");
	et_barrel->GetYaxis()->SetTitle("a.u.");
	et_endcap->GetYaxis()->SetTitle("a.u.");

	eta_barrel->GetXaxis()->SetTitle("#eta^{e#gamma, L1} - #eta^{e#gamma, offline}");
	eta_endcap->GetXaxis()->SetTitle("#eta^{e#gamma, L1} - #eta^{e#gamma, offline}");
	eta_barrel->GetYaxis()->SetTitle("a.u.");
	eta_endcap->GetYaxis()->SetTitle("a.u.");

	phi_barrel->GetXaxis()->SetTitle("#varphi^{e#gamma, L1} - #varphi^{e#gamma, offline}");
	phi_endcap->GetXaxis()->SetTitle("#varphi^{e#gamma, L1} - #varphi^{e#gamma, offline}");
	phi_barrel->GetYaxis()->SetTitle("a.u.");
	phi_endcap->GetYaxis()->SetTitle("a.u.");

	const Int_t PtBINS = 14; 

	Double_t PtEdges[PtBINS + 1] = {5., 10., 14., 18., 20., 23., 26., 30., 40., 50., 60., 70., 80., 90., 100.};

	//const Int_t EtaBINS = 9; const Int_t YBINS_Eta = 30;
	//Double_t EtaEdges[EtaBINS + 1] = {0., 0.25, 0.5, 0.75, 1.0, 1.25, 1.50, 1.75, 2.0, 2.5};
	const Int_t EtaBINS = 23; 
	Double_t EtaEdges[EtaBINS + 1] = {0.,0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0, 1.1, 1.2, 1.305, 1.479, 1.6, 1.7, 1.8, 1.9, 2.0, 2.1, 2.2, 2.3, 2.4};

        TH2D *res_vs_et_barrel_2d = new TH2D("2d res vs et barrel","2d res vs et barrel",PtBINS,PtEdges,1000,0.,3.);
        TH2D *res_vs_et_endcap_2d = new TH2D("2d res vs et endcap","2d res vs et endcap",PtBINS,PtEdges,1000,0.,3.);
        TH2D *res_vs_et_inclusive_2d = new TH2D("2d res vs et inclusive","2d res vs et inclusive",PtBINS,PtEdges,1000,0.,3.);
        TH1D *res_vs_et_barrel = new TH1D("res vs et barrel","res vs et barrel",PtBINS,PtEdges);
        TH1D *res_vs_et_endcap = new TH1D("res vs et endcap","res vs et endcap",PtBINS,PtEdges);
        TH1D *res_vs_et_inclusive = new TH1D("res vs et inclusive","res vs et inclusive",PtBINS,PtEdges);

        TH2D *res_vs_eta_inclusive_2d = new TH2D("2d res vs eta inclusive","2d res vs eta inclusive",EtaBINS,EtaEdges,1000,0.,3.);
        TH1D *res_vs_eta_inclusive = new TH1D("res vs eta inclusive","res vs eta inclusive",EtaBINS,EtaEdges);

	res_vs_et_barrel->GetYaxis()->SetTitle("FWHM / (E_{T}^{e#gamma, L1}/E_{T}^{e#gamma, offline})_{at maximum}");
	res_vs_et_endcap->GetYaxis()->SetTitle("FWHM / (E_{T}^{e#gamma, L1}/E_{T}^{e#gamma, offline})_{at maximum}");
	res_vs_et_inclusive->GetYaxis()->SetTitle("FWHM / (E_{T}^{e#gamma, L1}/E_{T}^{e#gamma, offline})_{at maximum}");
	res_vs_et_barrel->GetXaxis()->SetTitle("E_{T}^{e#gamma, offline} [GeV]");
	res_vs_et_endcap->GetXaxis()->SetTitle("E_{T}^{e#gamma, offline} [GeV]");
	res_vs_et_inclusive->GetXaxis()->SetTitle("E_{T}^{e#gamma, offline} [GeV]");

	res_vs_eta_inclusive->GetYaxis()->SetTitle("FWHM / (E_{T}^{e#gamma, L1}/E_{T}^{e#gamma, offline})_{at maximum}");
	//res_vs_eta_inclusive->GetYaxis()->SetTitle("RMS / <E_{T}^{e#gamma, L1}/E_{T}^{e#gamma, offline}>");
	res_vs_eta_inclusive->GetXaxis()->SetTitle("|#eta^{e#gamma, offline}|");


	/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	et_barrel->SetLineColor(kBlue);
	et_barrel->SetMarkerColor(kBlue);
	et_barrel->SetMarkerSize(0.8);
	et_barrel->SetMarkerStyle(21);

	et_endcap->SetLineColor(kGreen+3);
	et_endcap->SetMarkerColor(kGreen+3);
	et_endcap->SetMarkerSize(0.8);
	et_endcap->SetMarkerStyle(22);

	eta_barrel->SetLineColor(kBlue);
	eta_barrel->SetMarkerColor(kBlue);
	eta_barrel->SetMarkerSize(0.8);
	eta_barrel->SetMarkerStyle(21);

	eta_endcap->SetLineColor(kGreen+3);
	eta_endcap->SetMarkerColor(kGreen+3);
	eta_endcap->SetMarkerSize(0.8);
	eta_endcap->SetMarkerStyle(22);

	phi_barrel->SetLineColor(kBlue);
	phi_barrel->SetMarkerColor(kBlue);
	phi_barrel->SetMarkerSize(0.8);
	phi_barrel->SetMarkerStyle(21);

	phi_endcap->SetLineColor(kGreen+3);
	phi_endcap->SetMarkerColor(kGreen+3);
	phi_endcap->SetMarkerSize(0.8);
	phi_endcap->SetMarkerStyle(22);


	/////////////////////////////////////////////////////////////////////////////////////////////////////

       // TFile *f2=new TFile("/afs/cern.ch/user/c/ckoraka/L1EG_for_UL2016/CMSSW_10_6_8/src/EGTagAndProbe/EGTagAndProbe/test/DoubleEG_TagAndProbe_DoubleEG_Run2016G-ForValUL2016-v1.root","READ");
        TFile *f2=new TFile("/afs/cern.ch/user/c/ckoraka/L1EG_for_UL2016/CMSSW_10_6_8/src/EGTagAndProbe/EGTagAndProbe/test/ZEE_CMSSW_10_6_8-PU25ns_106X_mcRun2_asymptotic_v9_UL16_CP5_postVFP-v1.root","READ");
        //TFile *f2=new TFile("/afs/cern.ch/user/c/ckoraka/L1EG_for_UL2016/CMSSW_10_6_8/src/EGTagAndProbe/EGTagAndProbe/test/DoubleEG_TagAndProbe_Run2016B-ForValUL2016_HIPM-v1.root","READ");
	TTree *t1=(TTree*)(f2->Get("Ntuplizer/TagAndProbe"));

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



        int nentries = t1->GetEntries();

	int bar=0;
	int end=0;
	int incl=0;

        for (int jentry=0; jentry<nentries; jentry++){   
        //for (int jentry=0; jentry<100000; jentry++){   
                t1->GetEntry(jentry);
		if(jentry%100000==0) cout<<jentry<<endl;

		//if(!(eleProbeSclEt>60. && eleProbeSclEt<70.)) continue;
		if(isProbeLoose==1){
			if(fabs(eleProbeEta) < 2.4){

				res_vs_et_inclusive_2d->Fill(eleProbeSclEt,l1tPt/eleProbeSclEt);
				res_vs_eta_inclusive_2d->Fill(eleProbeEta,l1tPt/eleProbeSclEt);	
				++incl;				

				if(fabs(eleProbeEta)<1.305) {	
					et_barrel->Fill(l1tPt/eleProbeSclEt,1.);
					eta_barrel->Fill(l1tEta-eleProbeEta,1.);
					phi_barrel->Fill(l1tPhi-eleProbePhi,1.);
					res_vs_et_barrel_2d->Fill(eleProbeSclEt,l1tPt/eleProbeSclEt);
					++bar;
				}
				if(fabs(eleProbeEta)>1.479){
					et_endcap->Fill(l1tPt/eleProbeSclEt,1.);
					eta_endcap->Fill(l1tEta-eleProbeEta,1.);
					phi_endcap->Fill(l1tPhi-eleProbePhi,1.);
					res_vs_et_endcap_2d->Fill(eleProbeSclEt,l1tPt/eleProbeSclEt);
					++end;
				}
			}
			
		}

	}


	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	/*	
	// 2D Resolution Plots vs Et
		
	int bin1,bin2,bin3;
	double fwhm,max;
	double rms,mean;

	for(int i=1; i<=res_vs_et_inclusive->GetNbinsX(); ++i){
		////	Inclusive	//////////////////
	        TH1D *projectionY_inclusive = res_vs_et_inclusive_2d->ProjectionY("proj_inclusive",i,i);
		//RMS/Mean
		rms = projectionY_inclusive->GetRMS();
		mean = projectionY_inclusive->GetMean();
		//FWHM/max
		bin1 = projectionY_inclusive->FindFirstBinAbove(projectionY_inclusive->GetMaximum()/2);
		bin2 = projectionY_inclusive->FindLastBinAbove(projectionY_inclusive->GetMaximum()/2);
		fwhm = projectionY_inclusive->GetBinCenter(bin2) - projectionY_inclusive->GetBinCenter(bin1);
		max  = projectionY_inclusive->GetXaxis()->GetBinCenter(projectionY_inclusive->GetMaximumBin());

		res_vs_et_inclusive->SetBinContent(i,fwhm / max);	
	        res_vs_et_inclusive->SetBinError(i,fwhm / (sqrt(projectionY_inclusive->Integral())));
		//res_vs_et_inclusive->SetBinContent(i,rms / mean);
	        //res_vs_et_inclusive->SetBinError(i,rms / (sqrt(projectionY_inclusive->Integral())));
	

		////	Barrel		///////////////////
		TH1D *projectionY_barrel = res_vs_et_barrel_2d->ProjectionY("proj_barrel",i,i);
		//RMS/Mean	
		rms = projectionY_barrel->GetRMS();
		mean = projectionY_barrel->GetMean();
		//FWHM/max
		bin1 = projectionY_barrel->FindFirstBinAbove(projectionY_barrel->GetMaximum()/2);
		bin2 = projectionY_barrel->FindLastBinAbove(projectionY_barrel->GetMaximum()/2);
		fwhm = projectionY_barrel->GetBinCenter(bin2) - projectionY_barrel->GetBinCenter(bin1);
		max  = projectionY_barrel->GetXaxis()->GetBinCenter(projectionY_barrel->GetMaximumBin());

		res_vs_et_barrel->SetBinContent(i,fwhm / max);		
	        res_vs_et_barrel->SetBinError(i,fwhm / (sqrt(projectionY_barrel->Integral())));
	       	//res_vs_et_barrel->SetBinContent(i,rms / mean);
	        //res_vs_et_barrel->SetBinError(i,rms / (sqrt(projectionY_barrel->Integral())));


		////	Endcap		//////////////////////
		TH1D *projectionY_endcap = res_vs_et_endcap_2d->ProjectionY("proj_endcap",i,i);
		//RMS/Mean	
		rms = projectionY_endcap->GetRMS();
		mean = projectionY_endcap->GetMean();
		//FWHM/max
		bin1 = projectionY_endcap->FindFirstBinAbove(projectionY_endcap->GetMaximum()/2);
		bin2 = projectionY_endcap->FindLastBinAbove(projectionY_endcap->GetMaximum()/2);
		fwhm = projectionY_endcap->GetBinCenter(bin2) - projectionY_endcap->GetBinCenter(bin1);
		max  = projectionY_endcap->GetXaxis()->GetBinCenter(projectionY_endcap->GetMaximumBin());

		res_vs_et_endcap->SetBinContent(i,fwhm / max);	
	        res_vs_et_endcap->SetBinError(i,fwhm / (sqrt(projectionY_endcap->Integral())));
        	//res_vs_et_endcap->SetBinContent(i,rms / mean);
	        //res_vs_et_endcap->SetBinError(i,rms / (sqrt(projectionY_endcap->Integral())));

	}

	// 2D Resolution Plots vs eta

	for(int i=1; i<=res_vs_eta_inclusive->GetNbinsX(); ++i){
		if(i==14) continue;	//Gep between Barrel & EE
		////	Inclusive		////////////////////////////
	        TH1D *projectionY_inclusive = res_vs_eta_inclusive_2d->ProjectionY("proj_inclusive",i,i);
		//RMS/Mean
		rms = projectionY_inclusive->GetRMS();
		mean = projectionY_inclusive->GetMean();
		//FWHM/max
		bin1 = projectionY_inclusive->FindFirstBinAbove(projectionY_inclusive->GetMaximum()/2);
		bin2 = projectionY_inclusive->FindLastBinAbove(projectionY_inclusive->GetMaximum()/2);
		fwhm = projectionY_inclusive->GetBinCenter(bin2) - projectionY_inclusive->GetBinCenter(bin1);
		max  = projectionY_inclusive->GetXaxis()->GetBinCenter(projectionY_inclusive->GetMaximumBin());

		res_vs_eta_inclusive->SetBinContent(i,fwhm / max);	
	        res_vs_eta_inclusive->SetBinError(i,fwhm / (sqrt(projectionY_inclusive->Integral())));
	 	//res_vs_eta_inclusive->SetBinContent(i,rms / projectionY_inclusive->GetMean());
	        //res_vs_eta_inclusive->SetBinError(i,rms / (sqrt(projectionY_inclusive->Integral())));
	}
	*/
	/////////////////////////////////////////////////////
	//Scaling usinig Integral of distribution
	/*
	et_barrel->Scale(1./et_barrel->Integral());
	et_endcap->Scale(1./et_endcap->Integral());

	eta_barrel->Scale(1./eta_barrel->Integral());
	eta_endcap->Scale(1./eta_endcap->Integral());

	phi_barrel->Scale(1./phi_barrel->Integral());
	phi_endcap->Scale(1./phi_endcap->Integral());
	*/
	//Scaling using total number of events for each distribution
	et_barrel->Scale(1./bar);
	et_endcap->Scale(1./end);

	eta_barrel->Scale(1./bar);
	eta_endcap->Scale(1./end);

	phi_barrel->Scale(1./bar);
	phi_endcap->Scale(1./end);

	
	double flag=0.;
/*        TFile *f3=new TFile("old_scripts/plotOutData_charis_fit.root","READ");

	TF1 *fit_et_barrel = (TF1*) f3->Get("fit_hEt_Data_barrel"); 
        fit_et_barrel->SetLineColor(kBlue);
        fit_et_barrel->SetLineWidth(1);
	TH1D *hfit_et_barrel = (TH1D*)fit_et_barrel->GetHistogram()->Clone("r");
	flag = hfit_et_barrel->Integral();
        hfit_et_barrel->Scale(bar/flag);

        TF1 *fit_et_endcap = (TF1*) f3->Get("fit_hEt_Data_endcap");
        fit_et_endcap->SetLineColor(kGreen+3);
        fit_et_endcap->SetLineWidth(1);
        TH1D *hfit_et_endcap = (TH1D*)fit_et_endcap->GetHistogram()->Clone("r"); 
        flag = hfit_et_endcap->Integral();
        hfit_et_endcap->Scale(end/flag);  

        TF1 *fit_eta_barrel = (TF1*) f3->Get("fit_hEta_Data_barrel");
        fit_eta_barrel->SetLineColor(kBlue);
        fit_eta_barrel->SetLineWidth(1);
        
        TF1 *fit_eta_endcap = (TF1*) f3->Get("fit_hEta_Data_endcap");
        fit_eta_endcap->SetLineColor(kGreen+3);
        fit_eta_endcap->SetLineWidth(1);
*/

	// Plotting		////////////////////////////

	TPaveText *l1eg = new TPaveText(.7,.70,.85,.80,"NDC");
        l1eg->AddText("L1 Single EG");
	l1eg->SetTextSize(0.035);
        l1eg->SetFillColor(0);
	l1eg->SetFillStyle(0);
	l1eg->SetBorderSize(0);
        l1eg->SetTextFont(12);
        l1eg->SetTextColor(kBlack);

	
        TLegend *l = new TLegend(.7, .6, .85, .7);
        l->SetTextSize(0.022);
        l->AddEntry(et_barrel,"Barrel","lp");
        l->AddEntry(et_endcap,"Endcap","lp");
	l->SetBorderSize(1);

	TCanvas *c1 = new TCanvas("c1","c1",800,800);
	c1->cd();
	c1->SetGrid();
	et_barrel->Draw("p");
	et_endcap->Draw("p same");
        //hfit_et_barrel->Draw();  
        //hfit_et_endcap->Draw("same");
        paveCMS->Draw("same");
	l->Draw("same");
	//l1eg->Draw("same");
	c1->SaveAs("resolution_Et.png");

	TCanvas *c2 = new TCanvas("c2","c2",800,800);
	c2->cd();
        //fit_eta_barrel->Draw();  
        //fit_eta_endcap->Draw("same");
	eta_barrel->Draw("p");
	eta_endcap->Draw("p same");
	c2->SetGrid();
        paveCMS->Draw("same");
	l->Draw("same");
	//l1eg->Draw("same");
	c2->SaveAs("resolution_Eta.png");
	
	TCanvas *c3 = new TCanvas("c3","c3");
	c3->cd();
	phi_barrel->Draw("p");
	phi_endcap->Draw("p same");
	c3->SetGrid();
        paveCMS->Draw("same");
	l->Draw("same");
	//l1eg->Draw("same");
	c3->SaveAs("resolution_Phi.png");

        TLegend *ll = new TLegend(.7, .6, .85, .7);
        ll->SetTextSize(0.022);
        ll->AddEntry(res_vs_et_barrel,"Barrel","lp");
        ll->AddEntry(res_vs_et_endcap,"Endcap","lp");
        ll->AddEntry(res_vs_et_inclusive,"Inclusive","lp");
	ll->SetBorderSize(1);

	TCanvas *c4 = new TCanvas("c4","c4");
	c4->cd();
	c4->SetGrid();
	c4->SetLogx();
	res_vs_et_barrel->SetMarkerColor(kBlue);
	res_vs_et_endcap->SetMarkerColor(kGreen+3);
	res_vs_et_inclusive->SetMarkerColor(kRed+2);
	res_vs_et_barrel->SetLineColor(kBlue);
	res_vs_et_endcap->SetLineColor(kGreen+3);
	res_vs_et_inclusive->SetLineColor(kRed+2);
	res_vs_et_barrel->SetMarkerStyle(33);
	res_vs_et_endcap->SetMarkerStyle(33);
	res_vs_et_inclusive->SetMarkerStyle(33);
	res_vs_et_endcap->GetYaxis()->SetRangeUser(0.,0.65);
	res_vs_et_barrel->GetYaxis()->SetRangeUser(0.,0.65);
	res_vs_et_inclusive->GetYaxis()->SetRangeUser(0.,0.65);
	res_vs_et_endcap->Draw("p");
	res_vs_et_barrel->Draw("p same");
	res_vs_et_inclusive->Draw("p same");
        paveCMS->Draw("same");
	ll->Draw("same");
	//l1eg->Draw("same");
	c4->SaveAs("resolution_vs_et_coarse.png");

	TCanvas *c5 = new TCanvas("c5","c5");
	c5->cd();
	c5->SetGrid();
	c5->SetGrid();
	res_vs_eta_inclusive->SetMarkerColor(kRed+2);
	res_vs_eta_inclusive->SetLineColor(kRed+2);
	res_vs_eta_inclusive->SetMarkerStyle(33);
	res_vs_eta_inclusive->GetYaxis()->SetRangeUser(0.,0.3);
	res_vs_eta_inclusive->Draw("p");


	TPaveText *trans = new TPaveText(1.307,0.,1.477,0.299,"trans");
	trans->SetFillColor(16);
	trans->Draw("same");
	TText *tt = new TText(1.42,0.06,"Transition between EB and EE");
	tt->SetTextAlign(11); tt->SetTextSize(0.03);
	tt->SetTextAngle(90);
	tt->Draw("same");

	TLine *line1 = new TLine(1.479,0.,1.479,0.3);
	line1->SetLineColor(kBlack);
	line1->SetLineStyle(7);
	line1->SetLineWidth(1);
	line1->Draw("same");
	TLine *line2 = new TLine(1.305,0.,1.305,0.3);
	line2->SetLineColor(kBlack);
	line2->SetLineStyle(7);
	line2->SetLineWidth(1);
	line2->Draw("same");
        paveCMS->Draw("same");
	//l1eg->Draw("same");
	c5->SaveAs("resolution_vs_eta_fwhm.png");
	

	/*
        TFile *file =new TFile("L1EG_resolution_2018.root","RECREATE");
        file->cd();

	//1-D Plots
        et_barrel->SetName("Energy_resolution_Barrel");
        et_barrel->Write();

        et_endcap->SetName("Energy_resolution_Endcap");
        et_endcap->Write();

        eta_barrel->SetName("Eta_resolution_Barrel");
        eta_barrel->Write();

        eta_endcap->SetName("Eta_resolution_Endcap");
        eta_endcap->Write();

	*/

        

	/*
        TFile *file =new TFile("old_scripts/plotOutData_charis.root","RECREATE");
        file->cd();
        
        //1-D Plots
        et_barrel->SetName("hEt_Data_barrel");
        et_barrel->Write();
        
        et_endcap->SetName("hEt_Data_endcap");
        et_endcap->Write();

        //et_inclusive->SetName("hEt_Data");
        //et_inclusive->Write();

        eta_barrel->SetName("hEta_Data_barrel");
        eta_barrel->Write();
        
        eta_endcap->SetName("hEta_Data_endcap");
        eta_endcap->Write();

        //eta_inclusive->SetName("hEta_Data");
        //eta_inclusive->Write();

        phi_barrel->SetName("hPhi_Data_barrel");
        phi_barrel->Write();
        
        phi_endcap->SetName("hPhi_Data_endcap");
        phi_endcap->Write();
  
        //phi_inclusive->SetName("hPhi_Data");
        //phi_inclusive->Write();
	*/

	// 2-D Plots

        TFile *file =new TFile("L1EG_resolution_2016UL_MC_pre.root","RECREATE");
	
        res_vs_et_barrel->SetName("res_vs_et_barrel"); 
        res_vs_et_barrel->Write();

        res_vs_et_endcap->SetName("res_vs_et_endcap");
        res_vs_et_endcap->Write();

        res_vs_et_inclusive->SetName("res_vs_et_inclusive");
        res_vs_et_inclusive->Write();

        res_vs_eta_inclusive->SetName("res_vs_eta_inclusive");
        res_vs_eta_inclusive->Write();
        


}

