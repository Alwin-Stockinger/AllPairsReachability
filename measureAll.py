import subprocess
import sys

algorithms = "-A "
algorithms += "StaticBFS "
algorithms += "StaticDFS "
algorithms += "LazyDFS "
algorithms += "LazyBFS "
algorithms += "CachingDFS "
algorithms += "CachingBFS "
algorithms += "SimpleInc "
algorithms += "SimpleESTree "
algorithms += "ESTreeML "
algorithms += "OldESTree "
algorithms += "ESTreeQ "

timeOutHours = 0
timeOutSeconds = 60 * 60 * timeOutHours
nanoConverter = 1000 * 1000 * 1000  # seconds to nanoseconds
timeOutNumber = nanoConverter * timeOutSeconds
timeOut = "-t " + str(timeOutNumber)

exponentialKNumber = 1
exponentialK = "--exponentialK " + str(exponentialKNumber)

verticesNumber = 10000
arcsNumber = 5000
operationsNumber = 10000

minDepthNumber = 0
maxDepthNumber = 2

withoutPartitionNumber = 1

multiArcsNumber = 1

queriesNumber = 30
insertsNumber = 30
removalNumber = 30

k = "-k 100"
kMin = "--kMin 2"
vertices = "-s " + str(verticesNumber)
arcs = "--arcSize " + str(arcsNumber)
operations = "-o " + str(operationsNumber)

minDepth = "--minDepth " + str(minDepthNumber)
maxDepth = "--maxDepth " + str(maxDepthNumber)
withoutPartition = "--withoutPartition " + str(withoutPartitionNumber)
multiArcs = "--multiArcs " + str(multiArcsNumber)

queries = "-q " + str(queriesNumber)
inserts = "-a " + str(insertsNumber)
removes = "-r " + str(removalNumber)

callString = "nice -n -20 ./AllPairReach"

callString += " " + k + " " + algorithms + " " + timeOut \
              + " " + kMin + " " + minDepth + " " + maxDepth + " " + withoutPartition + " " + multiArcs \
              + " " + exponentialK

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
