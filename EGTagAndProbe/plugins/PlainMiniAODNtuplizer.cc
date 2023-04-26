#include <memory>
/***************************************************************/
/********************HHbbggEvent selection**********************/
/***************************************************************/
/************************TIFR August20202***********************/



// class declaration
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"
#include "FWCore/Framework/interface/one/EDProducer.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Utilities/interface/EDGetToken.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Photon.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/MET.h"
#include "DataFormats/PatCandidates/interface/PackedGenParticle.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/GenRunInfoProduct.h"
#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"
#include "DataFormats/PatCandidates/interface/PackedCandidate.h"
#include "DataFormats/PatCandidates/interface/PackedGenParticle.h"
#include "DataFormats/L1Trigger/interface/EGamma.h"


#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/EgammaCandidates/interface/Conversion.h"
#include "TLorentzVector.h"
#include "FWCore/Framework/interface/Run.h"

#include "TTree.h"
#include "TH1.h"

#define N_ITEMS_MAX 100
#define N_SC_MAX 100

using namespace std;

const Int_t kMaxVertices = 300;
const Int_t kMaxGenJet = 300;


//redefinition of delta_phi variable between -pi < delta_phi < pi
namespace {
double PhiInRange(const double& phi) {
  double phiout = phi;

  if( phiout > 2*M_PI || phiout < -2*M_PI) {
    phiout = fmod( phiout, 2*M_PI);
  }
  if (phiout <= -M_PI) phiout += 2*M_PI;
  else if (phiout >  M_PI) phiout -= 2*M_PI;

  return phiout;
}

}
class PlainMiniAODNtuplizer : public edm::one::EDAnalyzer<edm::one::SharedResources>  {
   public:
      explicit PlainMiniAODNtuplizer(const edm::ParameterSet&);
      ~PlainMiniAODNtuplizer();
      
      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


   private:
      int getNPU(edm::Handle <std::vector <PileupSummaryInfo> >  puInfo);
      virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
      // ----------member data ---------------------------
       //edm::EDGetTokenT <reco::GenParticleCollection> genparticlesToken;
       edm::EDGetTokenT <double > rhoToken;
       edm::EDGetTokenT <edm::View<pat::Jet>> recJetToken_;
       edm::EDGetTokenT <edm::View<pat::Photon>> photonsToken_ ;
       edm::EDGetTokenT <edm::View<pat::Electron>> electronsToken_ ;
       edm::EDGetTokenT <edm::View<reco::SuperCluster>> scToken_ ;
       edm::EDGetTokenT <std::vector<reco::Vertex>> verticesToken_;

       edm::EDGetTokenT<std::vector<reco::SuperCluster>> MustacheSCBarrelCollection_;
       edm::EDGetTokenT<std::vector<reco::SuperCluster>> MustacheSCEndcapCollection_;
       edm::EDGetTokenT<reco::VertexCollection>          primaryVtxToken_;
       edm::Handle<reco::VertexCollection> pvHandle_;
	   edm::EDGetTokenT<l1t::EGammaBxCollection> l1EGToken  ;

       TTree* m_tree;
       std::map<TString,Int_t>storageMapInt;
       std::map<TString,Float_t*>storageMapFloatArray;

       void addL1EGBranches();
       void fillL1EGBranches(const edm::Event & iEvent);
       
       void addSCBranches();
       void fillSCBranches(const edm::Event & iEvent);

       void addElectronBranches();
       void fillElectronBranches(const edm::Event & iEvent);

       void addPhotonBranches();
       void fillPhotonBranches(const edm::Event & iEvent);

       void addJetBranches();
       void fillJetBranches(const edm::Event & iEvent);

       int n_PU, Tnpv;
       int eventId,runID,lumiId;

       bool isMiniAOD;
};

