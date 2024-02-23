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
#include <vector>
#include "Efficiency.h"
#include "TDirectory.h"
#include "TGraphAsymmErrors.h"
#include "TLorentzVector.h"
#include <iomanip>
#include <chrono>

#include "Resolution.h"

using namespace std;

TString getFloatAsTstring(Float_t val,Int_t nDes=2)
{

    std::stringstream ss;
    ss << std::fixed << setprecision(nDes) << val;
    std::string mystring = ss.str();

    return TString(mystring.c_str());

}
void resolution_production(int mode=0)
{
/*
    //produceResolution(infile,ofileName,prefix,maxEvents);
  if(mode==0)
  produceResolution("/grid_mnt/t3storage3/athachay/l1egamma/data/run3Data/TagAndProbe_DataRun3_2022C_unpacked_jul29.root",
                    "resolution.root",
                    "TagAndProbe_DataRun3_2022C_unpacked_jul29_",
                    false,
                   -1e4);
  if(mode==1)
  produceResolution("/grid_mnt/t3storage3/athachay/l1egamma/data/2018/dataEGamma_120X_Run2018A-PromptReco-v3_TP.root",
                    "resolution.root",
                    "data2018_default_",
                    false,
                   1e6);

 */

}

struct dataConfig{
    
    std::string infile;
    std::vector<Int_t> RunNumbers;
    std::string ofileName;
    TString treeName;
    std::string prefix;
    bool doEmulationBranches;
    bool doAllRuns;
    Int_t reportEvery;
    Long64_t maxEvents;
    
    dataConfig(){
        infile ="";
        ofileName="resolutions.root";
        treeName="Ntuplizer/TagAndProbe";
        prefix="";
        doEmulationBranches=false;
        doAllRuns=false;
        Int_t reportEvery(500);
        Long64_t maxEvents(-1);
    }
};


void tokenize(const std::string& str, std::vector<std::string>& tokens, const std::string& delimiters) {
  std::string::size_type lastPos = str.find_first_not_of(delimiters, 0);
  std::string::size_type pos = str.find_first_of(delimiters, lastPos);
  while (std::string::npos != pos || std::string::npos != lastPos)  {
    tokens.push_back(str.substr(lastPos, pos - lastPos));
    lastPos = str.find_first_not_of(delimiters, pos);
    pos = str.find_first_of(delimiters, lastPos);
  }
}



void readParameters(const std::string jfile , struct dataConfig & cfg) {
  std::cout<<"Reading Config file  : " << jfile << std::endl;
  std::ifstream jobcardFile(jfile.c_str());
  if (!jobcardFile) {
    std::cerr << "Input Config File: " << jfile << " could not be opened!" << std::endl;
    return;
  }

  std::string line;

  if(jobcardFile.is_open()) {

    while(std::getline(jobcardFile,line)) {

      if (line.substr(0,1) == "#" || line.substr(0,2) == "//") continue;

      std::vector<std::string> tokens;
      tokenize(line,tokens,"=");
      std::cout << tokens[0] << ":" << tokens[1] << std::endl;
      std::string key   = tokens.at(0);
      std::string value = tokens.at(1); 
      if(key=="infile"){
        cfg.infile= value;
      }
      //else if (key=="ofileName")	    {
      //          cfg.ofileName= value;
      //}
      else if (key=="treeName")	    {
                cfg.treeName = value;
      }
      else if (key=="prefix")	    {
                cfg.prefix= value;
      }
      else if (key=="RunNumbers")
	  {
	    std::string tmp_string = value;
	    std::vector<std::string> tmp_vec;
	    tokenize(tmp_string,tmp_vec,",");
	    for (auto it : tmp_vec) {
	      cfg.RunNumbers.push_back(std::atoi(it.c_str()));
	    }	  
	  }         
      else if (key=="doAllRuns")	    {
                cfg.doAllRuns= bool(atoi(value.c_str()));
      }
      else if (key=="doEmulationBranches")	    {
                cfg.doEmulationBranches= bool(atoi(value.c_str()));
      }
      else if (key=="reportEvery")	    {
                cfg.reportEvery= atoi(value.c_str());
      }
      else if (key=="MaxEntries")	    {
                cfg.maxEvents= atoi(value.c_str());
      }
      else 
	std::cout << " unknown option " << " key " << key << std::endl;
    }

  }
  jobcardFile.close();
}  






