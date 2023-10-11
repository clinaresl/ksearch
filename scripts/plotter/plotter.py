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
import re

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
# return a list of dictionaries with the data read from the spreadsheet. Every
# dictionary contains the data from a single line where the keyword is the
# column name and the value is taken from the corresponding cell
#
# Data is filtered according to the given series, which consist of a list of
# strings. Each serie is a valid Python boolean expression which can use
# variables that have to be found in the spreadsheet
# -----------------------------------------------------------------------------
def get_data(spreadsheet: str,
             series: list, xname: str, yname: str,
             gnufilename: str) -> pltGNUfile.PLTGNUfile:
    """return a list of dictionaries with the data read from the spreadsheet.
       Every dictionary contains the data from a single line where the keyword
       is the column name and the value is taken from the corresponding cell

       Data is filtered according to the given series, which consist of a list
       of strings. Each serie is a valid Python boolean expression which can use
       variables that have to be found in the spreadsheet

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

        # once the entire line has been retrieved in an ordinary dictionary,
        # check what series are verified
        checker = pltchecker.PLTChecker(line, conditions)
        results = checker.check()
        for index, iresult in enumerate(results):

            # if this condition was satisfied
            if iresult:

                # verify (repeatedly, ...) that both the x and y names exist in
                # the current line
                if xname not in line:
                    LOGGER.error(ERROR_UNKNOWN_HEADER.format("x", line))
                    continue
                if yname not in line:
                    LOGGER.error(ERROR_UNKNOWN_HEADER.format("y", line))
                    continue

                # then add the corresponding point of this line into its
                # respective serie
                data[index] += (line[xname], line[yname])

                # in case a debug level was set, show the data line added to the
                # pool
                LOGGER.debug(DEBUG_DATALINE.format(data[index].get_legend(),
                                                   (line[xname], line[yname])))

        # and increment the number of processed lines
        nblines += 1

    # show the number of lines processed
    LOGGER.info(INFO_NUMBER_DATALINES.format(nblines))

    # once the series have been created, create a GNUplot file with the
    # information of all series, in case any has been given. Note the titles for
    # the x and y axis are the same for all series
    gnustream = None
    if gnufilename is not None and len(gnufilename) > 0:
        gnustream = pltGNUfile.PLTGNUfile(gnufilename, xname, yname)

        # and add all series
        for iserie in data:
            gnustream += iserie

    # finally, return the GNUplot file with all series extracted from the
    # spreadsheet
    return gnustream


# -----------------------------------------------------------------------------
# main body
# -----------------------------------------------------------------------------
def main():
    """Main body

    """

    # -------------------------------------------------------------------------
    # --initialization

    # invoke the parser and parse all commands
    params = argparser.createPrgArgParser().parse_args()

    # set the requested logging level
    LOGGER.setLevel(utils.get_logging_level(params.level))

    # spreadsheets have to be given as .xlsx files
    spreadsheet = utils.get_filename(params.file, ".xlsx")

    # ensure the spreadsheet is readable
    readable, err = utils.check_file_readable(spreadsheet)
    if not readable:
        LOGGER.critical(err)
        raise ValueError(err)

    # get the data from the spreadsheet and show information on the standard
    # output
    data = get_data(spreadsheet, params.series, params.x, params.y, params.output)
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

# main
# -----------------------------------------------------------------------------
if __name__ == '__main__':
    main()



# Local Variables:
# mode:python
# fill-column:80
# End:
