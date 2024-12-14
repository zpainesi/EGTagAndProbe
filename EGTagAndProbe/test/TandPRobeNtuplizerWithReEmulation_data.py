import FWCore.ParameterSet.VarParsing as VarParsing
import FWCore.PythonUtilities.LumiList as LumiList
import FWCore.ParameterSet.Config as cms
import FWCore.Utilities.FileUtils as FileUtils
from Configuration.StandardSequences.Eras import eras
from Configuration.Eras.Era_Run3_cff import Run3

isMC = False
isMINIAOD = False

process = cms.Process("TagAndProbe",eras.Run3)
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.load('Configuration.StandardSequences.RawToDigi_Data_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.MagneticField_AutoFromDBCurrent_cff')


options = VarParsing.VarParsing ('analysis')
options.register ('secondaryFilesList','',VarParsing.VarParsing.multiplicity.singleton,VarParsing.VarParsing.varType.string,  "List of secondary input files")

options.register ('skipEvents',
                  -1, # default value
                  VarParsing.VarParsing.multiplicity.singleton, # singleton or list
                  VarParsing.VarParsing.varType.int,          # string, int, or float
                  "Number of events to skip")
options.register ('JSONfile',
                  "", # default value
                  VarParsing.VarParsing.multiplicity.singleton, # singleton or list
                  VarParsing.VarParsing.varType.string,          # string, int, or float
                  "JSON file (empty for no JSON)")
options.outputFile = 'NTuple.root'
options.inputFiles = []
options.maxEvents  = -999

options.parseArguments()

if not isMC: # will use 80X
    from Configuration.AlCa.autoCond import autoCond
    process.GlobalTag.globaltag = '140X_dataRun3_Prompt_v4'
    process.load('EGTagAndProbe.EGTagAndProbe.tagAndProbe_cff')
    process.source = cms.Source("PoolSource",
        fileNames = cms.untracked.vstring(
        'file:/eos/home-a/athachay/workarea/120e6d8c-ec45-40ed-9f5c-986a02b2492b.root'
        )
    )
else:
    process.GlobalTag.globaltag = '123X_mcRun3_2021_realistic_v13'
    process.load('EGTagAndProbe.EGTagAndProbe.MCanalysis_cff')
    process.source = cms.Source("PoolSource",
     fileNames= cms.untracked.vstring(
    '/store/mc/Run3Winter21DRMiniAOD/DYToLL_M-50_TuneCP5_14TeV-pythia8/MINIAODSIM/FlatPU30to80FEVT_112X_mcRun3_2021_realistic_v16-v2/120000/08ea458b-8a11-4822-b49c-cee9b4a85630.root'
     ),
   )
    
    process.Ntuplizer.useHLTMatch = cms.bool(False) #In case no HLT object in MC sample considered or you're fed up with trying to find the right HLT collections


if isMINIAOD:
    process.egmGsfElectronIDSequence = cms.Sequence()
    process.Ntuplizer.electrons = cms.InputTag("slimmedElectrons")
    process.Ntuplizer.genParticles = cms.InputTag("prunedGenParticles")
    process.Ntuplizer.Vertices = cms.InputTag("offlineSlimmedPrimaryVertices")
else :
    from PhysicsTools.SelectorUtils.tools.vid_id_tools import *
    dataFormat = DataFormat.AOD
    process.load("RecoEgamma.ElectronIdentification.ElectronMVAValueMapProducer_cfi")
    switchOnVIDElectronIdProducer(process, dataFormat)
    my_id_modules = [
        'RecoEgamma.ElectronIdentification.Identification.mvaElectronID_RunIIIWinter22_iso_V1_cff',
        'RecoEgamma.ElectronIdentification.Identification.mvaElectronID_RunIIIWinter22_noIso_V1_cff',
        'RecoEgamma.ElectronIdentification.Identification.cutBasedElectronID_Winter22_122X_V1_cff'
    ]
    
    process.electronMVAValueMapProducer.src=cms.InputTag("gedGsfElectrons")

    for idmod in my_id_modules:
        setupAllVIDIdsInModule(process, idmod, setupVIDElectronSelection)
    process.Ntuplizer.eleLooseIdMap  = cms.InputTag("egmGsfElectronIDs:cutBasedElectronID-RunIIIWinter22-V1-loose")
    process.Ntuplizer.eleTightIdMap = cms.InputTag("egmGsfElectronIDs:mvaEleID-RunIIIWinter22-iso-V1-wp80")
    process.Ntuplizer.eleMediumIdMap  = cms.InputTag("egmGsfElectronIDs:mvaEleID-RunIIIWinter22-iso-V1-wp90")

process.schedule = cms.Schedule()

## L1 emulation stuff

if not isMC:
    from L1Trigger.Configuration.customiseReEmul import L1TReEmulFromRAW 
    process = L1TReEmulFromRAW(process)
else:
    from L1Trigger.Configuration.customiseReEmul import L1TReEmulMCFromRAW
    process = L1TReEmulMCFromRAW(process) 
    from L1Trigger.Configuration.customiseUtils import L1TTurnOffUnpackStage2GtGmtAndCalo 
    process = L1TTurnOffUnpackStage2GtGmtAndCalo(process)


#process.load("L1Trigger.L1TCalorimeter.caloParams_2023_v0_4_eTRecalib_cfi")

#### handling of cms line options for tier3 submission
#### the following are dummy defaults, so that one can normally use the config changing file list by hand etc.

if options.JSONfile:
    print( "Using JSON: " , options.JSONfile)
    process.source.lumisToProcess = LumiList.LumiList(filename = options.JSONfile).getVLuminosityBlockRange()

if options.inputFiles:
    process.source.fileNames = cms.untracked.vstring(options.inputFiles)

if options.secondaryFilesList:
    listSecondaryFiles = FileUtils.loadListFromFile(options.secondaryFilesList)
    process.source.secondaryFileNames = cms.untracked.vstring(listSecondaryFiles)

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(100)
)

if options.maxEvents >= -1:
    process.maxEvents.input = cms.untracked.int32(options.maxEvents)
if options.skipEvents >= 0:
    process.source.skipEvents = cms.untracked.uint32(options.skipEvents)

process.options = cms.untracked.PSet(
    wantSummary = cms.untracked.bool(True)
)

process.p = cms.Path (
    process.egmGsfElectronIDSequence +
    process.RawToDigi +
    process.L1TReEmul +
    process.NtupleSeq
)


process.schedule = cms.Schedule(process.p) # do my sequence pls

# Silence output
process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 100

# Adding ntuplizer
process.TFileService=cms.Service('TFileService',fileName=cms.string(options.outputFile))
