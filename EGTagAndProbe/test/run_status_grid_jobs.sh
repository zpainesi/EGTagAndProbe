
# comment

for EGamma in {0,1}
do
   for ver in {1,2}
   do
#	for run in 0 0_v2 1_v1 1_v2# or {B,C,D}
#	do
	   source /cvmfs/cms.cern.ch/crab3/crab.sh 
	   eval `scramv1 runtime -sh` #equivalent to cmsenv

#	   crab resubmit crab_projects/crab_TnP_EGamma"$EGamma"_Run2023D_v"$ver"_newHcal_JAX_2023_19_51_eTRecalib
          crab status crab_projects/crab_TnP_EGamma"$EGamma"_Run2023D_v"$ver"_newHcal_JAX_2023_19_51_eTRecalib

# 	done
   done   
done