PlainMiniAODNtuplizer::PlainMiniAODNtuplizer(const edm::ParameterSet& iConfig)
{
   //now do what ever initialization is needed
   usesResource("TFileService");
   isMiniAOD=true;
   photonsToken_       = consumes<edm::View<pat::Photon>>(iConfig.getParameter<edm::InputTag>("photons"));
   recJetToken_       = consumes<edm::View<pat::Jet>>(iConfig.getParameter<edm::InputTag>("recJets"));
   electronsToken_       = consumes<edm::View<pat::Electron>>(iConfig.getParameter<edm::InputTag>("electrons"));

   primaryVtxToken_       =consumes<reco::VertexCollection>(iConfig.getParameter<edm::InputTag>("vertices"));
   l1EGToken =       consumes<l1t::EGammaBxCollection>                   (iConfig.getParameter<edm::InputTag>("l1EG"));

    edm::Service<TFileService> fs;
    m_tree = fs->make<TTree>("tree", "");

    addPhotonBranches();
    addL1EGBranches();
    addJetBranches();
    addElectronBranches();
   
   if(not isMiniAOD) {
    MustacheSCBarrelCollection_             = consumes<std::vector<reco::SuperCluster>>(iConfig.getParameter<edm::InputTag>("MustacheSCBarrelSrc"));
    MustacheSCEndcapCollection_             = consumes<std::vector<reco::SuperCluster>>(iConfig.getParameter<edm::InputTag>("MustacheSCEndcapSrc"));
    addSCBranches();
   }
   m_tree->Branch("run",   &runID);
   m_tree->Branch("lumi",  &lumiId);
   m_tree->Branch("event", &eventId);
   
}


PlainMiniAODNtuplizer::~PlainMiniAODNtuplizer()
{

   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}

void PlainMiniAODNtuplizer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
    iEvent.getByToken(primaryVtxToken_, pvHandle_);
    n_PU= (*pvHandle_).size();
    eventId = iEvent.id().event();
    runID  = iEvent.id().run();
    lumiId = iEvent.luminosityBlock();
    
    fillL1EGBranches(iEvent);
    fillJetBranches(iEvent);
    fillPhotonBranches(iEvent);
    fillElectronBranches(iEvent);
    if(not isMiniAOD) 
        fillSCBranches(iEvent);
    m_tree->Fill();
}

void PlainMiniAODNtuplizer::addPhotonBranches()
{
      storageMapInt["nPhotons"]=0;
      m_tree->Branch("nPhotons",   &storageMapInt["nPhotons"]);
      storageMapFloatArray["photons_pt"] = new Float_t[N_ITEMS_MAX];
      m_tree->Branch("photons_pt",   storageMapFloatArray["photons_pt"],"photons_pt[nPhotons]/F");
      storageMapFloatArray["photons_scEt"] = new Float_t[N_ITEMS_MAX];
      m_tree->Branch("photons_scEt",   storageMapFloatArray["photons_scEt"],"photons_scEt[nPhotons]/F");
      storageMapFloatArray["photons_eta"] = new Float_t[N_ITEMS_MAX];
      m_tree->Branch("photons_eta",   storageMapFloatArray["photons_eta"],"photons_eta[nPhotons]/F");
      storageMapFloatArray["photons_phi"] = new Float_t[N_ITEMS_MAX];
      m_tree->Branch("photons_phi",   storageMapFloatArray["photons_phi"],"photons_phi[nPhotons]/F");
      storageMapFloatArray["photons_e"] = new Float_t[N_ITEMS_MAX];
      m_tree->Branch("photons_e",   storageMapFloatArray["photons_e"],"photons_e[nPhotons]/F");
      storageMapFloatArray["photons_id_wp80"] = new Float_t[N_ITEMS_MAX];
      m_tree->Branch("photons_id_wp80",   storageMapFloatArray["photons_id_wp80"],"photons_id_wp80[nPhotons]/F");
      storageMapFloatArray["photons_id_wp90"] = new Float_t[N_ITEMS_MAX];
      m_tree->Branch("photons_id_wp90",   storageMapFloatArray["photons_id_wp90"],"photons_id_wp90[nPhotons]/F");
}

