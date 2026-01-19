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


# ###################################################################################################################################################################################################################################################################################################################################################
# FUNCTIONS USED BOTH FOR TAU AND EG TURNONS

# ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
# APPROXIMATE ERROR FUNCTION
def ApproxErf(arg):
    erflim = 5.0;
    if arg > erflim:
        return 1.0
    if arg < -erflim:
        return -1.0

    return sp.erf(arg)

# ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
# CUMULATIVE CRYSTAL-BALL : this is the most important part of the turnon and can be used for both eg and tau
def CB(x, mean=1, sigma=1, alpha=1, n=1, norm=1):
    pi = np.pi
    sqrt2 = np.sqrt(2)
    sqrtPiOver2 = np.sqrt(np.pi / 2)

    # Variable std deviation
    sig = abs(sigma)
    t = (x - mean)/sig
    if alpha < 0:
        t = -t

    # Crystal Ball part
    absAlpha = abs(alpha)
    A = pow(n / absAlpha, n) * np.exp(-0.5 * absAlpha * absAlpha)
    B = absAlpha - n / absAlpha
    C = n / absAlpha * np.exp(-0.5*absAlpha*absAlpha) / (n - 1)
    D = (1 + ApproxErf(absAlpha / sqrt2)) * sqrtPiOver2
    N = norm / (D + C)

    if t <= absAlpha:
        crystalBall = N * (1 + ApproxErf( t / sqrt2 )) * sqrtPiOver2
    else:
        crystalBall = N * (D +  A * (1/pow(t-B,n-1) - 1/pow(absAlpha - B,n-1)) / (1 - n))

    return crystalBall

vectCB = np.vectorize(CB)
##            [   mean, sigma, alpha,    n, norm]
# p0 =          [thr    ,    3.,   3. , 100., 0.95]
# param_bounds=([thr-10.,    1.,   0.1,   1., 0.9 ],
#               [thr+10.,   10.,  10. , 200., 1.  ])

# ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
# CRYSTAL-BALL CDF TRUNCATED IN THE LOW TAIL : this can be used to convolute with some other low tail function
def ApproxCB(x, xcut=1, mean=1, sigma=1, alpha=1, n=1, norm=1,  p=1, slope=1, square=1, cube=1, inflation=1, scale=1, integral=1, ct=1):
    if x <= xcut:
        # cb = x * CB(xcut, mean=mean, sigma=sigma, alpha=alpha, n=n, norm=norm) / xcut # LINEAR APPROXIMATION
        cb = CB(xcut, mean=mean, sigma=sigma, alpha=alpha, n=n, norm=norm)            # CONSTANT APPROXIMATION
    if x > xcut:
        cb = CB(x, mean=mean, sigma=sigma, alpha=alpha, n=n, norm=norm)

    return cb

vectApproxCB = np.vectorize(ApproxCB)
##            [   mean, sigma, alpha,    n, norm]
# p0 =          [thr    ,    3.,   3. , 100., 0.95]
# param_bounds=([thr-10.,    1.,   0.1,   1., 0.9 ],
#               [thr+10.,   10.,  10. , 200., 1.  ])


# ###################################################################################################################################################################################################################################################################################################################################################
# FUNCTIONS USED ONLY FOR TAU TURNONS

# ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
# ARCTAN TRUNCATED IN THE LOW TAIL : used with the CB CDF fits well and almost out of the box the tau turnons
def ApproxATAN(x, xturn=1, p=1, width=1):
    pi = np.pi

    # Arctan part
    arctan = 0.
    if x < xturn:
        arctan = p
    if x >= xturn:
        arctan = pow(ApproxErf((x - xturn) / 5.), 2) * 2. * (1. - p) / pi * np.arctan(pi / 80. * width * (x - xturn)) + p

    return arctan

vectApproxATAN = np.vectorize(ApproxATAN)

# ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
# CONVOLUTION OF CB CDF AND ApproxATAN: works pretty well and out of the box to fit tau turnons
def CBconvATAN(x, mean=1, sigma=1, alpha=1, n=1, norm=1, xturn=1, p=1, width=1):
    return convolve(vectCB(x, mean=mean, sigma=sigma, alpha=alpha, n=n, norm=norm), vectApproxATAN(x, xturn=xturn, p=p, width=width), mode='full', method='direct')

