from __future__ import print_function
import os
import json
import argparse

if __name__=='__main__':
    
    parser = argparse.ArgumentParser()
    parser.add_argument('-i',"--input", help="input json of filelist",default='toProcessFile.json')
    parser.add_argument('-n',"--itemToProcess", help="funmber files to get the parents",default='-1')
    args = parser.parse_args()

    inputFname=args.input
    n=int(args.itemToProcess)

    print("processing the filenames ! ")

    miniAodFiles={'fnames':[]}
    with open(inputFname,'r') as f:
        miniAodFiles=json.load(f)
        
    parentdaughterMap={}
    if os.path.exists('parentmap.json'):
        with open("parentmap.json",'r') as f:
           parentdaughterMap=json.load(f)
    if n < 0:
        n=len( miniAodFiles['fnames'])
    for i in range(n):
        fname=miniAodFiles['fnames'].pop(0)
        print(i+1,"/",n," ] Processing fname : ",fname)
        os.system('dasgoclient --query="parent file='+fname+'"> .tmp ')
        
        parents=[]
        with open('.tmp','r') as f :
            txt=f.readlines()
        for l in txt:
            parents.append("root://se01.indiacms.res.in/"+l[:-1]) ## Mapping the raw files to T2_IN_TIFR
        
        parentdaughterMap[fname]=parents

    with open("parentmap.json",'w') as f:
       json.dump(parentdaughterMap,f,indent=4)

    with open("yetToProcess.json",'w') as f:
       json.dump(miniAodFiles,f,indent=4)