void PlainMiniAODNtuplizer::fillPhotonBranches(const edm::Event & iEvent)
{
   edm::Handle<edm::View<pat::Photon>>  photonsHandle ; 
   iEvent.getByToken(photonsToken_,photonsHandle);
   storageMapInt["nPhotons"]=0;
   Int_t idx=0;
    for (auto const& pho : *photonsHandle){
        storageMapFloatArray["photons_pt"][idx]       =             pho.et();
        storageMapFloatArray["photons_scEt"][idx]       =           pho.superCluster()->energy()/cosh(pho.superCluster()->eta());;
        storageMapFloatArray["photons_eta"][idx]      =             pho.eta();
        storageMapFloatArray["photons_phi"][idx]      =             pho.phi();
        storageMapFloatArray["photons_e"][idx]        =             pho.energy();
        storageMapFloatArray["photons_id_wp80"][idx] =             pho.photonID("mvaPhoID-RunIIFall17-v2-wp80");
        storageMapFloatArray["photons_id_wp90"][idx] =             pho.photonID("mvaPhoID-RunIIFall17-v2-wp90");
        idx++;
    }
    storageMapInt["nPhotons"]=idx;
}


void PlainMiniAODNtuplizer::addElectronBranches()
{
      storageMapInt["nElectrons"]=0;
      m_tree->Branch("nElectrons",   &storageMapInt["nElectrons"]);
      storageMapFloatArray["electrons_scEt"] = new Float_t[N_ITEMS_MAX];
      m_tree->Branch("electrons_scEt",   storageMapFloatArray["electrons_scEt"],"electrons_scEt[nElectrons]/F");
      storageMapFloatArray["electrons_pt"] = new Float_t[N_ITEMS_MAX];
      m_tree->Branch("electrons_pt",   storageMapFloatArray["electrons_pt"],"electrons_pt[nElectrons]/F");
      storageMapFloatArray["electrons_eta"] = new Float_t[N_ITEMS_MAX];
      m_tree->Branch("electrons_eta",   storageMapFloatArray["electrons_eta"],"electrons_eta[nElectrons]/F");
      storageMapFloatArray["electrons_phi"] = new Float_t[N_ITEMS_MAX];
      m_tree->Branch("electrons_phi",   storageMapFloatArray["electrons_phi"],"electrons_phi[nElectrons]/F");
      storageMapFloatArray["electrons_e"] = new Float_t[N_ITEMS_MAX];
      m_tree->Branch("electrons_e",   storageMapFloatArray["electrons_e"],"electrons_e[nElectrons]/F");
      storageMapFloatArray["electrons_id_wp80"] = new Float_t[N_ITEMS_MAX];
      m_tree->Branch("electrons_id_wp80",   storageMapFloatArray["electrons_id_wp80"],"electrons_id_wp80[nElectrons]/F");
      storageMapFloatArray["electrons_id_wp90"] = new Float_t[N_ITEMS_MAX];
      m_tree->Branch("electrons_id_wp90",   storageMapFloatArray["electrons_id_wp90"],"electrons_id_wp90[nElectrons]/F");

}

void PlainMiniAODNtuplizer::fillElectronBranches(const edm::Event & iEvent)
{
   edm::Handle<edm::View<pat::Electron>>  electronsHandle ; 
   iEvent.getByToken(electronsToken_,electronsHandle);
   storageMapInt["nElectrons"]=0;
   Int_t idx=0;
    for (auto const& ele : *electronsHandle){
        storageMapFloatArray["electrons_scEt"][idx]     =             (ele.correctedEcalEnergy())/cosh(ele.eta());
        storageMapFloatArray["electrons_pt"][idx]       =             ele.et();
        storageMapFloatArray["electrons_eta"][idx]      =             ele.eta();
        storageMapFloatArray["electrons_phi"][idx]      =             ele.phi();
        storageMapFloatArray["electrons_e"][idx]        =             ele.energy();
        storageMapFloatArray["electrons_id_wp80"][idx]    =             ele.electronID("mvaEleID-Fall17-iso-V2-wp80");
        storageMapFloatArray["electrons_id_wp90"][idx]    =             ele.electronID("mvaEleID-Fall17-iso-V2-wp90");
        idx++;
    }
    storageMapInt["nElectrons"]=idx;
}


