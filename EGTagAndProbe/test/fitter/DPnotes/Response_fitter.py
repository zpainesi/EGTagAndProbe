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

    if options.obj == "tau":
        inFile = ROOT.TFile('/home/llr/cms/motta/Run3preparation/CMSSW_11_0_2/src/TauObjectsOptimization/PlotCalibrationResolution/ROOTs/resolutions_of_Run'+options.tag+'.root')
    
        scale_barrel      = inFile.Get('pt_barrel_resp_ptInclusive')
        scale_endcap      = inFile.Get('pt_endcap_resp_ptInclusive')
        scale_inclusive   = inFile.Get('pt_response_ptInclusive')
        ptResol_barrel    = inFile.Get('pt_resol_barrel_fctPt')
        ptResol_endcap    = inFile.Get('pt_resol_endcap_fctPt')
        ptResol_inclusive = inFile.Get('pt_resol_fctPt')
        etaResp_barrel    = inFile.Get('eta_resp_barrel')
        etaResp_endcap    = inFile.Get('eta_resp_endcap')
        etaResp_inclusive = inFile.Get('eta_resp_inclusive')
        phiResp_barrel    = inFile.Get('phi_resp_barrel')
        phiResp_endcap    = inFile.Get('phi_resp_endcap')
        phiResp_inclusive = inFile.Get('phi_resp_inclusive')

    elif options.obj == "eg":
        inFile = ROOT.TFile('/eos/cms/store/group/dpg_trigger/comm_trigger/L1Trigger/atjaiswa/DPS_Jul2023/Hist_2023Run3_EraBC_EGamma_UnpackedPerformance.root')

        # if   '20GeV' in options.tag: cut = '20'
        # elif '32GeV' in options.tag: cut = '32'
        # else:
        #     print('** WARNING : wrong tag')
        #     print('** EXITING')
        #     exit()

        cut = '32'
        scale_barrel      = inFile.Get('EnergyResponse_OffEt'+cut+'_Barrel')
        scale_endcap      = inFile.Get('EnergyResponse_OffEt'+cut+'_EndCap')
        scale_inclusive   = inFile.Get('EnergyResponse_OffEt'+cut+'_Barrel')
        ptResol_barrel    = inFile.Get('EnergyResolutionVsOffEt_Barrel')
        ptResol_endcap    = inFile.Get('EnergyResolutionVsOffEt_EndCap')
        ptResol_inclusive = inFile.Get('EnergyResolutionVsOffEt_Barrel')
        etaResp_barrel    = inFile.Get('EtaResponse_OffEt'+cut+'_Barrel')
        etaResp_endcap    = inFile.Get('EtaResponse_OffEt'+cut+'_EndCap')
        etaResp_inclusive = inFile.Get('EtaResponse_OffEt'+cut+'_Barrel')
        phiResp_barrel    = inFile.Get('PhiResponse_OffEt'+cut+'_Barrel')
        phiResp_endcap    = inFile.Get('PhiResponse_OffEt'+cut+'_EndCap')
        phiResp_inclusive = inFile.Get('PhiResponse_OffEt'+cut+'_Barrel')

    # CONVERT TO LISTS FOR PYPLOT
    x_scale_barrel = []
    y_scale_barrel = []
    x_err_scale_barrel = []
    y_err_scale_barrel = []
    x_scale_endcap = []
    y_scale_endcap = []
    x_err_scale_endcap = []
    y_err_scale_endcap = []
    x_scale_inclusive = []
    y_scale_inclusive = []
    x_err_scale_inclusive = []
    y_err_scale_inclusive = []
    for ibin in range(scale_barrel.GetNbinsX()):
        x_scale_barrel.append(scale_barrel.GetBinLowEdge(ibin+1) + scale_barrel.GetBinWidth(ibin+1)/2.)
        y_scale_barrel.append(scale_barrel.GetBinContent(ibin+1))
        x_err_scale_barrel.append(scale_barrel.GetBinWidth(ibin+1)/2.)
        y_err_scale_barrel.append(scale_barrel.GetBinError(ibin+1))

        x_scale_endcap.append(scale_endcap.GetBinLowEdge(ibin+1) + scale_endcap.GetBinWidth(ibin+1)/2.)
        y_scale_endcap.append(scale_endcap.GetBinContent(ibin+1))
        x_err_scale_endcap.append(scale_endcap.GetBinWidth(ibin+1)/2.)
        y_err_scale_endcap.append(scale_endcap.GetBinError(ibin+1))

        x_scale_inclusive.append(scale_inclusive.GetBinLowEdge(ibin+1) + scale_inclusive.GetBinWidth(ibin+1)/2.)
        y_scale_inclusive.append(scale_inclusive.GetBinContent(ibin+1))
        x_err_scale_inclusive.append(scale_inclusive.GetBinWidth(ibin+1)/2.)
        y_err_scale_inclusive.append(scale_inclusive.GetBinError(ibin+1))

    # CONVERT TO LISTS FOR PYPLOT
    x_ptResol_barrel = []
    y_ptResol_barrel = []
    x_err_ptResol_barrel = []
    y_err_ptResol_barrel = []
    x_ptResol_endcap = []
    y_ptResol_endcap = []
    x_err_ptResol_endcap = []
    y_err_ptResol_endcap = []
    x_ptResol_inclusive = []
    y_ptResol_inclusive = []
    x_err_ptResol_inclusive = []
    y_err_ptResol_inclusive = []
    for ibin in range(ptResol_barrel.GetNbinsX()):
        x_ptResol_barrel.append(ptResol_barrel.GetBinLowEdge(ibin+1) + ptResol_barrel.GetBinWidth(ibin+1)/2.)
        y_ptResol_barrel.append(ptResol_barrel.GetBinContent(ibin+1))
        x_err_ptResol_barrel.append(ptResol_barrel.GetBinWidth(ibin+1)/2.)
        y_err_ptResol_barrel.append(ptResol_barrel.GetBinError(ibin+1))

        x_ptResol_endcap.append(ptResol_endcap.GetBinLowEdge(ibin+1) + ptResol_endcap.GetBinWidth(ibin+1)/2.)
        y_ptResol_endcap.append(ptResol_endcap.GetBinContent(ibin+1))
        x_err_ptResol_endcap.append(ptResol_endcap.GetBinWidth(ibin+1)/2.)
        y_err_ptResol_endcap.append(ptResol_endcap.GetBinError(ibin+1))

        x_ptResol_inclusive.append(ptResol_inclusive.GetBinLowEdge(ibin+1) + ptResol_inclusive.GetBinWidth(ibin+1)/2.)
        y_ptResol_inclusive.append(ptResol_inclusive.GetBinContent(ibin+1))
        x_err_ptResol_inclusive.append(ptResol_inclusive.GetBinWidth(ibin+1)/2.)
        y_err_ptResol_inclusive.append(ptResol_inclusive.GetBinError(ibin+1))

    # CONVERT TO LISTS FOR PYPLOT
    x_etaResp_barrel = []
    y_etaResp_barrel = []
    x_err_etaResp_barrel = []
    y_err_etaResp_barrel = []
    x_etaResp_endcap = []
    y_etaResp_endcap = []
    x_err_etaResp_endcap = []
    y_err_etaResp_endcap = []
    x_etaResp_inclusive = []
    y_etaResp_inclusive = []
    x_err_etaResp_inclusive = []
    y_err_etaResp_inclusive = []
    for ibin in range(etaResp_barrel.GetNbinsX()):
        x_etaResp_barrel.append(etaResp_barrel.GetBinLowEdge(ibin+1) + etaResp_barrel.GetBinWidth(ibin+1)/2.)
        y_etaResp_barrel.append(etaResp_barrel.GetBinContent(ibin+1))
        x_err_etaResp_barrel.append(etaResp_barrel.GetBinWidth(ibin+1)/2.)
        y_err_etaResp_barrel.append(etaResp_barrel.GetBinError(ibin+1))

        x_etaResp_endcap.append(etaResp_endcap.GetBinLowEdge(ibin+1) + etaResp_endcap.GetBinWidth(ibin+1)/2.)
        y_etaResp_endcap.append(etaResp_endcap.GetBinContent(ibin+1))
        x_err_etaResp_endcap.append(etaResp_endcap.GetBinWidth(ibin+1)/2.)
        y_err_etaResp_endcap.append(etaResp_endcap.GetBinError(ibin+1))

        x_etaResp_inclusive.append(etaResp_inclusive.GetBinLowEdge(ibin+1) + etaResp_inclusive.GetBinWidth(ibin+1)/2.)
        y_etaResp_inclusive.append(etaResp_inclusive.GetBinContent(ibin+1))
        x_err_etaResp_inclusive.append(etaResp_inclusive.GetBinWidth(ibin+1)/2.)
        y_err_etaResp_inclusive.append(etaResp_inclusive.GetBinError(ibin+1))

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
    
    

    # PLOT PT SCALE
    if options.obj == 'tau':
        x_lim = (0.,2.)
        x_label = r'$E_{T}^{\tau, L1}/p_{T}^{\tau, offline}$'
        barrel_label = r'$Barrel\ |\eta^{\tau, offline}|<1.305$'
        endcap_label = r'$Endcaps\ 1.479<|\eta^{\tau, offline}|<2.1$'
        inclusive_label = r'$Inclusive\ |\eta^{\tau, offline}|<2.1$'
        legend_title = r'$p_{T}^{\tau, offline}>30\ GeV$'
        plot_name = 'responses/tau_pt_scale_'+options.tag
        if options.inclusive: plot_name = plot_name[:-4] + '_inclusive'

    elif options.obj == 'eg':
        x_lim = (0.7,1.3)
        x_label = r'$E_{T}^{e/\gamma, L1}/E_{T}^{e, offline}$'
        barrel_label = r'$Barrel\ |\eta^{e, offline}|<1.440$'
        endcap_label = r'$Endcaps\ 1.479<|\eta^{e, offline}|<2.5$'
        inclusive_label = r'$Inclusive\ |\eta^{e, offline}|<2.5$'
        legend_title = r'$E_{T}^{e, offline}>$'+cut+r'$\ GeV$'
        plot_name = 'responses/eg_pt_scale_'+options.tag
        if options.inclusive: plot_name = plot_name[:-4] + '_inclusive'

    plt.rcParams['legend.title_fontsize'] = 'small'
    cmap = matplotlib.cm.get_cmap('Set1')
    plot_x = np.linspace(-3,3,4000)
    fig, ax = plt.subplots(figsize=(10,10))
    
    ax.errorbar(x_scale_barrel, y_scale_barrel, xerr=x_err_scale_barrel, yerr=y_err_scale_barrel, ls='None', label=barrel_label, lw=2, marker='o', color=cmap(0))
    ##            [ mean, sigma, alphaL,    nL, alphaR,    nR,  norm]
    p0 =          [   1.,   0.2,     1.,    1.,     1.,    1.,  0.1]
    param_bounds=([ -10.,   0. ,     0.,    0.,     0.,    0.,  0. ],
                  [  10.,   1. ,    10.,   50.,    10.,   50.,  1. ])
    popt, pcov = curve_fit(vectDoubleCB, x_scale_barrel[2:], y_scale_barrel[2:], p0, maxfev=5000, bounds=param_bounds)
    ax.plot(plot_x, vectDoubleCB(plot_x, *popt), '-', label='_', lw=2, color=cmap(0))

    ax.errorbar(x_scale_endcap, y_scale_endcap, xerr=x_err_scale_endcap, yerr=y_err_scale_endcap, ls='None', label=endcap_label, lw=2, marker='s', color=cmap(1))
    ##            [ mean, sigma, alphaL,    nL, alphaR,    nR,  norm]
    p0 =          [   1.,   0.2,     1.,    1.,     1.,    1.,  0.1]
    param_bounds=([ -10.,   0. ,     0.,    0.,     0.,    0.,  0. ],
                  [  10.,   1. ,    10.,   25.,    10.,   25.,  1. ])
    popt, pcov = curve_fit(vectDoubleCB, x_scale_endcap[2:], y_scale_endcap[2:], p0, maxfev=5000, bounds=param_bounds)
    ax.plot(plot_x, vectDoubleCB(plot_x, *popt), '-', label='_', lw=2, color=cmap(1))

    if options.inclusive:
        ax.errorbar(x_scale_inclusive, y_scale_inclusive, xerr=x_err_scale_inclusive, yerr=y_err_scale_inclusive, ls='None', label=inclusive_label, lw=2, marker='^', color=cmap(2))
        ##            [ mean, sigma, alphaL,    nL, alphaR,    nR,  norm]
        p0 =          [   1.,   0.2,     1.,    1.,     1.,    1.,  0.1]
        param_bounds=([ -10.,   0. ,     0.,    0.,     0.,    0.,  0. ],
                      [  10.,   1. ,    10.,   50.,    10.,   50.,  1. ])
        popt, pcov = curve_fit(vectDoubleCB, x_scale_inclusive[2:], y_scale_inclusive[2:], p0, maxfev=5000, bounds=param_bounds)
        ax.plot(plot_x, vectDoubleCB(plot_x, *popt), '-', label='_', lw=2, color=cmap(2))

    leg = plt.legend(loc = 'upper left', fontsize=20, title=legend_title)
    leg._legend_box.align = "left"
    plt.ylim(0., max(max(y_scale_barrel), max(y_scale_endcap)) * 1.4)
    plt.xlim(x_lim)
    # plt.yscale('log')
    plt.xlabel(x_label)
    plt.ylabel(r'a.u.')
    plt.grid()
    for xtick in ax.xaxis.get_major_ticks():
        xtick.set_pad(10)
    mplhep.cms.label('Preliminary', data=True, rlabel=r'18 fb$^{-1}$ (13.6 TeV)')
    plt.savefig(plot_name+'.pdf')
    plt.savefig(plot_name+'.png')
    plt.close()


    # PLOT PT RESOLUTION
    if options.obj == 'tau':
        x_lim = (20.,110.)
        y_lim = (min(y_ptResol_barrel[9], y_ptResol_endcap[9]) * 0.6, max(max(y_ptResol_barrel), max(y_ptResol_endcap)) * 1.1)
        x_label = r'$p_{T}^{\tau, offline}\ [GeV]$'
        barrel_label = r'$Barrel\ |\eta^{\tau, offline}|<1.305$'
        endcap_label = r'$Endcaps\ 1.479<|\eta^{\tau, offline}|<2.1$'
        inclusive_label = r'$Inclusive\ |\eta^{\tau, offline}|<2.1$'
        plot_name = 'responses/tau_pt_resolution_'+options.tag
        if options.inclusive: plot_name = plot_name[:-4] + '_inclusive'
    elif options.obj == 'eg':
        x_lim = (5.,100.)
        y_lim = (min(min(y_ptResol_barrel), min(y_ptResol_endcap)) * 0.5, max(max(y_ptResol_barrel), max(y_ptResol_endcap)) * 1.1)
        x_label = r'$E_{T}^{e, offline}\ [GeV]$'
        barrel_label = r'$Barrel\ |\eta^{e, offline}|<1.440$'
        endcap_label = r'$Endcaps\ 1.479<|\eta^{e, offline}|<2.5$'
        inclusive_label = r'$Inclusive\ |\eta^{e, offline}|<2.5$'
        plot_name = 'responses/eg_pt_resolution_'+options.tag
        if options.inclusive: plot_name = plot_name[:-4] + '_inclusive'

    fig, ax = plt.subplots(figsize=(10,10))
    
    ax.errorbar(x_ptResol_barrel, y_ptResol_barrel, xerr=x_err_ptResol_barrel, yerr=y_err_ptResol_barrel, ls='None', label=barrel_label, lw=2, marker='o', color=cmap(0))
    ax.errorbar(x_ptResol_endcap, y_ptResol_endcap, xerr=x_err_ptResol_endcap, yerr=y_err_ptResol_endcap, ls='None', label=endcap_label, lw=2, marker='s', color=cmap(1))
    if options.inclusive: ax.errorbar(x_ptResol_inclusive, y_ptResol_inclusive, xerr=x_err_ptResol_inclusive, yerr=y_err_ptResol_inclusive, ls='None', label=inclusive_label, lw=2, marker='^', color=cmap(2))

    leg = plt.legend(loc = 'upper right', fontsize=20)
    leg._legend_box.align = "left"
    plt.ylim(y_lim)
    plt.xlim(x_lim)
    # plt.yscale('log')
    plt.xlabel(x_label)
    plt.ylabel(r'Energy resolution')
    plt.grid()
    for xtick in ax.xaxis.get_major_ticks():
        xtick.set_pad(10)
    mplhep.cms.label('Preliminary', data=True, rlabel=r'18 fb$^{-1}$ (13.6 TeV)')
    plt.savefig(plot_name+'.pdf')
    plt.savefig(plot_name+'.png')
    plt.close()


    # PLOT ETA RESPONSE
    if options.obj == 'tau':
        x_lim = (-0.15,0.15)
        x_label = r'$\eta^{\tau, L1}-\eta^{\tau, offline}$'
        barrel_label = r'$Barrel\ |\eta^{\tau, offline}|<1.305$'
        endcap_label = r'$Endcaps\ 1.479<|\eta^{\tau, offline}|<2.1$'
        inclusive_label = r'$Inclusive\ |\eta^{\tau, offline}|<2.1$'
        legend_title = r'$p_{T}^{\tau, offline}>30\ GeV$'
        plot_name = 'responses/tau_eta_response_'+options.tag
        if options.inclusive: plot_name = plot_name[:-4] + '_inclusive'
    elif options.obj == 'eg':
        x_lim = (-0.15,0.15)
        x_label = r'$\eta^{e/\gamma, L1}-\eta^{e, offline}$'
        barrel_label = r'$Barrel\ |\eta^{e, offline}|<1.440$'
        endcap_label = r'$Endcaps\ 1.479<|\eta^{e, offline}|<2.5$'
        inclusive_label = r'$Inclusive\ |\eta^{e, offline}|<2.5$'
        legend_title = r'$E_{T}^{e, offline}>$'+cut+r'$\ GeV$'
        plot_name = 'responses/eg_eta_response_'+options.tag
        if options.inclusive: plot_name = plot_name[:-4] + '_inclusive'

    fig, ax = plt.subplots(figsize=(10,10))
    
    ax.errorbar(x_etaResp_barrel, y_etaResp_barrel, xerr=x_err_etaResp_barrel, yerr=y_err_etaResp_barrel, ls='None', label=barrel_label, lw=2, marker='o', color=cmap(0))
    ##            [ mean, sigma, alphaL,    nL, alphaR,    nR,  norm]
    p0 =          [   0.,  0.05,     1.,    1.,     1.,    1.,  0.1]
    param_bounds=([ -10.,  0.  ,     0.,    0.,     0.,    0.,  0. ],
                  [  10.,  0.5 ,    10.,   50.,    10.,   50.,  1. ])
    popt, pcov = curve_fit(vectDoubleCB, x_etaResp_barrel[2:], y_etaResp_barrel[2:], p0, maxfev=5000, bounds=param_bounds)
    ax.plot(plot_x, vectDoubleCB(plot_x, *popt), '-', label='_', lw=2, color=cmap(0))

    ax.errorbar(x_etaResp_endcap, y_etaResp_endcap, xerr=x_err_etaResp_endcap, yerr=y_err_etaResp_endcap, ls='None', label=endcap_label, lw=2, marker='s', color=cmap(1))
    ##            [ mean, sigma, alphaL,    nL, alphaR,    nR,  norm]
    p0 =          [   0.,  0.05,     1.,    1.,     1.,    1.,  0.1]
    param_bounds=([ -10.,  0.  ,     0.,    0.,     0.,    0.,  0. ],
                  [  10.,  0.5 ,    10.,   25.,    10.,   25.,  1. ])
    popt, pcov = curve_fit(vectDoubleCB, x_etaResp_endcap[2:], y_etaResp_endcap[2:], p0, maxfev=5000, bounds=param_bounds)
    ax.plot(plot_x, vectDoubleCB(plot_x, *popt), '-', label='_', lw=2, color=cmap(1))

    if options.inclusive:
        ax.errorbar(x_etaResp_inclusive, y_etaResp_inclusive, xerr=x_err_etaResp_inclusive, yerr=y_err_etaResp_inclusive, ls='None', label=inclusive_label, lw=2, marker='^', color=cmap(2))
        ##            [ mean, sigma, alphaL,    nL, alphaR,    nR,  norm]
        p0 =          [   0.,  0.05,     1.,    1.,     1.,    1.,  0.1]
        param_bounds=([ -10.,  0.  ,     0.,    0.,     0.,    0.,  0. ],
                      [  10.,  0.5 ,    10.,   50.,    10.,   50.,  1. ])
        popt, pcov = curve_fit(vectDoubleCB, x_etaResp_inclusive[2:], y_etaResp_inclusive[2:], p0, maxfev=5000, bounds=param_bounds)
        ax.plot(plot_x, vectDoubleCB(plot_x, *popt), '-', label='_', lw=2, color=cmap(2))

    leg = plt.legend(loc = 'upper left', fontsize=20, title=legend_title)
    leg._legend_box.align = "left"
    plt.ylim(0., max(max(y_etaResp_barrel), max(y_etaResp_endcap)) * 1.4)
    plt.xlim(x_lim)
    # plt.yscale('log')
    plt.xlabel(x_label)
    plt.ylabel(r'a.u.')
    plt.grid()
    for xtick in ax.xaxis.get_major_ticks():
        xtick.set_pad(10)
    mplhep.cms.label('Preliminary', data=True, rlabel=r'18 fb$^{-1}$ (13.6 TeV)')
    plt.savefig(plot_name+'.pdf')
    plt.savefig(plot_name+'.png')
    plt.close()


    # PLOT PHI RESPONSE
    if options.obj == 'tau':
        x_lim = (-0.25,0.25)
        x_label = r'$\phi^{\tau, L1}-\phi^{\tau, offline}$'
        barrel_label = r'$Barrel\ |\eta^{\tau, offline}|<1.305$'
        endcap_label = r'$Endcaps\ 1.479<|\eta^{\tau, offline}|<2.1$'
        inclusive_label = r'$Inclusive\ |\eta^{\tau, offline}|<2.1$'
        legend_title = r'$p_{T}^{\tau, offline}>30\ GeV$'
        plot_name = 'responses/tau_phi_response_'+options.tag
        if options.inclusive: plot_name = plot_name[:-4] + '_inclusive'
    elif options.obj == 'eg':
        x_lim = (-0.15,0.15)
        x_label = r'$\phi^{e/\gamma, L1}-\phi^{e, offline}$'
        barrel_label = r'$Barrel\ |\eta^{e, offline}|<1.440$'
        endcap_label = r'$Endcaps\ 1.479<|\eta^{e, offline}|<2.5$'
        inclusive_label = r'$Inclusive\ |\eta^{e, offline}|<2.5$'
        legend_title = r'$E_{T}^{e, offline}>$'+cut+r'$\ GeV$'
        plot_name = 'responses/eg_phi_response_'+options.tag
        if options.inclusive: plot_name = plot_name[:-4] + '_inclusive'

    fig, ax = plt.subplots(figsize=(10,10))
    
    ax.errorbar(x_phiResp_barrel, y_phiResp_barrel, xerr=x_err_phiResp_barrel, yerr=y_err_phiResp_barrel, ls='None', label=barrel_label, lw=2, marker='o', color=cmap(0))
    ##            [ mean, sigma, alphaL,    nL, alphaR,    nR,  norm]
    p0 =          [   0.,  0.05,     1.,    1.,     1.,    1.,  0.1]
    param_bounds=([ -10.,  0.  ,     0.,    0.,     0.,    0.,  0. ],
                  [  10.,  0.5 ,    10.,   50.,    10.,   50.,  1. ])
    popt, pcov = curve_fit(vectDoubleCB, x_phiResp_barrel[2:95]+x_phiResp_barrel[97:], y_phiResp_barrel[2:95]+y_phiResp_barrel[97:], p0, maxfev=5000, bounds=param_bounds)
    ax.plot(plot_x, vectDoubleCB(plot_x, *popt), '-', label='_', lw=2, color=cmap(0))

    print(y_phiResp_barrel)
    print(y_phiResp_barrel[:95]+y_phiResp_barrel[97:])
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
    mplhep.cms.label('Preliminary', data=True, rlabel=r'18 fb$^{-1}$ (13.6 TeV)')
    plt.savefig(plot_name+'.pdf')
    plt.savefig(plot_name+'.png')
    plt.close()
    

    





