void addEtaBinsForResolution( std::map<TString,TH1F * > &resolutionMapOnEta)
{

    TString trPrefixA="L1EtResolutionInEta";
    std::vector<float> etaEdge;
    float eT=-3.2;
    for(Int_t i=0;i<33;i++)
    {
        eT+=0.2;
        etaEdge.push_back(eT);
    }
    for(Int_t ii=0;ii<etaEdge.size()-1;ii++)
    {   
           auto name=trPrefixA+getFloatAsTstring(etaEdge[ii])+"To"+getFloatAsTstring(etaEdge[ii+1]); 
           resolutionMapOnEta[ name ] = new TH1F(name,name,150,0.0,3.0);            
    }

}
void fillEtaBinsForResolution( std::map<TString,TH1F * > &resolutionMapOnEta,Float_t eta,Float_t resolution)
{

    TString trPrefixA="L1EtResolutionInEta";
    std::vector<float> etaEdge;
    float eT=-3.2;
    for(Int_t i=0;i<33;i++)
    {
        eT+=0.2;
        etaEdge.push_back(eT);
    }
    //std::cout<<"Eta = "<<eta<<"\n";
    for(Int_t ii=0;ii<etaEdge.size()-1;ii++)
    {   
        //std::cout<<"  "<<ii<<"  chk : "<<etaEdge[ii]<<" , "<<etaEdge[ii+1]<<"\n";
        if(eta > etaEdge[ii+1])
            continue;   
 //       std::cout<<"\t"<<eta<<"\t Got the window !!"<<etaEdge[ii]<<" , "<<etaEdge[ii+1]<<" --> "<<resolution<<" \n";
        auto name=trPrefixA+getFloatAsTstring(etaEdge[ii])+"To"+getFloatAsTstring(etaEdge[ii+1]); 
        resolutionMapOnEta[ name ]->Fill(resolution);            
        break;
    }

}

