import FWCore.ParameterSet.VarParsing as VarParsing
import FWCore.PythonUtilities.LumiList as LumiList
import FWCore.ParameterSet.Config as cms
from Configuration.StandardSequences.Eras import eras
from Configuration.Eras.Era_Run3_cff import Run3

process = cms.Process("TagAndProbe",eras.Run3)

isMC = False
isMINIAOD = True

process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.load("Configuration.StandardSequences.GeometryRecoDB_cff")

#### handling of cms line options for tier3 submission
#### the following are dummy defaults, so that one can normally use the config changing file list by hand etc.

options = VarParsing.VarParsing ('analysis')
#options.register ('skipEvents',
#                  -1, # default value
#                  VarParsing.VarParsing.multiplicity.singleton, # singleton or list
#                  VarParsing.VarParsing.varType.int,          # string, int, or float
#                  "Number of events to skip")

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
# Set up everything that is needed to compute electron IDs and
# add the ValueMaps with ID decisions into the event data stream#

# Load tools and function definitions
from PhysicsTools.SelectorUtils.tools.vid_id_tools import *

from RecoEgamma.ElectronIdentification.egmGsfElectronIDs_cfi import *
from PhysicsTools.SelectorUtils.centralIDRegistry import central_id_registry

#process.load("RecoEgamma.ElectronIdentification.ElectronMVAValueMapProducer_cfi")

#**********************
dataFormat = DataFormat.AOD
if isMINIAOD:
    dataFormat = DataFormat.MiniAOD
#switchOnVIDElectronIdProducer(process, dataFormat)
#**********************

#process.load("RecoEgamma.ElectronIdentification.egmGsfElectronIDs_cfi")
#from RecoEgamma.ElectronIdentification.ElectronMVAValueMapProducer_cfi import *
## overwrite a default parameter: for miniAOD, the collection name is a slimmed one
#if isMINIAOD:
#    process.egmGsfElectronIDs.physicsObjectSrc = cms.InputTag('slimmedElectrons')
#from PhysicsTools.SelectorUtils.centralIDRegistry import central_id_registry
#process.egmGsfElectronIDSequence = cms.Sequence(process.egmGsfElectronIDs)

# Define which IDs we want to produce
# Each of these two example IDs contains all four standard 
#my_id_modules =[
#'RecoEgamma.ElectronIdentification.Identification.mvaElectronID_Fall17_iso_V1_cff',
#'RecoEgamma.ElectronIdentification.Identification.cutBasedElectronID_Winter22_122X_V1_cff'
#
#] 
#
##Add them to the VID producer
#for idmod in my_id_modules:
#    setupAllVIDIdsInModule(process,idmod,setupVIDElectronSelection)

# define which IDs we want to produce
#my_id_modules = [
#'RecoEgamma.PhotonIdentification.Identification.cutBasedPhotonID_RunIIIWinter22_122X_V1_cff',
#'RecoEgamma.PhotonIdentification.Identification.mvaPhotonID_Winter22_122X_V1_cff',
#'RecoEgamma.ElectronIdentification.Identification.mvaElectronID_RunIIIWinter22_iso_V1_cff',
#'RecoEgamma.ElectronIdentification.Identification.mvaElectronID_RunIIIWinter22_noIso_V1_cff',
#'RecoEgamma.ElectronIdentification.Identification.cutBasedElectronID_Winter22_122X_V1_cff'
#]

#for idmod in my_id_modules:
#   setupAllVIDIdsInModule(process, idmod, setupVIDElectronSelection)


egmGsfElectronIDTask = cms.Task(
    #electronMVAVariableHelper,
    #electronMVAValueMapProducer,
    #egmGsfElectronIDs
)
egmGsfElectronIDSequence = cms.Sequence(egmGsfElectronIDTask)

