Ntuple Production :

cmsDriver command used :

```
cmsDriver.py l1Ntuple -s RAW2DIGI   \
        --python_filename=egcheck.py -n 6060   \
		--no_output   \
		--era=Run3   \
		--data   \
		--conditions=124X_dataRun3_Prompt_v5   \
        --customise=L1Trigger/Configuration/customiseReEmul.L1TReEmulFromRAW   \
		--customise=L1Trigger/Configuration/customiseSettings.L1TSettingsToCaloParams_2022_v0_4   \
		--customise=L1Trigger/L1TNtuples/customiseL1Ntuple.L1NtupleAODRAWEMUCalo   \
		--filein=/store/data/Run2022E/EGamma/RAW-RECO/ZElectron-PromptReco-v1/000/359/356/00000/7c91b7a4-74cd-4fa4-9b5f-daef651d6471.root   \
		--no_exec
```
## Fix for missing tau objects in the file
- The raw configuration fil produced in the previous step does not run out of the box due to some issue with the `offline Tau reco treee` sequence. This part need to be manually commented out for smooth running. In egcheck.py  ,Add the following line
```py
process.L1NtupleAODCalo = cms.Sequence(process.l1EventTree+process.l1RecoTree+process.l1JetRecoTree+process.l1MetFilterRecoTree+process.l1ElectronRecoTree)
```
Use `egcheck.py` for crab jobs that make the Ntuples, 


### Analysis scheme
- Setup the list of files to be processed here `fileList/eraXX.fls`

- Step 1 : making run vs File map
    - Uses  : python/eventScanner.py
    - Condor job 
        ```sh
           ./misc/runScan.sh
        ```
    - submit jobs
    - merge all the txt file made [ not the extensuion is root but they are txt ]
       ```
       cat results/l1RunNumbberScan/1p1/* > allRunNumbers
       ```
    - produce the run --> File Map json
      ```
      python3  misc/makRunFileMapJson.py
      ```
    - produce the fill -> File Map json
      ```
      python3 misc/makeFillFileMap.py  --fillInuput data.csv --runInuput misc/runFileMapEraG.json
      ```
- Step 2 make Histograms
    - Uses : python/L1RecoAnalyzer.py
    - set the proper `certification json` in the template configuration [ if no need for imposing cerfication , remove the specific line from the template json]
        - misc/cfg/l1PerformanceV1.cfg
    - Condor Jobs
    ```sh
     ./misc/makeRunBasedAnalysis.sh    # For Run Based
     ./misc/makeFillBasedAnalysis.sh   # For Fill Based
    ```
    - See the results in `results/l1HealthAnalysis/`

- Step 3
    - Anyze the histograms using the notebook   'ipynb/conditionScan'
    


### Producing TagAndProbe Ntuples
 `python/L1TAndPProducer.py` merges the objects from TagAndProbe Ntuple and l1Ntuples from Raw-Reco re-emulated jobs.  It can take in a `certification json` as well. Note :
   - The `certification json` should be hardcoded in the templte json being used
   - Path to the `TagAndProbe Unpacked` ntuples are also hardcoded in the template config.
 The template config being used is : `misc/cfg/l1TagAndP.cfg`
 
 The jobs can be parallalized in condor. Execute the `misc/makeRunBasedTandPMaker.sh` after customization to make the condor jobs. This step uses the run vs file map produced for the previos workflow
