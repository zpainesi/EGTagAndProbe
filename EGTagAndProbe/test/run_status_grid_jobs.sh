
# comment

for EGamma in {0,1}
do
   for ver in {"emu_run3Id_ManualShift1","emu_run3Id_ManualShift2","unpacked_run3Id"}
   do
#	for run in 0 0_v2 1_v1 1_v2# or {B,C,D}
#	do
	   source /cvmfs/cms.cern.ch/crab3/crab.sh 
	   eval `scramv1 runtime -sh` #equivalent to cmsenv

#	   crab resubmit crab_projects/crab_TandP_EG"$EGamma"_2024G_"$ver"
          crab status crab_projects/crab_TandP_EG"$EGamma"_2024G_"$ver"

# 	done
   done   
done
