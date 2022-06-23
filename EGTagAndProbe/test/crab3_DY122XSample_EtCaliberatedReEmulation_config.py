# TEMPLATE used for automatic script submission of multiple datasets

from WMCore.Configuration import Configuration
config = Configuration()

config.section_("General")
config.General.requestName = 'TagAndProbe_EtCaliberatedReEmulation_122Xsample_CaloParams_2022_v0_2_1203pre6_v0p1'
config.General.workArea = 'crab_projects'

config.section_("JobType")
config.JobType.pluginName = 'Analysis'
config.JobType.psetName = 'TandPRobeNtuplizerWithReEmulation.py'
#config.JobType.psetName = 'reEmulL1.py'
config.JobType.allowUndistributedCMSSW = True

config.section_("Data")
#config.Data.inputDataset = '/RelValZEE_13UP16/CMSSW_10_6_8-PU25ns_106X_mcRun2_asymptotic_preVFP_v5_UL16hltval_preVFP_v5-v1/MINIAODSIM'
config.Data.inputDataset = '/DYToLL_M-50_TuneCP5_13p6TeV-pythia8/Run3Winter22MiniAOD-L1TPU0to99FEVT_122X_mcRun3_2021_realistic_v9-v2/MINIAODSIM'
########################################### Parent Dataset #######################################
config.Data.secondaryInputDataset= '/DYToLL_M-50_TuneCP5_13p6TeV-pythia8/Run3Winter22DR-L1TPU0to99FEVT_122X_mcRun3_2021_realistic_v9-v2/GEN-SIM-DIGI-RAW'
##################################################################################################

config.Data.inputDBS = 'global'
config.Data.splitting = 'FileBased'
config.Data.unitsPerJob = 1 #number of events per jobs
config.Data.totalUnits  = -1  #number of event
config.Data.outLFNDirBase = '/store/group/dpg_trigger/comm_trigger/L1Trigger/athachay/crabOutputStorage'
config.Data.publication = False
config.Data.allowNonValidInputDataset = True
config.section_("Site")
config.Site.storageSite = 'T2_CH_CERN'