void PlainMiniAODNtuplizer::addJetBranches()
{
      storageMapInt["nJets"]=0;
      m_tree->Branch("nJets",   &storageMapInt["nJets"]);
      storageMapFloatArray["jets_pt"] = new Float_t[N_ITEMS_MAX];
      m_tree->Branch("jets_pt",   storageMapFloatArray["jets_pt"],"jets_pt[nJets]/F");
      storageMapFloatArray["jets_eta"] = new Float_t[N_ITEMS_MAX];
      m_tree->Branch("jets_eta",   storageMapFloatArray["jets_eta"],"jets_eta[nJets]/F");
      storageMapFloatArray["jets_phi"] = new Float_t[N_ITEMS_MAX];
      m_tree->Branch("jets_phi",   storageMapFloatArray["jets_phi"],"jets_phi[nJets]/F");
      storageMapFloatArray["jets_mass"] = new Float_t[N_ITEMS_MAX];
      m_tree->Branch("jets_mass",   storageMapFloatArray["jets_mass"],"jets_mass[nJets]/F");
      storageMapFloatArray["jets_flavour"] = new Float_t[N_ITEMS_MAX];
      m_tree->Branch("jets_flavour",   storageMapFloatArray["jets_flavour"],"jets_flavour[nJets]/F");
    //  storageMapFloatArray["jets_pFlavour"] = new Float_t[N_ITEMS_MAX];
    //  m_tree->Branch("jets_pFlavour",   storageMapFloatArray["jets_pFlavour"],"jets_pFlavour[nJets]/F");
}


void PlainMiniAODNtuplizer::addL1EGBranches()
{
      storageMapInt["nL1eg"]=0;
      m_tree->Branch("nL1eg",   &storageMapInt["nL1eg"]);
      storageMapFloatArray["l1eg_pt"] = new Float_t[N_ITEMS_MAX];
      m_tree->Branch("l1eg_pt",   storageMapFloatArray["l1eg_pt"],"l1eg_pt[nL1eg]/F");
      storageMapFloatArray["l1eg_eta"] = new Float_t[N_ITEMS_MAX];
      m_tree->Branch("l1eg_eta",   storageMapFloatArray["l1eg_eta"],"l1eg_eta[nL1eg]/F");
      storageMapFloatArray["l1eg_phi"] = new Float_t[N_ITEMS_MAX];
      m_tree->Branch("l1eg_phi",   storageMapFloatArray["l1eg_phi"],"l1eg_phi[nL1eg]/F");
      storageMapFloatArray["l1eg_hwIso"] = new Float_t[N_ITEMS_MAX];
      m_tree->Branch("l1eg_hwIso",   storageMapFloatArray["l1eg_hwIso"],"l1eg_hwIso[nL1eg]/F");
      storageMapFloatArray["l1eg_hwQual"] = new Float_t[N_ITEMS_MAX];
      m_tree->Branch("l1eg_hwQual",   storageMapFloatArray["l1eg_hwQual"],"l1eg_hwQual[nL1eg]/F");
     
     storageMapFloatArray["l1eg_towerIEta"] = new Float_t[N_ITEMS_MAX];
     m_tree->Branch("l1eg_towerIEta",   storageMapFloatArray["l1eg_towerIEta"],"l1eg_towerIEta[nL1eg]/F");
     storageMapFloatArray["l1eg_towerIPhi"] = new Float_t[N_ITEMS_MAX];
     m_tree->Branch("l1eg_towerIPhi",   storageMapFloatArray["l1eg_towerIPhi"],"l1eg_towerIPhi[nL1eg]/F");
     storageMapFloatArray["l1eg_rawEt"] = new Float_t[N_ITEMS_MAX];
     m_tree->Branch("l1eg_rawEt",   storageMapFloatArray["l1eg_rawEt"],"l1eg_rawEt[nL1eg]/F");
     storageMapFloatArray["l1eg_isoEt"] = new Float_t[N_ITEMS_MAX];
     m_tree->Branch("l1eg_isoEt",   storageMapFloatArray["l1eg_isoEt"],"l1eg_isoEt[nL1eg]/F");
     storageMapFloatArray["l1eg_footprintEt"] = new Float_t[N_ITEMS_MAX];
     m_tree->Branch("l1eg_footprintEt",   storageMapFloatArray["l1eg_footprintEt"],"l1eg_footprintEt[nL1eg]/F");
     storageMapFloatArray["l1eg_nTT"] = new Float_t[N_ITEMS_MAX];
     m_tree->Branch("l1eg_nTT",   storageMapFloatArray["l1eg_nTT"],"l1eg_nTT[nL1eg]/F");
     storageMapFloatArray["l1eg_shape"] = new Float_t[N_ITEMS_MAX];
     m_tree->Branch("l1eg_shape",   storageMapFloatArray["l1eg_shape"],"l1eg_shape[nL1eg]/F");
     storageMapFloatArray["l1eg_towerHoE"] = new Float_t[N_ITEMS_MAX];
     m_tree->Branch("l1eg_towerHoE",   storageMapFloatArray["l1eg_towerHoE"],"l1eg_towerHoE[nL1eg]/F");
     storageMapFloatArray["l1eg_hwPt"] = new Float_t[N_ITEMS_MAX];
     m_tree->Branch("l1eg_hwPt",   storageMapFloatArray["l1eg_hwPt"],"l1eg_hwPt[nL1eg]/F");
     storageMapFloatArray["l1eg_hwEta"] = new Float_t[N_ITEMS_MAX];
     m_tree->Branch("l1eg_hwEta",   storageMapFloatArray["l1eg_hwEta"],"l1eg_hwEta[nL1eg]/F");
     storageMapFloatArray["l1eg_hwPhi"] = new Float_t[N_ITEMS_MAX];
     m_tree->Branch("l1eg_hwPhi",   storageMapFloatArray["l1eg_hwPhi"],"l1eg_hwPhi[nL1eg]/F");
}




