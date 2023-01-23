import FWCore.ParameterSet.VarParsing as VarParsing
import FWCore.PythonUtilities.LumiList as LumiList
import FWCore.ParameterSet.Config as cms
import FWCore.Utilities.FileUtils as FileUtils
from Configuration.StandardSequences.Eras import eras
from Configuration.Eras.Era_Run3_cff import Run3

isMC = False
isMINIAOD = True
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
#process.load('Configuration.Geometry.GeometryExtended2016Reco_cff')
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

# START ELECTRON CUT BASED ID SECTION
#
# Set up everything that is needed to compute electron IDs and
# add the ValueMaps with ID decisions into the event data stream
#

# Load tools and function definitions
from PhysicsTools.SelectorUtils.tools.vid_id_tools import *
from RecoEgamma.ElectronIdentification.egmGsfElectronIDs_cfi import *
from PhysicsTools.SelectorUtils.centralIDRegistry import central_id_registry

process.load("RecoEgamma.ElectronIdentification.ElectronMVAValueMapProducer_cfi")

#**********************
dataFormat = DataFormat.AOD
if isMINIAOD:
    dataFormat = DataFormat.MiniAOD
switchOnVIDElectronIdProducer(process, dataFormat)
#**********************

process.load("RecoEgamma.ElectronIdentification.egmGsfElectronIDs_cfi")
# overwrite a default parameter: for miniAOD, the collection name is a slimmed one
if isMINIAOD:
    process.egmGsfElectronIDs.physicsObjectSrc = cms.InputTag('slimmedElectrons')


from PhysicsTools.SelectorUtils.centralIDRegistry import central_id_registry
process.egmGsfElectronIDSequence = cms.Sequence(process.egmGsfElectronIDs)

# Define which IDs we want to produce
# Each of these two example IDs contains all four standard 
my_id_modules =[
'RecoEgamma.ElectronIdentification.Identification.mvaElectronID_Fall17_iso_V1_cff'
] 


#Add them to the VID producer
for idmod in my_id_modules:
    setupAllVIDIdsInModule(process,idmod,setupVIDElectronSelection)

from RecoEgamma.ElectronIdentification.ElectronMVAValueMapProducer_cfi import *

egmGsfElectronIDTask = cms.Task(
    #electronMVAVariableHelper,
    electronMVAValueMapProducer,
    egmGsfElectronIDs
)
egmGsfElectronIDSequence = cms.Sequence(egmGsfElectronIDTask)

if not isMC: # will use 80X
    from Configuration.AlCa.autoCond import autoCond
    process.GlobalTag.globaltag = '124X_dataRun3_Prompt_v10'
    process.load('EGTagAndProbe.EGTagAndProbe.tagAndProbe_cff')
    process.source = cms.Source("PoolSource",
        fileNames = cms.untracked.vstring(
            '/store/data/Run2022F/EGamma/MINIAOD/PromptReco-v1/000/362/154/00000/045d56d5-569d-4380-907a-e251680f98fe.root'
        ),

        secondaryFileNames = cms.untracked.vstring(
    '/store/data/Run2022F/EGamma/RAW/v1/000/362/154/00000/8e3a25da-1ca5-4bde-b966-f4151557bb21.root',
    '/store/data/Run2022F/EGamma/RAW/v1/000/362/154/00000/92a10dba-8fa4-4988-a990-a3d0ca149b5f.root',
    '/store/data/Run2022F/EGamma/RAW/v1/000/362/154/00000/a8597577-282a-4f12-9f3e-9abfe4920a2b.root',
    '/store/data/Run2022F/EGamma/RAW/v1/000/362/154/00000/a935cb5a-7679-4907-9918-1024c5e4a764.root',
    '/store/data/Run2022F/EGamma/RAW/v1/000/362/154/00000/b575a335-a0b1-4252-a8f7-26ab5cb86218.root',
    '/store/data/Run2022F/EGamma/RAW/v1/000/362/154/00000/e75aa9aa-3f82-4f35-af50-697e30da4cfd.root',
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
    process.Ntuplizer.electrons = cms.InputTag("slimmedElectrons")
    process.Ntuplizer.genParticles = cms.InputTag("prunedGenParticles")
    process.Ntuplizer.Vertices = cms.InputTag("offlineSlimmedPrimaryVertices")

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


process.load("L1Trigger.L1TCalorimeter.caloParams_2022_v0_4_cfi")

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
    input = cms.untracked.int32(2000)
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

## Job Customization
try:
    with open('cfg.json','r') as f:
        customizations=json.load(f)
        process.TFileService.fileName = customizations['outName']
        process.maxEvents.input= customizations["maxEvents"]
        process.source.fileNames= customizations["inputFiles"]
        process.source.secondaryFileNames=customizations["parentFiles"]
except:
    print("Customization file not loaded ! ")
print()
print(process.TFileService.fileName )
print(process.maxEvents.input)
print(process.source.fileNames)
print(process.source.secondaryFileNames)
print()
print()
