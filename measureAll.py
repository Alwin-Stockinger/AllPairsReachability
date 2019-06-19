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
    k = "-k 10"
    vertices = "-s 100000"
    arcs = "--arcSize 100000"

    subprocess.call(["./AllPairReach" + " " + k + " " + algorithms + " " + vertices + " " + arcs], shell=True)
elif len(sys.argv) == 2:
    inputFile = "-i " + sys.argv[1]
    subprocess.call(["./AllPairReach" + " " + inputFile + " " + algorithms], shell=True)
else:
    print("too many arguments")