void produceResolution(string infile,string ofileName, TString treeName,string prefix="",bool doEmulation=false, bool doAllRuns=false ,std::vector<Int_t> RunNumbers = std::vector<Int_t>(),Long64_t maxEvents=100000, Int_t reportEvery=100)
{
	// gROOT->LoadMacro("/afs/cern.ch/work/c/ckoraka/codes/setTDRStyle_teliko.C");
	// setTDRStyle_teliko();
    
    TFile *f2=new TFile(infile.c_str());
	TTree *t1=(TTree*)(f2->Get(treeName));
    if(doEmulation)
    {
        ofileName = "reEmulated_"+ofileName;
    }
    else{
        ofileName = "unpacked_"+ofileName;
    }
    TFile *ofile =new TFile((prefix+ofileName).c_str(),"RECREATE");

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

        Int_t RunNumber     ;

	/////////////////////////////////////////////////////////////////////////////////////////////////////////	
    
    std::map<Int_t , std::map<string,resolutionMeasurement * > > resolutionMap;
    std::map<Int_t , std::map<string,TH2D * > > occupancyMap;

    const Int_t PtBINS = 14; 
	Double_t PtEdges[PtBINS + 1] = {5., 10., 14., 18., 20., 23., 26., 30., 40., 50., 60., 70., 80., 90., 100.};
    const Int_t dPtBINS = 400; 
	Double_t dPtEdges[dPtBINS + 1];
    for(int i=0;i<=dPtBINS;i++) dPtEdges[i] = -0.005 + 0.01*i ;
	
    const Int_t dPtHighResBINS = 4000; 
	Double_t dPtHighResEdges[dPtHighResBINS + 1];
    for(int i=0;i<=dPtHighResBINS;i++) dPtHighResEdges[i] = ( 0.0 - 2.0/dPtHighResBINS ) + (4.0/dPtHighResBINS)*i ;
	
    const Int_t EtaBINS = 23; 
	Double_t EtaEdges[EtaBINS + 1] = {0.,0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0, 1.1, 1.2, 1.305, 1.479, 1.6, 1.7, 1.8, 1.9, 2.0, 2.1, 2.2, 2.3, 2.4};
    const Int_t dEtaBINS = 200; 
	Double_t dEtaEdges[dEtaBINS + 1];
    for(int i=0;i<=dEtaBINS;i++) dEtaEdges[i] = -1.005 + 0.01*i ;
	
    const Int_t PhiBINS = 68; 
	Double_t PhiEdges[PhiBINS + 1] ;
    for(int i=0;i<=PhiBINS;i++) PhiEdges[i] = -3.4 + 0.1*i ;
    const Int_t dPhiBINS = 200; 
	Double_t dPhiEdges[dPhiBINS + 1];
    for(int i=0;i<=dPhiBINS;i++) dPhiEdges[i] = -1.005 + 0.01*i ;
    
    auto mass= new TH1F("mass","mass",200.0,0.5,199.5);
    auto pT  = new TH1F("pT","pT",200.0,0.5,199.5);
    TLorentzVector eA,eB;

    std::map<Int_t , std::map<TString,TH1F * > >resolutionMapOnEta;


    for( auto idx : RunNumbers) {
        if( doAllRuns)
            idx=0;
        
        TString rnStr("");
        rnStr+=idx;
        ofile->mkdir( rnStr );

        resolutionMap[idx] =  std::map<string,resolutionMeasurement * > () ;
        resolutionMapOnEta[idx]      =  std::map<TString,TH1F * > () ;
        occupancyMap[idx]  =   std::map<string,TH2D * > () ;
        std::cout<<"Making efficiencies for Run Number "<<idx<<"\n"; 
      
        addEtaBinsForResolution( resolutionMapOnEta[idx] );
        resolutionMap[idx]["PFEtVsSCEtovePFEt"    ] = new resolutionMeasurement("PFEtVsSCEtovePFEt",PtBINS,PtEdges,dPtBINS,dPtEdges);

        resolutionMap[idx]["EtaVsdEta"] = new resolutionMeasurement("EtaVsdEta",EtaBINS,EtaEdges,dEtaBINS,dEtaEdges);
        resolutionMap[idx]["PhiVsdPhi"] = new resolutionMeasurement("PhiVsdPhi",PhiBINS,PhiEdges,dPhiBINS,dPhiEdges);
        
        resolutionMap[idx]["EtVsdEt"  ] = new resolutionMeasurement("EtVsdEt",PtBINS,PtEdges,dPtBINS,dPtEdges);
        resolutionMap[idx]["EtaVsdEt"  ] = new resolutionMeasurement("EtaVsdEt",EtaBINS,EtaEdges,dPtBINS,dPtEdges);
        resolutionMap[idx]["EtVsdEt_highReso"  ] = new resolutionMeasurement("EtVsdEt_highReso",PtBINS,PtEdges,dPtHighResBINS,dPtHighResEdges);
        resolutionMap[idx]["EtaVsdEt_highReso"  ] = new resolutionMeasurement("EtaVsdEt_highReso",EtaBINS,EtaEdges,dPtHighResBINS,dPtHighResEdges);

        resolutionMap[idx]["Pt5To15EtVsdEt"  ] = new resolutionMeasurement("Pt5To15EtVsdEt",PtBINS,PtEdges,dPtBINS,dPtEdges);
        resolutionMap[idx]["Pt5To15EtaVsdEt"  ] = new resolutionMeasurement("Pt5To15VsdEt",EtaBINS,EtaEdges,dPtBINS,dPtEdges);
        resolutionMap[idx]["Pt5To15EtVsdEt_highReso"  ] = new resolutionMeasurement("Pt5To15EtVsdEt_highReso",PtBINS,PtEdges,dPtHighResBINS,dPtHighResEdges);
        resolutionMap[idx]["Pt5To15EtaVsdEt_highReso"  ] = new resolutionMeasurement("Pt5To15EtaVsdEt_highReso",EtaBINS,EtaEdges,dPtHighResBINS,dPtHighResEdges);
        
        resolutionMap[idx]["PtGThan20EtVsdEt"  ] = new resolutionMeasurement("PtGThan20EtVsdEt",PtBINS,PtEdges,dPtBINS,dPtEdges);
        resolutionMap[idx]["PtGThan20EtaVsdEt"  ] = new resolutionMeasurement("PtGThan20EtaVsdEt",EtaBINS,EtaEdges,dPtBINS,dPtEdges);
        resolutionMap[idx]["PtGThan20EtVsdEt_highReso"  ] = new resolutionMeasurement("PtGThan20EtVsdEt_highReso",PtBINS,PtEdges,dPtHighResBINS,dPtHighResEdges);
        resolutionMap[idx]["PtGThan20EtaVsdEt_highReso"  ] = new resolutionMeasurement("PtGThan20EtaVsdEt_highReso",EtaBINS,EtaEdges,dPtHighResBINS,dPtHighResEdges);

        resolutionMap[idx]["Pt0To20EtVsdEt"  ] = new resolutionMeasurement("Pt0To20EtVsdEt",PtBINS,PtEdges,dPtBINS,dPtEdges);
        resolutionMap[idx]["Pt0To20EtaVsdEt"  ] = new resolutionMeasurement("Pt0To20EtaVsdEt",EtaBINS,EtaEdges,dPtBINS,dPtEdges);
        resolutionMap[idx]["Pt0To20EtVsdEt_highReso"  ] = new resolutionMeasurement("Pt0To20EtVsdEt_highReso",PtBINS,PtEdges,dPtHighResBINS,dPtHighResEdges);
        resolutionMap[idx]["Pt0To20EtaVsdEt_highReso"  ] = new resolutionMeasurement("Pt0To20EtaVsdEt_highReso",EtaBINS,EtaEdges,dPtHighResBINS,dPtHighResEdges);

        resolutionMap[idx]["Pt20To40EtVsdEt"  ] = new resolutionMeasurement("Pt20To40EtVsdEt",PtBINS,PtEdges,dPtBINS,dPtEdges);
        resolutionMap[idx]["Pt20To40EtaVsdEt"  ] = new resolutionMeasurement("Pt20To40EtaVsdEt",EtaBINS,EtaEdges,dPtBINS,dPtEdges);
        resolutionMap[idx]["Pt20To40EtVsdEt_highReso"  ] = new resolutionMeasurement("Pt20To40EtVsdEt_highReso",PtBINS,PtEdges,dPtHighResBINS,dPtHighResEdges);
        resolutionMap[idx]["Pt20To40EtaVsdEt_highReso"  ] = new resolutionMeasurement("Pt20To40EtaVsdEt_highReso",EtaBINS,EtaEdges,dPtHighResBINS,dPtHighResEdges);

        resolutionMap[idx]["Pt40To60EtVsdEt"  ] = new resolutionMeasurement("Pt40To60EtVsdEt",PtBINS,PtEdges,dPtBINS,dPtEdges);
        resolutionMap[idx]["Pt40To60EtaVsdEt"  ] = new resolutionMeasurement("Pt40To60EtaVsdEt",EtaBINS,EtaEdges,dPtBINS,dPtEdges);
        resolutionMap[idx]["Pt40To60EtVsdEt_highReso"  ] = new resolutionMeasurement("Pt40To60EtVsdEt_highReso",PtBINS,PtEdges,dPtHighResBINS,dPtHighResEdges);
        resolutionMap[idx]["Pt40To60EtaVsdEt_highReso"  ] = new resolutionMeasurement("Pt40To60EtaVsdEt_highReso",EtaBINS,EtaEdges,dPtHighResBINS,dPtHighResEdges);

        resolutionMap[idx]["Pt60To80EtVsdEt"  ] = new resolutionMeasurement("Pt60To80EtVsdEt",PtBINS,PtEdges,dPtBINS,dPtEdges);
        resolutionMap[idx]["Pt60To80EtaVsdEt"  ] = new resolutionMeasurement("Pt60To80EtaVsdEt",EtaBINS,EtaEdges,dPtBINS,dPtEdges);
        resolutionMap[idx]["Pt60To80EtVsdEt_highReso"  ] = new resolutionMeasurement("Pt60To80EtVsdEt_highReso",PtBINS,PtEdges,dPtHighResBINS,dPtHighResEdges);
        resolutionMap[idx]["Pt60To80EtaVsdEt_highReso"  ] = new resolutionMeasurement("Pt60To80EtaVsdEt_highReso",EtaBINS,EtaEdges,dPtHighResBINS,dPtHighResEdges);

        resolutionMap[idx]["Pt80To120EtVsdEt"  ] = new resolutionMeasurement("Pt80To120EtVsdEt",PtBINS,PtEdges,dPtBINS,dPtEdges);
        resolutionMap[idx]["Pt80To120EtaVsdEt"  ] = new resolutionMeasurement("Pt80To120EtaVsdEt",EtaBINS,EtaEdges,dPtBINS,dPtEdges);
        resolutionMap[idx]["Pt80To120EtVsdEt_highReso"  ] = new resolutionMeasurement("Pt80To120EtVsdEt_highReso",PtBINS,PtEdges,dPtHighResBINS,dPtHighResEdges);
        resolutionMap[idx]["Pt80To120EtaVsdEt_highReso"  ] = new resolutionMeasurement("Pt80To120EtaVsdEt_highReso",EtaBINS,EtaEdges,dPtHighResBINS,dPtHighResEdges);

        resolutionMap[idx]["Pt120To200EtVsdEt"  ] = new resolutionMeasurement("Pt120To200EtVsdEt",PtBINS,PtEdges,dPtBINS,dPtEdges);
        resolutionMap[idx]["Pt120To200EtaVsdEt"  ] = new resolutionMeasurement("Pt120To200EtaVsdEt",EtaBINS,EtaEdges,dPtBINS,dPtEdges);
        resolutionMap[idx]["Pt120To200EtVsdEt_highReso"  ] = new resolutionMeasurement("Pt120To200EtVsdEt_highReso",PtBINS,PtEdges,dPtHighResBINS,dPtHighResEdges);
        resolutionMap[idx]["Pt120To200EtaVsdEt_highReso"  ] = new resolutionMeasurement("Pt120To200EtaVsdEt_highReso",EtaBINS,EtaEdges,dPtHighResBINS,dPtHighResEdges);

        resolutionMap[idx]["Pt200ToXEtVsdEt"  ] = new resolutionMeasurement("Pt200ToXEtVsdEt",PtBINS,PtEdges,dPtBINS,dPtEdges);
        resolutionMap[idx]["Pt200ToXEtaVsdEt"  ] = new resolutionMeasurement("Pt200ToXEtaVsdEt",EtaBINS,EtaEdges,dPtBINS,dPtEdges);
        resolutionMap[idx]["Pt200ToXEtVsdEt_highReso"  ] = new resolutionMeasurement("Pt200ToXEtVsdEt_highReso",PtBINS,PtEdges,dPtHighResBINS,dPtHighResEdges);
        resolutionMap[idx]["Pt200ToXEtaVsdEt_highReso"  ] = new resolutionMeasurement("Pt200ToXEtaVsdEt_highReso",EtaBINS,EtaEdges,dPtHighResBINS,dPtHighResEdges);

        occupancyMap[idx]["ocuppancyInclusive"  ] = new TH2D("ocuppancyInclusive","ocuppancyInclusive",50,-2.5,2.5,64,-3.2,3.2);
        occupancyMap[idx]["ocuppancy0To20"  ]     = new TH2D("ocuppancy20To40"  ,"ocuppancy20To40"  ,50,-2.5,2.5,64,-3.2,3.2);
        occupancyMap[idx]["ocuppancy20To40"  ]    = new TH2D("ocuppancy20To40"  ,"ocuppancy20To40"  ,50,-2.5,2.5,64,-3.2,3.2);
        occupancyMap[idx]["ocuppancy40To60"  ]    = new TH2D("ocuppancy40To60"  ,"ocuppancy40To60"  ,50,-2.5,2.5,64,-3.2,3.2);
        occupancyMap[idx]["ocuppancy60To80"  ]    = new TH2D("ocuppancy60To80"  ,"ocuppancy60To80"  ,50,-2.5,2.5,64,-3.2,3.2);
        occupancyMap[idx]["ocuppancy80To120"  ]   = new TH2D("ocuppancy80To120" ,"ocuppancy80To120" ,50,-2.5,2.5,64,-3.2,3.2);
        occupancyMap[idx]["ocuppancy120To200"  ]  = new TH2D("ocuppancy120To200","ocuppancy120To200",50,-2.5,2.5,64,-3.2,3.2);
        occupancyMap[idx]["ocuppancy200ToX"  ]    = new TH2D("ocuppancy200ToX"  ,"ocuppancy200ToX"  ,50,-2.5,2.5,64,-3.2,3.2);

        if(doAllRuns)
            break;
    }
    //res_vs_et_barrel->GetYaxis()->SetTitle("FWHM / (E_{T}^{e#gamma, L1}/E_{T}^{e#gamma, offline})_{at maximum}");
	//res_vs_et_barrel->GetXaxis()->SetTitle("E_{T}^{e#gamma, offline} [GeV]");
    //res_vs_eta_inclusive->GetXaxis()->SetTitle("|#eta^{e#gamma, offline}|");
	//res_vs_eta_inclusive->GetYaxis()->SetTitle("FWHM / (E_{T}^{e#gamma, L1}/E_{T}^{e#gamma, offline})_{at maximum}");

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    t1->SetBranchAddress("eleProbePt",&eleProbePt);
    t1->SetBranchAddress("eleProbeEta",&eleProbeEta);
    t1->SetBranchAddress("eleProbePhi",&eleProbePhi);
    t1->SetBranchAddress("eleProbeSclEt",&eleProbeSclEt);
    t1->SetBranchAddress("eleTagPt",&eleTagPt);
    t1->SetBranchAddress("eleTagEta",&eleTagEta);
    t1->SetBranchAddress("eleTagPhi",&eleTagPhi);
    t1->SetBranchAddress("RunNumber",&RunNumber	);
    //t1->SetBranchAddress("hltPt",&hltPt);
    //t1->SetBranchAddress("hltEta",&hltEta);
    //t1->SetBranchAddress("hltPhi",&hltPhi);
    Int_t l1tEmuRawEt(100);
    if( doEmulation)
    {
        std::cout<<"Setting emulation branches ! \n";
        t1->SetBranchAddress("l1tEmuRawEt",&l1tEmuRawEt);
        t1->SetBranchAddress("l1tEmuPt",&l1tPt);
        t1->SetBranchAddress("l1tEmuEta",&l1tEta);
        t1->SetBranchAddress("l1tEmuPhi",&l1tPhi);
    }
    else
    {
        t1->SetBranchAddress("l1tPt",&l1tPt);
        t1->SetBranchAddress("l1tEta",&l1tEta);
        t1->SetBranchAddress("l1tPhi",&l1tPhi);
    }
    t1->SetBranchAddress("isProbeLoose",&isProbeLoose);
    t1->SetBranchAddress("Nvtx",&Nvtx);
    
    int nentries = t1->GetEntries();

    std::cout<<" Total number events available for processing : "<<nentries<<"\n";
    if(maxEvents>0) nentries= nentries > maxEvents ? maxEvents : nentries;
    std::cout<<" Total number events scheduled for processing : "<<nentries<<"\n";
	int bar=0;
    int end=0;
	int incl=0;
    
    auto t_start = std::chrono::high_resolution_clock::now();
    auto t_end = std::chrono::high_resolution_clock::now();

    Float_t avgPU=0.0;
    Float_t total=0.0;
    int k=0;
    int nProcessedEvents(0);
    
    for (int jentry=0; jentry<nentries; jentry++){
        t1->GetEntry(jentry);
       if(l1tEmuRawEt < 0 ){   k++;   continue ;  }
       if(l1tPt < 0.0 ){   continue ;  }
       // if(Nvtx >52) continue;   
        
        // For PF Et
        eleProbeSclEt=eleProbePt;
        if(jentry%reportEvery==0)
        {
             t_end = std::chrono::high_resolution_clock::now();
             cout<<"Processing Entry "<<jentry<<" / "<<nentries<<"  [ "<<100.0*jentry/nentries<<"  % ]  "
             << " Elapsed time : "<< std::chrono::duration<double, std::milli>(t_end-t_start).count()/1000.0
             <<"  Estimated time left : "<< std::chrono::duration<double, std::milli>(t_end-t_start).count()*( nentries - jentry)/jentry * 0.001
             <<" [ np : "<< nProcessedEvents<<" ] "
             <<endl;
        }
 
       //if(not doAllRuns)
       //{
       if( doAllRuns)
       {
            RunNumber=0;
       }
       else if( std::find(RunNumbers.begin(),RunNumbers.end(),RunNumber)  == RunNumbers.end() )
           {
                continue;
           }
       //}
        
        eA.SetPtEtaPhiM(eleProbePt,eleProbeEta,eleProbePhi,0.5e-3);
        eB.SetPtEtaPhiM(eleTagPt,eleTagEta,eleTagPhi,0.5e-3);
        //std::cout<<"Ea , eb "<<eA.Pt()<<" , "<<eB.Pt()<<" Mass  : "<<(eA+eB).M()<<"\n";
		
        nProcessedEvents++;

        if(isProbeLoose==1 and   (fabs(eleProbeEta) < 2.4) ){
        pT->Fill(eleProbeSclEt);
        mass->Fill((eA+eB).M());
		avgPU+=Nvtx;
        total+=1;
          
          
            resolutionMap[RunNumber]["PFEtVsSCEtovePFEt"  ]->Fill(  eleProbeEta,eleProbePt,eleProbeSclEt/eleProbePt  );
            fillEtaBinsForResolution(resolutionMapOnEta[RunNumber],eleProbeEta,l1tPt/eleProbeSclEt);

            resolutionMap[RunNumber]["EtaVsdEta"] ->Fill(eleProbeEta,abs(eleProbeEta),l1tEta -  eleProbeEta);
            resolutionMap[RunNumber]["PhiVsdPhi"] ->Fill(eleProbeEta,eleProbePhi,l1tPhi-eleProbePhi);
            resolutionMap[RunNumber]["EtaVsdEt"]  ->Fill(eleProbeEta,abs(eleProbeEta),l1tPt/eleProbeSclEt);
            resolutionMap[RunNumber]["EtVsdEt"]   ->Fill(eleProbeEta,eleProbeSclEt,l1tPt/eleProbeSclEt);
            resolutionMap[RunNumber]["EtaVsdEt_highReso"]  ->Fill(eleProbeEta,abs(eleProbeEta),l1tPt/eleProbeSclEt);
            resolutionMap[RunNumber]["EtVsdEt_highReso"]   ->Fill(eleProbeEta,eleProbeSclEt,l1tPt/eleProbeSclEt);
            occupancyMap[RunNumber]["ocuppancyInclusive"]->Fill(l1tEta,l1tPhi);
		 if( ( eleProbeSclEt < 15.0 ) and ( eleProbeSclEt > 5.0 ) )
          {
            resolutionMap[RunNumber]["Pt5To15EtaVsdEt"]  ->Fill(eleProbeEta,abs(eleProbeEta),l1tPt/eleProbeSclEt);
            resolutionMap[RunNumber]["Pt5To15EtVsdEt"]   ->Fill(eleProbeEta,eleProbeSclEt,l1tPt/eleProbeSclEt);
            resolutionMap[RunNumber]["Pt5To15EtaVsdEt_highReso"]  ->Fill(eleProbeEta,abs(eleProbeEta),l1tPt/eleProbeSclEt);
            resolutionMap[RunNumber]["Pt5To15EtVsdEt_highReso"]   ->Fill(eleProbeEta,eleProbeSclEt,l1tPt/eleProbeSclEt);
          } 
       if( eleProbeSclEt > 20.0 )
          {
            resolutionMap[RunNumber]["PtGThan20EtaVsdEt"]  ->Fill(eleProbeEta,abs(eleProbeEta),l1tPt/eleProbeSclEt);
            resolutionMap[RunNumber]["PtGThan20EtVsdEt"]   ->Fill(eleProbeEta,eleProbeSclEt,l1tPt/eleProbeSclEt);
            resolutionMap[RunNumber]["PtGThan20EtaVsdEt_highReso"]  ->Fill(eleProbeEta,abs(eleProbeEta),l1tPt/eleProbeSclEt);
            resolutionMap[RunNumber]["PtGThan20EtVsdEt_highReso"]   ->Fill(eleProbeEta,eleProbeSclEt,l1tPt/eleProbeSclEt);
          }
          if( eleProbeSclEt < 20.0 )
          {
            occupancyMap[RunNumber]["ocuppancy0To20"]->Fill(l1tEta,l1tPhi);
            resolutionMap[RunNumber]["Pt0To20EtaVsdEt"]  ->Fill(eleProbeEta,abs(eleProbeEta),l1tPt/eleProbeSclEt);
            resolutionMap[RunNumber]["Pt0To20EtVsdEt"]   ->Fill(eleProbeEta,eleProbeSclEt,l1tPt/eleProbeSclEt);
            resolutionMap[RunNumber]["Pt0To20EtaVsdEt_highReso"]  ->Fill(eleProbeEta,abs(eleProbeEta),l1tPt/eleProbeSclEt);
            resolutionMap[RunNumber]["Pt0To20EtVsdEt_highReso"]   ->Fill(eleProbeEta,eleProbeSclEt,l1tPt/eleProbeSclEt);
          }
          else if ( eleProbeSclEt < 40.0 )
          {
            occupancyMap[RunNumber]["ocuppancy20To40"]->Fill(l1tEta,l1tPhi);
            resolutionMap[RunNumber]["Pt20To40EtaVsdEt"]  ->Fill(eleProbeEta,abs(eleProbeEta),l1tPt/eleProbeSclEt);
            resolutionMap[RunNumber]["Pt20To40EtVsdEt"]   ->Fill(eleProbeEta,eleProbeSclEt,l1tPt/eleProbeSclEt);
            resolutionMap[RunNumber]["Pt20To40EtaVsdEt_highReso"]  ->Fill(eleProbeEta,abs(eleProbeEta),l1tPt/eleProbeSclEt);
            resolutionMap[RunNumber]["Pt20To40EtVsdEt_highReso"]   ->Fill(eleProbeEta,eleProbeSclEt,l1tPt/eleProbeSclEt);
          }
          else if ( eleProbeSclEt < 60.0 )
          {
            occupancyMap[RunNumber]["ocuppancy40To60"]->Fill(l1tEta,l1tPhi);
            resolutionMap[RunNumber]["Pt40To60EtaVsdEt"]  ->Fill(eleProbeEta,abs(eleProbeEta),l1tPt/eleProbeSclEt);
            resolutionMap[RunNumber]["Pt40To60EtVsdEt"]   ->Fill(eleProbeEta,eleProbeSclEt,l1tPt/eleProbeSclEt);
            resolutionMap[RunNumber]["Pt40To60EtaVsdEt_highReso"]  ->Fill(eleProbeEta,abs(eleProbeEta),l1tPt/eleProbeSclEt);
            resolutionMap[RunNumber]["Pt40To60EtVsdEt_highReso"]   ->Fill(eleProbeEta,eleProbeSclEt,l1tPt/eleProbeSclEt);
          }
          if( eleProbeSclEt < 80.0 )
          {
            occupancyMap[RunNumber]["ocuppancy60To80"]->Fill(l1tEta,l1tPhi);
            resolutionMap[RunNumber]["Pt60To80EtaVsdEt"]  ->Fill(eleProbeEta,abs(eleProbeEta),l1tPt/eleProbeSclEt);
            resolutionMap[RunNumber]["Pt60To80EtVsdEt"]   ->Fill(eleProbeEta,eleProbeSclEt,l1tPt/eleProbeSclEt);
            resolutionMap[RunNumber]["Pt60To80EtaVsdEt_highReso"]  ->Fill(eleProbeEta,abs(eleProbeEta),l1tPt/eleProbeSclEt);
            resolutionMap[RunNumber]["Pt60To80EtVsdEt_highReso"]   ->Fill(eleProbeEta,eleProbeSclEt,l1tPt/eleProbeSclEt);
          }
          else if( eleProbeSclEt < 120.0 )
          {
            occupancyMap[RunNumber]["ocuppancy80To120"]->Fill(l1tEta,l1tPhi);
            resolutionMap[RunNumber]["Pt80To120EtaVsdEt"]  ->Fill(eleProbeEta,abs(eleProbeEta),l1tPt/eleProbeSclEt);
            resolutionMap[RunNumber]["Pt80To120EtVsdEt"]   ->Fill(eleProbeEta,eleProbeSclEt,l1tPt/eleProbeSclEt);
            resolutionMap[RunNumber]["Pt80To120EtaVsdEt_highReso"]  ->Fill(eleProbeEta,abs(eleProbeEta),l1tPt/eleProbeSclEt);
            resolutionMap[RunNumber]["Pt80To120EtVsdEt_highReso"]   ->Fill(eleProbeEta,eleProbeSclEt,l1tPt/eleProbeSclEt);
          }
          else if( eleProbeSclEt < 200.0 )
          {
            occupancyMap[RunNumber]["ocuppancy120To200"]->Fill(l1tEta,l1tPhi);
            resolutionMap[RunNumber]["Pt120To200EtaVsdEt"]  ->Fill(eleProbeEta,abs(eleProbeEta),l1tPt/eleProbeSclEt);
            resolutionMap[RunNumber]["Pt120To200EtVsdEt"]   ->Fill(eleProbeEta,eleProbeSclEt,l1tPt/eleProbeSclEt);
            resolutionMap[RunNumber]["Pt120To200EtaVsdEt_highReso"]  ->Fill(eleProbeEta,abs(eleProbeEta),l1tPt/eleProbeSclEt);
            resolutionMap[RunNumber]["Pt120To200EtVsdEt_highReso"]   ->Fill(eleProbeEta,eleProbeSclEt,l1tPt/eleProbeSclEt);
          }
          else
          {
            occupancyMap[RunNumber]["ocuppancy200ToX"]->Fill(l1tEta,l1tPhi);
            resolutionMap[RunNumber]["Pt200ToXEtaVsdEt"]  ->Fill(eleProbeEta,abs(eleProbeEta),l1tPt/eleProbeSclEt);
            resolutionMap[RunNumber]["Pt200ToXEtVsdEt"]   ->Fill(eleProbeEta,eleProbeSclEt,l1tPt/eleProbeSclEt);
            resolutionMap[RunNumber]["Pt200ToXEtaVsdEt_highReso"]  ->Fill(eleProbeEta,abs(eleProbeEta),l1tPt/eleProbeSclEt);
            resolutionMap[RunNumber]["Pt200ToXEtVsdEt_highReso"]   ->Fill(eleProbeEta,eleProbeSclEt,l1tPt/eleProbeSclEt);
          }
			

		}

     }

     std::cout<<" nProcessedEvents : "<<nProcessedEvents<<"\n";

   for( auto idx : RunNumbers)
    {   
        if(doAllRuns)
            idx=0;

        for (std::map<string,resolutionMeasurement * >::iterator it=resolutionMap[idx].begin() ; it!=resolutionMap[idx].end(); ++it)
	    {
            
            std::cout<<"Processing "<<it->first<<"  \n";
            auto &resolutionItem = *(it->second); 
            resolutionItem.scaleBareVarHists();
            resolutionItem.calculateResolutions();
        }

        if(doAllRuns)
            break;

    }



    pT->Write();
    mass->Write();
    for( auto idx : RunNumbers)
    {
        if(doAllRuns)
            idx=0;

        TString rnStr("");
        rnStr+=idx;
        ofile->cd( rnStr );
        
        if(idx%5==0)
            std::cout<<"  IDX  :   "<<idx<<" \n";

        for (std::map<string,resolutionMeasurement * >::iterator it=resolutionMap[idx].begin() ; it!=resolutionMap[idx].end(); ++it)
	    {
  //          std::cout<<"Writing to file !! [  "<<it->first<<" ] \n";
            auto &resolutionItem = *(it->second); 
            resolutionItem.Write();
        }
    
        for (std::map<string,TH2D * >::iterator it=occupancyMap[idx].begin() ; it!=occupancyMap[idx].end(); ++it)
	    {
 //           std::cout<<"Writing to file !! [  "<<it->first<<" ] \n";
            auto &Item = *(it->second); 
            Item.Write();
        }
    
        for (std::map<TString,TH1F * >::iterator it=resolutionMapOnEta[idx].begin() ; it!=resolutionMapOnEta[idx].end(); ++it)
	    {
            
            auto &Item = *(it->second); 
 //           std::cout<<"Writing to file !! [  "<<it->first<<"  integrl : "<<Item.Integral()<<" ] \n";
            Item.Write();
        }
        if(doAllRuns)
            break;


    }
    std::cout<<"Averate PU : " <<avgPU/total<<"\n";;
    


    ofile->Purge();
    ofile->Close();


}

int  main(int argc , char **argv )
{
    
    dataConfig cfg;
    readParameters( argv[1], cfg);

    std::cout<<" infile : "<<cfg.infile<<"\n";
    std::cout<<" ofile  : "<<cfg.ofileName<<"\n";
    std::cout<<" treeName  : "<<cfg.treeName<<"\n";
    std::cout<<" doEmulationBranches : "<<cfg.doEmulationBranches<<"\n";
    std::cout<<" doAllRuns : "<<cfg.doAllRuns<<"\n";
    std::cout<<" maxEvents : "<<cfg.maxEvents<<"\n";
    std::cout<<" reportEvery : "<<cfg.reportEvery<<"\n";
    std::cout<<" RunNumbers          : ";
    
    for( auto i : cfg.RunNumbers)
    {
            std::cout<<i<<",";
    }

    produceResolution(
                        cfg.infile,
                        cfg.ofileName,
                        cfg.treeName,
                        cfg.prefix,
                        cfg.doEmulationBranches,
                        cfg.doAllRuns,
                        cfg.RunNumbers,
                        cfg.maxEvents,
                        cfg.reportEvery
                     );
}

