import subprocess
import sys

algorithms = "-A " \
             "StaticBFS " \
             "StaticDFS " \
             "LazyDFS " \
             "LazyBFS " \
             "CachingDFS " \
             "CachingBFS " \
             "SimpleInc " \
             "SimpleESTree " \
             "ESTreeML " \
             "OldESTree " \
             "ESTreeQ "

if len(sys.argv) == 1:

    verticesNumber = 10000
    arcsNumber = 5000
    operationsNumber = 10000

    minDepthNumber = 0
    maxDepthNumber = 2

    withoutPartitionNumber = 0

    timeOutHours = 3
    timeOutSeconds = 60 * 60 * timeOutHours
    nanoConverter = 1000 * 1000 * 1000  # seconds to nanoseconds
    timeOutNumber = nanoConverter * timeOutSeconds
    multiArcsNumber = 0

    for i in range(1, 5):
        k = "-k 10"
        kMin = "--kMin 2"
        vertices = "-s " + str(verticesNumber)
        arcs = "--arcSize " + str(arcsNumber)
        operations = "-o " + str(operationsNumber)
        timeOut = "-t " + str(timeOutNumber)
        minDepth = "--minDepth " + str(minDepthNumber)
        maxDepth = "--maxDepth " + str(maxDepthNumber)
        withoutPartition = "--withoutPartition " + str(withoutPartitionNumber)
        multiArcs = "--multiArcs" + str(multiArcsNumber)

        callString = "./AllPairReach"
        callString += " " + k + " " + algorithms + " " + vertices + " " + arcs + " " + operations + " " + timeOut \
                      + " " + kMin + " " + minDepth + " " + maxDepth + " " + withoutPartition
        subprocess.call([callString], shell=True)

        arcsNumber *= 2

elif len(sys.argv) == 2:
    inputFile = "-i " + sys.argv[1]
    subprocess.call(["./AllPairReach" + " " + inputFile + " " + algorithms], shell=True)

elif len(sys.argv) == 3:
    inputFile = "-i " + sys.argv[1]
    squashRatio = "-S " + sys.argv[2]
    subprocess.call(["./AllPairReach" + " " + inputFile + " " + algorithms + " " + squashRatio], shell=True)

else:
    print("too many arguments")
