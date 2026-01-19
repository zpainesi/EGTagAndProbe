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


def CB(x, mean=1, sigma=1, alpha=1, n=1, norm=1):
    t = (x - mean)/sigma

    # Crystal Ball definitions
    A = pow(n / abs(alpha), n) * np.exp(-0.5 * alpha * alpha)
    B = n / abs(alpha) - abs(alpha)

    if t > -alpha:
        crystalBall = norm * np.exp(-0.5 * pow(t,2))
    else:
        crystalBall = norm * A * pow(B-t,-n)

    return crystalBall

vectCB = np.vectorize(CB)
##              [ mean, sigma, alpha,    n, norm]
# p0 =          [   1.,    1.,    1.,   1.,  0.1]
# param_bounds=([ -10.,  -10.,  -10., -200.,  0. ],
#               [  10.,   10.,   10.,  200.,  1. ])

def DoubleCB(x, mean=1, sigma=1, alphaL=1, nL=1, alphaR=1, nR=1, norm=1):
    t = (x - mean)/sigma

    # Crystal Ball definitions
    Al = pow(nL / abs(alphaL), nL) * np.exp(-0.5 * alphaL * alphaL)
    Bl = nL / abs(alphaL) - abs(alphaL)
    Ar = pow(nR / abs(alphaR), nR) * np.exp(-0.5 * alphaR * alphaR)
    Br = nR / abs(alphaR) - abs(alphaR)

    if t < -alphaL:
        crystalBall = norm * Al * pow(Bl-t,-nL)
    elif t < alphaR:
        crystalBall = norm * np.exp(-0.5 * pow(t,2))
    else:
        crystalBall = norm * Ar * pow(Br+t,-nR)
        
        
    return crystalBall

vectDoubleCB = np.vectorize(DoubleCB)
##              [ mean, sigma, alphaL,   nL, alphaR,   nR, norm]
# p0 =          [   1.,    1.,     1.,   1.,     1.,   1.,  0.1]
# param_bounds=([ -10.,  -10.,   -10., -10.,   -10., -10.,  0. ],
#               [  10.,   10.,    10.,  10.,    10.,  10.,  1. ])



#######################################################################
######################### SCRIPT BODY #################################
#######################################################################

