#include <iostream>
#include <fstream>
#include "TSystem.h"
#include "TROOT.h"
#include <iomanip>
#include <string>
#include <vector>
#include <stdlib.h>
#include <fstream>

void Run_plotting()
{
        gSystem->Load("/afs/cern.ch/user/c/ckoraka/L1EG_for_UL2016/CMSSW_10_6_8/src/EGTagAndProbe/EGTagAndProbe/test/fitter/results2016_UL/draw_turnons_C.so");

        char InputFiles_Data[8][1000] = {
        "Efficiency_UL2016_L1_EG30_RunB.root",
        "Efficiency_UL2016_L1_EG30_RunG.root",
        "Efficiency_UL2016_L1_EG40_RunB.root",
        "Efficiency_UL2016_L1_EG40_RunG.root",
        "Efficiency_UL2016_L1_TightIsoEG32_RunB.root",
        "Efficiency_UL2016_L1_TightIsoEG32_RunG.root",
	"Efficiency_UL2016_L1_SingleNoIsoEG32_vs_nvtx_offlEt40_RunB.root",
	"Efficiency_UL2016_L1_SingleNoIsoEG32_vs_nvtx_offlEt40_RunG.root"
        };
/*
        char InputFiles_MC[8][1000] = {
        "Efficiency_UL2016_L1_EG30_106X_mcRun2_asymptotic_preVFP_v3_UL16_CP5.root",
        "Efficiency_UL2016_L1_EG30_mcRun2_asymptotic_v9_UL16_CP5_postVFP-v1.root",
        "Efficiency_UL2016_L1_EG40_106X_mcRun2_asymptotic_preVFP_v3_UL16_CP5.root",
        "Efficiency_UL2016_L1_EG40_mcRun2_asymptotic_v9_UL16_CP5_postVFP-v1.root",
        "Efficiency_UL2016_L1_TightIsoEG32_106X_mcRun2_asymptotic_preVFP_v3_UL16_CP5.root",
	"Efficiency_UL2016_L1_TightIsoEG32_mcRun2_asymptotic_v9_UL16_CP5_postVFP-v1.root",
	"Efficiency_UL2016_L1_SingleNoIsoEG32_vs_nvtx_offlEt40_106X_mcRun2_asymptotic_preVFP_v3_UL16_CP5.root",
	"Efficiency_UL2016_L1_SingleNoIsoEG32_vs_nvtx_offlEt40_mcRun2_asymptotic_v9_UL16_CP5_postVFP-v1.root"
        };

*/

        char InputFiles_MC[8][1000] = {
        "Efficiency_UL2016_L1_EG30_RunG.root",
        "Efficiency_UL2016_L1_EG30_RunB.root",
        "Efficiency_UL2016_L1_EG40_RunG.root",
        "Efficiency_UL2016_L1_EG40_RunB.root",
        "Efficiency_UL2016_L1_TightIsoEG32_RunG.root",
        "Efficiency_UL2016_L1_TightIsoEG32_RunB.root",
	"Efficiency_UL2016_L1_SingleNoIsoEG32_vs_nvtx_offlEt40_RunG.root",
	"Efficiency_UL2016_L1_SingleNoIsoEG32_vs_nvtx_offlEt40_RunB.root"
        };

/*
        char InputFiles_Data[8][1000] = {
        "Efficiency_UL2016_L1_EG30_mcRun2_asymptotic_v9_UL16_CP5_postVFP-v1.root",
        "Efficiency_UL2016_L1_EG30_106X_mcRun2_asymptotic_preVFP_v3_UL16_CP5.root",
        "Efficiency_UL2016_L1_EG40_mcRun2_asymptotic_v9_UL16_CP5_postVFP-v1.root",
        "Efficiency_UL2016_L1_EG40_106X_mcRun2_asymptotic_preVFP_v3_UL16_CP5.root",
	"Efficiency_UL2016_L1_TightIsoEG32_mcRun2_asymptotic_v9_UL16_CP5_postVFP-v1.root",
        "Efficiency_UL2016_L1_TightIsoEG32_106X_mcRun2_asymptotic_preVFP_v3_UL16_CP5.root",
	"Efficiency_UL2016_L1_SingleNoIsoEG32_vs_nvtx_offlEt40_mcRun2_asymptotic_v9_UL16_CP5_postVFP-v1.root",
	"Efficiency_UL2016_L1_SingleNoIsoEG32_vs_nvtx_offlEt40_106X_mcRun2_asymptotic_preVFP_v3_UL16_CP5.root"
        };

*/
        char InputHistos[8][1000] = {
        "histo_SingleEG30",
        "histo_SingleEG30",
        "histo_SingleEG40",
        "histo_SingleEG40",
        "histo_SingleTightIsoEG32",
        "histo_SingleTightIsoEG32",
        "histo_SingleNoIsoEG32_vs_nvtx_offlEt40",
        "histo_SingleNoIsoEG32_vs_nvtx_offlEt40"
        };
        char PlotNames[8][1000] = {
        "L1EG_Efficiency2016UL_L130_Pre",
        "L1EG_Efficiency2016UL_L130_Post",
        "L1EG_Efficiency2016UL_L140_Pre",
        "L1EG_Efficiency2016UL_L140_Post",
        "L1EG_Efficiency2016UL_TightIso32_Pre",
        "L1EG_Efficiency2016UL_TightIso32_Post",
        "L1EG_Efficiency2016UL_vs_Nvtx_Pre",
        "L1EG_Efficiency2016UL_vs_Nvtx_Post"
        };

        for (int i = 6; i<8; ++i)
        {
                if(i%2==0) draw_turnons(PlotNames[i],"UL 2016 RunB+RunG",InputFiles_Data[i],InputFiles_MC[i],InputHistos[i],i);
                //else draw_turnons(PlotNames[i],"UL 2016 RunG",InputFiles_Data[i],InputFiles_MC[i],InputHistos[i],i);
                //if(i%2==0) draw_turnons(PlotNames[i],"UL 2016 RunB",InputFiles_Data[i],InputFiles_MC[i],InputHistos[i],i);
                //else draw_turnons(PlotNames[i],"UL 2016 RunG",InputFiles_Data[i],InputFiles_MC[i],InputHistos[i],i);
        }

}


