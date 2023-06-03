"""
#!/usr/bin/env python
Source : https://twiki.cern.ch/twiki/bin/view/CMSPublic/CRAB3AdvancedTutorial#Exercise_6_CRABAPI_library

This is a small script that does the equivalent of multicrab.
"""
import os
from optparse import OptionParser

import CRABClient
from CRABAPI.RawCommand import crabCommand
from CRABClient.ClientExceptions import ClientException
import argparse
from http.client import HTTPException


def getOptions():
   
    return args


def main():

    parser = argparse.ArgumentParser()
    parser.add_argument("-c","--crabCmd", help="crab command")
    parser.add_argument("-i","--cmsconfig", help="configuration to use")
    parser.add_argument("-o","--dest", help="stageout destination", default='CERN' )
    parser.add_argument("-e","--ext", help="job tag extension", default='' )
    parser.add_argument("-u","--unitsPerJob", help="Lumi Per Job", type=int ,default=5 )
    parser.add_argument("--crabCmdOpts", help="commadline option for the carb cmd", default="" )
    parser.add_argument("--splitting", help="splitting type of the job", default="LumiBased" )
    parser.add_argument("--lumiMask", help="Lumi Mask", default=None )
    options=parser.parse_args()

  # The submit command needs special treatment.
    if options.crabCmd == 'submit':

        #--------------------------------------------------------
        # This is the base config:
        #--------------------------------------------------------
        from CRABClient.UserUtilities import config
        config = config()

        config.General.requestName = None
        config.General.workArea = 'workarea'

        config.JobType.pluginName = 'Analysis'
        config.JobType.psetName = options.cmsconfig

        config.Data.inputDataset = None
        config.Data.splitting =  options.splitting
        config.Data.unitsPerJob = options.unitsPerJob
        config.Data.totalUnits = None
        config.Data.outputDatasetTag = None
        config.Data.outLFNDirBase = '/store/group/dpg_trigger/comm_trigger/L1Trigger/athachay/crabOutputStorage/L1RawRecoPerformance2023'
        config.Data.lumiMask = options.lumiMask
        config.Site.storageSite = "T2_CH_CERN" # Choose your site. 
        
        #--------------------------------------------------------

        # Will submit one task for each of these input datasets.
        inputDatasets = [
                            "/EGamma0/Run2023B-ZElectron-PromptReco-v1/RAW-RECO",
                            "/EGamma1/Run2023B-ZElectron-PromptReco-v1/RAW-RECO",
                            "/EGamma0/Run2023C-ZElectron-PromptReco-v1/RAW-RECO",
                            "/EGamma1/Run2023C-ZElectron-PromptReco-v1/RAW-RECO",
                        ]

        for inDS in inputDatasets:
            # inDS is of the form /A/B/C. Since B is unique for each inDS, use this in the CRAB request name.
            config.General.requestName = '_'.join(inDS.split('/')[1:3])+options.ext
            config.Data.inputDataset = inDS
            config.Data.outputDatasetTag = '%s_%s' % (config.General.workArea, config.General.requestName)
            # Submit.
            try:
                print( "Submitting for input dataset %s" % (inDS))
                print( f"\t{config.Data.inputDataset = }" )
                print( f"\t{config.Data.splitting = }" )
                print( f"\t{config.General.requestName = }" )
                crabCommand(options.crabCmd, config = config, *options.crabCmdOpts.split())
            except HTTPException as hte:
                print( "Submission for input dataset %s failed: %s" % (inDS, hte.headers))
            except ClientException as cle:
                print( "Submission for input dataset %s failed: %s" % (inDS, cle))

    # All other commands can be simply executed.
    elif options.workArea:

        for dir in os.listdir(options.workArea):
            projDir = os.path.join(options.workArea, dir)
            if not os.path.isdir(projDir):
                continue
            # Execute the crab command.
            msg = "Executing (the equivalent of): crab %s --dir %s %s" % (options.crabCmd, projDir, options.crabCmdOpts)
            print( "-"*len(msg))
            print( msg)
            print( "-"*len(msg))
            try:
                crabCommand(options.crabCmd, dir = projDir, *options.crabCmdOpts.split())
            except HTTPException as hte:
                print("Failed executing command %s for task %s: %s" % (options.crabCmd, projDir, hte.headers))
            except ClientException as cle:
                print("Failed executing command %s for task %s: %s" % (options.crabCmd, projDir, cle))


if __name__ == '__main__':
    main()

