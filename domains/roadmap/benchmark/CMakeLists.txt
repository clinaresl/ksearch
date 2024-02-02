# This file allows test data to be downloaded at CMake config time


# The following defines a macro for unzipping .gz files from https://github.com/Amber-MD/cmake-buildscripts
#MIT License
#
#Copyright (c) 2017 Amber Molecular Dynamics
#
#Permission is hereby granted, free of charge, to any person obtaining a copy
#of this software and associated documentation files (the "Software"), to deal
#in the Software without restriction, including without limitation the rights
#to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
#copies of the Software, and to permit persons to whom the Software is
#furnished to do so, subject to the following conditions:
#
#The above copyright notice and this permission notice shall be included in all
#copies or substantial portions of the Software.
#
#THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
#IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
#FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
#AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
#LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
#OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
#SOFTWARE.

# macro for uncompressing gzipped files.  Can utilize (p)7zip and gunzip.
# un-gzips SOURCE into DESTINATION
# This does not create a target to force DESTINATION to be built, use add_custom_target to do that.

#figure out which ungz command to use
find_program(GUNZIP_LOCATION gunzip DOC "Path to gunzip program. Set either this one, GZIP_LOCATION, or 7Z_LOCATION to build amber.")
find_program(GZIP_LOCATION gzip DOC "Path to gzip program. Set either this one, GUNZIP_LOCATION, or 7Z_LOCATION to build amber.")
find_program(7Z_LOCATION 7z 7za 7-Zip/7z.exe PATHS ENV PROGRAMFILES DOC "Path to the (p)7-zip command line program, either 7z or 7za. Set either this one, GZIP_LOCATION, or GUNZIP_LOCATION to build amber.")

if(GZIP_LOCATION)
    set(UNZIP_COMMAND ${GZIP_LOCATION} -dc)
elseif(GUNZIP_LOCATION)
    set(UNZIP_COMMAND ${GUNZIP_LOCATION} -c)
elseif(7Z_LOCATION)
    set(UNZIP_COMMAND ${7Z_LOCATION} e -so)
else()
    #cause the searches to be repeated
    unset(GZIP_LOCATION CACHE)
    unset(GUNZIP_LOCATION CACHE)
    unset(7Z_LOCATION CACHE)

    if(WIN32)
        message(FATAL_ERROR "A gzip unarchiver is required to build AMBER, but was not found.  Please install gzip or 7-zip and set GZIP_LOCATION, GUNZIP_LOCATION, or 7Z_LOCATION to the gzip or 7z executable.")
    else()
        message(FATAL_ERROR "A gzip unarchiver is required to build AMBER, but was not found.  Please install gzip, gunzip, or p7zip.")
    endif()
endif()

#message(STATUS "Un-gzip command: ${UNZIP_COMMAND}")

macro(ungzip_file SOURCE DESTINATION)
    execute_process(COMMAND ${UNZIP_COMMAND} ${SOURCE} > ${DESTINATION})
endmacro(ungzip_file)


option(DATA_ALL "Download all DIMACS data files")

function(download_data AREA)
    option(DATA_${AREA} "Download DIMACS data for area: ${AREA}")
    if (DATA_${AREA} OR DATA_ALL)
        # If graph file doesn't exist, download and extract it
        if (NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/USA-road-d.${AREA}.gr")
            message(STATUS "Downloading ${AREA} graph file")
            file(DOWNLOAD "http://www.diag.uniroma1.it/~challenge9/data/USA-road-d/USA-road-d.${AREA}.gr.gz" tmp.gz SHOW_PROGRESS)
            ungzip_file(tmp.gz ${CMAKE_CURRENT_SOURCE_DIR}/USA-road-d.${AREA}.gr)
        endif ()
        # If coordinate file doesn't exist, download and extract it
        if (NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/USA-road-d.${AREA}.co")
            message(STATUS "Downloading ${AREA} coordinate file")
            file(DOWNLOAD "http://www.diag.uniroma1.it/~challenge9/data/USA-road-d/USA-road-d.${AREA}.co.gz" tmp.gz SHOW_PROGRESS)
            ungzip_file(tmp.gz ${CMAKE_CURRENT_SOURCE_DIR}/USA-road-d.${AREA}.co)
        endif ()
    endif ()
endfunction()

download_data("USA")
download_data("CTR")
download_data("W")
download_data("E")
download_data("LKS")
download_data("CAL")
download_data("NE")
download_data("NW")
download_data("FLA")
download_data("COL")
download_data("BAY")
download_data("NY")

file(REMOVE ./tmp.gz)