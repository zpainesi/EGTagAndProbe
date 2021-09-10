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
#include <TColor.h>
#include <TF1.h>
#include <TH1F.h>
#include <TFile.h>


// double CB symmetric (two-sided tails)
double CrystalBall(double* x, double* par){ 
	//http://en.wikipedia.org/wiki/Crystal_Ball_function 
	double xcur = x[0]; 
	double alpha = par[0]; 
	double n = par[1]; 
	double mu = par[2]; 
	double sigma = par[3]; 
  	double N = par[4]; 
  	TF1* exp = new TF1("exp","exp(x)",1e-20,1e20); 
  	double A; double B; 
  	if (alpha < 0){ 
    		A = pow((n/(-1*alpha)),n)*exp->Eval((-1)*alpha*alpha/2); 
    		B = n/(-1*alpha) + alpha;
	} 
  	else { 
   		A = pow((n/alpha),n)*exp->Eval((-1)*alpha*alpha/2); 
    		B = n/alpha - alpha;
	} 
    	double f; 
  	if (TMath::Abs((xcur-mu)/sigma) < TMath::Abs(alpha) ) 
    		f = N*exp->Eval((-1)*(xcur-mu)*(xcur-mu)/(2*sigma*sigma)) + par[5]*TMath::Exp(-0.5*pow(((xcur-par[6])/par[7]),2)); //charis-added par[5] 
  	else if (((xcur-mu)/sigma) < (-1.)*alpha )
    		f = N*A*pow((B- (xcur-mu)/sigma),(-1*n)); // left tail
	else
    		f = N*A*pow( (B- (mu-xcur)/sigma),(-1*n)); // right tail
  	delete exp; 
  	return f; 
} 


// double CB asymmetric
double DoubleCrystalBall(double* x, double* par){ 
	//http://en.wikipedia.org/wiki/Crystal_Ball_function 
 	double xcur = x[0]; 
 	double alphaL = par[0]; 
  	double nL = par[1]; 
  	double alphaR = par[2]; 
  	double nR = par[3]; 

  	double mu = par[4]; 
  	double sigma = par[5]; 
  	double N = par[6]; 
 
  	TF1* exp = new TF1("exp","exp(x)",1e-20,1e20); 
  	double AL; double BL; double AR; double BR; 
 
  	if (alphaL < 0){ 
    		AL = pow((nL/(-1*alphaL)),nL)*exp->Eval((-1)*alphaL*alphaL/2); 
    		BL = nL/(-1*alphaL) + alphaL;
	} 
  	else { 
    		AL = pow((nL/alphaL),nL)*exp->Eval((-1)*alphaL*alphaL/2); 
    		BL = nL/alphaL - alphaL;
	} 
  	if (alphaR < 0){ 
    		AR = pow((nR/(-1*alphaR)),nR)*exp->Eval((-1)*alphaR*alphaR/2); 
    		BR = nR/(-1*alphaR) + alphaR;
	} 
  	else { 
    		AR = pow((nR/alphaR),nR)*exp->Eval((-1)*alphaR*alphaR/2); 
    		BR = nR/alphaR - alphaR;	
	} 
   
    	double f; 
  	if ( ((xcur-mu)/sigma) > (-1.)*alphaL  && ((xcur-mu)/sigma) < (1.)*alphaR) 
    		//f = N*exp->Eval((-1)*(xcur-mu)*(xcur-mu)/(2*sigma*sigma)); 
		f = N*exp->Eval((-1)*(xcur-mu)*(xcur-mu)/(2*sigma*sigma)) + par[7]*TMath::Exp(-0.5*pow(((xcur-par[8])/par[9]),2)); //charis-added par[5] 

  	// left
  	else if ( ((xcur-mu)/sigma) <= (-1.)*alphaL )
    		f = N*AL*pow((BL- (xcur-mu)/sigma),(-1*nL)); // left tail
  	//right
  	else
    		f = N*AR*pow( (BR- (mu-xcur)/sigma),(-1*nR)); // right tail
  	delete exp; 
  	return f; 
} 


void doFits()
{
   	TFile* fIn = new TFile ("plotOutData_charis.root");
    	TFile* fOut = new TFile ("plotOutData_charis_fit.root", "recreate");
    	//vector<TString> fitSingleCB = {"hEta_Data", "hPhi_Data", "hEta_Data_barrel", "hPhi_Data_barrel", "hEta_Data_endcap", "hPhi_Data_endcap"};
    	//vector<TString> fitDoubleCB = {"hEt_Data", "hEt_Data_barrel", "hEt_Data_endcap"};
    
        vector<TString> fitSingleCB = {"hEta_Data_barrel", "hPhi_Data_barrel", "hEta_Data_endcap", "hPhi_Data_endcap"};
        vector<TString> fitDoubleCB = {"hEt_Data_barrel", "hEt_Data_endcap"};


    	TH1::SetDefaultSumw2();
   
 
    	//TF1* CBFunc = new TF1("CBFunc",&CrystalBall,-0.3,0.3,5);	//-0.3 - 0.3
    	//CBFunc->SetParameters(3, 1, 0, 0.05, 0.06);    
	
   	TF1* CBFunc = new TF1("CBFunc",&CrystalBall,-0.3,0.3,8);	//-0.3 - 0.3 //Charis
    	CBFunc->SetParameters(3, 1, 0, 0.05, 0.06,0.12,0.,0.01);    



    	for (TString name : fitSingleCB)
    	{
    		cout << "... fitting: " << name << endl;
        	TH1F* h = (TH1F*) fIn->Get(name);
        	h->Scale(1./h->Integral());
        	h->Fit("CBFunc");
        	fOut->cd();
        	h->Write();
                //CBFunc->SetName("fit_"+name);
                //CBFunc->Write();
    	}
   

    	//TF1* CBFuncAsymm = new TF1("CBFuncAsymm",&DoubleCrystalBall,0.,600.,7);
    	//for (TString name : fitDoubleCB)

    	//TF1* CBFuncAsymm = new TF1("CBFuncAsymm",&DoubleCrystalBall,0.,3.,7);	//0 - 3.
    	TF1* CBFuncAsymm = new TF1("CBFuncAsymm",&DoubleCrystalBall,0.,3.,10);	//0 - 3. //Charis
	CBFuncAsymm->SetParameter(8,0.06);
	CBFuncAsymm->SetParameter(9,1);
	CBFuncAsymm->SetParameter(10,0.01);
    	for (TString name : fitDoubleCB)
    	{
        	cout << "... fitting: " << name << endl;
	
        	TH1F* h = (TH1F*) fIn->Get(name);
        	cout<<" before scale"<<endl;

        	h->Scale(1./h->Integral());
		cout<<"after scale "<<endl;
        	//CBFuncAsymm->SetParameters   (1.0, 0.1, 1.3, 4.3, 1., 0.4, 1.);
        	CBFuncAsymm->SetParameters   (1.0, 0.1, 1.3, 4.3, h->GetMean(), h->GetRMS(), 1.);
        	// CBFuncAsymm->SetParameters(0.9, 4.3, 1.3, 4.3, h->GetMean(), h->GetRMS(), 1.);
        	h->Fit("CBFuncAsymm");
        	fOut->cd();
        	h->Write(); 
	        //CBFuncAsymm->SetName("fit_"+name);
		//CBFuncAsymm->Write();
    	}
	
}