if not isMC: # will use 80X
    from Configuration.AlCa.autoCond import autoCond
    process.GlobalTag.globaltag = '140X_dataRun3_Prompt_v4'
    process.load('EGTagAndProbe.EGTagAndProbe.tagAndProbe_cff')
    process.source = cms.Source("PoolSource",
        fileNames = cms.untracked.vstring(
            "/store/data/Run2023D/EGamma0/MINIAOD/PromptReco-v2/000/370/667/00000/85f33e1c-f4d0-4534-8257-140030ce8783.root",
            "/store/data/Run2023D/EGamma0/MINIAOD/PromptReco-v2/000/370/667/00000/a69f9784-c0f4-40a8-8498-affed33d9444.root",
            "/store/data/Run2023D/EGamma0/MINIAOD/PromptReco-v2/000/370/667/00000/cae35b2f-8d01-4518-9b32-bfbdb6ef1709.root",
            "/store/data/Run2023D/EGamma0/MINIAOD/PromptReco-v2/000/370/667/00000/d28a27d9-475c-4850-b55b-7660a0d10559.root",
            "/store/data/Run2023D/EGamma0/MINIAOD/PromptReco-v2/000/370/667/00000/eb014129-e70d-4f13-bbf0-525b2322d4c6.root",
            "/store/data/Run2023D/EGamma0/MINIAOD/PromptReco-v2/000/370/667/00000/f198995b-2c20-40a0-8f86-543ba3dd1c50.root",
            "/store/data/Run2023D/EGamma0/MINIAOD/PromptReco-v2/000/370/667/00000/f4ca6e0f-f204-4e80-95ed-678dc8f2212b.root",
            "/store/data/Run2023D/EGamma0/MINIAOD/PromptReco-v2/000/370/667/00000/f7fa86b6-5e59-4495-9d73-59a7acd41d91.root",
            "/store/data/Run2023D/EGamma0/MINIAOD/PromptReco-v2/000/370/667/00000/f996f1da-d590-4e7b-b072-1ba647b99f8e.root"
        ),
    )

else:
    process.GlobalTag.globaltag = '133X_mcRun3_2024_realistic_v8'
    process.load('EGTagAndProbe.EGTagAndProbe.MCanalysis_cff')
    process.source = cms.Source("PoolSource",
        fileNames = cms.untracked.vstring(
        "/store/mc/Run3Winter24MiniAOD/DYTo2L_MLL-50_TuneCP5_13p6TeV_pythia8/MINIAODSIM/KeepSi_133X_mcRun3_2024_realistic_v8-v2/2560000/001d128f-6406-4358-bee3-785f92b40734.root",
        "/store/mc/Run3Winter24MiniAOD/DYTo2L_MLL-50_TuneCP5_13p6TeV_pythia8/MINIAODSIM/KeepSi_133X_mcRun3_2024_realistic_v8-v2/2560000/00ce7a53-2845-48da-9152-d955ec8fc218.root",
        "/store/mc/Run3Winter24MiniAOD/DYTo2L_MLL-50_TuneCP5_13p6TeV_pythia8/MINIAODSIM/KeepSi_133X_mcRun3_2024_realistic_v8-v2/2560000/0123ec47-e87b-43e0-a45a-35a2d151843c.root",
        "/store/mc/Run3Winter24MiniAOD/DYTo2L_MLL-50_TuneCP5_13p6TeV_pythia8/MINIAODSIM/KeepSi_133X_mcRun3_2024_realistic_v8-v2/2560000/01819f90-699c-4e9e-9c04-ce5c8392aeeb.root",
        "/store/mc/Run3Winter24MiniAOD/DYTo2L_MLL-50_TuneCP5_13p6TeV_pythia8/MINIAODSIM/KeepSi_133X_mcRun3_2024_realistic_v8-v2/2560000/0245abf7-e4e7-459a-bb42-724d6c4e4fe9.root"
     )
    )
    process.Ntuplizer.useHLTMatch = cms.bool(False) #In case no HLT object in MC sample considered or you're fed up with trying to find the right HLT collections

if isMINIAOD:
    process.Ntuplizer.photons = cms.InputTag("slimmedPhotons")
    process.Ntuplizer.electrons = cms.InputTag("slimmedElectrons")
    #process.Ntuplizer.egmGsfElectronIDSequence = cms.InputTag("slimmedElectrons")
    process.Ntuplizer.genParticles = cms.InputTag("prunedGenParticles")
    process.Ntuplizer.Vertices = cms.InputTag("offlineSlimmedPrimaryVertices")
    print(process.Ntuplizer.electrons)

if options.JSONfile:
    #print "Using JSON: " , options.JSONfile
    process.source.lumisToProcess = LumiList.LumiList(filename = options.JSONfile).getVLuminosityBlockRange()

if options.inputFiles:
    process.source.fileNames = cms.untracked.vstring(options.inputFiles)

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(5000)
)

if options.maxEvents >= -1:
    process.maxEvents.input = cms.untracked.int32(options.maxEvents)
#if options.skipEvents >= 0:
#    process.source.skipEvents = cms.untracked.uint32(options.skipEvents)
print("Max Events set as : ",process.maxEvents.input)
process.options = cms.untracked.PSet(
    wantSummary = cms.untracked.bool(True)
)

process.p = cms.Path(
 #   process.egmGsfElectronIDSequence +
    process.NtupleSeq
)

# Silence output
process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 1000

# Adding ntuplizer
process.TFileService=cms.Service('TFileService',fileName=cms.string(options.outputFile))
