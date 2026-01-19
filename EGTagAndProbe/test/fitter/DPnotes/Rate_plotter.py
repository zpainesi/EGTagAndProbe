from scipy.optimize import curve_fit
from scipy.signal import convolve
from optparse import OptionParser
import scipy.special as sp
import numpy as np
import ROOT
import sys
import os

from matplotlib.ticker import FixedLocator, FixedFormatter
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
    (options, args) = parser.parse_args()
    print(options)

    if options.obj == "tau":
        inFile = ROOT.TFile('/home/llr/cms/motta/Run3preparation/CMSSW_11_0_2/src/TauObjectsOptimization/MakeRates/histos_2022/histos_rate_EphemeralZeroBias0_'+options.tag+'_unpacked.root')
    elif options.obj == "eg":
        inFile = ROOT.TFile('/eos/cms/store/group/dpg_trigger/comm_trigger/L1Trigger/atjaiswa/DPS_Jul2023/Hist_Rate_EphemeralZB_2023C_368822.root')
    else:
        print('** ERROR : only supported objects are tau and eg')
        print('** EXITING')
        exit()

    plt.rcParams['legend.title_fontsize'] = 'small'
    cmap = matplotlib.cm.get_cmap('Set1'); imap=0
    markers = ['o', 's', '^', 'D']

    if options.obj == "tau":
        DoubleTau = inFile.Get('rate_DiTau')
        DoubleTau_Iso = inFile.Get('rate_Iso_DiTau')

        legend_title=r'Inst. Lumi = $2.05\times10^{18}\ cm^{-2}s^{-1}$'
        label_DoubleIso = r'Double-$\tau$'
        label_DoubleIso_Iso = r'Double-$\tau$ & Isolation'

        fig, ax = plt.subplots(figsize=(10,10))
        
        for rate_TH1, label in zip([DoubleTau, DoubleTau_Iso], [label_DoubleIso, label_DoubleIso_Iso]):
            x = []
            y = []
            x_err = []
            y_err = []
            for ibin in range(rate_TH1.GetNbinsX()):
                x.append(rate_TH1.GetBinLowEdge(ibin+1) + rate_TH1.GetBinWidth(ibin+1)/2.)
                y.append(rate_TH1.GetBinContent(ibin+1))
                x_err.append(rate_TH1.GetBinWidth(ibin+1)/2.)
                y_err.append(rate_TH1.GetBinError(ibin+1))


            ax.errorbar(x, y, xerr=x_err, yerr=y_err, ls='None', label=label, lw=2, marker=markers[imap], color=cmap(imap))
            imap += 1

        leg = plt.legend(loc='upper right', fontsize=20, title=legend_title)
        leg._legend_box.align = "left"
        plt.ylim(1,4E4)
        plt.xlim(0,60)
        plt.yscale('log')
        ax.yaxis.set_major_locator(FixedLocator([1, 10, 100, 1000, 10000]))
        ax.yaxis.set_major_formatter(FixedFormatter([r'1',r'10',r'$10^2$',r'$10^3$',r'$10^4$']))
        plt.xlabel(r'$E_{T}^{\tau, L1}\ [GeV]$')
        plt.ylabel(r'Rate[kHz]')
        plt.grid()
        for xtick in ax.xaxis.get_major_ticks():
            xtick.set_pad(10)
        mplhep.cms.label('Preliminary', data=True, rlabel=r'7 pb$^{-1}$ (13.6 TeV)')
        plt.savefig('various/tau_rate_'+options.tag+'.pdf')
        plt.savefig('various/tau_rate_'+options.tag+'.png')
        plt.close()

    elif options.obj == "eg":
        singleEG = inFile.Get('SingleEG_rate_inclusive')
        singleEG_TighIso = inFile.Get('SingleEG_rate_TightIso')
        DoubleEG = inFile.Get('DoubleEG_rate_inclusive')
        DoubleEG_LooseIso = inFile.Get('DoubleEG_rate_DoubleLooseIso')

        legend_title=r'Inst. Lumi = $2.0\times10^{34}\ cm^{-2}s^{-1}$'
        label_singleEG = r'Single-$e/\gamma$'
        label_singleEG_TighIso = r'Single-$e/\gamma$ & Tight Isolation'
        label_DoubleEG = r'Double-$e/\gamma$'
        label_DoubleEG_LooseIso = r'Double-$e/\gamma$ & Loose Isolation'

        fig, ax = plt.subplots(figsize=(10,10))
        
        for rate_TH1, label in zip([singleEG, singleEG_TighIso, DoubleEG, DoubleEG_LooseIso], [label_singleEG, label_singleEG_TighIso, label_DoubleEG, label_DoubleEG_LooseIso]):
            x = []
            y = []
            x_err = []
            y_err = []
            for ibin in range(rate_TH1.GetNbinsX()):
                x.append(rate_TH1.GetBinLowEdge(ibin+1) + rate_TH1.GetBinWidth(ibin+1)/2.)
                y.append(rate_TH1.GetBinContent(ibin+1))
                x_err.append(rate_TH1.GetBinWidth(ibin+1)/2.)
                y_err.append(rate_TH1.GetBinError(ibin+1))


            ax.errorbar(x, y, xerr=x_err, yerr=y_err, ls='None', label=label, lw=2, marker=markers[imap], color=cmap(imap))
            imap += 1

        leg = plt.legend(loc='upper right', fontsize=20, title=legend_title)
        leg._legend_box.align = "left"
        plt.ylim(0.04,4E4)
        plt.xlim(0,60)
        plt.yscale('log')
        ax.yaxis.set_major_locator(FixedLocator([0.1, 1, 10, 100, 1000, 10000]))
        ax.yaxis.set_major_formatter(FixedFormatter([r'$10^{-1}$',r'1',r'10',r'$10^2$',r'$10^3$',r'$10^4$']))
        plt.xlabel(r'$E_{T}^{e/\gamma, L1}\ [GeV]$')
        plt.ylabel(r'Rate [kHz]')
        plt.grid()
        for xtick in ax.xaxis.get_major_ticks():
            xtick.set_pad(10)
        mplhep.cms.label('Preliminary', data=True, rlabel=r'7 pb$^{-1}$ (13.6 TeV)')
        plt.savefig('various/eg_rate_'+options.tag+'.pdf')
        plt.savefig('various/eg_rate_'+options.tag+'.png')
        plt.close()
