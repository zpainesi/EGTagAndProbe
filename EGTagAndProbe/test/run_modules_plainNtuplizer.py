import FWCore.ParameterSet.Config as cms
process = cms.Process("Validation")

process.load("FWCore.MessageService.MessageLogger_cfi")
#process.load("Configuration.StandardSequences.GeometryRecoDB_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
from Configuration.AlCa.autoCond import autoCond
process.GlobalTag.globaltag = '106X_dataRun2_v25'
process.source = cms.Source("PoolSource",
                                # replace 'myfile.root' with the source file you want to use
                                fileNames = cms.untracked.vstring(
                    "file:f94ebbeb-4ac5-4668-bc60-0503ef541bc5.root"
                    ),
                duplicateCheckMode=cms.untracked.string("noDuplicateCheck"),
                            )

process.Ntuples = cms.EDAnalyzer("PlainMiniAODNtuplizer",
    vertices = cms.InputTag("offlineSlimmedPrimaryVertices"),
    photons = cms.InputTag("slimmedPhotons"),
    electrons = cms.InputTag("slimmedElectrons"),
    recJets = cms.InputTag("slimmedJets"),
    MustacheSCBarrelSrc= cms.InputTag("particleFlowSuperClusterECAL:particleFlowSuperClusterECALBarrel"),
    MustacheSCEndcapSrc= cms.InputTag("particleFlowSuperClusterECAL:particleFlowSuperClusterECALEndcapWithPreshower"),    
    l1EG = cms.InputTag("caloStage2Digis", "EGamma", "RECO"),
    )

process.TFileService = cms.Service("TFileService", fileName = cms.string('l1EGNtuple_ZB.root'))

process.p = cms.Path(
     process.Ntuples
    )

process.schedule = cms.Schedule(process.p)
process.MessageLogger.cerr.FwkReport.reportEvery = 100
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(120))
