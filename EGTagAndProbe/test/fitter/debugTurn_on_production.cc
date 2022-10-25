#include <vector>
#include "Efficiency.h"
#include "Resolution.h"
#include <chrono>

using namespace std;
#define ECAL_BARREL_ETAMAX 1.4

const Double_t TWO_PI(3.141592653589*2) ;
const Double_t PI(3.141592653589)       ;

//#include "secondary_peak.h"
//#include "/afs/cern.ch/work/c/ckoraka/codes/setTDRStyle_teliko.C"
Double_t getDPHI( Double_t phi1, Double_t phi2) ;
Double_t getDETA(Double_t eta1, Double_t eta2);
Double_t getDR( Double_t eta1, Double_t phi1,Double_t eta2 ,Double_t phi2) ;

void produceTurnOns(string infile,string ofileName,string prefix="", bool doEmulationBranches=false,Long64_t maxEvents=100000);
void debugTurn_on_production(int val, int doFull=-1)
{



     if(val==0)
     produceTurnOns("/grid_mnt/t3storage3/athachay/l1egamma/data/2018/dataEGamma_120X_Run2018A-PromptReco-v3_TP.root",
                    "turnon.root",
                    "data2018_",
                    false,
                   -1*doFull*0.5e6);
     if(val==1)
     produceTurnOns("/grid_mnt/t3storage3/athachay/l1egamma/data/run3MC/TagAndProbe_120Xsample_UnpackedV0.root",
     //produceTurnOns("/grid_mnt/t3storage3/athachay/l1egamma/data/run3MC/TagAndProbe_Unpacked_120Xsample_withGen.root",
                    "turnon.root",
                    "run3MC_Recalib_debug_",
                    false,
                    100.0+0.0*doFull*0.5e6);
     if(val==2)
     {
        
       produceTurnOns("TagAndProbe_EtCaliberatedReEmulationV2p5_deg_120Xsample_CaloParams_2022_v0_1.root",
                    "turnon.root",
                    "run3MC_Recalib_debugSC_",
                    false,
                    100+0*1e6);
    }

    return;
     produceTurnOns("/grid_mnt/t3storage3/athachay/l1egamma/data/run3MC/lowPtReg/V1/lowPtRegV1_TandPNtuple.root",
                    "turnon.root",
                    "run3MCLowPtRegressionV1DebugEmul_",
                    true,
                   -1*doFull*0.5e6);
 
     produceTurnOns("/grid_mnt/t3storage3/athachay/l1egamma/data/run3MC/lowPtReg/V2/lowPtRegV2_TandPNtuple.root",
                    "turnon.root",
                    "run3MCLowPtRegressionV2DebugEmul_",
                    true,
                   -1*doFull*0.5e6);
 
     produceTurnOns("/grid_mnt/t3storage3/athachay/l1egamma/data/run3MC/lowPtReg/V3/lowPtRegV3_TandPNtuple.root",
                    "turnon.root",
                    "run3MCLowPtRegressionV3DebugEmul_",
                    true,
                   -1*doFull*0.5e6);

     produceTurnOns("/grid_mnt/t3storage3/athachay/l1egamma/data/run3MC/lowPtReg/V4/lowPtRegV4_TandPNtuple.root",
                    "turnon.root",
                    "run3MCLowPtRegressionV4DebugEmul_",
                    true,
                   -1*doFull*0.5e6);


    
    return ;

    produceTurnOns("/grid_mnt/t3storage3/athachay/l1egamma/triggerPerformance/CMSSW_10_6_25/src/EGTagAndProbe/EGTagAndProbe/test/fitter/haddTmp/MergedFile0.root",
                    "turnon.root",
                    "run3MCLowPtDebugEmul_",
                    true,
                   -1*doFull*0.5e6);


    produceTurnOns("/grid_mnt/t3storage3/athachay/l1egamma/data/run3MC/NTuple_crab_4841files_fromSweta.root",
                    "turnon.root",
                    "run3MCDebugEmul_",
                    true,
                   -1*doFull*0.5e6);
    return;
    produceTurnOns("/grid_mnt/t3storage3/athachay/l1egamma/data/run3MC/NTuple_crab_4841files_fromSweta.root",
                    "turnon.root",
                    "run3MCDebug_",
                    false,
                   -1*doFull*1e6);
}

