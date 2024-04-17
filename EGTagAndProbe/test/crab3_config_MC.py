# TEMPLATE used for automatic script submission of multiple datasets

from WMCore.Configuration import Configuration
config = Configuration()

config.section_("General")
config.General.requestName = 'TandP_2023D_reEmulated_v0'
config.General.workArea = 'crab_projects'

config.section_("JobType")
config.JobType.pluginName = 'Analysis'
config.JobType.psetName = 'TAandPReEmulated_run3.py'#'TandPRobeNtuplizer.py'
config.JobType.allowUndistributedCMSSW = True

config.section_("Data")
config.Data.inputDataset = '/DYto2L_M-50_TuneCP5_13p6TeV_pythia8/Run3Winter24MiniAOD-KeepSi_133X_mcRun3_2024_realistic_v8-v2/MINIAODSIM'
########################################### Parent Dataset #######################################
config.Data.secondaryInputDataset= '/DYto2L_M-50_TuneCP5_13p6TeV_pythia8/Run3Winter24MiniAOD-KeepSi_133X_mcRun3_2024_realistic_v8-v2/MINIAODSIM'
##################################################################################################

config.Data.inputDBS = 'global'
config.Data.splitting = 'EventAwareLumiBased'
config.Data.unitsPerJob = 1000  #number of events per jobs
config.Data.totalUnits  = 1000*2000  #number of event
config.Data.outLFNDirBase = '/store/group/dpg_trigger/comm_trigger/L1Trigger/zpainesi/CrabOutputStorage/TnP_ntuples/2024/mc'
#config.Data.lumiMask    ='Cert_Collisions2022_355100_362760_Golden.json'
#config.Data.runRange    ='362616-362616'
config.Data.publication = False
config.Data.allowNonValidInputDataset = True
config.section_("Site")
config.Site.storageSite = 'T2_CH_CERN'
