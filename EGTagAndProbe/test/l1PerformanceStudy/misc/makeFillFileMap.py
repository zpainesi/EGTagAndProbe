import json
import argparse

parser= argparse.ArgumentParser()
parser.add_argument('-f','--fillInput',help='Input file with comma separated "run,fill"')
parser.add_argument('-r','--runInput' ,help='Json Input file with run file map')
parser.add_argument('-o','--output'   ,help='Output file map',default='fillFileMap.json')

args=parser.parse_args()

f=open(args.fillInput,'r')
txt=f.readlines()
f.close()

runFillMap={}
for l in txt:
    item=l[:-1].split(',')
    runFillMap[item[0]]=item[1]

runFnameMap={}
with open(args.runInput) as f:
    runFnameMap=json.load(f)

fillFileMap={}
nFile=0
for run in runFnameMap:
    fill=runFillMap[run] 
    if fill not in fillFileMap:
        fillFileMap[fill]=[]
    for fname  in runFnameMap[run]:
        fillFileMap[fill].append(fname)
        nFile+=1

print("Number of files : " , nFile )
print("Number of runs : "  ,len(runFnameMap))
print("Number of fills : " ,len(fillFileMap))

f=open(args.output,'w')
json.dump(fillFileMap,f,indent=4)
f.close()