void PlainMiniAODNtuplizer::fillL1EGBranches( const edm::Event& iEvent)
{
	edm::Handle< BXVector<l1t::EGamma> >  L1EGHandle;
	iEvent.getByToken(l1EGToken, L1EGHandle);
    Int_t idx=0;
	for (l1t::EGammaBxCollection::const_iterator bx0EGIt = L1EGHandle->begin(0); bx0EGIt != L1EGHandle->end(0) ; bx0EGIt++)
	{    
       const l1t::EGamma lojEG  = *bx0EGIt;                            
       storageMapFloatArray["l1eg_pt"][idx]=lojEG.pt();
       storageMapFloatArray["l1eg_eta"][idx]    = lojEG.eta();
       storageMapFloatArray["l1eg_phi"][idx]    = lojEG.phi();
       storageMapFloatArray["l1eg_hwIso"][idx]  = lojEG.hwIso();
       storageMapFloatArray["l1eg_hwQual"][idx] = lojEG.hwQual();
       storageMapFloatArray["l1eg_towerIEta"][idx] = lojEG.towerIEta(); 
       storageMapFloatArray["l1eg_towerIPhi"][idx] = lojEG.towerIPhi(); 
       storageMapFloatArray["l1eg_rawEt"][idx] = lojEG.rawEt(); 
       storageMapFloatArray["l1eg_isoEt"][idx] = lojEG.isoEt(); 
       storageMapFloatArray["l1eg_footprintEt"][idx] = lojEG.footprintEt(); 
       storageMapFloatArray["l1eg_nTT"][idx] = lojEG.nTT(); 
       storageMapFloatArray["l1eg_shape"][idx] = lojEG.shape(); 
       storageMapFloatArray["l1eg_towerHoE"][idx] = lojEG.towerHoE(); 
       storageMapFloatArray["l1eg_hwPt"][idx] = lojEG.hwPt(); 
       storageMapFloatArray["l1eg_hwEta"][idx] = lojEG.hwEta(); 
       storageMapFloatArray["l1eg_hwPhi"][idx] = lojEG.hwPhi(); 

       idx+=1;
    }
    storageMapInt["nL1eg"]=idx;
}

