import subprocess

algorithms = "-A " \
             "StaticBFS "\
            "StaticDFS "\
            "LazyDFS "\
            "LazyBFS "\
            "CachingDFS "\
            "CachingBFS "\
            "SimpleInc "\
            "ESTreeML "\
            "OldESTree "\
            "ESTreeQ "\
            "SimpleESTree"

k = "-k 10"
vertices = " -s 10000"
arcs = "--arcSize 100000"


subprocess.call(["AllPairReach", k, algorithms, vertices, arcs])
