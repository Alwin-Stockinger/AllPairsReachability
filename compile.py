import subprocess

subprocess.call(["cmake", "-DCMAKE_BUILD_TYPE=Release", "CMakeLists.txt"])
subprocess.call(["make"])
subprocess.call(["./runTests"])