vectCBconvATAN = np.vectorize(CBconvATAN)
##            [   mean, sigma, alpha,    n,     norm, xturn,   p, width]
# p0 =          [thr    ,    3.,   3. , 100.,     0.95,   10., 0.8,   10.]
# param_bounds=([thr-10.,    1.,   0.1,   1.,     0.9 ,    0., 0.2,    1.],
#               [thr+10.,   10.,  10. , 200.,     1.  ,   thr, 1. ,  100.])


# ###################################################################################################################################################################################################################################################################################################################################################
# FUNCTIONS USED ONLY FOR EG TURNONS

def ATAN(x, a=1, b=1, c=1):
    return a*np.arctan(x-c)+b

# ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
# CONVOLUTION OF AN ATAN WITH AN APPROXIAMTE ATAN: fits very well the low threshold eg turnons
def ATANconvApproxATAN(x, a=1, b=1, c=1,  xturn=1, p=1, width=1):
    return convolve(ATAN(x, a=a, b=b, c=c), vectApproxATAN(x, xturn=xturn, p=p, width=width), mode='full', method='direct')

vectATANconvApproxATAN = np.vectorize(ATANconvApproxATAN)

# ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
# LOW TAIL POLYNOMIAL CONTINUATED WITH CRYSTAL-BALL CDF (the continuation is not smooth) : this fits the eg turnons pretty well
def CBtimesPOLYlow(x, xturn=1, p=1, slope=1, square=1, cube=1, inflation=1, scale=1, integral=1, ct=1, mean=1, sigma=1, alpha=1, n=1, norm=1):
    # Poly part for the low energy tail
    poly = 0.
    if x >= xturn:
        poly = p + (slope * (xturn-inflation) + square * pow(xturn-inflation,2) + cube * pow(xturn-inflation,3)) / scale + CB(x, mean=mean, sigma=sigma, alpha=alpha, n=n, norm=norm)
    if x < xturn:
        poly = p + (slope * (x-inflation) + square * pow(x-inflation,2) + cube * pow(x-inflation,3)) / scale

    return poly

vectCBtimesPOLYlow = np.vectorize(CBtimesPOLYlow)
##            [xturn,    p, slope, square, cube, inflation, scale, integral,  ct        mean, sigma, alpha,    n, norm]
# p0 =          [20.,   0.05, 0.005,     0.,   0.,        15,   1E5,      0.8,  3.,    thr    ,    3.,   3. , 100., 0.95]
# param_bounds=([5. ,   0.  , 0.   ,    -1.,  -2.,        10,   1E4,      0.6,  1.,    thr-10.,    1.,   0.1,   1., 0.9 ],
#               [35.,   0.1 , 0.01 ,     1.,   2.,        20,   1E6,      1. , 10.,    thr+10.,   10.,  10. , 200., 1.  ])

# ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
# POLYNOMIAL TO FIT HIGHER TAIL OF THE TURNONS : works pretty well when convoluted with CB CDF and fitting eg turnons only above turnon point
def POLYhigh(x, xturn=1, p=1, slope=1, square=1, cube=1):
    # Poly part for the high energy tail
    poly = 0.
    if x <= xturn:
        poly = p
    if x > xturn:
        poly = p + slope * (x-xturn) + square * pow(x-xturn,2) + cube * pow(x-xturn,3)

    return poly

vectPOLYhigh = np.vectorize(POLYhigh)

# ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
# POLYNOMIAL TO FIT LOWER TAIL OF THE TURNONS AND ERF TURNON CORE : works pretty well alone when fitting eg turnons only below ~50GeV (below the POLYhigh would kick in)
def POLYlow(x, xturn=1, p=1, slope=1, square=1, cube=1, inflation=1, scale=1, integral=1, ct=1):
    # Poly part for the low energy tail
    poly = 0.
    if x >= xturn:
        poly = p + (slope * (xturn-inflation) + square * pow(xturn-inflation,2) + cube * pow(xturn-inflation,3)) / scale + pow(integral*ApproxErf((x-xturn)/ct),2)
    if x < xturn:
        poly = p + (slope * (x-inflation) + square * pow(x-inflation,2) + cube * pow(x-inflation,3)) / scale

    return poly

