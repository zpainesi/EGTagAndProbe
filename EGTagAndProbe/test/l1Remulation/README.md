##  A Lean EG ReEmulation workflow for condor

### Step 1  : Make the Daughter > Parent Map json

 - Usage
 ```
 python python/makeParentDaughterJason.py -i <input jaosn to process>  -n <number of files to process>
 ```
A template file or the imput json maybe found in the `data` folder
 Two files will be produced :
    - `yetToProcess.json` remaining files in the input josn [ use this json incase u want to process the rest of fines in the original json]
    - `parentmap.json` the json  file  with daughter - parent map [ a test output for the `parentmap.json` can be found in the `data` folder]

### Step 2 : ready the cms configuration file to be used 
 - A template file is available in `cmsCfgs/TAandPReEmulated_run3.py`
 - these additional lines needs to be added to the end of your configuration file
    ```
    ## Job Customization
    try:
        with open('cfg.json','r') as f:
            customizations=json.load(f)
            process.TFileService.fileName = customizations['outName']
            process.maxEvents.input= customizations["maxEvents"]
            process.source.fileNames= customizations["inputFiles"]
            process.source.secondaryFileNames=customizations["parentFiles"]
    except:
        print("Customization file not loaded ! ")

    ```


### Step 3 Produce the Condor Jobs
 - Customize the job maker here `misc/makeL1ReEmulJobs.sh`   
