# TEMPLATE used for automatic script submission of multiple datasets

from WMCore.Configuration import Configuration
config = Configuration()

config.section_("General")
config.General.requestName = 'TandP_npacked_eg0_2024G'
config.General.workArea = 'crab_projects'

config.section_("JobType")
config.JobType.pluginName = 'Analysis'
config.JobType.psetName = 'TandPRobeNtuplizer.py'
config.JobType.allowUndistributedCMSSW = True

config.section_("Data")
config.Data.inputDataset = '/EGamma0/Run2024G-PromptReco-v1/MINIAOD'
########################################### Parent Dataset #######################################
#config.Data.secondaryInputDataset= '/EGamma0/Run2023D-v1/RAW'
##################################################################################################

config.Data.inputDBS = 'global'
config.Data.splitting = 'FileBased'
config.Data.unitsPerJob = 1  #number of events per jobs
config.Data.totalUnits  = -1  #number of event
config.Data.outLFNDirBase = '/store/group/dpg_trigger/comm_trigger/L1Trigger/athachay/crabOutputStorage/2024/Data/'
config.Data.lumiMask    ='Collisions24_13p6TeV_378981_384383_DCSOnly_TkPx.json'
#config.Data.runRange    ='362616-362616'
config.Data.publication = False
config.Data.allowNonValidInputDataset = True
config.section_("Site")
config.Site.storageSite = 'T2_CH_CERN'
