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
             "SimpleESTree"\
             "ESTreeML " \
             "OldESTree " \
             "ESTreeQ "


if len(sys.argv) == 1:

    verticesNumber = 10000
    arcsNumber = 5000
    operationsNumber = 10000

    for i in range(1, 5):
        k = "-k 10"
        vertices = "-s " + str(verticesNumber)
        arcs = "--arcSize " + str(arcsNumber)
        operations = "-o " + str(operationsNumber)

        callString = "./AllPairReach" + " " + k + " " + algorithms + " " + vertices + " " + arcs + " " + operations
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