void produceTurnOns(string infile,string ofileName,string prefix="",bool doEmulationBranches=false,Long64_t maxEvents=100000)
{

	//////		For plotting 		////////////////////////////////////////////////////
	//gROOT->LoadMacro("/afs/cern.ch/work/c/ckoraka/codes/setTDRStyle_teliko.C");
	//setTDRStyle_teliko();

	const Int_t XBINS = 38; 

	Double_t xEdges[XBINS + 1] = {1., 3., 5., 7., 9.,  10., 12., 15., 18., 20., 22., 24., 26., 28., 29., 30., 31., 32., 33., 34., 35., 36., 37., 38., 39., 40., 41., 42., 43., 45., 50., 60., 70., 100., 200., 300., 400., 600., 1000.};
    std::vector<TH1D*> efficHists;

    std::map<string,efficiencyMeasurement * > triggers;
    std::map<string,resolutionMeasurement * > resolutionMap;
    std::map<TString,TH1F*> th1fStore;
    std::map<TString,TH2F*> th2fStore;

    th1fStore["L1PtDistIn0p3"] = new TH1F("L1PtDistIn0p3","L1 EG Object Pt Distribution in #Delta R < 0.3", 250 ,0.0,250.0  );
    th1fStore["L1PtOverOfflinePtDistIn0p3"] = new TH1F("L1PtOverOfflinePtDistIn0p3","EG_{L1}^{Et}/EG_{offline}^{L1} Distribution in #Delta R < 0.3", 2000 ,0.0,40.0 );
    th1fStore["L1EGMultiplicityIn0p3"] = new TH1F("L1EGMultiplicityIn0p3","L1 EG Multiplicity in #Delta R < 0.3", 20 ,-0.5, 19.5  );
    th1fStore["MatchedL1PtDistIn0p3"] = new TH1F("MatchedL1PtDistIn0p3","L1 EG Object Pt Distribution in #Delta R < 0.3", 250 ,0.0,250.0  );
    th1fStore["MatchedL1PtOverOfflinePtDistIn0p3"] = new TH1F("MatchedL1PtOverOfflinePtDistIn0p3","EG_{L1}^{Et}/EG_{offline}^{L1} Distribution in #Delta R < 0.3", 2000 ,0.0,40.0 );
    th1fStore["NtuplizerMatchedProblematicL1PtDistIn0p3"] = new TH1F("NtuplizerMatchedProblematicL1PtDistIn0p3","L1 EG Object Pt Distribution in #Delta R < 0.3", 250 ,0.0,250.0  );
    th1fStore["NtuplizerMatchedProblematicL1PtOverOfflinePtDistIn0p3"] = new TH1F("NtuplizerMatchedProblematicL1PtOverOfflinePtDistIn0p3","EG_{L1}^{Et}/EG_{offline}^{L1} Distribution in #Delta R < 0.3", 2000 ,0.0,40.0 );

    th1fStore["ProblematicL1PtDistIn0p3"] = new TH1F("ProblematicL1PtDistIn0p3","L1 EG Object Pt Distribution in #Delta R < 0.3", 250 ,0.0,250.0  );
    th1fStore["ProblematicL1MatchFoundHere"] = new TH1F("ProblematicL1MatchFoundHere","2= All problem evts , 1 == match found here , 0 == match not found", 5 ,-0.5, 4.5 );
    th1fStore["ProblematicL1PtOverOfflinePtDistIn0p3"] = new TH1F("ProblematicL1PtOverOfflinePtDistIn0p3","EG_{L1}^{Et}/EG_{offline}^{L1} Distribution in #Delta R < 0.3", 2000 ,0.0,40.0 );
    th1fStore["ProblematicEleMultiplicityIn0p3"] = new TH1F("ProblematicEleMultiplicityIn0p3","Electron Multiplicity in #Delta R < 0.3", 20 ,-0.5, 19.5 );
    th1fStore["ProblematicL1EGMultiplicityIn0p5"] = new TH1F("ProblematicL1EGMultiplicityIn0p5","L1 EG Object Multiplicity in #Delta R < 0.5", 20 ,-0.5, 19.5 );
    th1fStore["ProblematicL1EGMultiplicityIn0p3"] = new TH1F("ProblematicL1EGMultiplicityIn0p3","L1 EG Object Multiplicity in #Delta R < 0.3", 20 ,-0.5, 19.5 );
    th1fStore["ProblematicDrL1EGwithMatchedL1EG"] = new TH1F("ProblematicDrL1EGwithMatchedL1EG","L1 EG Object Multiplicity in #Delta R < 0.3", 100 ,0.0, 2.0 );

    th2fStore["ProblematicOfflinePtVsL1PtDistIn0p3"] = new TH2F("ProblematicOfflinePtVsL1PtDistIn0p3","Offlive vs L1 EG Object Pt Distribution in #Delta R < 0.3", 250 ,0.0,250.0,250,0.0,250 );
    th2fStore["ProblematicDrToEleVsL1PtDistIn0p3"] = new TH2F("ProblematicDrToEleVsL1PtDistIn0p3","#Delta R(ele ,L1EG) vs L1 EG Object Pt Distribution in #Delta R < 0.3", 30 ,0.0,30.0,250,0.0,250 );

   triggers["ProblematicL1_barrel_30"] = new efficiencyMeasurement("L1_barrel_30",XBINS,xEdges);
   triggers["ProblematicL1_barrel_looseiso_30"] = new efficiencyMeasurement("L1_barrel_looseiso_30",XBINS,xEdges);
   triggers["ProblematicL1_barrel_tightiso_30"] = new efficiencyMeasurement("L1_barrel_tightiso_30",XBINS,xEdges);
   

   th1fStore["ProblematicGenEleMultiplicityIn0p5"] =new TH1F("ProblematicGenEleMultiplicityIn0p5","GEN Electron Multiplicity in #Delta R < 0.5", 20 ,-0.5, 19.5 );;
   th1fStore["ProblematicGenPhoMultiplicityIn0p5"] =new TH1F("ProblematicGenPhoMultiplicityIn0p5","GEN Photon Object Multiplicity in #Delta R < 0.5", 20 ,-0.5, 19.5 );;
   th1fStore["ProblematicGenEleMother"]= new TH1F("ProblematicGenEleMother","Mother of the probe gen electron", 1000 ,-494.5, 495.5 );


   vector<float>   *allL1tPt(nullptr);
   vector<float>   *allL1tEta(nullptr);
   vector<float>   *allL1tPhi(nullptr);
   vector<float>   *allL1tIso(nullptr);
   vector<float>   *allL1tQual(nullptr);
   TBranch        *b_allL1tPt;   //!
   TBranch        *b_allL1tEta;   //!
   TBranch        *b_allL1tPhi;   //!
   TBranch        *b_allL1tIso;   //!
   TBranch        *b_allL1tQual;   //!

   ///////////////////////////////////////////////////////////////////////////////////////////

	double dR,dphi,phi1,phi2;	
    float eleProbePt	;
    float eleProbeEta	;
    float eleProbePhi	;
    float eleTagEta		;
    float eleTagPhi		;
    float l1tPt 		;
    float l1tEta		;
    float l1tPhi		;
    Float_t genPt       ;
    Float_t genEta       ;
    Float_t genPhi       ;
    Int_t Nvtx			;
    Int_t isProbeLoose	;
    float eleProbeSclEt	;
   
   Int_t           nGenParticle;
   Float_t         genParticlePDGID[5000];   //[nGenParticle]
   Float_t         genParticlePt[5000];   //[nGenParticle]
   Float_t         genParticleEta[1250];   //[nGenParticle]
   Float_t         genParticlePhi[1250];   //[nGenParticle]
   Float_t         genParticleMass[1250];   //[nGenParticle]
   Float_t         genParticleMother[1205];   //[nGenParticle]
   Int_t           nPhotons;
   Float_t         allPhotonPt[7];   //[nPhotons]
   Float_t         allPhotonScEt[7];   //[nPhotons]
   Float_t         allPhotonEta[7];   //[nPhotons]
   Float_t         allPhotonPhi[7];   //[nPhotons]


   TBranch        *b_nGenParticle;   //!
   TBranch        *b_genParticlePDGID;   //!
   TBranch        *b_genParticlePt;   //!
   TBranch        *b_genParticleEta;   //!
   TBranch        *b_genParticlePhi;   //!
   TBranch        *b_genParticleMass;   //!
   TBranch        *b_genParticleMother;   //!

   TBranch        *b_nPhotons;   //!
   TBranch        *b_allPhotonPt;   //!
   TBranch        *b_allPhotonScEt;   //!
   TBranch        *b_allPhotonEta;   //!
   TBranch        *b_allPhotonPhi;   //!

    TFile *f=new TFile(infile.c_str(),"READ");
    TTree *t1=(TTree*)(f->Get("Ntuplizer/TagAndProbe"));
	
    t1->SetBranchAddress("genPt"  ,&genPt			);
    t1->SetBranchAddress("genEta"  ,&genEta			);
    t1->SetBranchAddress("genPhi"  ,&genPhi			);
    t1->SetBranchAddress("eleProbePt"  ,&eleProbePt			);
    t1->SetBranchAddress("eleProbeEta"  ,&eleProbeEta			);
    t1->SetBranchAddress("eleProbePhi"  ,&eleProbePhi			);
    t1->SetBranchAddress("eleTagEta"    ,&eleTagEta		    	);
    t1->SetBranchAddress("eleTagPhi"    ,&eleTagPhi			    );
    t1->SetBranchAddress("Nvtx"         ,&Nvtx			        );
    t1->SetBranchAddress("isProbeLoose" ,&isProbeLoose			);
    t1->SetBranchAddress("eleProbeSclEt",&eleProbeSclEt			);
    t1->SetBranchAddress("allL1tPt"     ,&allL1tPt, &b_allL1tPt);
    t1->SetBranchAddress("allL1tEta"    ,&allL1tEta, &b_allL1tEta);
    t1->SetBranchAddress("allL1tPhi"    ,&allL1tPhi, &b_allL1tPhi);
    t1->SetBranchAddress("allL1tIso"    ,&allL1tIso, &b_allL1tIso);
    t1->SetBranchAddress("allL1tQual"   ,&allL1tQual, &b_allL1tQual);
   
   t1->SetBranchAddress("nGenParticle", &nGenParticle, &b_nGenParticle);
   t1->SetBranchAddress("genParticlePDGID", genParticlePDGID, &b_genParticlePDGID);
   t1->SetBranchAddress("genParticlePt", genParticlePt, &b_genParticlePt);
   t1->SetBranchAddress("genParticleEta", genParticleEta, &b_genParticleEta);
   t1->SetBranchAddress("genParticlePhi", genParticlePhi, &b_genParticlePhi);
   t1->SetBranchAddress("genParticleMass", genParticleMass, &b_genParticleMass);
   t1->SetBranchAddress("genParticleMother", genParticleMother, &b_genParticleMother);
   
   t1->SetBranchAddress("nPhotons", &nPhotons, &b_nPhotons);
   t1->SetBranchAddress("allPhotonPt", allPhotonPt, &b_allPhotonPt);
   t1->SetBranchAddress("allPhotonScEt", allPhotonScEt, &b_allPhotonScEt);
   t1->SetBranchAddress("allPhotonEta", allPhotonEta, &b_allPhotonEta);
   t1->SetBranchAddress("allPhotonPhi", allPhotonPhi, &b_allPhotonPhi);
 
   vector<float>   *allEl_Pt   (nullptr);
   vector<float>   *allEl_Eta  (nullptr);
   vector<float>   *allEl_Phi  (nullptr);
   vector<float>   *allEl_SclEt(nullptr);
   vector<float>   *allEl_Charge(nullptr);
   TBranch        *b_allEl_Pt;   //!
   TBranch        *b_allEl_Eta;   //!
   TBranch        *b_allEl_Phi;   //!
   TBranch        *b_allEl_SclEt;   //!
   TBranch        *b_allEl_Charge;   //!
   t1->SetBranchAddress("allEl_Pt", &allEl_Pt, &b_allEl_Pt);
   t1->SetBranchAddress("allEl_Eta", &allEl_Eta, &b_allEl_Eta);
   t1->SetBranchAddress("allEl_Phi", &allEl_Phi, &b_allEl_Phi);
   t1->SetBranchAddress("allEl_SclEt", &allEl_SclEt, &b_allEl_SclEt);
   t1->SetBranchAddress("allEl_Charge", &allEl_Charge, &b_allEl_Charge);
    string emulationSelection="";
    string emulationSelection2="_";
    if( doEmulationBranches)
    {
        emulationSelection="Emu";
        emulationSelection2="";
        t1->SetBranchAddress("l1tEmuPt" ,&l1tPt);
        t1->SetBranchAddress("l1tEmuEta",&l1tEta);
        t1->SetBranchAddress("l1tEmuPhi",&l1tPhi);
    }
    else
    {
        t1->SetBranchAddress("l1tPt" ,&l1tPt);
        t1->SetBranchAddress("l1tEta",&l1tEta);
        t1->SetBranchAddress("l1tPhi",&l1tPhi);
    }


    int   hasL1_30, hasL1_looseiso_30, hasL1_tightiso_30 ;
    t1->SetBranchAddress(("hasL1"+emulationSelection+"_30"         ).c_str(),&hasL1_30			    );
    t1->SetBranchAddress(("hasL1"+emulationSelection+"_tightiso"+emulationSelection2+"30").c_str(),&hasL1_tightiso_30 );
    t1->SetBranchAddress(("hasL1"+emulationSelection+"_looseiso"+emulationSelection2+"30").c_str(),&hasL1_tightiso_30 );
    
    triggers["L1_m2p5Tom2p0_30"] = new efficiencyMeasurement("L1_m2p5Tom2p0_30",XBINS,xEdges);
    triggers["L1_m2p0Tom1p5_30"] = new efficiencyMeasurement("L1_m2p0Tom1p5_30",XBINS,xEdges);
    triggers["L1_m1p5Tom1p0_30"] = new efficiencyMeasurement("L1_m1p5Tom1p0_30",XBINS,xEdges);
    triggers["L1_m1p0Tom0p5_30"] = new efficiencyMeasurement("L1_m1p0Tom0p5_30",XBINS,xEdges);
    triggers["L1_m0p5Tom0p0_30"] = new efficiencyMeasurement("L1_m0p5Tom0p0_30",XBINS,xEdges);
    
    triggers["L1_2p5To2p0_30"]   = new efficiencyMeasurement("L1_2p5To2p0_30",XBINS,xEdges);
    triggers["L1_2p0To1p5_30"]   = new efficiencyMeasurement("L1_2p0To1p5_30",XBINS,xEdges);
    triggers["L1_1p5To1p0_30"]   = new efficiencyMeasurement("L1_1p5To1p0_30",XBINS,xEdges);
    triggers["L1_1p0To0p5_30"]   = new efficiencyMeasurement("L1_1p0To0p5_30",XBINS,xEdges);
    triggers["L1_0p5To0p0_30"]   = new efficiencyMeasurement("L1_0p5To0p0_30",XBINS,xEdges);
    

    triggers["L1_barrel_30"] = new efficiencyMeasurement("L1_barrel_30",XBINS,xEdges);
    triggers["L1_barrel_looseiso_30"] = new efficiencyMeasurement("L1_barrel_looseiso_30",XBINS,xEdges);
    triggers["L1_barrel_tightiso_30"] = new efficiencyMeasurement("L1_barrel_tightiso_30",XBINS,xEdges);
    triggers["L1_ecap_30"] = new efficiencyMeasurement("L1_ecap_30",XBINS,xEdges);
    triggers["L1_ecap_looseiso_30"] = new efficiencyMeasurement("L1_ecap_looseiso_30",XBINS,xEdges);
    triggers["L1_ecap_tightiso_30"] = new efficiencyMeasurement("L1_ecap_tightiso_30",XBINS,xEdges);
    triggers["L1_30_elePt"] = new efficiencyMeasurement("L1_30_elePt",XBINS,xEdges);
    triggers["L1_30_correctedWithPhotonPt"] = new efficiencyMeasurement("L1_30_correctedWithPhotonPt",XBINS,xEdges);
    triggers["L1_30_correctedWithElectronPt"] = new efficiencyMeasurement("L1_30_correctedWithElectronPt",XBINS,xEdges);
    triggers["L1_30_correctedWithEGammaPt"] = new efficiencyMeasurement("L1_30_correctedWithEGammaPt",XBINS,xEdges);
    triggers["L1_30_correctedV1GenPtInclusive"] = new efficiencyMeasurement("L1_30_correctedV1GenPtInclusive",XBINS,xEdges);
    triggers["L1_30_correctedV1GenPtBarrel"] = new efficiencyMeasurement("L1_30_correctedV1GenPtBarrel",XBINS,xEdges);
    triggers["L1_30_correctedV1GenPtECap"] = new efficiencyMeasurement("L1_30_correctedV1GenPtECap",XBINS,xEdges);
    triggers["L1_30_correctedV2GenPtInclusive"] = new efficiencyMeasurement("L1_30_correctedV2GenPtInclusive",XBINS,xEdges);
    triggers["L1_30_correctedV2GenPtBarrel"] = new efficiencyMeasurement("L1_30_correctedV2GenPtBarrel",XBINS,xEdges);
    triggers["L1_30_correctedV2GenPtECap"] = new efficiencyMeasurement("L1_30_correctedV2GenPtECap",XBINS,xEdges);
    triggers["L1_30_genPtInclusive"] = new efficiencyMeasurement("L1_30_genPtInclusive",XBINS,xEdges);
    triggers["L1_30_genPtBarrel"] = new efficiencyMeasurement("L1_30_genPtBarrel",XBINS,xEdges);
    triggers["L1_30_genPtECap"] = new efficiencyMeasurement("L1_30_genPtECap",XBINS,xEdges);
    triggers["L1_30"] = new efficiencyMeasurement("L1_30",XBINS,xEdges);
    triggers["L1_looseiso_30"] = new efficiencyMeasurement("L1_looseiso_30",XBINS,xEdges);
    triggers["L1_tightiso_30"] = new efficiencyMeasurement("L1_tightiso_30",XBINS,xEdges);
    
    int   hasL1_40, hasL1_looseiso_40, hasL1_tightiso_40 ;
    t1->SetBranchAddress(("hasL1"+emulationSelection+"_40"         ).c_str(),&hasL1_40		    );
    t1->SetBranchAddress(("hasL1"+emulationSelection+"_tightiso"+emulationSelection2+"40").c_str(),&hasL1_tightiso_40 );
    t1->SetBranchAddress(("hasL1"+emulationSelection+"_looseiso"+emulationSelection2+"40").c_str(),&hasL1_tightiso_40 );

    triggers["L1_barrel_40"] = new efficiencyMeasurement("L1_barrel_40",XBINS,xEdges);
    triggers["L1_barrel_looseiso_40"] = new efficiencyMeasurement("L1_barrel_looseiso_40",XBINS,xEdges);
    triggers["L1_barrel_tightiso_40"] = new efficiencyMeasurement("L1_barrel_tightiso_40",XBINS,xEdges);
    triggers["L1_ecap_40"] = new efficiencyMeasurement("L1_ecap_40",XBINS,xEdges);
    triggers["L1_ecap_looseiso_40"] = new efficiencyMeasurement("L1_ecap_looseiso_40",XBINS,xEdges);
    triggers["L1_ecap_tightiso_40"] = new efficiencyMeasurement("L1_ecap_tightiso_40",XBINS,xEdges);
    triggers["L1_40"] = new efficiencyMeasurement("L1_40",XBINS,xEdges);
    triggers["L1_looseiso_40"] = new efficiencyMeasurement("L1_looseiso_40",XBINS,xEdges);
    triggers["L1_tightiso_40"] = new efficiencyMeasurement("L1_tightiso_40",XBINS,xEdges);
    

    const int XBINS_forPt=900;
    Double_t xEdges_forPt[XBINS_forPt+1];
    for(int i=0;i<=XBINS_forPt;i++) xEdges_forPt[i]= 0.0 + i*1.0;
    const int XBINS_forDPt=1000;
    Double_t xEdges_forDPt[XBINS_forDPt+1];
    for(int i=0;i<=XBINS_forDPt;i++) xEdges_forDPt[i]= 0.0 + i*0.05;
    const int XBINS_forInvDPt=512*5;
    Double_t xEdges_forInvDPt[XBINS_forInvDPt+1];
    for(int i=0;i<=XBINS_forInvDPt;i++) xEdges_forInvDPt[i]= 0.0 + i*2.0*5.0/XBINS_forInvDPt;
    std::cout<<" xEdges_forInvDPt "<<xEdges_forInvDPt[0]<<"  , "<<xEdges_forInvDPt[1]<<"\n";

    //xEdges_forDPt[1001] = 10.0;
    //xEdges_forDPt[1002] = 20.0;
    //xEdges_forDPt[1003] = 30.0;
    //xEdges_forDPt[1004] = 40.0;
    //xEdges_forDPt[1005] = 50.0;
    //xEdges_forDPt[1006] = 60.0;
    //xEdges_forDPt[1007] = 70.0;
    //xEdges_forDPt[1008] = 90.0;
    //xEdges_forDPt[1009] = 110.0;
    //xEdges_forDPt[1010] = 120.0;
    //xEdges_forDPt[1011] = 140.0;
    //xEdges_forDPt[1012] = 150.0;
    //xEdges_forDPt[1013] = 150.0;
    
    const int XBINS_forEta=60;
    Double_t xEdges_forEta[XBINS_forEta+1];
    for(int i=0;i<=XBINS_forEta;i++) xEdges_forEta[i]= -3.0 + i*0.1;
    const int XBINS_forPhi=60;
    Double_t xEdges_forPhi[XBINS_forPhi+1];
    for(int i=0;i<=XBINS_forPhi;i++) xEdges_forPhi[i]= -3.5 + i*0.1;
    const int XBINS_forMass=120;

    std::map<string, TH1D > histMap;

    histMap["recoL1_pt"]       = TH1D("recoL1_pt", "recoL1_pt",XBINS_forPt,xEdges_forPt);
    histMap["recoL1_eta"]      = TH1D("recoL1_eta","recoL1_eta",XBINS_forEta,xEdges_forEta);
    histMap["recoL1_phi"]      = TH1D("recoL1_phi","recoL1_phi",XBINS_forPhi,xEdges_forPhi);    
    
    histMap["recoOffline_pt"]  = TH1D("recoOffline_pt", "recoOffline_pt",XBINS_forPt,xEdges_forPt);
    histMap["recoOffline_scEt"]= TH1D("recoOffline_scEt", "recoOffline_scEt",XBINS_forPt,xEdges_forPt);
    histMap["recoOffline_eta"] = TH1D("recoOffline_eta","recoOffline_eta",XBINS_forEta,xEdges_forEta);
    histMap["recoOffline_phi"] = TH1D("recoOffline_phi","recoOffline_phi",XBINS_forPhi,xEdges_forPhi);    
 
    histMap["recoL1_has30_pt"]       = TH1D("recoL1_has30_pt", "recoL1_has30_pt",XBINS_forPt,xEdges_forPt);
    histMap["recoL1_has30_scEt"]     = TH1D("recoL1_has30_scEt", "recoL1_has30_scEt",XBINS_forPt,xEdges_forPt);
    histMap["recoL1_has30_eta"]      = TH1D("recoL1_has30_eta","recoL1_has30_eta",XBINS_forEta,xEdges_forEta);
    histMap["recoL1_has30_phi"]      = TH1D("recoL1_has30_phi","recoL1_has30_phi",XBINS_forPhi,xEdges_forPhi);    
    
    histMap["recoOffline_has30_pt"]  = TH1D("recoOffline_has30_pt", "recoOffline_has30_pt",XBINS_forPt,xEdges_forPt);
    histMap["recoOffline_has30_eta"] = TH1D("recoOffline_has30_eta","recoOffline_has30_eta",XBINS_forEta,xEdges_forEta);
    histMap["recoOffline_has30_phi"] = TH1D("recoOffline_has30_phi","recoOffline_has30_phi",XBINS_forPhi,xEdges_forPhi);    


    std::cout<<" making resolutions !\n";
    resolutionMap["PtVsL1Pt"]  = new resolutionMeasurement("PtVsL1Pt",XBINS_forPt,xEdges_forPt,XBINS_forPt,xEdges_forPt);
    resolutionMap["PtVsdL1Pt"]  = new resolutionMeasurement("PtVsdL1Pt",XBINS_forPt,xEdges_forPt,XBINS_forDPt,xEdges_forDPt);
    resolutionMap["PtVsInvdL1Pt"]  = new resolutionMeasurement("PtVsInvdL1Pt",XBINS_forPt,xEdges_forPt,XBINS_forInvDPt,xEdges_forInvDPt);
    
    resolutionMap["PtVsL1Pt_has30"]  = new resolutionMeasurement("PtVsL1Pt_has30",XBINS_forPt,xEdges_forPt,XBINS_forPt,xEdges_forPt);
    resolutionMap["PtVsdL1Pt_has30"]  = new resolutionMeasurement("PtVsdL1Pt_has30",XBINS_forPt,xEdges_forPt,XBINS_forDPt,xEdges_forDPt);
    resolutionMap["PtVsInvdL1Pt_has30"]  = new resolutionMeasurement("PtVsInvdL1Pt_has30",XBINS_forPt,xEdges_forPt,XBINS_forInvDPt,xEdges_forInvDPt);
    std::cout<<" made resolutions !\n";


	Long64_t nentries = t1->GetEntries();
    std::cout<<" Available total "<<nentries<<" \n";
    if (maxEvents >0 ) nentries = nentries > maxEvents ? maxEvents : nentries;
    std::cout<<" Processing total "<<nentries<<" \n";
	
    auto t_start = std::chrono::high_resolution_clock::now();
    auto t_end = std::chrono::high_resolution_clock::now();
    
    Int_t matchIdx,nMul,nEleMul,matchEleIdx,nMul0p5;
    Float_t dr,drMin;


    for (Long64_t jentry=0; jentry<nentries; jentry++){

       if(jentry%5000 ==0 )
       {
         t_end = std::chrono::high_resolution_clock::now();
         cout<<"Processing Entry "<<jentry<<" / "<<nentries<<"  [ "<<100.0*jentry/nentries<<"  % ]  "
             << " Elapsed time : "<< std::chrono::duration<double, std::milli>(t_end-t_start).count()/1000.0
             <<"  Estimated time left : "<< std::chrono::duration<double, std::milli>(t_end-t_start).count()*( nentries - jentry)/jentry * 0.001
             <<endl;
       }
	
		t1->GetEntry(jentry);  		
        std::cout<<"\n"<<__LINE__<<" jentry : "<<jentry<<" n Photons : "<<nPhotons<<" n Electron : "<<allEl_Pt->size()<<"\n";
        //continue;
        //std::cout<< isProbeLoose<<"==1 && "<<fabs(eleProbeEta)<<" < 2.5"<<"  && "<<sqrt(pow(eleProbeEta-eleTagEta,2)+pow(eleProbePhi-eleTagPhi,2))<<">0.6 "<<"\n";
               if( isProbeLoose==1 && fabs(eleProbeEta) < 2.5  && sqrt(pow(eleProbeEta-eleTagEta,2)+pow(eleProbePhi-eleTagPhi,2))>0.6 ) 
               {
                

                 /*****************************/
                matchIdx=-1;
                nMul=0;
                drMin=0.3;
                nMul0p5=0;
                for(auto i=0; i< allL1tEta->size() ; i++)
                {
                    dr=getDR(eleProbeEta,eleProbePhi, allL1tEta->at(i) , allL1tPhi->at(i));
                    if( dr < 0.5 ) nMul0p5++;
                    if( dr > 0.3 ) continue;
                    if(dr < drMin)
                    {
                        drMin=dr;
                        matchIdx=i;
                    }
                
                    th1fStore["L1PtDistIn0p3"]->Fill( allL1tPt->at(i) ) ;
                    th1fStore["L1PtOverOfflinePtDistIn0p3"]->Fill( allL1tPt->at(i)/eleProbeEta ) ;
                    nMul++;
                    // problematic evemts
                    if( l1tPt > 30.0  and eleProbeSclEt <20.0) {
                        th2fStore["ProblematicOfflinePtVsL1PtDistIn0p3"]->Fill(eleProbeSclEt,allL1tPt->at(i) );
                        th2fStore["ProblematicDrToEleVsL1PtDistIn0p3"]->Fill(dr,allL1tPt->at(i) );
                    }
                }





                if(matchIdx > 0)
                {
                    th1fStore["MatchedL1PtDistIn0p3"]->Fill( allL1tPt->at(matchIdx) ) ;
                    th1fStore["MatchedL1PtOverOfflinePtDistIn0p3"]->Fill( allL1tPt->at(matchIdx)/eleProbeEta ) ;
                }
                
                matchEleIdx=-1;
                nEleMul=0;
                drMin=0.3;
                for(auto i=0; i< allEl_Eta->size() ; i++)
                {
                    dr=getDR( allEl_Eta->at(i), allEl_Phi->at(i), l1tEta , l1tPhi);
                    if(dr > 0.3 ) continue;
                    if(dr < drMin)
                    {
                        drMin=dr;
                        matchEleIdx=i;
                    }
                
                    nEleMul++;
                }                
                // problematic evemts
                auto correctedV1GenPt=genPt;
                auto correctedV2GenPt=genPt;
                TLorentzVector correctedP4(0.0,0.0,0.0,0.0);
                TLorentzVector bremGamma(0.0,0.0,0.0,0.0);
                //if( l1tPt > 30.0  and eleProbeSclEt <20.0 )
                auto pTCut(-1e10);
                //if( l1tPt > 30.0  and eleProbeSclEt <20.0 )
                //if( l1tPt > 30.0  and genPt <20.0 )
                {
                    th1fStore["ProblematicL1MatchFoundHere"]->Fill( 2 ) ;
                    th1fStore["NtuplizerMatchedProblematicL1PtDistIn0p3"]->Fill( l1tPt ) ;
                    th1fStore["NtuplizerMatchedProblematicL1PtOverOfflinePtDistIn0p3"]->Fill( l1tPt/eleProbeEta ) ;
                    if(matchIdx > -1) {
                    th1fStore["ProblematicL1MatchFoundHere"]->Fill( 1 ) ;
                    th1fStore["ProblematicL1PtDistIn0p3"]->Fill( allL1tPt->at(matchIdx) ) ;
                    th1fStore["ProblematicL1PtOverOfflinePtDistIn0p3"]->Fill( allL1tPt->at(matchIdx)/eleProbeEta ) ;
                    th1fStore["ProblematicL1EGMultiplicityIn0p3"]->Fill(nMul);
                    th1fStore["ProblematicL1EGMultiplicityIn0p5"]->Fill(nMul0p5);
                    th1fStore["ProblematicDrL1EGwithMatchedL1EG"]->Fill( getDR(l1tEta, l1tPhi, allL1tEta->at(matchIdx), allL1tPhi->at(matchIdx) ) ) ;
                    }
                    else {
                    th1fStore["ProblematicL1MatchFoundHere"]->Fill( 0 ) ;
                    th1fStore["ProblematicDrL1EGwithMatchedL1EG"]->Fill(  0.9 );
                    }
                    th1fStore["ProblematicEleMultiplicityIn0p3"]->Fill(nEleMul);
                    

                    //////////////////////////////////////////////////////////////////
                    int nPhoGen(0);
                    int nEleGen(0);
                    int eleMother(0);
                    int nOth(0);
                    drMin=0.3;
                    int matchEleIdx(-1);
                    if(genPt < pTCut) std::cout<<"\nProblematic event : "<<jentry<<" || L1 Et : "<<l1tPt<<" with sc pT [ offline ]  = "<<eleProbeSclEt<< " pf Et : "<<eleProbePt<<"\n";
                    float sumPhoPt(0.0);
                    float sumElePt(0.0);
                    float minElePt(genPt);
                    float maxElePt(genPt);
                    for(int jj=0;jj<nGenParticle;jj++)
                    {
                        dr=getDR( genParticleEta[jj] , genParticlePhi[jj], l1tEta , l1tPhi);
                        if(dr > 0.5 ) continue;
                        //std::cout<<"\t\t\t partile detatils : "<<genParticlePDGID[jj]<<" : "<<genParticlePt[jj]<<"[ M : "<<genParticleMother[jj]<<" ] \n ";

                        if( abs(genParticlePDGID[jj]) ==22 ) {
                            if(genPt < pTCut) std::cout<<"\tFound a gen gamma with pT : "<<genParticlePt[jj]<<" and mother  : "<<genParticleMother[jj]<<"\n";
                            sumPhoPt+=genParticlePt[jj];
                            nPhoGen++;
                        }
                        else if( abs(genParticlePDGID[jj]) ==11 ) {
                            if(genPt < pTCut) std::cout<<"\tFound a gen Ele with pT : "<<genParticlePt[jj]<<" [ M : "<<genParticleMother[jj]<<" ] "<<"\n";
                            sumElePt+=genParticlePt[jj];
                            nEleGen++;
                            if( (drMin > dr) and (abs(genParticleMother[jj])==23) )
                            {
                                drMin=dr;
                                eleMother=genParticleMother[jj];
                                matchEleIdx=jj;
                            }
                        }
                        else  nOth++;
                        

                    }
                    th1fStore["ProblematicGenEleMultiplicityIn0p5"]->Fill(nEleGen);
                    th1fStore["ProblematicGenPhoMultiplicityIn0p5"]->Fill(nPhoGen);
                    if(matchEleIdx >=0 and genPt < pTCut ){
                    std::cout<<"  Mother of problematic electron : "<<eleMother
                             <<" dr with match : "<<getDR(genParticleEta[matchEleIdx],genParticlePhi[matchEleIdx],genEta,genPhi) 
                             <<" genMatch pt :  "<<genParticlePt[matchEleIdx]<<" / "<<genPt
                             <<" dGenEle+sumPho : "<<(maxElePt-minElePt)<<" + "<<sumPhoPt<<" -> "<<sumPhoPt+(maxElePt-minElePt)
                             <<"\n";
                    th1fStore["ProblematicGenEleMother"]->Fill(eleMother);
                 }
 
                    if(matchEleIdx >=0 )
                     {
                     }

                
                    correctedV2GenPt=(maxElePt);
                }
                
                Float_t photonSC_pT(0.0);
                //std::cout<<"nPhotons : "<<nPhotons<<"\n";

                bool skip=false;
                if( l1tPt > 30.0  and eleProbeSclEt <20.0 )
                {
                    std::cout<<"Problematic event : "<<jentry<<" || L1 "<<l1tPt<<" ofline Sc : "<<eleProbeSclEt<<"\n";
                }
                cout<<"\n\n";
                for( int ii=0;ii< nPhotons;ii++)
                {   

                         std::cout<<jentry<<" Photon : i "<<ii<<"  , eta : "<<allPhotonEta[ii]<<" , phi : "<<allPhotonPhi[ii]<< " , pt : "<<allPhotonPt[ii]<<"\n";
                        skip=false;
                        //if( getDR(allPhotonEta[ii] , allPhotonPhi[ii] , eleProbeEta,eleProbePhi ) < 0.01 ) 
                        //    continue;
                        for(int jj=0;jj<allEl_Pt->size() ;jj++)
                        {
                            if( (getDR(allPhotonEta[ii] , allPhotonPhi[ii] , allEl_Eta->at(jj), allEl_Phi->at(jj) ) < 0.15)  and( abs(allPhotonPt[ii]-eleProbeSclEt) < 1.0 )  )
                                skip=true;
                            if(skip) break;
                        }
                        if(skip) continue;
                        if(abs(allPhotonPt[ii]-eleProbeSclEt)<0.5 )continue;

                        dr=getDR( allPhotonEta[ii] , allPhotonPhi[ii], l1tEta , l1tPhi);
                        if(dr > 0.5 ) continue;
                        //if( l1tPt > 30.0  and eleProbeSclEt <20.0 )
                        std::cout<<"\t\t Found  Photon Obj : "<<allPhotonPt[ii]<<" pT : scEt "<<allPhotonScEt[ii]<<"[ drL1 = "<<dr<<", "
                                 << "dr ele"<<getDR(allPhotonEta[ii] , allPhotonPhi[ii] , eleProbeEta,eleProbePhi )<<" , "
                                 <<" dSCEt : "<<abs(allPhotonPt[ii]-eleProbeSclEt)<<" ] "
                                 <<" L1Et  : "<<l1tPt<<" "
                                 <<"\n";
                        photonSC_pT+=allPhotonScEt[ii];

                }
                
                Float_t eleSC_ectraPt=0.0;
                for(int ii=0;ii<allEl_Pt->size() ;ii++)
                   {
                       std::cout<<jentry<<" Electron  i = "<<ii<<" eta "<<allEl_Eta->at(ii) <<" , phi : "<<allEl_Phi->at(ii)<< " , pt : "<<allEl_Pt->at(ii) <<"\n"; 
                       if( getDR(  allEl_Eta->at(ii) , allEl_Phi->at(ii) , eleProbeEta, eleProbePhi ) < 0.01 )
                           continue;
                      
                      dr=getDR( allEl_Eta->at(ii) , allEl_Phi->at(ii), l1tEta , l1tPhi);
                      if(dr > 0.5 ) continue;
                      eleSC_ectraPt+=allEl_SclEt->at(ii);
                      if( l1tPt > 30.0  and eleProbeSclEt <20.0 )
                        std::cout<<"\t\t Found  Electron Obj : "<<allEl_SclEt->at(ii)<<" pT : scEt "<<allEl_SclEt->at(ii)<<"[ drL1 = "<<dr<<", "
                                 <<"dr ele : "<<getDR(allEl_Eta->at(ii) , allEl_Phi->at(ii) , eleProbeEta,eleProbePhi )<<" , "
                                 <<" dSCEt : "<<abs(allEl_SclEt->at(ii)-eleProbeSclEt)<<" ] , "
                                 <<" L1Et  : "<<l1tPt<<" "
                                 <<"\n";

                        
                   }
                
                auto correctedWithPhotonPt   = photonSC_pT + eleProbeSclEt ;
                auto correctedWithElectronPt = eleSC_ectraPt + eleProbeSclEt ;
                auto correctedWithEGammaPt   = photonSC_pT + eleSC_ectraPt + eleProbeSclEt ;



                 /*****************************/
 
                 histMap["recoL1_pt"]       .Fill(l1tPt);    
                 histMap["recoL1_eta"]      .Fill(l1tEta);
                 histMap["recoL1_phi"]      .Fill(l1tPhi);

                 histMap["recoOffline_pt"]  .Fill(eleProbePt );
                 histMap["recoOffline_scEt"]  .Fill(eleProbeSclEt );
                 histMap["recoOffline_eta"] .Fill(eleProbeEta);                    
                 histMap["recoOffline_phi"] .Fill(eleProbePhi);

                resolutionMap["PtVsL1Pt"]    ->Fill(eleProbeEta , eleProbeSclEt,l1tPt);
                resolutionMap["PtVsdL1Pt"]   ->Fill(eleProbeEta , eleProbeSclEt,l1tPt/eleProbeSclEt);
                resolutionMap["PtVsInvdL1Pt"]   ->Fill(eleProbeEta , eleProbeSclEt,eleProbeSclEt/l1tPt);
                
                if(hasL1_30)
                    {

                        histMap["recoL1_has30_pt"]       .Fill(l1tPt);    
                        histMap["recoL1_has30_eta"]      .Fill(l1tEta);
                        histMap["recoL1_has30_phi"]      .Fill(l1tPhi);

                        histMap["recoOffline_has30_pt"]  .Fill(eleProbePt );
                        histMap["recoOffline_has30_scEt"]  .Fill(eleProbePt );
                        histMap["recoOffline_has30_eta"] .Fill(eleProbeEta);                    
                        histMap["recoOffline_has30_phi"] .Fill(eleProbePhi);
                
                        resolutionMap["PtVsL1Pt_has30"]    ->Fill(eleProbeEta , eleProbeSclEt,l1tPt);
                        resolutionMap["PtVsdL1Pt_has30"]   ->Fill(eleProbeEta , eleProbeSclEt,l1tPt/eleProbeSclEt);
                        resolutionMap["PtVsInvdL1Pt_has30"]   ->Fill(eleProbeEta , eleProbeSclEt,eleProbeSclEt/l1tPt);
                
                    }

                    triggers["L1_30_correctedWithPhotonPt"]->fill(hasL1_30,correctedWithPhotonPt);
                    triggers["L1_30_correctedWithElectronPt"]->fill(hasL1_30,correctedWithElectronPt);
                    triggers["L1_30_correctedWithEGammaPt"]->fill(hasL1_30,correctedWithEGammaPt);
                    
                    triggers["L1_30_correctedV1GenPtInclusive"]->fill(hasL1_30,correctedV1GenPt);
                    if( abs(eleProbeEta) < 1.305 )   
                        triggers["L1_30_correctedV1GenPtBarrel"]->fill(hasL1_30,correctedV1GenPt);
                    if( abs(eleProbeEta) > 1.479 and abs(eleProbeEta) < 2.40 )   
                        triggers["L1_30_correctedV1GenPtECap"]->fill(hasL1_30,correctedV1GenPt);

                    triggers["L1_30_correctedV2GenPtInclusive"]->fill(hasL1_30,correctedV2GenPt);
                    if( abs(eleProbeEta) < 1.305 )   
                        triggers["L1_30_correctedV2GenPtBarrel"]->fill(hasL1_30,correctedV2GenPt);
                    if( abs(eleProbeEta) > 1.479 and abs(eleProbeEta) < 2.40 )   
                        triggers["L1_30_correctedV2GenPtECap"]->fill(hasL1_30,correctedV2GenPt);

                    triggers["L1_30_genPtInclusive"]->fill(hasL1_30,genPt);
                    if( abs(eleProbeEta) < 1.305 )   
                        triggers["L1_30_genPtBarrel"]->fill(hasL1_30,genPt);
                    if( abs(eleProbeEta) > 1.479 and abs(eleProbeEta) < 2.40 )   
                        triggers["L1_30_genPtECap"]->fill(hasL1_30,genPt);
                    
                        
                    triggers["L1_30_elePt"]->fill(hasL1_30,eleProbePt);
                    triggers["L1_30"]->fill(hasL1_30,eleProbeSclEt);
                    triggers["L1_looseiso_30"]->fill(hasL1_30,eleProbeSclEt);
                    triggers["L1_tightiso_30"]->fill(hasL1_tightiso_30,eleProbeSclEt);

                    triggers["L1_40"]->fill(hasL1_40,eleProbeSclEt);
                    triggers["L1_looseiso_40"]->fill(hasL1_40,eleProbeSclEt);
                    triggers["L1_tightiso_40"]->fill(hasL1_tightiso_40,eleProbeSclEt);
                    
                    if((eleProbeEta < -2.0) and (eleProbeEta > -2.5))
                           triggers["L1_m2p5Tom2p0_30"]->fill(hasL1_30,eleProbeSclEt);
                    if((eleProbeEta < -1.5) and (eleProbeEta > -2.0))
                            triggers["L1_m2p0Tom1p5_30"]->fill(hasL1_30,eleProbeSclEt);
                    if((eleProbeEta < -1.0) and (eleProbeEta > -1.5))
                            triggers["L1_m1p5Tom1p0_30"]->fill(hasL1_30,eleProbeSclEt);
                    if((eleProbeEta < -0.5) and (eleProbeEta > -1.0))
                            triggers["L1_m1p0Tom0p5_30"]->fill(hasL1_30,eleProbeSclEt);
                    if((eleProbeEta < 0.0) and (eleProbeEta > -0.5))
                            triggers["L1_m0p5Tom0p0_30"]->fill(hasL1_30,eleProbeSclEt);
                    
                    if((eleProbeEta < 2.5) and (eleProbeEta > 2.0))
                            triggers["L1_2p5To2p0_30"]->fill(hasL1_30,eleProbeSclEt);
                    if((eleProbeEta < 2.0) and (eleProbeEta > 1.5))
                            triggers["L1_2p0To1p5_30"]->fill(hasL1_30,eleProbeSclEt);
                    if((eleProbeEta < 1.5) and (eleProbeEta > 1.0))
                            triggers["L1_1p5To1p0_30"]->fill(hasL1_30,eleProbeSclEt);
                    if((eleProbeEta < 1.0) and (eleProbeEta > 0.5))
                            triggers["L1_1p0To0p5_30"]->fill(hasL1_30,eleProbeSclEt);
                    if((eleProbeEta < 0.5) and (eleProbeEta > 0.0))
                            triggers["L1_0p5To0p0_30"]->fill(hasL1_30,eleProbeSclEt);

                    if (  fabs(eleProbeEta) < ECAL_BARREL_ETAMAX )
                    {
                    triggers["L1_barrel_30"]->fill(hasL1_30,eleProbeSclEt);
                    triggers["L1_barrel_looseiso_30"]->fill(hasL1_30,eleProbeSclEt);
                    triggers["L1_barrel_tightiso_30"]->fill(hasL1_tightiso_30,eleProbeSclEt);

                    triggers["L1_barrel_40"]->fill(hasL1_40,eleProbeSclEt);
                    triggers["L1_barrel_looseiso_40"]->fill(hasL1_40,eleProbeSclEt);
                    triggers["L1_barrel_tightiso_40"]->fill(hasL1_tightiso_40,eleProbeSclEt);

                    }

                    if (  fabs(eleProbeEta) >= ECAL_BARREL_ETAMAX )
                    {
                        triggers["L1_ecap_30"]->fill(hasL1_30,eleProbeSclEt);
                        triggers["L1_ecap_looseiso_30"]->fill(hasL1_30,eleProbeSclEt);
                        triggers["L1_ecap_tightiso_30"]->fill(hasL1_tightiso_30,eleProbeSclEt);

                        triggers["L1_ecap_40"]->fill(hasL1_40,eleProbeSclEt);
                        triggers["L1_ecap_looseiso_40"]->fill(hasL1_40,eleProbeSclEt);
                        triggers["L1_ecap_tightiso_40"]->fill(hasL1_tightiso_40,eleProbeSclEt);
                    }
                    
                }
               
                
	}
 	

   
    double a  = 0.3173;
	double den,num;

    auto ofile=new TFile((prefix+ofileName).c_str(),"RECREATE");
    ofile->cd();
    
    for (std::map<string,efficiencyMeasurement * >::iterator it=triggers.begin() ; it!=triggers.end(); ++it)
    {
        std::cout<<"Processing Efficiency "<<it->first<<"  \n";
        auto &curTrigger = *(it->second); 
        curTrigger.calculateEfficiency();
        curTrigger.Write();
    }
    
    ofile->Write();
    ofile->cd();

    for (std::map<string,TH1D >::iterator it=histMap.begin() ; it!=histMap.end(); ++it)
    {
        cout<<"Processing Hist "<<it->first<<"  \n";
        auto curTrigger = it->second; 
        curTrigger.Write();
    }
    
    for (std::map<string,resolutionMeasurement * >::iterator it=resolutionMap.begin() ; it!=resolutionMap.end(); ++it)
	{
        std::cout<<"Processing Resolution "<<it->first<<"  \n";
        auto &resolutionItem = *(it->second); 
        resolutionItem.scaleBareVarHists();
        resolutionItem.calculateResolutions();
        resolutionItem.Write();
    }
   
    for (std::map<TString,TH2F *>::iterator it=th2fStore.begin() ; it!=th2fStore.end(); ++it)
    {
        //std::cout<<"Writing "<<it->first<<" to file ! \n";
        auto &ahist = *(it->second);
        ahist.Write();
    } 
    for (std::map<TString,TH1F *>::iterator it=th1fStore.begin() ; it!=th1fStore.end(); ++it)
    {
        //std::cout<<"Writing "<<it->first<<" to file ! \n";
        auto &ahist = *(it->second);
        ahist.Write();
    }

   std::cout<<"\n";
   ofile->Purge();
   ofile->Close();
	f->Close();
}


Double_t getDPHI( Double_t phi1, Double_t phi2) {

  Double_t dphi = phi1 - phi2;

  while( dphi > PI)
        dphi-= TWO_PI;
  while( dphi < -PI)
        dphi += TWO_PI;
  //std::cout<<"phi1  : "<<phi1<<" , phi2 : "<<phi2<<" dphi : "<<dphi<<"\n";
  if ( TMath::Abs(dphi) > 3.141592653589 ) {
    cout << " commonUtility::getDPHI error!!! dphi is bigger than 3.141592653589 "<< dphi << endl;
  }

  return TMath::Abs(dphi);
  //return dphi;
}

Double_t getDETA(Double_t eta1, Double_t eta2)
{

    return TMath::Abs(eta1 - eta2);
}

Double_t getDR( Double_t eta1, Double_t phi1,Double_t eta2 ,Double_t phi2) {

    Double_t de = getDETA(eta1,eta2);
    Double_t dp = getDPHI(phi1,phi2);
    return sqrt(de*de + dp*dp);

}