vectPOLYlow = np.vectorize(POLYlow)
##            [xturn,    p, slope, square, cube, inflation, scale, integral,  ct]
# p0 =          [20.,   0.05, 0.005,     0.,   0.,        15,   1E5,      0.8,  3.]
# param_bounds=([5. ,   0.  , 0.   ,    -1.,  -2.,        10,   1E4,      0.6,  1.],
#               [35.,   0.1 , 0.01 ,     1.,   2.,        20,   1E6,      1. , 10.])

# ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
# CONVOLUTION OF LOW TAIL POLYNOMIAL CONTINUATED WITH CRYSTAL-BALL CDF AND  ApproxATAN: works ok to fit the low threhsold eg turnons but has a sharp edge at the continuation point
def CBtimesPOLYlowConvATAN(x, xturn=1, p=1, slope=1, square=1, cube=1, inflation=1, scale=1, integral=1, ct=1, mean=1, sigma=1, alpha=1, n=1, norm=1, xturn1=1, p1=1, width1=1):
    return convolve(vectCBtimesPOLYlow(x, xturn=xturn, p=p, slope=slope, square=square, cube=cube, inflation=inflation, scale=scale, integral=integral, ct=ct, mean=mean, sigma=sigma, alpha=alpha, n=n, norm=norm), vectApproxATAN(x, xturn=xturn1, p=p1, width=width1), mode='full', method='direct')

vectCBtimesPOLYlowConvATAN = np.vectorize(CBtimesPOLYlowConvATAN)
##            [xturn,    p, slope, square, cube, inflation, scale, integral,  ct        mean, sigma, alpha,    n, norm, xturn,   p, width]
# p0 =          [20.,   0.05, 0.005,     0.,   0.,        15,   1E5,      0.8,  3.,    thr    ,    3.,   3. , 100., 0.95,   30., 0.8,   10.]
# param_bounds=([5. ,   0.  , 0.   ,    -1.,  -2.,        10,   1E4,      0.6,  1.,    thr-10.,    1.,   0.1,   1., 0.9 ,   25., 0.2,    1.],
#               [35.,   0.1 , 0.01 ,     1.,   2.,        20,   1E6,      1. , 10.,    thr+10.,   10.,  10. , 200., 1.  ,   40., 1. ,  100.])

# ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
# CONVOLUTION OF CB CDF AND POLYNOMIAL FOR HIGHER TAIL : works pretty well when fitting eg turnons only above turnon point
def CBconvPOLYhigh(x, mean=1, sigma=1, alpha=1, n=1, norm=1, xturn=1, p=1, slope=1, square=1, cube=1):
    return convolve(vectCB(x, mean=mean, sigma=sigma, alpha=alpha, n=n, norm=norm), vectPOLYhigh(x, xturn=xturn, p=p, slope=slope, square=square, cube=cube), mode='full', method='direct')

vectCBconvPOLYhigh = np.vectorize(CBconvPOLYhigh)
##            [   mean, sigma, alpha,    n,   norm,          xturn,     p,  slope, square, cube]
# p0 =          [thr    ,    3.,   3. , 100., 0.95,            50.,   0.8,    0.5,     0.,   0.]
# param_bounds=([thr-10.,    1.,   0.1,   1., 0.9 ,            45.,   0.2,    0. ,    -1.,  -1.],
#               [thr+10.,   10.,  10. , 200., 1.  ,            55.,   1. ,    1. ,     1.,   1.])

# ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
# CONVOLUTION OF CB CDF AND POLYNOMIAL FOR LOWER TAIL : this one does not work due to the fact that 1. there is both and erf and a cb cdf for the higher tail , 2. the low tail gets sent to zero by the cb cdf
def CBconvPOLYlow(x, mean=1, sigma=1, alpha=1, n=1, norm=1, xturn=1, p=1, slope=1, square=1, cube=1, inflation=1, scale=1, integral=1, ct=1):
    return convolve(vectCB(x, mean=mean, sigma=sigma, alpha=alpha, n=n, norm=norm), vectPOLYlow(x, xturn=xturn, p=p, slope=slope, square=square, cube=cube, inflation=inflation, scale=scale, integral=integral, ct=ct), mode='full', method='direct')

