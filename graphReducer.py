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
