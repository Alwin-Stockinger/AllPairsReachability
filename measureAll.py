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
             "ESTreeML " \
             "OldESTree " \
             "ESTreeQ " \
             "SimpleESTree"

if len(sys.argv) == 1:

    arcsNumber = 50000

    for i in range(1, 5):
        k = "-k 10"
        vertices = "-s 100000"
        arcs = "--arcSize " + str(arcsNumber)
        arcsNumber *= 2
        subprocess.call(["./AllPairReach" + " " + k + " " + algorithms + " " + vertices + " " + arcs], shell=True)

elif len(sys.argv) == 2:
    inputFile = "-i " + sys.argv[1]
    subprocess.call(["./AllPairReach" + " " + inputFile + " " + algorithms], shell=True)

elif len(sys.argv) == 3:
    inputFile = "-i " + sys.argv[1]
    squashRatio = "-S " + sys.argv[2]
    subprocess.call(["./AllPairReach" + " " + inputFile + " " + algorithms + " " + squashRatio], shell=True)

else:
    print("too many arguments")
