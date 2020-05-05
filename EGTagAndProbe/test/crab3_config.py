# TEMPLATE used for automatic script submission of multiple datasets

from WMCore.Configuration import Configuration
config = Configuration()

config.section_("General")
config.General.requestName = 'TagAndProbe_RelValZEE_13UP16_2016_LATEST'
config.General.workArea = 'DefaultCrab3Area'

config.section_("JobType")
config.JobType.pluginName = 'Analysis'
config.JobType.psetName = 'test.py'
#config.JobType.psetName = 'reEmulL1.py'
config.JobType.allowUndistributedCMSSW = True

config.section_("Data")
#config.Data.inputDataset = '/RelValZEE_13UP16/CMSSW_10_6_8-PU25ns_106X_mcRun2_asymptotic_preVFP_v5_UL16hltval_preVFP_v5-v1/MINIAODSIM'
config.Data.inputDataset = '/RelValZpEE_2250_13UP16/CMSSW_10_6_11_CANDIDATE-PU25ns_106X_mcRun2_asymptotic_v12_hlt16post-v1/MINIAODSIM'
########################################### Parent Dataset #######################################
#config.Data.secondaryInputDataset= '/RelValZEE_13UP16/CMSSW_10_6_8-PU25ns_80X_mcRun2_asymptotic_v20_UL16hltval_preVFP_v5-v1/GEN-SIM-RAW'
##################################################################################################

config.Data.inputDBS = 'global'
config.Data.splitting = 'EventAwareLumiBased'
config.Data.unitsPerJob = 10000 #number of events per jobs
config.Data.totalUnits = -1 #number of event
config.Data.outLFNDirBase = '/store/group/dpg_trigger/comm_trigger/L1Trigger/ckoraka/TagAndProbe_RelValZEE_13UP16_2016_LATEST'
config.Data.publication = False
config.Data.allowNonValidInputDataset = True
config.Data.outputDatasetTag = 'TagAndProbe_TagAndProbe_RelValZEE_13UP16_2016_LATEST'
config.section_("Site")
config.Site.storageSite = 'T2_CH_CERN'
