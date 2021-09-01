## Rate Calculations

For calculating the rate of the L1 e/g objects, one should use the l1Ntuples. Instructions on how to set-up the appropriate environment can be found here : 

https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideL1TStage2Instructions#Environment_Setup_with_Integrati


### Producing L1 Ntuples

In order to produce the l1Ntuples, one can follow the following insrtuctions. For the rate calculations, the ZeroBias samples are used.

https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideL1TStage2Instructions#Making_L1Ntuple


An example of a command used to produce the l1Ntuples for 2018 data is the following :

'''
cmsDriver.py l1Ntuple -s RAW2DIGI --python_filename=data.py -n -1 --no_output --era=Run2_2018 --data 
--conditions=102X_dataRun2_Prompt_v4 --customise=L1Trigger/Configuration/customiseReEmul.L1TReEmulFromRAW 
--customise=L1Trigger/L1TNtuples/customiseL1Ntuple.L1NtupleRAWEMU 
--customise=L1Trigger/Configuration/customiseSettings.L1TSettingsToCaloParams_2018_v1_3 –
filein=/store/data/Run2018D/ZeroBias/RAW/v1/000/321/457/00000/28BC606A-86A3-E811-A897-02163E00B16E.root
'''

### Calculating the Rate

The **rate_calculation.C** macro is used to derive the L1 e/g rate. The rate plots are usually derived as a function of the ET threshold. 

Make sure to modify the correct number of bunches and luminosity for the run you are considering using the following link :  
https://cmsoms.cern.ch/cms/runs/report?cms_run=322079&cms_run_sequence=GLOBAL-RUN


By clicking on **Fill**
https://cmsoms.cern.ch/cms/fills/report?cms_fill=7118


The number of bunches can be found under the name **Colliding Bunches**.


The rate then is computed as:


Rate = N_pass/N_total x scale


where : 

**N_tot** : Number of zero bias events
**N_pass** : Number of zero bias events that pass the trigger that we are testing 
**scale** : frequency at which you see a zero bias event at the LHC (Hz) = (bunch revolution frequency) x (#bunches) = (c/27km) x #bunches = 11245.6 x #bunches (1900-2600)

