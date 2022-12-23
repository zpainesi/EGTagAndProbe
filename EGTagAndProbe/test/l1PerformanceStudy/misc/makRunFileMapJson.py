import json

f=open('allRunNumbers','r')
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

f=open('misc/runFileMap.json','w')
json.dump(runFnameMap,f,indent=4)
f.close()
