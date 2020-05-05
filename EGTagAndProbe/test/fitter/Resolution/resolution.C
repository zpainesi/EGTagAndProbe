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

        float   l1tEmuPt;
        float   l1tEmuEta;
        float   l1tEmuPhi;

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

        TH1D *et_barrel = new TH1D("et barrel","et barrel",100,0.5,1.5);
        TH1D *et_endcap = new TH1D("et endcap","et endcap",100,0.5,1.5);

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

	//TFile *f2=new TFile("RelValZEE_13UP16_2016_v3_3_1_2018_EcalSF.root");
	//TFile *f2=new TFile("RelValZEE_13UP16_2016_v2.root");
	//TFile *f2=new TFile("RelValZEE_13UP16_2016_v3_3_1_2018_EcalSF_EGcalib.root");
	TFile *f2=new TFile("NTuple.root");

	TTree *t1=(TTree*)(f2->Get("Ntuplizer/TagAndProbe"));

        t1->SetBranchAddress("Nvtx",&Nvtx);
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
        t1->SetBranchAddress("l1tEmuPt",&l1tEmuPt);
        t1->SetBranchAddress("l1tEmuEta",&l1tEmuEta);            
        t1->SetBranchAddress("l1tEmuPhi",&l1tEmuPhi);            

        int nentries = t1->GetEntries();

	int bar=0;
	int end=0;
	int incl=0;

        TFile *fl = new TFile("MyPileupHistograms.root","READ");
        TH1D *nvtx_ratio = (TH1D*)fl -> Get("nvtx_ratio");
        TAxis *xaxis = nvtx_ratio -> GetXaxis();


        for (int jentry=0; jentry<nentries; jentry++){   
        //for (int jentry=0; jentry<100000; jentry++){   
                t1->GetEntry(jentry);
		if(jentry%100000==0) cout<<jentry<<endl;

		//if(!(eleProbeSclEt>60. && eleProbeSclEt<70.)) continue;
		if(isProbeLoose==1){
			if(fabs(eleProbeEta) < 2.4){

				res_vs_et_inclusive_2d->Fill(eleProbeSclEt,l1tPt/eleProbeSclEt);
				res_vs_eta_inclusive_2d->Fill(eleProbeEta,l1tPt/eleProbeSclEt);	
                                //res_vs_et_inclusive_2d->Fill(eleProbeSclEt,l1tEmuPt/eleProbeSclEt);
                                //res_vs_eta_inclusive_2d->Fill(eleProbeEta,l1tEmuPt/eleProbeSclEt);

				++incl;				
				
				double weight = 1.;
				//double weight = nvtx_ratio->GetBinContent(xaxis->FindBin(Nvtx));
				//cout<<"weight = "<<weight<<" Nvtx = "<<Nvtx<<endl;
				if(fabs(eleProbeEta)<1.305) {	
					et_barrel->Fill(l1tPt/eleProbeSclEt,weight);
					eta_barrel->Fill(l1tEta-eleProbeEta,weight);
					phi_barrel->Fill(l1tPhi-eleProbePhi,weight);
					res_vs_et_barrel_2d->Fill(eleProbeSclEt,l1tPt/eleProbeSclEt);
                                        //et_barrel->Fill(l1tEmuPt/eleProbeSclEt,weight);
                                        //eta_barrel->Fill(l1tEmuEta-eleProbeEta,weight);
                                        //phi_barrel->Fill(l1tEmuPhi-eleProbePhi,weight);
                                        //res_vs_et_barrel_2d->Fill(eleProbeSclEt,l1tEmuPt/eleProbeSclEt);
					++bar;
				}
				if(fabs(eleProbeEta)>1.479){
					et_endcap->Fill(l1tPt/eleProbeSclEt,weight);
					eta_endcap->Fill(l1tEta-eleProbeEta,weight);
					phi_endcap->Fill(l1tPhi-eleProbePhi,weight);
					res_vs_et_endcap_2d->Fill(eleProbeSclEt,l1tPt/eleProbeSclEt);

                                        //et_endcap->Fill(l1tEmuPt/eleProbeSclEt,weight);
                                        //eta_endcap->Fill(l1tEmuEta-eleProbeEta,weight);
                                        //phi_endcap->Fill(l1tEmuPhi-eleProbePhi,weight);
                                        //res_vs_et_endcap_2d->Fill(eleProbeSclEt,l1tEmuPt/eleProbeSclEt);

					++end;
				}
			}
			
		}

	}


	et_barrel->Scale(1./bar);
	et_endcap->Scale(1./end);

	eta_barrel->Scale(1./bar);
	eta_endcap->Scale(1./end);

	phi_barrel->Scale(1./bar);
	phi_endcap->Scale(1./end);

	
	double flag=0.;

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
        paveCMS->Draw("same");
	l->Draw("same");
	c1->SaveAs("resolution_Et.png");

	TCanvas *c2 = new TCanvas("c2","c2",800,800);
	c2->cd();
	eta_barrel->Draw("p");
	eta_endcap->Draw("p same");
	c2->SetGrid();
        paveCMS->Draw("same");
	l->Draw("same");
	c2->SaveAs("resolution_Eta.png");
	
	TCanvas *c3 = new TCanvas("c3","c3");
	c3->cd();
	phi_barrel->Draw("p");
	phi_endcap->Draw("p same");
	c3->SetGrid();
        paveCMS->Draw("same");
	l->Draw("same");
	c3->SaveAs("resolution_Phi.png");



	// 2-D Plots
        //TFile *file =new TFile("L1EG_resolution_2016UL_Unpacked.root","RECREATE");
        //TFile *file =new TFile("L1EG_resolution_2016UL_v2_reEmu.root","RECREATE");
        //TFile *file =new TFile("L1EG_resolution_2016UL_EcalSF.root","RECREATE");
        //TFile *file =new TFile("L1EG_resolution_2016UL_EcalSF_EGcalib.root","RECREATE");	
        TFile *file =new TFile("L1EG_resolution_NEW.root","RECREATE");	


        et_barrel->SetName("hEt_MC_barrel");
        et_barrel->Write();
        
        et_endcap->SetName("hEt_MC_endcap");
        et_endcap->Write();


        eta_barrel->SetName("hEta_MC_barrel");
        eta_barrel->Write();
        
        eta_endcap->SetName("hEta_MC_endcap");
        eta_endcap->Write();


        phi_barrel->SetName("hPhi_MC_barrel");
        phi_barrel->Write();
        
        phi_endcap->SetName("hPhi_MC_endcap");
        phi_endcap->Write();


}

