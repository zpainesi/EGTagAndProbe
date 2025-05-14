import FWCore.ParameterSet.Config as cms

print("Running on data")

# filter HLT paths for T&P
import HLTrigger.HLTfilters.hltHighLevel_cfi as hlt



HLTLISTTAG = cms.VPSet(
    cms.PSet (
        #HLT = cms.string("HLT_Ele27_WPTight_Gsf_v"),
        HLT = cms.string("HLT_Ele32_WPTight_Gsf_v*"),
        path1 = cms.vstring ("hltEle32WPTightGsfTrackIsoFilter"), #FIXME: to check
        path2 = cms.vstring (""),
        leg1 = cms.int32(11),
        leg2 = cms.int32(999)
        ),
)

HLTLISTPROBE = cms.VPSet(
    cms.PSet (
        HLT = cms.string("HLT_Ele32_WPTight_Gsf_v*"),
        path1 = cms.vstring ("hltEle32WPTightGsfTrackIsoFilter"), #FIXME: to check
        path2 = cms.vstring (""),
        leg1 = cms.int32(11),
        leg2 = cms.int32(999)
        ),  
)



# filter HLT paths for T&P
import HLTrigger.HLTfilters.hltHighLevel_cfi as hlt
hltFilter = hlt.hltHighLevel.clone(
			TriggerResultsTag = cms.InputTag("TriggerResults","","HLT"),
			HLTPaths = ['HLT_Ele32_WPTight_Gsf_v*'],
			#HLTPaths = ['*'],
                        #HLTPaths = ['HLT_Ele27_WPTight_Gsf_v*'],
			andOr = cms.bool(True), # how to deal with multiple triggers: True (OR) accept if ANY is true, False (AND) accept if ALL are true
			throw = cms.bool(True) #if True: throws exception if a trigger path is invalid
)

from EventFilter.L1TRawToDigi.gtStage2Digis_cfi import gtStage2Digis
from PhysicsTools.PatAlgos.triggerLayer1.triggerProducer_cfi import patTrigger
from PhysicsTools.PatAlgos.slimming.selectedPatTrigger_cfi import selectedPatTrigger
from PhysicsTools.PatAlgos.slimming.slimmedPatTrigger_cfi import slimmedPatTrigger


patTriggerUnpacker = cms.EDProducer("PATTriggerObjectStandAloneUnpacker",
                                    patTriggerObjectsStandAlone = cms.InputTag("slimmedPatTrigger"),
                                    triggerResults = cms.InputTag('TriggerResults', '', "HLT"),
                                    unpackFilterLabels = cms.bool(True)
)

Ntuplizer = cms.EDAnalyzer("Ntuplizer",
    		treeName       = cms.string("TagAndProbe"),
            electrons      = cms.InputTag("gedGsfElectrons"),
			genParticles   = cms.InputTag("prunedGenParticles"),                       
			eleLooseIdMap  = cms.InputTag("egmGsfElectronIDs:cutBasedElectronID-RunIIIWinter22-V1-loose"),
			eleMediumIdMap = cms.InputTag("egmGsfElectronIDs:mvaEleID-RunIIIWinter22-iso-V1-wp80"),
			eleTightIdMap  = cms.InputTag("egmGsfElectronIDs:mvaEleID-RunIIIWinter22-iso-V1-wp90"),
			triggerSet     = cms.InputTag("patTriggerUnpacker"),
			triggerResultsLabel = cms.InputTag("TriggerResults", "", "HLT"),   
    		L1EG = cms.InputTag("caloStage2Digis", "EGamma", "RECO"),
    		L1EmuEG = cms.InputTag("simCaloStage2Digis"),
    		triggerListTag = HLTLISTTAG,
    		triggerListProbe = HLTLISTPROBE,
    		useGenMatch = cms.bool(False),
    		useHLTMatch = cms.bool(True),
            Vertices = cms.InputTag("offlinePrimaryVertices")
           
)

NtupleSeq = cms.Sequence(
    hltFilter        +
    patTrigger +
    selectedPatTrigger +
    slimmedPatTrigger +
    patTriggerUnpacker +
    Ntuplizer
)
