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

### Step Produce the Condor Jobs
 - Customize the job maked here `misc/makeL1ReEmulJobs.sh`   
