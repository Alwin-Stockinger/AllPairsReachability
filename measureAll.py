#
# Copyright (c) 2019 : Alwin Stockinger
#
#This program is free software: you can redistribute it and/or modify
#it under the terms of the GNU General Public License as published by
#the Free Software Foundation, either version 3 of the License, or
#(at your option) any later version.
#
#This program is distributed in the hope that it will be useful,
#but WITHOUT ANY WARRANTY; without even the implied warranty of
#MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#GNU General Public License for more details.
#
#You should have received a copy of the GNU General Public License
#along with this program.  If not, see <https://www.gnu.org/licenses/>.
#




import subprocess
import sys

algorithms = []
algoString = "-A "
algorithms.append(algoString + "StaticBFS ")
algorithms.append(algoString + "StaticDFS ")
algorithms.append(algoString + "LazyDFS ")
algorithms.append(algoString + "LazyBFS ")
algorithms.append(algoString + "SimpleInc ")
algorithms.append(algoString + "CachingDFS ")
algorithms.append(algoString + "CachingBFS ")
algorithms.append(algoString + "OldESTree ")
algorithms.append(algoString + "ESTreeQ ")
algorithms.append(algoString + "ESTreeML ")
algorithms.append(algoString + "SimpleESTree ")


overlayAlgorithm = "-O SimpleInc"

timeOutHours = 0
timeOutSeconds = 60 * 10 #60 * timeOutHours
nanoConverter = 1000 * 1000 * 1000  # seconds to nanoseconds
timeOutNumber = nanoConverter * timeOutSeconds
timeOut = "-t " + str(timeOutNumber)

percentageTimesNumber = 1

exponentialKNumber = 1
exponentialK = "--exponentialK " + str(exponentialKNumber)

verticesNumber = 100
arcsNumber = 100000
operationsNumber = 1000000

minDepthNumber = 0
maxDepthNumber = 0

withoutPartitionNumber = 0
simplePartitionNumber = 0
superVerticesNumber = 0
biSuperVerticesNumber = 1
reverseBFSNumber = 0
BFSONumber = 0
DFSONumber = 0
ABFSONumber = 1
SBFSONumber = 0
twoWayNumber = 0

randomVertexNumber = 1

multiArcsNumber = 1

queriesNumber = 30
insertsNumber = 30
removalNumber = 30

twoWayMinNumber = 3
twoWayMaxNumber = 3

twoWayMin = "--minTwoWay " + str(twoWayMinNumber)
twoWayMax = "--maxTwoWay " + str(twoWayMaxNumber)

kMax = "-k 5000"
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
biSuperVertices = "--testBiSuper " + str(biSuperVerticesNumber)
reverseBFS= "--testReverseBFS " + str(reverseBFSNumber)
BFSO = "--testBFSO " + str(BFSONumber)
DFSO = "--testDFSO " + str(DFSONumber)
ABFSO = "--testAdvancedBFS " + str(ABFSONumber)
SBFSO = "--testSBFS " + str(SBFSONumber)
twoWay = "--testTwoWay " + str(twoWayNumber)

randomVertex = "--randomVertexAdd " + str(randomVertexNumber)

multiArcs = "--multiArcs " + str(multiArcsNumber)

queries = "-q " + str(queriesNumber)
inserts = "-a " + str(insertsNumber)
removes = "-r " + str(removalNumber)

repartitionNumber = 0
repartition = "--repartition " + str(repartitionNumber)





#if len(sys.argv) == 1:

#    for i in range(1, 2):
 #       arcs = "--arcSize " + str(arcsNumber)

  #      callString += " " + vertices + " " + arcs + " " + operations + " " + queries + " " + inserts + " " + removes
   #     subprocess.call([callString], shell=True)

    #    arcsNumber *= 2

#elif len(sys.argv) == 2:
 #   inputFile = "-i " + sys.argv[1]
  #  subprocess.call([callString + " " + inputFile], shell=True)

if len(sys.argv) == 3:

    for i in range(1, 10):

        for algorithm in algorithms:

            callString = "taskset -c 3 ./AllPairReach"
            callString += " " + kMax + " " + algorithm + " " + timeOut \
                          + " " + kMin + " " + minDepth + " " + maxDepth + " " + withoutPartition + " " + multiArcs \
                          + " " + exponentialK + " " + repartition + " " + overlayAlgorithm + " " + simplePartition + " " + superVertices + " " + ABFSO + " " + SBFSO + " " + reverseBFS + " " + BFSO + " " + percentageTimes + " " + DFSO + " " + twoWay + " " + twoWayMin + " " + twoWayMax + " " + biSuperVertices + " " + randomVertex



            inputFile = "-i " + sys.argv[1]
            squashRatio = "-S " + sys.argv[2]
            subprocess.call([callString + " " + inputFile + " " + squashRatio],
                            shell=True)

            repartitionNumber *= 2
            repartition = "--repartition " + str(repartitionNumber)

else:
    print("too many arguments")
