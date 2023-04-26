import json
import argparse 
parser= argparse.ArgumentParser()
parser.add_argument('--runInput',help='Input file with comma separated "file,run1,run2.."')
parser.add_argument('--output' ,help='Json Input file with run file map')

args=parser.parse_args()

f=open(args.runInput,'r')
txt=f.readlines()
f.close()

runFnameMap={}

for l in txt:
    items=l[:-2].split(',')
    runs=items[1:]
    fname=items[0]
    for r in runs:
        if r not in runFnameMap:
            runFnameMap[r]=[]
        runFnameMap[r].append(fname)

print("Number of files : " ,len(txt))
print("Number of runs : "  ,len(runFnameMap))

f=open(args.output,'w')
json.dump(runFnameMap,f,indent=4)
f.close()
