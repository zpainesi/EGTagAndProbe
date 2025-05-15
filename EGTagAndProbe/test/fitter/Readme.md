## Performance Turons and Resolutions 

#### Make turnon executables 

* Turnon/Resolutions wrt. SC Et
```
make  turn_on_productionSCEt
make resolution_productionSCEt
```
* Turnons/Resolutions wrt.  PF Et
```
make  turn_on_productionPFEt
make resolution_productionPFEt
```
How to run :
```
./turn_on_productionPFEt.exe cfgs/turnon/customize_turnon_Unp.cfg
./resolution_productionPFEt.exe cfgs/reso/customize_reso_Unp.cfg
```
* Turnons/Resolutions wrt. Gen Et // Only for MC
```
make  turn_on_productionGenEt
make resolution_productionGenEt
```

Running the executable

```
./turn_on_productionXXXEt.exe cfgs/turnon/customize_turnon_Unp.cfg
./resolution_productionXXXEt.exe cfgs/reso/customize_reso_Unp.cfg
```

#### Rates 
Making the rate measuring executable 
```
make L1EGRatePlotter
```
customize the `cfgs/ParList_EGRate.dat` and `cfgs/Ntuple_rate.txt`
```
./L1EGRatePlotter.exe cfgs/ParList_EGRate.dat
```

#### Making plots
One can use the jupyter-notebooks in `notebooks/` to make the plots

PS : For posibility of easy customization , all the functions/classes have been kept on the to of the notebook rather than making it into a py module. Please use code-folding nb-extension for clutter free workspace .

TODO : Replace the notebooks with ,cleaner py scrips for each collection of plots .

---
**Configuration**
```bash
MaxEntries=< number of events to process , -1 for all evts> 
RunNumbers=1 < run number to process , 1 for MC >
doEmulationBranches=< 1 : if tru, 0 if u want to do unpacked >
prefix=< prefix to filename , cam be path to destination folder >
ofileName=< output file name  , actual out wpath will be prefix + ofileName >
reportEvery=< monitoring frequency >
infile=< input file name>
treeName=< name of the tree in file  , "Ntuplizer/TagAndProbe" in most of the cases>
doAllRuns=< 1 to integrate the results of all the runs in RunNumbers, 0 to do results for each run separately>

```
