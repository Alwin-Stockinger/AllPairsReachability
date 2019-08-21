file_name = "origGraph"
new_file_name = "reducedNLWiki"

endTime = 1072915200  # 2004

with open(file_name, "r") as origFile:
    with open(new_file_name, "w") as newFile:

        newFile.write(origFile.readline())
        newFile.write(origFile.readline())


        for lineStr in origFile:
            splitted_line = lineStr.split(" ")

            currentTime = int(splitted_line[3])

            if currentTime < endTime:
                newFile.write(lineStr)
