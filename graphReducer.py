file_name = "origGraph"
new_file_name = "reducedGraph"

reduced_size = 50000

with open(file_name, "r") as origFile:
    with open(new_file_name, "w") as newFile:

        newFile.write(origFile.readline())
        newFile.write(origFile.readline())

        for lineStr in origFile:
            splitted_line = lineStr.split(" ")

            if int(splitted_line[0]) <= reduced_size and int(splitted_line[1]) <= reduced_size:
                newFile.write(lineStr)
