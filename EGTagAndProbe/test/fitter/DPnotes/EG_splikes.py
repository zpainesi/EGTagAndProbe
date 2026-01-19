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

    inFile = ROOT.TFile('/eos/user/z/zpainesi/forL1DPS2023/ECAL_Spikes_2023__nonIso_er1p5.root')
    
    plt.rcParams['legend.title_fontsize'] = 'small'
    cmap = matplotlib.cm.get_cmap('Set1'); imap=0
    plot_x = np.linspace(0,1000,2000)
    fig, (ax1, ax2) = plt.subplots(2, figsize=(10,10))
    zorder = 1

    markers = ['o', 's', '^', 'D']

    spike_TH1 = inFile.Get('spikesRatio2023_vs_L1threshold')
    label = r'Zero Bias 2023'
    
    spike_TH1_ratio = inFile.Get('doubleRatio_2023over2022')
    label_ratio = r'Spike ratio'

    x = []
    y = []
    x_err = []
    y_err = []

    for ibin in range(spike_TH1.GetNbinsX()):
        x.append(spike_TH1.GetBinLowEdge(ibin+1) + spike_TH1.GetBinWidth(ibin+1)/2.)
        y.append(spike_TH1.GetBinContent(ibin+1))
        x_err.append(spike_TH1.GetBinWidth(ibin+1)/2.)
        y_err.append(spike_TH1.GetBinError(ibin+1))

    ax1.errorbar(x, y, xerr=x_err,yerr=y_err, ls='None', lw=2, color=cmap(imap), zorder=zorder)
    zorder = zorder + 1
    imap+=1

    x = []
    y = []
    x_err = []
    y_err = []

    for ibin in range(spike_TH1_ratio.GetNbinsX()):
        x.append(spike_TH1_ratio.GetBinLowEdge(ibin+1) + spike_TH1_ratio.GetBinWidth(ibin+1)/2.)
        y.append(spike_TH1_ratio.GetBinContent(ibin+1))
        x_err.append(spike_TH1_ratio.GetBinWidth(ibin+1)/2.)
        y_err.append(spike_TH1_ratio.GetBinError(ibin+1))

    ax2.errorbar(x, y, xerr=x_err,yerr=y_err, ls='None', lw=2, color=cmap(imap), zorder=zorder)

    
    plt.xlim(0., 200.)

    for ax in fig.get_axes():
        ax.label_outer()
        for xtick in ax.xaxis.get_major_ticks():
            xtick.set_pad(10)
    leg = ax1.legend(loc = 'upper right', fontsize=20, title=r'$|\eta^{e/\gamma}|<1.5$')
    leg = ax2.legend(loc = 'upper right', fontsize=20, title=r'$|\eta^{e/\gamma}|<1.5$')
    leg._legend_box.align = "left"
    #plt.xlabel(r'$p_{T}^{\tau, offline}\ [GeV]$')
    mplhep.cms.label('Preliminary', ax=ax1, data=True, rlabel=r'18 fb$^{-1}$ (13.6 TeV)')

    ax1.set_xlim(0., 200.)
    ax2.set_xlim(0., 200.)
    ax2.set_ylim(0.0, 2.5)
    ax1.set_ylabel(r'Spikes fraction')
    ax2.set_ylabel(r'$\frac{\mathrm{Spikes\ fraction\ in\ 2023}}{\mathrm{Spikes\ fraction\ in\ 2022}}$')
    ax2.set_xlabel(r'$E_{T}^{e/\gamma, L1}$ [GeV]')
    ax2.axhline(y=1, color='black', linestyle='--', label='y=1')

    ax1.grid()
    ax2.grid()
    plot_name = 'various/2023_eg_spikes_' 
    plot_name = plot_name[:-1]
    plt.savefig(plot_name+'.pdf')
    plt.savefig(plot_name+'.png')
    plt.close()


