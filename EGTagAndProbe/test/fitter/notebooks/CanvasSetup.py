def getDefaultCanvas():
    
    defaultCanvasParams=dict({"CMS":{},"TAG":{},"EnLumi":{}})
    defaultCanvasParams["CMS"]["text"]         = "CMS"
    defaultCanvasParams["CMS"]["textSize"]     = 0.05
    defaultCanvasParams["CMS"]["textFont"]     = 61   
    defaultCanvasParams["CMS"]["xpos"]         = 0.15   
    defaultCanvasParams["CMS"]["ypos"]         = 0.98+0.004
    
    # for the "preliminary"
    defaultCanvasParams["TAG"]["text"]         = "internal"
    defaultCanvasParams["TAG"]["textSize"]     = 0.76*defaultCanvasParams["CMS"]["textSize"]  
    defaultCanvasParams["TAG"]["textFont"]     = 52   
    defaultCanvasParams["TAG"]["xpos"]         = 0.15  + 0.12
    defaultCanvasParams["TAG"]["ypos"]         = 0.98  - 0.004
    
    # for the "2018A [ 1 fb^{-1}] 13 TeV" label
    defaultCanvasParams["EnLumi"]["text"]       = "               13 TeV"
    defaultCanvasParams["EnLumi"]["textSize"]  = 0.76*defaultCanvasParams["CMS"]["textSize"]  
    defaultCanvasParams["EnLumi"]["textFont"]  = 42   
    defaultCanvasParams["EnLumi"]["xpos"]      = 0.85   
    defaultCanvasParams["EnLumi"]["ypos"]      = 0.95

    return defaultCanvasParams

def getCanvasParams(tag,lumi="-1 fb"):
    cPars=getDefaultCanvas()
    print("Canvas Tag : ", tag )
    if tag=="2018Full":
        cPars["EnLumi"]["xpos"]       =  0.961
        cPars["EnLumi"]["text"]       = "SingleEG 2018 (58.9fb^{-1}) 13 TeV"
        return cPars
    if tag=="Run3MC":
        cPars["EnLumi"]["xpos"]       =  0.961
        cPars["EnLumi"]["text"]       = "Run 3 MC, 13,6 TeV"
        return cPars
    if tag=="Run3Data":
        cPars["EnLumi"]["xpos"]       =  0.961
        cPars["EnLumi"]["text"]       = "Run 3 Egamma ( "+lumi+")"
        return cPars
     
    return cPars

def getCanvas2018A():
    cPars=getDefaultCanvas()
 #   cPars["EnLumi"]["text"]="   13 TeV"
    return cPars
