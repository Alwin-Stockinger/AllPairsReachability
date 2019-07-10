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

timeOutHours = 1
timeOutSeconds = 60 * 60 * timeOutHours
nanoConverter = 1000 * 1000 * 1000  # seconds to nanoseconds
timeOutNumber = nanoConverter * timeOutSeconds
timeOut = "-t " + str(timeOutNumber)

if len(sys.argv) == 1:

    verticesNumber = 10000
    arcsNumber = 5000
    operationsNumber = 10000

    minDepthNumber = 0
    maxDepthNumber = 2

    withoutPartitionNumber = 0


    multiArcsNumber = 0
    exponentialKNumber = 0

    queriesNumber = 30
    insertsNumber = 30
    removalNumber = 30

    for i in range(1, 5):
        k = "-k 10"
        kMin = "--kMin 2"
        vertices = "-s " + str(verticesNumber)
        arcs = "--arcSize " + str(arcsNumber)
        operations = "-o " + str(operationsNumber)

        minDepth = "--minDepth " + str(minDepthNumber)
        maxDepth = "--maxDepth " + str(maxDepthNumber)
        withoutPartition = "--withoutPartition " + str(withoutPartitionNumber)
        multiArcs = "--multiArcs " + str(multiArcsNumber)
        exponentialK = "--exponentialK " + str(exponentialKNumber)
        queries = "-q " + str(queriesNumber)
        inserts = "-a " + str(insertsNumber)
        removes = "-r " + str(removalNumber)

        callString = "./AllPairReach"
        callString += " " + k + " " + algorithms + " " + vertices + " " + arcs + " " + operations + " " + timeOut \
                      + " " + kMin + " " + minDepth + " " + maxDepth + " " + withoutPartition + " " + multiArcs \
                      + " " + exponentialK + " " + queries + " " + inserts + " " + removes
        subprocess.call([callString], shell=True)

        arcsNumber *= 2

elif len(sys.argv) == 2:
    inputFile = "-i " + sys.argv[1]
    subprocess.call(["./AllPairReach" + " " + inputFile + " " + algorithms + " " + timeOut], shell=True)

elif len(sys.argv) == 3:
    inputFile = "-i " + sys.argv[1]
    squashRatio = "-S " + sys.argv[2]
    subprocess.call(["./AllPairReach" + " " + inputFile + " " + algorithms + " " + squashRatio + " " + timeOut],\
                    shell=True)

else:
    print("too many arguments")
