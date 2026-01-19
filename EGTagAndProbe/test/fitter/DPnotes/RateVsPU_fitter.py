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

def line(x, A, B):
    return A * x + B

def parabola(x, A, B, C):
    return A * x + B * x * x + C

#######################################################################
######################### SCRIPT BODY #################################
#######################################################################

if __name__ == "__main__" :
    parser = OptionParser()
    parser.add_option("--file",  dest="file",            default=None)
    (options, args) = parser.parse_args()
    print(options)

    ## CAREFUL WITH THIS AND WITH THE PLOTTING RANGE SPECIFIED BELOW!!! THE PLOTTING RANGE AVOIDS PERFECT ZEROS!!!
    minPU = 46
    maxPU = 62+1

    inFile = ROOT.TFile(options.file)
    inTree = inFile.Get("tree")

    L1SingleEG36er2p5_rate_PUbins = []
    L1SingleIsoEG30er2p5_rate_PUbins = []
    L1DoubleIsoTau34er2p1_rate_PUbins = []
    L1DoubleIsoTau36er2p1_rate_PUbins = []
    for pu in range(minPU,maxPU):
        L1SingleEG36er2p5_rate_PUbins.append(ROOT.TH1F("L1SingleEG36er2p5_rate_PU"+str(pu), "L1SingleEG36er2p5_rate_PU"+str(pu),200,0,20))
        L1SingleIsoEG30er2p5_rate_PUbins.append(ROOT.TH1F("L1SingleIsoEG30er2p5_rate_PU"+str(pu), "L1SingleIsoEG30er2p5_rate_PU"+str(pu),200,0,20))
        L1DoubleIsoTau34er2p1_rate_PUbins.append(ROOT.TH1F("L1DoubleIsoTau34er2p1_rate_PU"+str(pu), "L1DoubleIsoTau34er2p1_rate_PU"+str(pu),200,0,20))
        L1DoubleIsoTau36er2p1_rate_PUbins.append(ROOT.TH1F("L1DoubleIsoTau36er2p1_rate_PU"+str(pu), "L1DoubleIsoTau36er2p1_rate_PU"+str(pu),200,0,20))

    for ls in range(inTree.GetEntries()):
        inTree.GetEntry(ls)

        if inTree.pileup < minPU-0.5 or inTree.pileup > maxPU-0.5: continue

        idx = int(np.round(inTree.pileup)) - minPU
        L1SingleEG36er2p5_rate_PUbins[idx].Fill(inTree.L1_SingleEG36er2p5 / (1000*23.31))
        L1SingleIsoEG30er2p5_rate_PUbins[idx].Fill(inTree.L1_SingleIsoEG30er2p5 / (1000*23.31))
        L1DoubleIsoTau34er2p1_rate_PUbins[idx].Fill(inTree.L1_DoubleIsoTau34er2p1 / (1000*23.31))
        L1DoubleIsoTau36er2p1_rate_PUbins[idx].Fill(inTree.L1_DoubleIsoTau36er2p1 / (1000*23.31))

    x = []
    y_L1SingleEG36er2p5_rate_vs_pu = []
    y_L1SingleIsoEG30er2p5_rate_vs_pu = []
    y_L1DoubleIsoTau34er2p1_rate_vs_pu = []
    y_L1DoubleIsoTau36er2p1_rate_vs_pu = []
    y_err_L1SingleEG36er2p5_rate_vs_pu = []
    y_err_L1SingleIsoEG30er2p5_rate_vs_pu = []
    y_err_L1DoubleIsoTau34er2p1_rate_vs_pu = []
    y_err_L1DoubleIsoTau36er2p1_rate_vs_pu = []
    
    for pu in range(minPU,maxPU):
        idx = pu - minPU

        x.append(pu)

        y_L1SingleEG36er2p5_rate_vs_pu.append(L1SingleEG36er2p5_rate_PUbins[idx].GetMean())
        y_L1SingleIsoEG30er2p5_rate_vs_pu.append(L1SingleIsoEG30er2p5_rate_PUbins[idx].GetMean())
        y_L1DoubleIsoTau34er2p1_rate_vs_pu.append(L1DoubleIsoTau34er2p1_rate_PUbins[idx].GetMean())
        y_L1DoubleIsoTau36er2p1_rate_vs_pu.append(L1DoubleIsoTau36er2p1_rate_PUbins[idx].GetMean())

        y_err_L1SingleEG36er2p5_rate_vs_pu.append(L1SingleEG36er2p5_rate_PUbins[idx].GetRMS())
        y_err_L1SingleIsoEG30er2p5_rate_vs_pu.append(L1SingleIsoEG30er2p5_rate_PUbins[idx].GetRMS())
        y_err_L1DoubleIsoTau34er2p1_rate_vs_pu.append(L1DoubleIsoTau34er2p1_rate_PUbins[idx].GetRMS())
        y_err_L1DoubleIsoTau36er2p1_rate_vs_pu.append(L1DoubleIsoTau36er2p1_rate_PUbins[idx].GetRMS())


    plt.rcParams['legend.title_fontsize'] = 'small'
    cmap = matplotlib.cm.get_cmap('Set1')
    plot_x = np.linspace(0,70,1000)

    fig, ax = plt.subplots(figsize=(10,10))
    ax.errorbar(x, y_L1SingleEG36er2p5_rate_vs_pu, yerr=y_err_L1SingleEG36er2p5_rate_vs_pu, ls='None', label='L1SingleEG36er2p5', lw=2, marker='o', color=cmap(0))
    p0 = [1.,1.,0.]
    param_bounds = ([[0.,0.,-2.],[10.,10.,2.]])
    #popt, pcov = curve_fit(line, x, y_L1SingleEG36er2p5_rate_vs_pu, p0, maxfev=5000, bounds=param_bounds)
    #ax.plot(plot_x, line(plot_x, *popt), '-', label='_', lw=2, color=cmap(0))
    
    ax.errorbar(x, y_L1SingleIsoEG30er2p5_rate_vs_pu, yerr=y_err_L1SingleIsoEG30er2p5_rate_vs_pu, ls='None', label='L1SingleIsoEG30er2p5', lw=2, marker='s', color=cmap(1))
    p0 = [1.,1.,0.]
    param_bounds = ([[0.,0.,-2.],[10.,10.,2.]])
    #popt, pcov = curve_fit(line, x[:-4], y_L1SingleIsoEG30er2p5_rate_vs_pu[:-4], p0, maxfev=5000, bounds=param_bounds)
    #ax.plot(plot_x, line(plot_x, *popt), '-', label='_', lw=2, color=cmap(1))
    
    leg = plt.legend(loc='upper left', fontsize=20)
    leg._legend_box.align = "left"
    plt.ylim(9,18)
    plt.xlim(40,65)
    plt.xlabel(r'Pileup')
    plt.ylabel(r'Rate[kHz]')
    plt.grid()
    for xtick in ax.xaxis.get_major_ticks():
            xtick.set_pad(10)
    mplhep.cms.label('Preliminary', data=True, rlabel=r'319 pb$^{-1}$ (13.6 TeV)')
    plt.savefig('various/eg_rate_vs_PU.pdf')
    plt.savefig('various/eg_rate_vs_PU.png')
    plt.close()


    fig, ax = plt.subplots(figsize=(10,10))
    ax.errorbar(x, y_L1DoubleIsoTau34er2p1_rate_vs_pu, yerr=y_err_L1DoubleIsoTau34er2p1_rate_vs_pu, ls='None', label='L1DoubleIsoTau34er2p1', lw=2, marker='o', color=cmap(0))
    p0 = [1.,0.]
    param_bounds = ([[0.,-5.],
                     [5.,5.]])
    print(x[1:-3])
    popt, pcov = curve_fit(line, x[1:-3], y_L1DoubleIsoTau34er2p1_rate_vs_pu[1:-3], p0, maxfev=5000, bounds=param_bounds)
    ax.plot(plot_x, line(plot_x, *popt), '-', label='_', lw=2, color=cmap(0))
    print(popt)

    ax.errorbar(x, y_L1DoubleIsoTau36er2p1_rate_vs_pu, yerr=y_err_L1DoubleIsoTau36er2p1_rate_vs_pu, ls='None', label='L1DoubleIsoTau36er2p1', lw=2, marker='s', color=cmap(1))
    p0 = [1.,0.]
    param_bounds = ([[0.,-5.],
                     [10.,5.]])
    popt, pcov = curve_fit(line, x[1:-2], y_L1DoubleIsoTau36er2p1_rate_vs_pu[1:-2], p0, maxfev=5000)#, bounds=param_bounds)
    ax.plot(plot_x, line(plot_x, *popt), '-', label='_', lw=2, color=cmap(1))

    print(popt)
    leg = plt.legend(loc='upper left', fontsize=20)
    leg._legend_box.align = "left"
    plt.ylim(8,17)
    plt.xlim(40,65)
    plt.xlabel(r'Pileup')
    plt.ylabel(r'Rate[kHz]')
    plt.grid()
    for xtick in ax.xaxis.get_major_ticks():
            xtick.set_pad(10)
    mplhep.cms.label('Preliminary', data=True, rlabel=r'319 pb$^{-1}$ (13.6 TeV)')
    plt.savefig('various/tau_rate_vs_PU.pdf')
    plt.savefig('various/tau_rate_vs_PU.png')
    plt.close()

