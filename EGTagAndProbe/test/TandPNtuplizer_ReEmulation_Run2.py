import FWCore.ParameterSet.VarParsing as VarParsing
import FWCore.PythonUtilities.LumiList as LumiList
import FWCore.ParameterSet.Config as cms
import FWCore.Utilities.FileUtils as FileUtils
from Configuration.StandardSequences.Eras import eras

isMC = True
isMINIAOD = True

process = cms.Process("TagAndProbe",eras.Run2_2016)
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
    process.GlobalTag.globaltag = '110X_dataRun2_v12'
    process.load('EGTagAndProbe.EGTagAndProbe.tagAndProbe_cff')
    process.source = cms.Source("PoolSource",
        fileNames = cms.untracked.vstring(
            '/store/data/Run2018D/EGamma/MINIAOD/22Jan2019-v2/110000/10A4CCCC-3FB2-7F4B-841C-926A03DFBC61.root'
        ),

        secondaryFileNames = cms.untracked.vstring(
	'/store/data/Run2018D/EGamma/RAW/v1/000/321/233/00000/7A7B417B-969F-E811-9CC0-FA163EC80159.root',
	'/store/data/Run2018D/EGamma/RAW/v1/000/321/233/00000/9012D37D-969F-E811-913B-FA163EAF7E70.root',
	'/store/data/Run2018D/EGamma/RAW/v1/000/321/414/00000/004596BD-BBA2-E811-A445-FA163EB5B1F7.root',
	'/store/data/Run2018D/EGamma/RAW/v1/000/321/414/00000/C603AEE4-BBA2-E811-AE3E-FA163EFF1C10.root',
	'/store/data/Run2018D/EGamma/RAW/v1/000/321/887/00000/4210A02C-83AA-E811-A2FC-FA163E6CD89F.root',
	'/store/data/Run2018D/EGamma/RAW/v1/000/321/887/00000/18A5473E-82AA-E811-816D-02163E014BB9.root',
	'/store/data/Run2018D/EGamma/RAW/v1/000/321/887/00000/102E6C7D-7FAA-E811-892E-FA163ED1C998.root',
	'/store/data/Run2018D/EGamma/RAW/v1/000/321/414/00000/F21A0D70-BBA2-E811-A608-FA163EAEFBE1.root',
	'/store/data/Run2018D/EGamma/RAW/v1/000/321/414/00000/E675C0A9-BBA2-E811-810D-FA163E5ACC8E.root',
	'/store/data/Run2018D/EGamma/RAW/v1/000/321/233/00000/A8C98583-969F-E811-B653-FA163E8E54FF.root',
	'/store/data/Run2018D/EGamma/RAW/v1/000/321/887/00000/50D37695-81AA-E811-8B09-FA163E3116C8.root',
	'/store/data/Run2018D/EGamma/RAW/v1/000/321/887/00000/5E4E4A7C-7FAA-E811-B494-FA163E8A6FB6.root',
	'/store/data/Run2018D/EGamma/RAW/v1/000/321/887/00000/F404327D-81AA-E811-B1EF-FA163E818EF8.root',
	'/store/data/Run2018D/EGamma/RAW/v1/000/321/887/00000/F847F781-7FAA-E811-BF05-FA163EC968BB.root',
	'/store/data/Run2018D/EGamma/RAW/v1/000/325/057/00000/66BBB3F2-EB2F-7746-A8FE-3C865C2E652E.root',
	'/store/data/Run2018D/EGamma/RAW/v1/000/325/057/00000/4A0AB096-D0D7-524C-A5C1-69A429C5F609.root',
	'/store/data/Run2018D/EGamma/RAW/v1/000/321/233/00000/1C47877C-969F-E811-830E-FA163E346E5B.root',
	'/store/data/Run2018D/EGamma/RAW/v1/000/321/887/00000/4618ECCB-81AA-E811-8EAF-FA163E22EC2E.root'
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


process.load("L1Trigger.L1TCalorimeter.caloParams_2022_v0_2_cfi")

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
