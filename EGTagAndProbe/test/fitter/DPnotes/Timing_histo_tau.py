from scipy.optimize import curve_fit
from scipy.signal import convolve
from optparse import OptionParser
import scipy.special as sp
import numpy as np
import ROOT
import sys
import os

import argparse
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
    parser.add_option("--obj",         dest="obj",                                        default="tau")
    parser.add_option("--tag",         dest="tag",                                        default=None)
    parser.add_option("--sels",        dest="sels",        type=str, action='append',     default=None, help='selection wanted: e.g. 30(for thr>30), 30Iso(for thr>30v & isolation), 36OR30Iso(fir thr>36 OR thr>30 & isolation)')
    parser.add_option("--logx",        dest="logx",                  action='store_true', default=False)
    parser.add_option("--smallFitErr", dest="smallFitErr",           action='store_true', default=False)
    (options, args)  = parser.parse_args()
    print(options)

    inFile = ROOT.TFile('/eos/user/z/zpainesi/forL1DPS2023July/Timing_Taus_2023_wholeStat.root')
    
    plt.rcParams['legend.title_fontsize'] = 'small'
    cmap = matplotlib.cm.get_cmap('Set1'); imap=0
    plot_x = np.linspace(0,1000,2000)
    fig, ax = plt.subplots(figsize=(10,10))
    zorder = 1

    markers = ['o', 's', '^', 'D']

    spike_TH1 = inFile.Get('l1tTauBx')
    label = '$p^{L1}_{T}>20$ GeV & $p^{L1}_{T}<32$ GeV\n$p^{off}_{T}>18$ GeV & $p^{off}_{T}<30$ GeV\nTotal pre-firing fraction 0.78%\nTotal post-firing fraction 0.78%'
    
    x = []
    y = []
    x_err = []
    y_err = []

    for ibin in range(spike_TH1.GetNbinsX()):
        x.append(spike_TH1.GetBinLowEdge(ibin+1) + spike_TH1.GetBinWidth(ibin+1)/2.)
        y.append(spike_TH1.GetBinContent(ibin+1))
        x_err.append(spike_TH1.GetBinWidth(ibin+1)/2.)
        y_err.append(np.sqrt(spike_TH1.GetBinContent(ibin+1)))

    #plt.errorbar(x, y, xerr=x_err, yerr=y_err, ls='None', label=label, marker='s', lw=2, color=cmap(1), zorder=zorder)
    plt.bar(x, y, xerr=x_err, yerr=y_err, ls='None', label=label, lw=2, color=cmap(1), zorder=zorder)

    for ax in fig.get_axes():
        ax.label_outer()
    for xtick in ax.xaxis.get_major_ticks():
        xtick.set_pad(10)
    #leg = plt.legend(loc = 'upper right', fontsize=18, title=r'$|\eta^{\tau}|<2.1$')
    #leg._legend_box.align = "left"
    #plt.xlabel(r'$p_{T}^{\tau, offline}\ [GeV]$')
    mplhep.cms.label('Preliminary', data=True, rlabel=r'1.1 fb$^{-1}$ (13.6 TeV)')

    plt.ylabel(r'Counts')
    plt.xlabel(r'Bunch crossing')
    plt.xlim(-3,3)

    plt.yscale('log')
    plt.grid()
    plot_name = 'various/Timing_taus_2023_' 
    plot_name = plot_name[:-1]
    plt.savefig(plot_name+'.pdf')
    plt.savefig(plot_name+'.png')
    plt.close()


