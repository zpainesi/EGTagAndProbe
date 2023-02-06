/////////////////////////////////////////////////////////
// This class has been automatically generated on
// Wed Sep 22 19:52:45 2021 by ROOT version 6.24/02
// from TTree L1UpgradeTree/L1UpgradeTree
// found on file: L1Ntuple_5.root
//////////////////////////////////////////////////////////

#ifndef L1EGRatePlotter_h
#define L1EGRatePlotter_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TH1D.h>
#include <TH1F.h>

#include <vector>
#include <map>

// Header file for the classes stored in the TTree if any.

class L1EGRatePlotter {
public :
   TChain         *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   TChain         *fChainForPU;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrentForPU; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
 //L1Analysis::L1AnalysisL1UpgradeDataFormat *L1Upgrade;
   UShort_t              nEGs;
   std::vector<float>   egEt;
   std::vector<float>   egEta;
   std::vector<float>   egPhi;
   std::vector<short>   egIso;
   std::vector<short>   egBx;
   std::vector<short>   egNTT;
  
  Int_t           nPV_True,pvMax,pvMin,run,lumi;
  Int_t           nTT,bunches,doPVfilter,maxEvents;
  Float_t lumiScaleFactor;
  std::string treeName,treeNamePU;
  Float_t       sumEventWeight;
  
  TBranch *number_EGsBranch=0;
  TBranch *EG_ETBranch=0;
  TBranch *EG_ETABranch=0;
  TBranch *EG_PHIBranch=0;
  TBranch *EG_BXBranch=0;
  TBranch *EG_ISOBranch=0;


  L1EGRatePlotter(std::string& inutFileName);
   ~L1EGRatePlotter();
  void accessTree(std::string & input_filelist);
  void readParameters(const std::string jfile);

  void readTree();
  void loop();

  void bookHistograms();
  void scaleHistograms();
  void saveHistograms();
  
  void tokenize(const std::string& str, std::vector<std::string>& tokens, const std::string& delimiters);

  TFile* outputFile_;
  
  Long64_t nEntries_;
  TH1F* puProfileTrue;
  TH1F* nttProfileTrue;
  TH1F* puProfile;
  TH1F* nttProfile;
  std::map<std::string, TH1F*> histoMap_;
  std::string ntupleFileName_;
  std::string outputFileName_;
  int etMax_;
  Long64_t reportEvery_;
  bool bookedHistograms_;
  std::vector<std::string>  histoLabelVec_;  
};
  
#endif

