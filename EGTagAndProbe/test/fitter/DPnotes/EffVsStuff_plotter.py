from scipy.optimize import curve_fit
from scipy.signal import convolve
from optparse import OptionParser
import scipy.special as sp
import numpy as np
import ROOT
import sys
import os

import matplotlib.pyplot as plt
import matplotlib
import mplhep
plt.style.use(mplhep.style.CMS)


#######################################################################
######################### SCRIPT BODY #################################
#######################################################################

if __name__ == "__main__" :
    parser = OptionParser()
    parser.add_option("--obj",       dest="obj",                            default="tau")
    parser.add_option("--tag",       dest="tag",                            default=None)
    parser.add_option("--thr",       dest="thr",                            default=None)
    (options, args) = parser.parse_args()
    print(options)

    if options.obj == "tau":
        inFile = ROOT.TFile('/home/llr/cms/motta/Run3preparation/CMSSW_11_0_2/src/TauObjectsOptimization/PlotTurnOns/ROOTs/efficiencies_of_Run'+options.tag+'.root')
    elif options.obj == "eg":
        print('qua')
        inFile = ROOT.TFile('/eos/cms/store/group/dpg_trigger/comm_trigger/L1Trigger/atjaiswa/DPS_Jul2023/Hist_2023Run3_EraBC_EGamma_UnpackedPerformance.root')
    else:
        print('** ERROR : only supported objects are tau and eg')
        print('** EXITING')
        exit()
    
    thr_str = options.thr
    thr = int(thr_str)

    plt.rcParams['legend.title_fontsize'] = 'small'
    cmap = matplotlib.cm.get_cmap('Set1')

    if options.obj == "tau":
        ## VERSUS NUMBER OF VERTICES
        eff_TGraph = inFile.Get('EfficiencyVsPU_L1Et30NoIso_Off32_Eta2p5')
        eff_Iso_TGraph = inFile.Get('EfficiencyVsPU_L1Et30TightIso_Off32_Eta2p5')

        # CONVERT TO LISTS FOR PYPLOT
        x = []
        y = []
        x_err = []
        y_errU = []
        y_errD = []
        x_Iso = []
        y_Iso = []
        x_err_Iso = []
        y_errU_Iso = []
        y_errD_Iso = []
        for ibin in range(eff_TGraph.GetN()):
            x.append(eff_TGraph.GetPointX(ibin))
            y.append(eff_TGraph.GetPointY(ibin))
            x_err.append(eff_TGraph.GetErrorX(ibin))
            
            errU = eff_TGraph.GetErrorYhigh(ibin)
            errD = eff_TGraph.GetErrorYlow(ibin)
            y_errU.append(errU)
            y_errD.append(errD)

            x_Iso.append(eff_Iso_TGraph.GetPointX(ibin))
            y_Iso.append(eff_Iso_TGraph.GetPointY(ibin))
            x_err_Iso.append(eff_Iso_TGraph.GetErrorX(ibin))
            
            errU = eff_Iso_TGraph.GetErrorYhigh(ibin)
            errD = eff_Iso_TGraph.GetErrorYlow(ibin)
            y_errU_Iso.append(errU)
            y_errD_Iso.append(errD)


        fig, ax = plt.subplots(figsize=(10,10))
        ax.errorbar(x, y, xerr=x_err, yerr=[y_errD, y_errU], ls='None', label=r'$E_{T}^{\tau, L1} > %i$ GeV' % (thr), lw=2, marker='o', color=cmap(0))
        ax.errorbar(x_Iso, y_Iso, xerr=x_err_Iso, yerr=[y_errD_Iso, y_errU_Iso], ls='None', label=r'$E_{T}^{\tau, L1} > %i$ GeV & Isolation' % (thr), lw=2, marker='s', color=cmap(1))
        leg = plt.legend(loc = 'lower left', fontsize=20, title=r'$p_{T}^{\tau, offline}>40\ GeV$ & $|\eta^{\tau, offline}|<2.1$')
        leg._legend_box.align = "left"
        plt.ylim(0,1.05)
        plt.xlim(5,60)
        # plt.yscale('log')
        plt.xlabel('Number of vertices')
        plt.ylabel(r'Efficiency')
        plt.grid()
        for xtick in ax.xaxis.get_major_ticks():
            xtick.set_pad(10)
        mplhep.cms.label('Preliminary', data=True, rlabel=r'18 fb$^{-1}$ (13.6 TeV)')
        plt.savefig('various/tau_eff_vs_nvtx_'+options.tag+'.pdf')
        plt.savefig('various/tau_eff_vs_nvtx_'+options.tag+'.png')
        plt.close()


        ## VERSUS ETA
        eff_TGraph = inFile.Get('divide_etaprogressionAt'+thr_str+'_noIso_by_eta')
        eff_Iso_TGraph = inFile.Get('divide_etaprogressionAt'+thr_str+'_Iso_by_eta')

        # CONVERT TO LISTS FOR PYPLOT
        x = []
        y = []
        x_err = []
        y_errU = []
        y_errD = []
        x_Iso = []
        y_Iso = []
        x_err_Iso = []
        y_errU_Iso = []
        y_errD_Iso = []
        for ibin in range(eff_TGraph.GetN()):
            x.append(eff_TGraph.GetPointX(ibin))
            y.append(eff_TGraph.GetPointY(ibin))
            x_err.append(eff_TGraph.GetErrorX(ibin))
            
            errU = eff_TGraph.GetErrorYhigh(ibin)
            errD = eff_TGraph.GetErrorYlow(ibin)
            y_errU.append(errU)
            y_errD.append(errD)

            x_Iso.append(eff_Iso_TGraph.GetPointX(ibin))
            y_Iso.append(eff_Iso_TGraph.GetPointY(ibin))
            x_err_Iso.append(eff_Iso_TGraph.GetErrorX(ibin))
            
            errU = eff_Iso_TGraph.GetErrorYhigh(ibin)
            errD = eff_Iso_TGraph.GetErrorYlow(ibin)
            y_errU_Iso.append(errU)
            y_errD_Iso.append(errD)


        fig, ax = plt.subplots(figsize=(10,10))
        ax.errorbar(x, y, xerr=x_err, yerr=[y_errD, y_errU], ls='None', label=r'$E_{T}^{\tau, L1} > %i$ GeV' % (thr), lw=2, marker='o', color=cmap(0))
        ax.errorbar(x_Iso, y_Iso, xerr=x_err_Iso, yerr=[y_errD_Iso, y_errU_Iso], ls='None', label=r'$E_{T}^{\tau, L1} > %i$ GeV & Isolation' % (thr), lw=2, marker='s', color=cmap(1))
        leg = plt.legend(loc = 'lower left', fontsize=20, title=r'$p_{T}^{\tau, offline}>40\ GeV$ & $|\eta^{\tau, offline}|<2.1$')
        leg._legend_box.align = "left"
        plt.ylim(0,1.05)
        plt.xlim(-2.1,2.1)
        # plt.yscale('log')
        plt.xlabel(r'$|\eta^{\tau, offline}|$')
        plt.ylabel(r'Efficiency')
        plt.grid()
        for xtick in ax.xaxis.get_major_ticks():
            xtick.set_pad(10)
        mplhep.cms.label('Preliminary', data=True, rlabel=r'18 fb$^{-1}$ (13.6 TeV)')
        plt.savefig('various/tau_eff_vs_eta_'+options.tag+'.pdf')
        plt.savefig('various/tau_eff_vs_eta_'+options.tag+'.png')
        plt.close()



    elif options.obj == "eg":
        ## VERSUS NUMBER OF VERTICES
        print('qua')
        eff_TGraph = inFile.Get('EfficiencyVsPU_L1Et30NoIso_OffEt32_Eta2p5')
        eff_Iso_TGraph = inFile.Get('EfficiencyVsPU_L1Et30TightIso_OffEt32_Eta2p5')

        # CONVERT TO LISTS FOR PYPLOT
        x = []
        y = []
        x_err = []
        y_err = []
        x_Iso = []
        y_Iso = []
        x_err_Iso = []
        y_err_Iso = []
        for ibin in range(eff_TGraph.GetN()):
            x.append(eff_TGraph.GetPointX(ibin))
            y.append(eff_TGraph.GetPointY(ibin))
            x_err.append(eff_TGraph.GetErrorX(ibin))

            #errU = eff_TGraph.GetErrorYhigh(ibin)
            errD = eff_TGraph.GetErrorYlow(ibin)
            #y_err.append(errU)
            y_err.append(errD)
            
            x_Iso.append(eff_Iso_TGraph.GetPointX(ibin))
            y_Iso.append(eff_Iso_TGraph.GetPointY(ibin))
            x_err_Iso.append(eff_Iso_TGraph.GetErrorX(ibin))

            #errU_Iso = eff_Iso_TGraph.GetErrorYhigh(ibin)
            errD_Iso = eff_Iso_TGraph.GetErrorYlow(ibin)
            #y_err_Iso.append(errU_Iso)
            y_err_Iso.append(errD_Iso)

           # x.append(eff_TGraph.GetBinLowEdge(ibin+1) + eff_TGraph.GetBinWidth(ibin+1)/2.)
           # y.append(eff_TGraph.GetBinContent(ibin+1))
           # x_err.append(eff_TGraph.GetBinWidth(ibin+1)/2.)
           # y_err.append(eff_TGraph.GetBinError(ibin+1))

           # x_Iso.append(eff_Iso_TGraph.GetBinLowEdge(ibin+1) + eff_Iso_TGraph.GetBinWidth(ibin+1)/2.)
           # y_Iso.append(eff_Iso_TGraph.GetBinContent(ibin+1))
           # x_err_Iso.append(eff_Iso_TGraph.GetBinWidth(ibin+1)/2.)
           # y_err_Iso.append(eff_Iso_TGraph.GetBinError(ibin+1))


        fig, ax = plt.subplots(figsize=(10,10))
        ax.errorbar(x, y, xerr=x_err, yerr=y_err, ls='None', label=r'$E_{T}^{e/\gamma, L1} > %i$ GeV' % (thr), lw=2, marker='o', color=cmap(0))
        ax.errorbar(x_Iso, y_Iso, xerr=x_err_Iso, yerr=y_err_Iso, ls='None', label=r'$E_{T}^{e/\gamma, L1} > %i$ GeV & Tight Isolation' % (thr), lw=2, marker='s', color=cmap(1))
        leg = plt.legend(loc = 'lower left', fontsize=20, title=r'$E_{T}^{e, offline}>32\ GeV$ & $|\eta^{e, offline}|<2.5$')
        leg._legend_box.align = "left"
        plt.ylim(0.5,1.0)
        plt.xlim(0,60)
        # plt.yscale('log')
        plt.xlabel('Number of vertices')
        plt.ylabel(r'Efficiency')
        plt.grid()
        for xtick in ax.xaxis.get_major_ticks():
            xtick.set_pad(10)
        mplhep.cms.label('Preliminary', data=True, rlabel=r'18 fb$^{-1}$ (13.6 TeV)')
        plt.savefig('various/eg_eff_vs_nvtx_'+options.tag+'.pdf')
        plt.savefig('various/eg_eff_vs_nvtx_'+options.tag+'.png')
        plt.close()

        ### VERSUS ETA
        #inFile = ROOT.TFile('./EG/BtoG/HistogramFile_2022BToG_EffVsEta_Off35.root')
        #eff_TGraph = inFile.Get('EG'+thr_str+'/Efficiency_vs_eleProbeEta_EG'+thr_str)
        #eff_Iso_TGraph = inFile.Get('TightIsoEG'+thr_str+'/Efficiency_vs_eleProbeEta_TightIsoEG'+thr_str)

        ## CONVERT TO LISTS FOR PYPLOT
        #x = []
        #y = []
        #x_err = []
        #y_err = []
        #x_Iso = []
        #y_Iso = []
        #x_err_Iso = []
        #y_err_Iso = []
        #for ibin in range(eff_TGraph.GetNbinsX()):
        #    x.append(eff_TGraph.GetBinLowEdge(ibin+1) + eff_TGraph.GetBinWidth(ibin+1)/2.)
        #    y.append(eff_TGraph.GetBinContent(ibin+1))
        #    x_err.append(eff_TGraph.GetBinWidth(ibin+1)/2.)
        #    y_err.append(eff_TGraph.GetBinError(ibin+1))

        #    x_Iso.append(eff_Iso_TGraph.GetBinLowEdge(ibin+1) + eff_Iso_TGraph.GetBinWidth(ibin+1)/2.)
        #    y_Iso.append(eff_Iso_TGraph.GetBinContent(ibin+1))
        #    x_err_Iso.append(eff_Iso_TGraph.GetBinWidth(ibin+1)/2.)
        #    y_err_Iso.append(eff_Iso_TGraph.GetBinError(ibin+1))


        #fig, ax = plt.subplots(figsize=(10,10))
        #ax.errorbar(x, y, xerr=x_err, yerr=y_err, ls='None', label=r'$E_{T}^{e/\gamma, L1} > %i$ GeV' % (thr), lw=2, marker='o', color=cmap(0))
        #ax.errorbar(x_Iso, y_Iso, xerr=x_err_Iso, yerr=y_err_Iso, ls='None', label=r'$E_{T}^{e/\gamma, L1} > %i$ GeV & Tight Isolation' % (thr), lw=2, marker='s', color=cmap(1))
        #leg = plt.legend(loc = 'lower left', fontsize=20, title=r'$E_{T}^{e, offline}>35\ GeV$ & $|\eta^{e, offline}|<2.5$')
        #leg._legend_box.align = "left"
        #plt.ylim(0.5,1.05)
        #plt.xlim(-2.6,2.6)
        ## plt.yscale('log')
        #plt.xlabel(r'$|\eta^{e, offline}|$')
        #plt.ylabel(r'Efficiency')
        #plt.grid()
        #for xtick in ax.xaxis.get_major_ticks():
        #    xtick.set_pad(10)
        #mplhep.cms.label('Preliminary', data=True, rlabel=r'18 fb$^{-1}$ (13.6 TeV)')
        #plt.savefig('various/eg_eff_vs_eta_'+options.tag+'.pdf')
        #plt.savefig('various/eg_eff_vs_eta_'+options.tag+'.png')
        #plt.close()