void PlainMiniAODNtuplizer::fillJetBranches( const edm::Event& iEvent)
{
      edm::Handle<edm::View<pat::Jet>> recjets;
      iEvent.getByToken(recJetToken_, recjets);
      
      storageMapInt["nJets"]=0;
      Int_t idx=0;
      for(auto const & recJet : *recjets) {
            storageMapFloatArray["jets_pt"][idx]        = recJet.pt();
            storageMapFloatArray["jets_eta"][idx]       = recJet.eta();
            storageMapFloatArray["jets_phi"][idx]       = recJet.phi();
            storageMapFloatArray["jets_mass"][idx]         = recJet.mass();
         //   storageMapFloatArray["jets_flavour"][idx]        = recJet.hadronFlavour() ;
        idx++;
    }
    storageMapInt["nJets"]=idx;
}


void PlainMiniAODNtuplizer::addSCBranches()
{
    storageMapInt["nSC"]=0;
    m_tree->Branch("nSC",   &storageMapInt["nSC"]);
    
    storageMapFloatArray["scE"]   = new Float_t[N_SC_MAX];
    m_tree->Branch("scE", storageMapFloatArray["scE"],"scE[nSC]/F");
    storageMapFloatArray["scEt"]   = new Float_t[N_SC_MAX];
    m_tree->Branch("scEt", storageMapFloatArray["scEt"],"scEt[nSC]/F");
    storageMapFloatArray["scRawE"]   = new Float_t[N_SC_MAX];
    m_tree->Branch("scRawE", storageMapFloatArray["scRawE"],"scRawE[nSC]/F");
    storageMapFloatArray["scRawEt"]   = new Float_t[N_SC_MAX];
    m_tree->Branch("scRawEt", storageMapFloatArray["scRawEt"],"scRawEt[nSC]/F");
    storageMapFloatArray["scEta"]   = new Float_t[N_SC_MAX];
    m_tree->Branch("scEta", storageMapFloatArray["scEta"],"scEta[nSC]/F");
    storageMapFloatArray["scPhi"]   = new Float_t[N_SC_MAX];
    m_tree->Branch("scPhi", storageMapFloatArray["scPhi"],"scPhi[nSC]/F");

}


void PlainMiniAODNtuplizer::fillSCBranches(edm::Event const& e) {

  edm::Handle<reco::SuperClusterCollection> barrelSCHandle;
  e.getByToken(MustacheSCBarrelCollection_, barrelSCHandle);

  edm::Handle<reco::SuperClusterCollection> endcapSCHandle;
  e.getByToken(MustacheSCEndcapCollection_, endcapSCHandle);
 
 Int_t isECAP=-1;
  Int_t idx=0;
  storageMapInt["nSC"]=0;
  for (auto const& scs : { *barrelSCHandle.product(), *endcapSCHandle.product() }) {
    isECAP+=1; 
    for (auto const& sc : scs) {
      storageMapFloatArray["scE"][idx]          =                      sc.correctedEnergy();
      storageMapFloatArray["scEt"][idx]         =                      sc.correctedEnergy()/cosh(sc.eta());
      storageMapFloatArray["scRawE"][idx]       =                      sc.rawEnergy();
      storageMapFloatArray["scRawEt"][idx]      =                      sc.rawEnergy()/cosh(sc.eta());
      storageMapFloatArray["scEta"][idx]        =                      sc.eta();
      storageMapFloatArray["scPhi"][idx]        =                      sc.phi();
      idx++;
    } // supercluster loop
  }
  storageMapInt["nSC"]=idx;
}




// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
PlainMiniAODNtuplizer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(PlainMiniAODNtuplizer);