vectCBconvPOLYlow = np.vectorize(CBconvPOLYlow)
##            [   mean, sigma, alpha,    n,   norm,          xturn,    p, slope, square, cube, inflation, scale, integral,  ct]
# p0 =          [thr    ,    3.,   3. , 100., 0.95,            20.,   0.05, 0.005,     0.,   0.,        15,   1E5,      0.8,  3.]
# param_bounds=([thr-10.,    1.,   0.1,   1., 0.9 ,            5. ,   0.  , 0.   ,    -1.,  -2.,        10,   1E4,      0.6,  1.],
#               [thr+10.,   10.,  10. , 200., 1.  ,            35.,   0.1 , 0.01 ,     1.,   2.,        20,   1E6,      1. , 10.])

# ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
# CONVOLUTION OF APPROXIMATE CB CDF AND POLYNOMIAL FOR LOWER TAIL : this function fits relatively well the turnon and  the high part BUT the fact that the lower part has a CB truncation makes the fit fail in the low tail
def ApproxCBconvPOLYlow(x, xcut=1, mean=1, sigma=1, alpha=1, n=1, norm=1, xturn=1, p=1, slope=1, square=1, cube=1, inflation=1, scale=1, integral=1, ct=1):
    return convolve(vectApproxCB(x, xcut=xcut, mean=mean, sigma=sigma, alpha=alpha, n=n, norm=norm), vectPOLYlow(x, xturn=xturn, p=p, slope=slope, square=square, cube=cube, inflation=inflation, scale=scale, integral=integral, ct=ct), mode='full', method='direct')

vectApproxCBconvPOLYlow = np.vectorize(ApproxCBconvPOLYlow)
##            [xcut,    mean, sigma, alpha,    n,   norm,          xturn,    p, slope, square, cube, inflation, scale, integral,  ct]
# p0 =          [ 25., thr    ,    3.,   3. , 100., 0.95,            20.,   0.05, 0.005,     0.,   0.,        15,   1E5,      0.8,  3.]
# param_bounds=([ 20., thr-10.,    1.,   0.1,   1., 0.9 ,            5. ,   0.  , 0.   ,    -1.,  -2.,        10,   1E4,      0.6,  1.],
#               [ 30., thr+10.,   10.,  10. , 200., 1.  ,            35.,   0.1 , 0.01 ,     1.,   2.,        20,   1E6,      1. , 10.])

# ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
# CONVOLUTION OF POLYNOMIAL TO FIT LOWER TAIL OF THE TURNONS WITH ERF TURNON CORE AND POLYNOMIAL FOR HIGHER TAIL : this gets the shape of the function but has too sharp corners that do not make much sense
def POLYlowConvPOLYhigh(x, xturn=1, p=1, slope=1, square=1, cube=1, inflation=1, scale=1, integral=1, ct=1, xturn1=1, p1=1, slope1=1, square1=1, cube1=1):
    return convolve(vectPOLYlow(x, xturn=xturn, p=p, slope=slope, square=square, cube=cube, inflation=inflation, scale=scale, integral=integral, ct=ct), vectPOLYhigh(x, xturn=xturn1, p=p1, slope=slope1, square=square1, cube=cube1), mode='full', method='direct')

vectPOLYlowConvPOLYhigh = np.vectorize(POLYlowConvPOLYhigh)
##            [xturn,    p, slope, square, cube, inflation, scale, integral,  ct,        xturn1,     p1,  slope1, square1, cube1]
# p0 =          [20.,   0.05, 0.005,     0.,   0.,        15,   1E5,      0.8,  3.,           50.,    0.8,     0.5,      0.,    0.]
# param_bounds=([5. ,   0.  , 0.   ,    -1.,  -2.,        10,   1E4,      0.6,  1.,           45.,    0.2,     0. ,     -1.,   -1.],
#               [35.,   0.1 , 0.01 ,     1.,   2.,        20,   1E6,      1. , 10.,           55.,    1. ,     1. ,      1.,    1.])

# ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
# CONVOLUTION OF LOW TAIL POLYNOMIAL CONTINUATED WITH CRYSTAL-BALL CDF AND POLYNOMIAL FOR HIGHER TAIL : this does not work like this
def CBtimesPOLYlowConvPOLYhigh(x, xturn=1, p=1, slope=1, square=1, cube=1, inflation=1, scale=1, integral=1, ct=1,    mean=1, sigma=1, alpha=1, n=1, norm=1,   xturn1=1, p1=1, slope1=1, square1=1, cube1=1):
    return convolve(vectCBtimesPOLYlow(x, xturn=xturn, p=p, slope=slope, square=square, cube=cube, inflation=inflation, scale=scale, integral=integral, ct=ct, mean=mean, sigma=sigma, alpha=alpha, n=n, norm=norm), vectPOLYhigh(x, xturn=xturn1, p=p1, slope=slope1, square=square1, cube=cube1), mode='full', method='direct')

vectCBtimesPOLYlowConvPOLYhigh = np.vectorize(CBtimesPOLYlowConvPOLYhigh)
##            [xturn,    p, slope, square, cube, inflation, scale, integral,  ct        mean, sigma, alpha,    n, norm,        xturn1,     p1,  slope1, square1, cube1]
# p0 =          [20.,   0.05, 0.005,     0.,   0.,        15,   1E5,      0.8,  3.,    thr    ,    3.,   3. , 100., 0.95,           50.,    0.8,     0.5,      0.,    0.]
# param_bounds=([5. ,   0.  , 0.   ,    -1.,  -2.,        10,   1E4,      0.6,  1.,    thr-10.,    1.,   0.1,   1., 0.9 ,           45.,    0.2,     0. ,     -0.00001,   -0.00001],
#               [35.,   0.1 , 0.01 ,     1.,   2.,        20,   1E6,      1. , 10.,    thr+10.,   10.,  10. , 200., 1.  ,           55.,    1. ,     1. ,      0.00001,   0.00001])




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

    if options.obj == "tau":
        inFile = ROOT.TFile('/home/llr/cms/mchiusi/Run3preparation/Run3preparation_2023/CMSSW_11_0_2/src/TauObjectsOptimization/PlotTurnOns/ROOTs/ROOTs_2023/efficiencies_of_Run'+options.tag+'_unpacked.root')
    elif options.obj == "eg":
        inFile = ROOT.TFile('/eos/cms/store/group/dpg_trigger/comm_trigger/L1Trigger/atjaiswa/DPS_Jul2023/Hist_2023Run3_EraBC_EGamma_UnpackedPerformance.root')
    else:
        print('** ERROR : only supported objects are tau and eg')
        print('** EXITING')
        exit()
    
    # create plot where to print everything
    plt.rcParams['legend.title_fontsize'] = 'small'
    cmap = matplotlib.cm.get_cmap('Set1'); imap=0
    plot_x = np.linspace(0,1000,2000)
    fig, ax = plt.subplots(figsize=(10,10))
    zorder = 1

    markers = ['o', 's', '^', 'D']

    options.sels = sys.argv[sys.argv.index("--sels") + 1].split()
    for sel in options.sels:
        print("analizing sel", sel)
        if not 'OR' in sel:
            thr_string = sel[:2]
            iso_string = sel[2:]
            thr = int(thr_string)

            if options.obj == "tau":
                if iso_string:
                    eff_TGraph = inFile.Get('divide_ptProgressionAt'+thr_string+'_Iso_by_pt')
                    label = r'$E_{T}^{\tau, L1} > %i$ GeV & Isolation' % (thr)
                else:
                    eff_TGraph = inFile.Get('divide_ptProgressionAt'+thr_string+'_noIso_by_pt')
                    label = r'$E_{T}^{\tau, L1} > %i$ GeV' % (thr)
            
            elif options.obj == "eg":
                if iso_string:
                    print('loose but correct')
                    eff_TGraph = inFile.Get('EfficiencyVsOffEt_L1Et'+thr_string+'LooseIso_Eta2p5')
                    label = r'$E_{T}^{e/\gamma, L1} > %i$ GeV & Loose Iso' % (thr)
                    #eff_TGraph = inFile.Get('EfficiencyVsOffEt_L1Et'+thr_string+'TightIso_Eta2p5')
                    #label = r'$E_{T}^{e/\gamma, L1} > %i$ GeV & Tight Iso' % (thr)
                else:
                    eff_TGraph = inFile.Get('EfficiencyVsOffEt_L1Et'+thr_string+'NoIso_Eta2p5')
                    label = r'$E_{T}^{e/\gamma, L1} > %i$ GeV' % (thr)

            marker=markers[imap]

        else:
            # inFileOR = ROOT.TFile('./EG/HistogramFile_Eff_'+options.tag+'_newPath.root')

            if sel.count('OR') > 2:
                print('** WARNING : maximum one OR supported ofr the moment')
                print('** EXITING')
                exit()

            thr1_string = sel.split('OR')[0][:2]
            iso1_string = sel.split('OR')[0][2:]
            thr1 = int(thr1_string)

            thr2_string = sel.split('OR')[1][:2]
            iso2_string = sel.split('OR')[1][2:]
            thr2 = int(thr2_string)

            thr = min(thr1, thr2)

            if options.obj == "tau":
                print('** WARNING :no ORs availabe for taus')
                print('** EXITING')
                exit()
            elif options.obj == "eg":
                if iso1_string:
                    if iso2_string:
                        print("Not set")
                        #eff_TGraph = inFile.Get('EfficiencyVsOffEt_L1Et'+thr1_string+'_OR_EG'+thr2_string+'_by_pT_all')
                        #label = '$E_{T}^{e/\gamma, L1} > %i$ GeV & Tight Isolation OR \n$E_{T}^{e/\gamma, L1} > %i$ GeV' % (thr1, thr2)
                    else:
                        print('correct2')
                        eff_TGraph = inFile.Get('EfficiencyVsOffEt_L1Et'+thr1_string+'TightIsoOrL1Et'+thr2_string+'NoIso_Eta2p5')
                        label = '$E_{T}^{e/\gamma, L1} > %i$ GeV & Tight Iso OR \n$E_{T}^{e/\gamma, L1} > %i$ GeV - No Iso' % (thr1, thr2)
                else:
                    if iso2_string:
                        eff_TGraph = inFile.Get('TGraphs/divide_pT_pass_EG'+thr1_string+'TightIsoOrL1Et'+thr2_string+'_by_pT_all')
                        label = '$E_{T}^{e/\gamma, L1} > %i$ GeV OR \n$E_{T}^{e/\gamma, L1} > %i$ GeV & Tight Iso' % (thr1, thr2)
                    else:
                        eff_TGraph = inFile.Get('TGraphs/divide_pT_pass_EG'+thr1_string+'_OR_EG'+thr2_string+'_by_pT_all')
                        label = '$E_{T}^{e/\gamma, L1} > %i$ GeV OR\n$E_{T}^{e/\gamma, L1} > %i$ GeV' % (thr1, thr2)

            marker=7

        x = []
        y = []
        x_err = []
        y_errU = []
        y_errD = []
        y_errFit = []

        for ibin in range(eff_TGraph.GetN()):
            if options.obj == 'eg' and thr > 25 and eff_TGraph.GetPointX(ibin) < 5: continue
            # if eff_TGraph.GetPointX(ibin) < thr: continue

            x.append(eff_TGraph.GetPointX(ibin))
            y.append(eff_TGraph.GetPointY(ibin))
            x_err.append(eff_TGraph.GetErrorX(ibin))
            
            errU = eff_TGraph.GetErrorYhigh(ibin)
            errD = eff_TGraph.GetErrorYlow(ibin)
            y_errU.append(errU)
            y_errD.append(errD)

            # store two possibilities for fitting error: make the fit with the largest or the smallest error
            if errU > errD:
                y_errS = errD
                y_errL = errU
            else:
                y_errS = errU
                y_errL = errD
            if options.smallFitErr: y_errFit.append(y_errS)
            else:                   y_errFit.append(y_errL)
        
        ax.errorbar(x, y, xerr=x_err,yerr=[y_errD, y_errU], ls='None', label=label, lw=2, marker=marker, color=cmap(imap), zorder=zorder)
        zorder = zorder + 1

        if options.obj == 'tau':
            ##vectCBconvATAN
            #             [   mean, sigma, alpha,    n,     norm, xturn,   p, width]
            p0 =          [thr    ,    3.,   3. , 100.,     0.95,   10., 0.8,   10.]
            param_bounds=([thr-10.,    1.,   0.1,   1.,     0.9 ,    0., 0.2,    1.],
                          [thr+10.,   10.,  10. , 200.,     1.  ,  110., 1. ,  100.])
            popt, pcov = curve_fit(vectCBconvATAN, x, y, p0, maxfev=5000, bounds=param_bounds)

            ax.plot(plot_x, vectCBconvATAN(plot_x, *popt), '-', label='_', lw=2, color=cmap(imap), zorder=zorder)

        elif options.obj =='eg':
            if thr > 25:
                ##vectCBtimesPOLYlow
                ##            [xturn,    p, slope, square, cube, inflation, scale, integral,  ct        mean, sigma, alpha,    n, norm]
                p0 =          [20.,   0.05, 0.005,     0.,   0.,       15.,   1E5,      0.8,  3.,    thr    ,    3.,   3. , 100., 0.95]
                param_bounds=([5. ,   0.  , 0.   ,    -2.,  -3.,       5. ,   1E3,      0.6,  1.,    thr-10.,    1.,   0.1,   1., 0.9 ],
                              [35.,   0.1 , 0.02 ,     2.,   3.,       25.,   1E7,      1. , 100.,    thr+10.,   10.,  10. , 200., 1.  ])
                popt, pcov = curve_fit(vectCBtimesPOLYlow, x, y, p0, maxfev=5000, bounds=param_bounds)
                print(popt)
                ax.plot(plot_x, vectCBtimesPOLYlow(plot_x, *popt), '-', label='_', lw=2, color=cmap(imap), zorder=zorder)
            else:
                ##            [xturn,    p, slope, square, cube, inflation, scale, integral,  ct        mean, sigma, alpha,    n, norm, xturn,   p, width]
                p0 =          [10.,   0.05, 0.005,     0.,   0.,        15,   1E5,      0.8,  3.,    thr    ,    3.,   3. , 100., 0.95,   30., 0.8,   10.]
                param_bounds=([0. ,   0.  , 0.   ,    -1.,  -2.,        5 ,   1E4,      0.6,  1.,    thr-10.,    1.,   0.1,   1., 0.9 ,   25., 0.2,   0.9],
                              [35.,   0.1 , 0.01 ,     1.,   2.,        20,   1E6,      1. , 10.,    thr+10.,   10.,   10. , 200., 1.  ,  40., 1. ,  100.])
                #popt, pcov = curve_fit(vectCBtimesPOLYlowConvATAN, x[:13]+x[15:], y[:13]+y[15:], p0, maxfev=5000, bounds=param_bounds)
                popt, pcov = curve_fit(vectCBtimesPOLYlowConvATAN, x[:-2], y[:-2], p0, maxfev=5000, bounds=param_bounds)
                print(popt)
                ax.plot(plot_x, vectCBtimesPOLYlowConvATAN(plot_x, *popt), '-', label='_', lw=2, color=cmap(imap), zorder=zorder)

                ##            [xturn,    p, slope, square, cube, inflation, scale, integral,  ct        mean, sigma, alpha,    n, norm, xturn,   p, width]
