#!/usr/bin/env python3
# -*- coding: utf-8 -*-
#
# plotter.py
# Description:
# -----------------------------------------------------------------------------
#
# Started on <mié 11-10-2023 15:41:35.073882224 (1697031695)>
# Carlos Linares López <carlos.linares@uc3m.es>
#

"""
Generation of GNUplot images from spreadsheet data
"""

# imports
# -----------------------------------------------------------------------------
import argparse
import re
import time

import pyexcel

import argparser
import pltchecker
import pltGNUfile
import pltserie
import spsreader
import utils
import version

# globals
# -----------------------------------------------------------------------------

# default logger
LOGGER = utils.LOGGER

# regular expressions
RE_SERIES = "\s*(?P<legend>[^:]+)\s*:(?P<condition>.*)\s*"

# debug messsages
DEBUG_DATALINE = "Data accepted in serie '{}': {}"

# info messages
INFO_ACCESSING_SPREADSHEET = "Opening spreadsheet {} ..."
INFO_NUMBER_DATAPOINTS = "Number of datapoints:"
INFO_NUMBER_DATAPOINTS_SERIE = "\tSerie {}: {}"
INFO_NUMBER_DATALINES = "Number of data lines: {}"
INFO_ELAPSED_TIME = "Elapsed time: {0}"

# warning message
WARNING_NO_GNUPLOT_FILE = "No GNUplot file was created"

# error messages
ERROR_UNKNOWN_HEADER = "The {}-name does not exist in the current line and will be ignored {}"

# critical messages
CRITICAL_DUPLICATED_HEADER = "Duplicated header {}"
CRITICAL_INVALID_SERIE = "The serie {} can not be parsed. Type '--help' to get additional information"

# -----------------------------------------------------------------------------
# get_data
#
# return a list with all series of data accepted from the given spreadsheet,
# represented as instances of PLTserie.
#
# Each serie is defined by a legend and a condition separated by a colon, e.g.,
# "k=1:k==1" where the condition is any valid Python boolean expression
# (including matching regular expressions, e.g., ""Problem #0:
# re.match('00/\d+', id)"") which can use variables that have to be found in the
# spreadsheet as header names.
#
# Every datapoint of each serie consists of a tuple (x, y) whose values are
# given by the contents of the headers xname and yname respectively.
# -----------------------------------------------------------------------------
def get_data(spreadsheet: str,
             series: list, xname: str, yname: str) -> list:
    """return a list with all series of data accepted from the given
       spreadsheet, represented as instances of PLTserie.

       Each serie is defined by a legend and a condition separated by a colon,
       e.g., "k=1:k==1" where the condition is any valid Python boolean
       expression (including matching regular expressions, e.g., ""Problem #0:
       re.match('00/\d+', id)"") which can use variables that have to be found
       in the spreadsheet as header names.

       Every datapoint of each serie consists of a tuple (x, y) whose values are
       given by the contents of the headers xname and yname respectively.

    """

    # --initialization
    nblines = 0
    LOGGER.info(INFO_ACCESSING_SPREADSHEET.format(spreadsheet))

    # create a list of strings with the legends and conditions of each serie,
    # and also, a container for each serie to reteurn
    data = []
    legends = []
    conditions = []
    for iserie in series:

        # extract the legend and condition of this serie
        if (m:=re.match(RE_SERIES, iserie)):
            legend = m.group('legend').strip()
            legends.append(legend)
            conditions.append(m.group('condition').strip())
            data.append(pltserie.PLTSerie(legend, xname, yname))

        else:

            # otherweise, this serie was not correctly typed and process must
            # halt
            LOGGER.critical(CRITICAL_INVALID_SERIE.format(iserie))
            raise ValueError(CRITICAL_INVALID_SERIE.format(iserie))

    # process all records to get a list of ordinary dictionaries
    for irecord in pyexcel.get_records(file_name=spreadsheet):

        # create an ordinary dictionary to represent the information of this line
        line = {}
        for ikey in irecord:

            # check this header is not duplicated
            if ikey in line:
                LOGGER.critical(CRITICAL_DUPLICATED_HEADER.format(ikey))

            # add this key to the dictionary
            line[ikey] = irecord[ikey]

        # once the entire line has been retrieved, ensure that there are headers
        # named after the x and y names. If not, skip this line
        if xname not in line:
            LOGGER.error(ERROR_UNKNOWN_HEADER.format("x", line))
            continue
        if yname not in line:
            LOGGER.error(ERROR_UNKNOWN_HEADER.format("y", line))
            continue

        # once the entire line has been retrieved in an ordinary dictionary,
        # check what series are verified, in case any has been given
        if len(series) > 0:
            checker = pltchecker.PLTChecker(line, conditions)
            results = checker.check()
            for index, iresult in enumerate(results):

                # if this condition was satisfied
                if iresult:

                    # then add the corresponding point of this line into its
                    # respective serie
                    data[index] += (line[xname], line[yname])

                    # in case a debug level was set, show the data line added to the
                    # pool
                    LOGGER.debug(DEBUG_DATALINE.format(data[index].get_legend(),
                                                       (line[xname], line[yname])))

        else:

            # otherwise, if no serie was given, then accept all rows, and show a
            # DEBUG message
            data[index] += (line[xname], line[yname])
            LOGGER.debug(DEBUG_DATALINE.format(data[index].get_legend(),
                                               (line[xname], line[yname])))

        # and increment the number of processed lines
        nblines += 1

    # show the number of lines processed
    LOGGER.info(INFO_NUMBER_DATALINES.format(nblines))

    # and return the data computed with all series
    return data


