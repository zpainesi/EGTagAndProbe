Ntuple Production :

cmsDriver command used :

```
cmsDriver l1Ntuple -s RAW2DIGI   \
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
- Step 2 make Histograms
    - Uses : python/L1RecoAnalyzer.py
    - Condor Jobs
    ```sh
     ./misc/makeRunBasedAnalysis.sh   
    ```
    - See the results in `results/l1HealthAnalysis/`

- Step 3
    - Anyze the histograms using the notebook   'ipynb/conditionScan'
    