# p0 =          [20.,   0.05, 0.005,     0.,   0.,        15,   1E5,      0.8,  3.,    thr    ,    3.,   3. , 100., 0.95,   30., 0.8,   10.]
# param_bounds=([5. ,   0.  , 0.   ,    -1.,  -2.,        10,   1E4,      0.6,  1.,    thr-10.,    1.,   0.1,   1., 0.9 ,   25., 0.2,    1.],
#               [35.,   0.1 , 0.01 ,     1.,   2.,        20,   1E6,      1. , 10.,    thr+10.,   10.,  10. , 200., 1.  ,   40., 1. ,  100.])
                
                ##vectATANconvApproxATAN
                #p0 =          [0.3,1.5,15., 10.,0.5, 10.]
                #param_bounds=([0., 0., 1.,  1., 0.2,  0.],
                #              [10. ,10., 35., 70., 1.,80.])
                #popt, pcov = curve_fit(vectATANconvApproxATAN, x, y, p0, maxfev=5000, bounds=param_bounds)
                #print(popt)
                #ax.plot(plot_x, vectATANconvApproxATAN(plot_x, *popt), '-', label='_', lw=2, color=cmap(imap), zorder=zorder)
            
        imap+=1
        zorder = zorder + 1

   
    if options.obj == 'tau':
        if options.logx:
            plt.xlim(10., 500.)
            plt.xscale('log')
            ax.xaxis.set_major_locator(FixedLocator([10, 100, 500]))
            ax.xaxis.set_major_formatter(FixedFormatter(['10',r'$100$', r'$500$']))
        else:
            plt.xlim(0., 150.)
        leg = plt.legend(loc = 'lower right', fontsize=20, title=r'$|\eta^{\tau, offline}|<2.1$')
        leg._legend_box.align = "left"
        plt.xlabel(r'$p_{T}^{\tau, offline}\ [GeV]$')
        for xtick in ax.xaxis.get_major_ticks():
            xtick.set_pad(10)
        mplhep.cms.label('Preliminary', data=True, rlabel=r'18 fb$^{-1}$ (13.6 TeV)')

    elif options.obj =='eg':
        if options.logx:
            plt.xlim(9., 1000.)
            plt.xscale('log')
            ax.xaxis.set_major_locator(FixedLocator([10, 100, 1000]))
            ax.xaxis.set_major_formatter(FixedFormatter(['10',r'$10^2$', r'$10^3$']))
        else:
            # plt.xlim(5., 100.)
            plt.xlim(0., 70.)
        leg = plt.legend(loc = 'lower right', fontsize=20, title=r'$|\eta^{e, offline}|<2.5$')
        leg._legend_box.align = "left"
        plt.xlabel(r'$E_{T}^{e, offline}\ [GeV]$')
        for xtick in ax.xaxis.get_major_ticks():
            xtick.set_pad(10)
        mplhep.cms.label('Preliminary', data=True, rlabel=r'18 fb$^{-1}$ (13.6 TeV)')
        
    plt.ylim(0.000, 1.05)
    plt.ylabel(r'Efficiency')
    plt.grid()
    plot_name = 'turnons/'+options.obj+'_turnons_Run'+options.tag+'_'
    for sel in options.sels:
        plot_name += str(sel)+'_'
    if options.logx: plot_name += 'log_'
    plot_name = plot_name[:-1]
    plt.savefig(plot_name+'.pdf')
    plt.savefig(plot_name+'.png')
    plt.close()


    if options.obj == 'tau':
        plt.figure(figsize=(10,10))
        plt.plot(plot_x, vectCB(plot_x, *popt[:5]), '-', label='CB', lw=2, color=cmap(2))
        plt.plot(plot_x, vectApproxATAN(plot_x, *popt[5:]), '-', label='ApproxATAN', lw=2, color=cmap(1))
        plt.plot(plot_x, vectCBconvATAN(plot_x, *popt), '--', label='CBconvATAN', lw=2, color=cmap(0))
        leg = plt.legend(loc = 'lower right', fontsize=20, title='Convolution decomposition')
        leg._legend_box.align = "left"
        plt.ylim(0.000, 1.05)
        plt.xlim(0., 110.)
        # plt.yscale('log')
        plt.xlabel(r'$p_{T}^{\tau, offline}\ [GeV]$')
        plt.ylabel(r'Efficiency')
        plt.grid()
        mplhep.cms.label('Preliminary', data=True, rlabel=r'')
        plt.savefig('turnons/tau_convolution_decomposition_example.pdf')
        plt.savefig('turnons/tau_convolution_decomposition_example.png')
        plt.close()

    # no real convolution used for the eg so no need for the decomposition
    # elif options.obj == 'eg':
    #     plt.figure(figsize=(10,10))
    #     plt.plot(plot_x, vectCB(plot_x, *popt[:5]), '-', label='CB', lw=2, color=cmap(0), alpha=0.5)
    #     plt.plot(plot_x, vectPOLY(plot_x, *popt[5:]), '-', label='POLY', lw=2, color=cmap(1), alpha=0.5)
    #     plt.plot(plot_x, vectCBconvPOLY(plot_x, *popt), '-', label='CBconvPOLY', lw=2, color=cmap(2), alpha=0.5)
    #     plt.legend(loc = 'lower right', fontsize=20)
    #     plt.ylim(0.000, 1.05)
    #     plt.xlim(0., 110.)
    #     # plt.yscale('log')
    #     plt.xlabel(r'$p_{T}^{\tau, offline}\ [GeV]$')
    #     plt.ylabel(r'Efficiency')
    #     plt.grid()
    #     mplhep.cms.label('Preliminary', data=True, rlabel=r'')
    #     plt.savefig('test.pdf')
    #     plt.close()



























