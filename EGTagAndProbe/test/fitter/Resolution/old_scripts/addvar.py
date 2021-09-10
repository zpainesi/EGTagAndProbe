from ROOT import *

fIn = TFile.Open("outputForCalibration_2016B-HCollisions_MuTauSkim_02_06_16_TagAndProbe_Emu.root")
tree = fIn.Get("outTreeForCalibration")

fOut = TFile.Open("out_data.root", "recreate")
treeOut = tree.CloneTree(0)

for ev in range(0, tree.GetEntries()):
    tree.GetEntry(ev)
