import subprocess
import sys

algorithms = "-A "
algorithms += "StaticBFS "
algorithms += "StaticDFS "
#algorithms += "LazyDFS "
#algorithms += "LazyBFS "
#algorithms += "CachingDFS "
#algorithms += "CachingBFS "
#algorithms += "OldESTree "
#algorithms += "ESTreeQ "
#algorithms += "ESTreeML "
#algorithms += "SimpleESTree "
#algorithms += "SimpleInc "


overlayAlgorithm = "-O SimpleInc"

timeOutHours = 0
timeOutSeconds = 60 * timeOutHours
nanoConverter = 1000 * 1000 * 1000  # seconds to nanoseconds
timeOutNumber = nanoConverter * timeOutSeconds
timeOut = "-t " + str(timeOutNumber)

percentageTimesNumber = 1

exponentialKNumber = 1
exponentialK = "--exponentialK " + str(exponentialKNumber)

verticesNumber = 60000
arcsNumber = 120000
operationsNumber = 5

minDepthNumber = 0
maxDepthNumber = 0

withoutPartitionNumber = 0
simplePartitionNumber = 0
superVerticesNumber = 0
reverseBFSNumber = 0
BFSONumber = 0
DFSONumber = 0
ABFSONumber = 0
SBFSONumber = 1
twoWayNumber = 1

multiArcsNumber = 1

queriesNumber = 30
insertsNumber = 30
removalNumber = 30

twoWayMin = "--minTwoWay 1000"
twoWayMax = "--maxTwoWay 1000"

kMax = "-k 33000"
kMin = "--kMin 4096"


vertices = "-s " + str(verticesNumber)
arcs = "--arcSize " + str(arcsNumber)
operations = "-o " + str(operationsNumber)

percentageTimes = "--percentageTimes " + str(percentageTimesNumber)

minDepth = "--minDepth " + str(minDepthNumber)
maxDepth = "--maxDepth " + str(maxDepthNumber)

withoutPartition = "--testWithoutPartition " + str(withoutPartitionNumber)
simplePartition = "--testPartition " + str(simplePartitionNumber)
superVertices= "--testSuperVertex " + str(superVerticesNumber)
reverseBFS= "--testReverseBFS " + str(reverseBFSNumber)
BFSO = "--testBFSO " + str(BFSONumber)
DFSO = "--testDFSO " + str(DFSONumber)
ABFSO = "--testAdvancedBFS " + str(ABFSONumber)
SBFSO = "--testSBFS " + str(SBFSONumber)
twoWay = "--testTwoWay " + str(twoWayNumber)

multiArcs = "--multiArcs " + str(multiArcsNumber)

queries = "-q " + str(queriesNumber)
inserts = "-a " + str(insertsNumber)
removes = "-r " + str(removalNumber)

repartitionNumber = 0
repartition = "--repartition " + str(repartitionNumber)

callString = "taskset -c 3 ./AllPairReach"

callString += " " + kMax + " " + algorithms + " " + timeOut \
              + " " + kMin + " " + minDepth + " " + maxDepth + " " + withoutPartition + " " + multiArcs \
              + " " + exponentialK + " " + repartition + " " + overlayAlgorithm + " " + simplePartition + " " + superVertices + " " + ABFSO + " " + SBFSO + " " + reverseBFS + " " + BFSO + " " + percentageTimes + " " + DFSO + " " + twoWay + " " + twoWayMin + " " + twoWayMax
print("CallString: " + callString)

if len(sys.argv) == 1:

    for i in range(1, 5):
        arcs = "--arcSize " + str(arcsNumber)

        callString += " " + vertices + " " + arcs + " " + operations + " " + queries + " " + inserts + " " + removes
        subprocess.call([callString], shell=True)

        arcsNumber *= 2

elif len(sys.argv) == 2:
    inputFile = "-i " + sys.argv[1]
    subprocess.call([callString + " " + inputFile], shell=True)

elif len(sys.argv) == 3:
    inputFile = "-i " + sys.argv[1]
    squashRatio = "-S " + sys.argv[2]
    subprocess.call([callString + " " + inputFile + " " + squashRatio],
                    shell=True)

else:
    print("too many arguments")
