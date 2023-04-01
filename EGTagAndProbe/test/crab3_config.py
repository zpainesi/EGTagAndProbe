# TEMPLATE used for automatic script submission of multiple datasets

from WMCore.Configuration import Configuration
config = Configuration()

config.section_("General")
config.General.requestName = 'TagAndProbe_eraG_recaliberated_EGamma_ZS0p5_v4'
config.General.workArea = 'crab_projects'

config.section_("JobType")
config.JobType.pluginName = 'Analysis'
config.JobType.psetName = 'TAandPReEmulated_run3.py'
config.JobType.allowUndistributedCMSSW = True

config.section_("Data")
config.Data.inputDataset = '/EGamma/Run2022G-PromptReco-v1/MINIAOD'
########################################### Parent Dataset #######################################
config.Data.secondaryInputDataset= '/EGamma/Run2022G-v1/RAW'
##################################################################################################

config.Data.inputDBS = 'global'
config.Data.splitting = 'FileBased'
config.Data.unitsPerJob = 1  #number of events per jobs
config.Data.totalUnits  = -1  #number of event
config.Data.outLFNDirBase = '/store/group/dpg_trigger/comm_trigger/L1Trigger/athachay/crabOutputStorage/egLUTDerivation2023'
config.Data.lumiMask    ='Cert_Collisions2022_355100_362760_Golden.json'
#config.Data.runRange    ='362616-362616'
config.Data.publication = False
config.Data.allowNonValidInputDataset = True
config.section_("Site")
config.Site.storageSite = 'T2_CH_CERN'