# -----------------------------------------------------------------------------
# create_gnuplotfile
#
# Given a list of series represented as instances of PLTserie, return a
# GNUplotfile which contains all those series and is named after gnufilename
# -----------------------------------------------------------------------------
def create_gnuplotfile(gnufilename: str, data: list) -> pltGNUfile.PLTGNUfile:
    """Given a list of series represented as instances of PLTserie, return a
       GNUplotfile which contains all those series and is named after
       gnufilename

    """

    # --initialization
    gnustream = None

    # in case no serie has been created, then return immediately
    if len(data) > 0:

        # get the x- and y- legends. It is assumed that all series have been
        # created with the same values for the x- and y- axis
        (xname, yname) = (data[0].get_xtitle(), data[0].get_ytitle)

        # create a GNUplot file with the information of all the given series, in
        # case any has been given. Note the titles for the x and y axis are the same
        # for all series
        if gnufilename is not None and len(gnufilename) > 0:
            gnustream = pltGNUfile.PLTGNUfile(gnufilename, xname, yname)

            # and add all series
            for iserie in data:
                gnustream += iserie

    # finally, return the GNUplot file with all series extracted from the
    # spreadsheet
    return gnustream


# -----------------------------------------------------------------------------
# do_plot
#
# Execute the plot command with the given parameters
# -----------------------------------------------------------------------------
def do_plot(params: argparse.Namespace):
    """Execute the plot command with the given parameters"""

    # spreadsheets have to be given as .xlsx files
    spreadsheet = utils.get_filename(params.file, ".xlsx")

    # ensure the spreadsheet is readable
    readable, err = utils.check_file_readable(spreadsheet)
    if not readable:
        LOGGER.critical(err)
        raise ValueError(err)

    # get the data from the spreadsheet and create a gnuplot file with all
    # series extracted
    data = create_gnuplotfile(params.output,
                              get_data(spreadsheet, params.series, params.x, params.y))
    if data is not None:
        LOGGER.info(INFO_NUMBER_DATAPOINTS)
        for iserie in data:
            LOGGER.info(INFO_NUMBER_DATAPOINTS_SERIE.format(iserie.get_legend(), len(iserie)))

        # and generate the gnuplot file with the specified title
        if params.title is not None and len(params.title) > 0:
            data.set_title(params.title)
        data.write_gnuplot()

    else:
         LOGGER.warning(WARNING_NO_GNUPLOT_FILE)

# -----------------------------------------------------------------------------
# do_ktime
#
# Execute the ktime command with the given parameters
# -----------------------------------------------------------------------------
def do_ktime(params: argparse.Namespace):
    """Execute the ktime command with the given parameters"""

    print("Not implemented yet!")


# -----------------------------------------------------------------------------
# main body
# -----------------------------------------------------------------------------
def main():
    """Main body

    """

    # -------------------------------------------------------------------------
    # --initialization

    # get the current time
    start_time = time.time()

    # invoke the parser and parse all commands
    params = argparser.createPrgArgParser().parse_args()

    # set the requested logging level
    LOGGER.setLevel(utils.get_logging_level(params.level))

    # execute the specified command
    {
        "plot": do_plot,
        "ktime": do_ktime
    }[params.command](params)

    # show the elapsed time
    end_time = time.time()
    LOGGER.info(INFO_ELAPSED_TIME.format(utils.seconds_to_str(end_time - start_time)))


# main
# -----------------------------------------------------------------------------
if __name__ == '__main__':
    main()



# Local Variables:
# mode:python
# fill-column:80
# End:
