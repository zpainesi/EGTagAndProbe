#include <TH2.h>
#include <stdio.h>
#include <TGraph2D.h>
#include <TLatex.h>
#include <TPaveText.h>
#include <TPad.h>
#include <TVirtualPad.h>
#include <TLine.h>
#include "TH1D.h"
#include "TH1F.h"
#include "TH2D.h"
#include "TF1.h"
#include "TH2F.h"
#include "TTree.h"
#include "TFile.h"
#include "TDirectory.h"
#include "TLorentzVector.h"
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
#include "/afs/cern.ch/work/c/ckoraka/codes/setTDRStyle_teliko.C"



void plot_resolution_2017_vs_2018()
{

        gROOT->LoadMacro("/afs/cern.ch/work/c/ckoraka/codes/setTDRStyle_teliko.C");
        setTDRStyle_teliko();

        TPaveText *paveCMS = new TPaveText(0.21,0.96,0.93,0.99,"NDC");
        paveCMS->AddText("#bf{CMS #it{Simulation}}             2016 UL (13 TeV)");
        paveCMS->SetFillColor(0);
        paveCMS->SetBorderSize(0);
        paveCMS->SetTextSize(0.03);
        paveCMS->SetTextFont(42);


        TFile *res_2019 = new TFile("L1EG_resolution_2016UL_Data.root");
        TH1D *res_vs_et_barrel_2019= (TH1D*)res_2019->Get("hPhi_MC_barrel");
        TH1D *res_vs_et_endcap_2019= (TH1D*)res_2019->Get("hPhi_MC_endcap");
        TH1D *res_vs_eta_inclusive_2019= (TH1D*)res_2019->Get("hPhi_MC_barrel");

        TFile *res_2018 = new TFile("L1EG_resolution_2016UL_Unpacked.root");
        TH1D *res_vs_et_barrel_2018= (TH1D*)res_2018->Get("hPhi_MC_barrel");
        TH1D *res_vs_et_endcap_2018= (TH1D*)res_2018->Get("hPhi_MC_endcap");
        TH1D *res_vs_eta_inclusive_2018= (TH1D*)res_2018->Get("hPhi_MC_barrel");

        TFile *res_2017 = new TFile("L1EG_resolution_2016UL_v2_reEmu.root");
        TH1D *res_vs_et_barrel_2017= (TH1D*)res_2017->Get("hPhi_MC_barrel");
        TH1D *res_vs_et_endcap_2017= (TH1D*)res_2017->Get("hPhi_MC_endcap");
        TH1D *res_vs_eta_inclusive_2017= (TH1D*)res_2017->Get("hPhi_MC_barrel");

        TFile *res_2016 = new TFile("L1EG_resolution_2016UL_EcalSF.root");
        TH1D *res_vs_et_barrel_2016= (TH1D*)res_2016->Get("hPhi_MC_barrel");
        TH1D *res_vs_et_endcap_2016= (TH1D*)res_2016->Get("hPhi_MC_endcap");
        TH1D *res_vs_eta_inclusive_2016= (TH1D*)res_2016->Get("hEta_MC_barrel");

        TFile *res_2015 = new TFile("L1EG_resolution_2016UL_EcalSF_EGcalib.root");
        TH1D *res_vs_et_barrel_2015= (TH1D*)res_2015->Get("hPhi_MC_barrel");
        TH1D *res_vs_et_endcap_2015= (TH1D*)res_2015->Get("hPhi_MC_endcap");
        TH1D *res_vs_eta_inclusive_2015= (TH1D*)res_2015->Get("hPhi_MC_barrel");

	//////////////////////////////////      Canvases        ///////////////////////////////////////

        //TLegend *ll = new TLegend(.6, .7, .85, .85);
        TLegend *ll = new TLegend(.15, .50, .45, .65);
        ll->SetTextSize(0.022);
        ll->AddEntry(res_vs_et_barrel_2019,"Data","lp");
        ll->AddEntry(res_vs_et_barrel_2018,"Unpacked L1EG","lp");
        ll->AddEntry(res_vs_et_barrel_2017,"Re-emulated L1EG - A","lp");
        ll->AddEntry(res_vs_et_barrel_2016,"Re-emulated L1EG - B","lp");
        ll->AddEntry(res_vs_et_barrel_2015,"Re-emulated L1EG - C","lp");
        ll->SetBorderSize(1);

        
        TPaveText *l1egb = new TPaveText(.25,.70,.36,.80,"NDC");
        l1egb->AddText("L1 e/#gamma candidates");
        l1egb->AddText("Barrel");
        l1egb->SetTextSize(0.035);
        l1egb->SetFillColor(0);
        l1egb->SetFillStyle(0);
        l1egb->SetBorderSize(0);
        l1egb->SetTextFont(12);
        l1egb->SetTextColor(kBlack);

        TPaveText *l1eg = new TPaveText(.25,.70,.36,.80,"NDC");
        l1eg->AddText("L1 e/#gamma candidates");
        l1eg->AddText("Endcaps");
        l1eg->SetTextSize(0.035);
        l1eg->SetFillColor(0);
        l1eg->SetFillStyle(0);
        l1eg->SetBorderSize(0);
        l1eg->SetTextFont(12);
        l1eg->SetTextColor(kBlack);


        res_vs_et_barrel_2019->SetMarkerColor(kBlack);
        res_vs_et_endcap_2019->SetMarkerColor(kBlack);
        res_vs_et_barrel_2019->SetLineColor(kBlack);
        res_vs_et_endcap_2019->SetLineColor(kBlack);

        res_vs_et_barrel_2018->SetMarkerColor(kMagenta);
        res_vs_et_endcap_2018->SetMarkerColor(kMagenta);
        res_vs_et_barrel_2018->SetLineColor(kMagenta);
        res_vs_et_endcap_2018->SetLineColor(kMagenta);
    
        res_vs_et_barrel_2016->SetMarkerColor(kRed+2);
        res_vs_et_endcap_2016->SetMarkerColor(kRed+2);
        res_vs_et_barrel_2016->SetLineColor(kRed+2);
        res_vs_et_endcap_2016->SetLineColor(kRed+2);

        res_vs_et_barrel_2017->SetMarkerColor(kBlue+2);
        res_vs_et_endcap_2017->SetMarkerColor(kBlue+2);
        res_vs_et_barrel_2017->SetLineColor(kBlue+2);
        res_vs_et_endcap_2017->SetLineColor(kBlue+2);

        res_vs_et_barrel_2015->SetMarkerColor(kGreen+2);
        res_vs_et_endcap_2015->SetMarkerColor(kGreen+2);
        res_vs_et_barrel_2015->SetLineColor(kGreen+2);
        res_vs_et_endcap_2015->SetLineColor(kGreen+2);

        res_vs_et_barrel_2019->SetMarkerSize(0.8);
        res_vs_et_endcap_2019->SetMarkerSize(0.8);
        res_vs_et_barrel_2019->SetMarkerStyle(8);
        res_vs_et_endcap_2019->SetMarkerStyle(8);

        res_vs_et_barrel_2017->SetMarkerStyle(8);
        res_vs_et_endcap_2017->SetMarkerStyle(8);
        res_vs_et_barrel_2018->SetMarkerSize(0.8);
        res_vs_et_endcap_2018->SetMarkerSize(0.8);
        res_vs_et_barrel_2018->SetMarkerStyle(8);
        res_vs_et_endcap_2018->SetMarkerStyle(8);
        res_vs_et_barrel_2017->SetMarkerSize(0.8);
        res_vs_et_endcap_2017->SetMarkerSize(0.8);       

        res_vs_et_barrel_2016->SetMarkerStyle(8);
        res_vs_et_endcap_2016->SetMarkerStyle(8);
        res_vs_et_barrel_2015->SetMarkerSize(0.8);
        res_vs_et_endcap_2015->SetMarkerSize(0.8);
        res_vs_et_barrel_2015->SetMarkerStyle(8);
        res_vs_et_endcap_2015->SetMarkerStyle(8);
        res_vs_et_barrel_2016->SetMarkerSize(0.8);
        res_vs_et_endcap_2016->SetMarkerSize(0.8);  


	TCanvas *c4 = new TCanvas("c4","c4");
	c4->cd();
	c4->SetGrid();
	res_vs_et_barrel_2015->Draw("p");
	res_vs_et_barrel_2019->Draw("p same");
        res_vs_et_barrel_2016->Draw("p same");
        res_vs_et_barrel_2017->Draw("p same");
        res_vs_et_barrel_2018->Draw("p same");
        paveCMS->Draw("same");
        ll->Draw("same");
        l1egb->Draw("same");
        c4->SaveAs("resolution_phi_barrel.pdf");
        c4->SaveAs("resolution_phi_barrel.png");


	TCanvas *c5 = new TCanvas("c5","c5");
	c5->cd();
	c5->SetGrid();
	res_vs_et_endcap_2015->Draw("p");
	res_vs_et_endcap_2019->Draw("p same");
        res_vs_et_endcap_2016->Draw("p same");
	res_vs_et_endcap_2017->Draw("p same");
        res_vs_et_endcap_2018->Draw("p same");
        paveCMS->Draw("same");
	l1eg->Draw("same");
        ll->Draw("same");
        c5->SaveAs("resolution_phi_endcap.pdf");
        c5->SaveAs("resolution_phi_endcap.png");

}