if __name__ == "__main__" :
    parser = OptionParser()
    parser.add_option("--obj",       dest="obj",                            default="tau")
    parser.add_option("--tag",       dest="tag",                            default=None)
    parser.add_option("--inclusive", dest="inclusive", action='store_true', default=False)
    (options, args) = parser.parse_args()
    print(options)

    inFile = ROOT.TFile('EG/BtoG/TagAndProbe_DataRun3Unpacked_2022_resolution_postReco_v6_skimmed_selectionsAt32GeV.root')
    phiResp_barrel    = inFile.Get('phiResolution_offlineETcut32Gev_barrel')
    phiResp_endcap    = inFile.Get('phiResolution_offlineETcut32Gev_ecap')
    phiResp_inclusive = inFile.Get('phiResolution_offlineETcut32Gev_inclusive')


    # CONVERT TO LISTS FOR PYPLOT
    x_phiResp_barrel = []
    y_phiResp_barrel = []
    x_err_phiResp_barrel = []
    y_err_phiResp_barrel = []
    x_phiResp_endcap = []
    y_phiResp_endcap = []
    x_err_phiResp_endcap = []
    y_err_phiResp_endcap = []
    x_phiResp_inclusive = []
    y_phiResp_inclusive = []
    x_err_phiResp_inclusive = []
    y_err_phiResp_inclusive = []
    for ibin in range(phiResp_barrel.GetNbinsX()):
        x_phiResp_barrel.append(phiResp_barrel.GetBinLowEdge(ibin+1) + phiResp_barrel.GetBinWidth(ibin+1)/2.)
        y_phiResp_barrel.append(phiResp_barrel.GetBinContent(ibin+1))
        x_err_phiResp_barrel.append(phiResp_barrel.GetBinWidth(ibin+1)/2.)
        y_err_phiResp_barrel.append(phiResp_barrel.GetBinError(ibin+1))

        x_phiResp_endcap.append(phiResp_endcap.GetBinLowEdge(ibin+1) + phiResp_endcap.GetBinWidth(ibin+1)/2.)
        y_phiResp_endcap.append(phiResp_endcap.GetBinContent(ibin+1))
        x_err_phiResp_endcap.append(phiResp_endcap.GetBinWidth(ibin+1)/2.)
        y_err_phiResp_endcap.append(phiResp_endcap.GetBinError(ibin+1))

        x_phiResp_inclusive.append(phiResp_inclusive.GetBinLowEdge(ibin+1) + phiResp_inclusive.GetBinWidth(ibin+1)/2.)
        y_phiResp_inclusive.append(phiResp_inclusive.GetBinContent(ibin+1))
        x_err_phiResp_inclusive.append(phiResp_inclusive.GetBinWidth(ibin+1)/2.)
        y_err_phiResp_inclusive.append(phiResp_inclusive.GetBinError(ibin+1))

    # x_phiResp_barrel_new = []
    # y_phiResp_barrel_new = []
    # x_err_phiResp_barrel_new = []
    # y_err_phiResp_barrel_new = []
    # for i in range(0,len(x_phiResp_barrel)-1,2):
    #     x_phiResp_barrel_new.append((x_phiResp_barrel[i]+x_phiResp_barrel[i+1])/2)
    #     y_phiResp_barrel_new.append((y_phiResp_barrel[i]+y_phiResp_barrel[i+1]))
    #     x_err_phiResp_barrel_new.append(x_err_phiResp_barrel[i]+x_err_phiResp_barrel[i+1])
    #     if y_phiResp_barrel[i]+y_phiResp_barrel[i+1]>0:
    #         y_err_phiResp_barrel_new.append((y_err_phiResp_barrel[i]*y_phiResp_barrel[i]+y_err_phiResp_barrel[i+1]*y_phiResp_barrel[i+1])/(y_phiResp_barrel[i]+y_phiResp_barrel[i+1]))
    #     else:
    #         y_err_phiResp_barrel_new.append(0.0)

    # x_phiResp_barrel = x_phiResp_barrel_new
    # y_phiResp_barrel = y_phiResp_barrel_new
    # x_err_phiResp_barrel = x_err_phiResp_barrel_new
    # y_err_phiResp_barrel = y_err_phiResp_barrel_new

    x_lim = (-0.15,0.15)
    x_label = r'$\phi^{e/\gamma, L1}-\phi^{e, offline}$'
    barrel_label = r'$Barrel\ |\eta^{e, offline}|<1.440$'
    endcap_label = r'$Endcaps\ 1.479<|\eta^{e, offline}|<2.5$'
    inclusive_label = r'$Inclusive\ |\eta^{e, offline}|<2.5$'
    legend_title = r'$E_{T}^{e, offline}>32\ GeV$'
    plot_name = 'responses/eg_phi_response_'+options.tag
    if options.inclusive: plot_name = plot_name[:-4] + '_inclusive'

    plt.rcParams['legend.title_fontsize'] = 'small'
    cmap = matplotlib.cm.get_cmap('Set1')
    plot_x = np.linspace(-3,3,4000)
    fig, ax = plt.subplots(figsize=(10,10))

    fig, ax = plt.subplots(figsize=(10,10))
    
    ax.errorbar(x_phiResp_barrel, y_phiResp_barrel, xerr=x_err_phiResp_barrel, yerr=y_err_phiResp_barrel, ls='None', label=barrel_label, lw=2, marker='o', color=cmap(0))
    ##            [ mean, sigma, alphaL,    nL, alphaR,    nR,  norm]
    p0 =          [   0.,  0.05,     1.,    1.,     1.,    1.,  0.1]
    param_bounds=([ -10.,  0.  ,     0.,    0.,     0.,    0.,  0. ],
                  [  10.,  0.5 ,    10.,   50.,    10.,   50.,  1. ])
    popt, pcov = curve_fit(vectDoubleCB, x_phiResp_barrel[2:], y_phiResp_barrel[2:], p0, maxfev=5000, bounds=param_bounds)
    ax.plot(plot_x, vectDoubleCB(plot_x, *popt), '-', label='_', lw=2, color=cmap(0))

    ax.errorbar(x_phiResp_endcap, y_phiResp_endcap, xerr=x_err_phiResp_endcap, yerr=y_err_phiResp_endcap, ls='None', label=endcap_label, lw=2, marker='s', color=cmap(1))
    ##            [ mean, sigma, alphaL,    nL, alphaR,    nR,  norm]
    p0 =          [   0.,  0.05,     1.,    1.,     1.,    1.,  0.1]
    param_bounds=([ -10.,  0.  ,     0.,    0.,     0.,    0.,  0. ],
                  [  10.,  0.5 ,    10.,   25.,    10.,   25.,  1. ])
    popt, pcov = curve_fit(vectDoubleCB, x_phiResp_endcap[2:], y_phiResp_endcap[2:], p0, maxfev=5000, bounds=param_bounds)
    ax.plot(plot_x, vectDoubleCB(plot_x, *popt), '-', label='_', lw=2, color=cmap(1))

    if options.inclusive:
        ax.errorbar(x_phiResp_inclusive, y_phiResp_inclusive, xerr=x_err_phiResp_inclusive, yerr=y_err_phiResp_inclusive, ls='None', label=inclusive_label, lw=2, marker='^', color=cmap(2))
        ##            [ mean, sigma, alphaL,    nL, alphaR,    nR,  norm]
        p0 =          [   0.,  0.05,     1.,    1.,     1.,    1.,  0.1]
        param_bounds=([ -10.,  0.  ,     0.,    0.,     0.,    0.,  0. ],
                      [  10.,  0.5 ,    10.,   50.,    10.,   50.,  1. ])
        popt, pcov = curve_fit(vectDoubleCB, x_phiResp_inclusive[2:], y_phiResp_inclusive[2:], p0, maxfev=5000, bounds=param_bounds)
        ax.plot(plot_x, vectDoubleCB(plot_x, *popt), '-', label='_', lw=2, color=cmap(2))

    leg = plt.legend(loc = 'upper left', fontsize=20, title=legend_title)
    leg._legend_box.align = "left"
    plt.ylim(0., max(max(y_phiResp_barrel), max(y_phiResp_endcap)) * 1.4)
    plt.xlim(x_lim)
    # plt.yscale('log')
    plt.xlabel(x_label)
    plt.ylabel(r'a.u.')
    plt.grid()
    for xtick in ax.xaxis.get_major_ticks():
        xtick.set_pad(10)
    mplhep.cms.label('Preliminary', data=True, rlabel=r'34 fb$^{-1}$ (13.6 TeV)')
    plt.savefig(plot_name+'.pdf')
    plt.savefig(plot_name+'.png')
    plt.close()
    

    





























