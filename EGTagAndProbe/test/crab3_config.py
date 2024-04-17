# TEMPLATE used for automatic script submission of multiple datasets

from WMCore.Configuration import Configuration
config = Configuration()

config.section_("General")
config.General.requestName = 'TandP_EG0_2024B_v1_379367-379391'
config.General.workArea = 'crab_projects'

config.section_("JobType")
config.JobType.pluginName = 'Analysis'
config.JobType.psetName = 'TandPRobeNtuplizer.py'#'TAandPReEmulated_run3.py'
config.JobType.allowUndistributedCMSSW = True

config.section_("Data")
config.Data.inputDataset = '/EGamma0/Run2024B-PromptReco-v1/MINIAOD'
########################################### Parent Dataset #######################################
#config.Data.secondaryInputDataset= '/EGamma0/Run2023D-v1/RAW'
##################################################################################################

config.Data.inputDBS = 'global'
config.Data.splitting = 'FileBased'
config.Data.unitsPerJob = 1  #number of events per jobs
config.Data.totalUnits  = -1  #number of event
config.Data.outLFNDirBase = '/store/group/dpg_trigger/comm_trigger/L1Trigger/zpainesi/CrabOutputStorage/2024'
config.Data.lumiMask    ='/afs/cern.ch/work/z/zpainesi/public/Certification/Collisions24/Cert_2024B_DCSonly_170424.txt'
#config.Data.runRange    ='379367-379391'
config.Data.publication = False
config.Data.allowNonValidInputDataset = True
config.section_("Site")
config.Site.storageSite = 'T2_CH_CERN'
