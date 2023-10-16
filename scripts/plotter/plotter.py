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

import numpy
import pyexcel

import argparser
import pltchecker
import pltGNUfile
import pltkserie
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
RE_PROBLEM_ID = "^\s*(?P<id>\d+)/(?P<k>\d+)\s*$"

# debug messsages
DEBUG_DATALINE = "Data accepted in serie '{}': {}"

# info messages
INFO_ACCESSING_SPREADSHEET = "Opening spreadsheet {} ..."
INFO_NUMBER_DATAPOINTS = "Number of datapoints:"
INFO_NUMBER_DATAPOINTS_SERIE = "\tSerie {}: {}"
INFO_NUMBER_DATALINES = "Number of data lines: {}"
INFO_ELAPSED_TIME = "Elapsed time: {0}"

# warning message
WARNING_NO_DATA = "No data met the given criteria in the series"
WARNING_NO_GNUPLOT_FILE = "No GNUplot file was generated"

# error messages
ERROR_UNKNOWN_HEADER = "The {}-name does not exist in the current line and will be ignored {}"

# critical messages
CRITICAL_DUPLICATED_HEADER = "Duplicated header {}"
CRITICAL_INVALID_SERIE = "The serie {} can not be parsed. Type '--help' to get additional information"

# -----------------------------------------------------------------------------
# filter_data
#
# Given a dictionary whose keys are the header names of a spreadsheet and whose
# values are the cell contents retrieved from the same line, update data (which
# must be given as a list of instances of either pltserie or pltkserie) with
# those tuples (x, y) that match each condition respectively. The names of the
# variables x and y are given in xname and yname.
#
# In case no serie is provided update data right away with the corresponding
# tuple from the given line
# -----------------------------------------------------------------------------
def filter_data(data: list, line: dict,
                conditions: list, xname: str, yname: str) -> list:
    """Given a dictionary whose keys are the header names of a spreadsheet and
       whose values are the cell contents retrieved from the same line, update
       data (which must be given as a list of instances of pltserie) with those
       tuples (x, y) that match each condition respectively. The names of the
       variables x and y are given in xname and yname.

       In case no serie is provided update data right away with the
       corresponding tuple from the given line

    """

    if len(conditions) > 0:
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
        # DEBUG message. Note that only one serie is produced in this case
        data[0] += (line[xname], line[yname])
        LOGGER.debug(DEBUG_DATALINE.format(data[index].get_legend(),
                                           (line[xname], line[yname])))

    # and return the updated data
    return data


# -----------------------------------------------------------------------------
# get_data
#
# return a list with all series of data accepted from a list of spreadsheets,
# each represented as an intance of PLTSerie
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
def get_data(spreadsheets: list,
             series: list, xname: str, yname: str) -> list:
    """return a list with all series of data accepted from a list of
       spreadsheets, each represented as an intance of PLTSerie

       Each serie is defined by a legend and a condition separated by a colon,
       e.g., "k=1:k==1" where the condition is any valid Python boolean
       expression (including matching regular expressions, e.g., ""Problem #0:
       re.match('00/\d+', id)"") which can use variables that have to be found
       in the spreadsheet as header names.

       Every datapoint of each serie consists of a tuple (x, y) whose values are
       given by the contents of the headers xname and yname respectively.

    """

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

    # process all spreadsheets
    for spreadsheet in spreadsheets:

        LOGGER.info(INFO_ACCESSING_SPREADSHEET.format(spreadsheet))

        # count the processed lines
        nblines = 0

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
            filter_data(data, line, conditions, xname, yname)

            # and increment the number of processed lines
            nblines += 1

        # show the number of lines processed
        LOGGER.info(INFO_NUMBER_DATALINES.format(nblines))

    # before leaving, remove all series which contain no data
    output = []
    for iserie in data:
        if len(iserie) > 0:
            output.append(iserie)

    # and return the data computed with all series
    return output


# -----------------------------------------------------------------------------
# get_k_data
#
# return a list with all series of data accepted from a list of spreadsheets,
# each represented as an instance of PLTKSerie.
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
def get_k_data(spreadsheets: list,
               series: list, xname: str, yname: str) -> list:
    """return a list with all series of data accepted from a list of
       spreadsheets, each represented as an instance of PLTKSerie.

       Each serie is defined by a legend and a condition separated by a colon,
       e.g., "k=1:k==1" where the condition is any valid Python boolean
       expression (including matching regular expressions, e.g., ""Problem #0:
       re.match('00/\d+', id)"") which can use variables that have to be found
       in the spreadsheet as header names.

       Every datapoint of each serie consists of a tuple (x, y) whose values are
       given by the contents of the headers xname and yname respectively.

    """

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
            data.append(pltkserie.PLTKSerie(legend, xname, yname))

        else:

            # otherweise, this serie was not correctly typed and process must
            # halt
            LOGGER.critical(CRITICAL_INVALID_SERIE.format(iserie))
            raise ValueError(CRITICAL_INVALID_SERIE.format(iserie))

    # process all spreadsheets
    for spreadsheet in spreadsheets:

        LOGGER.info(INFO_ACCESSING_SPREADSHEET.format(spreadsheet))

        # count the processed lines
        nblines = 0

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

            # verify whether this is the case where the number of paths found equals
            # the number of paths requested. If not, skip it
            m = re.match(RE_PROBLEM_ID, line["id"])
            if int(m.group("k")) != int(line["k"]):
                continue

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
            filter_data(data, line, conditions, xname, yname)

            # and increment the number of processed lines
            nblines += 1

        # show the number of lines processed
        LOGGER.info(INFO_NUMBER_DATALINES.format(nblines))

    # before leaving, remove all series which contain no data
    output = []
    for iserie in data:
        if len(iserie) > 0:
            output.append(iserie)

    # and return the data computed with all series
    return output


# -----------------------------------------------------------------------------
# create_gnuplotfile
#
# Given a list of series represented as instances of either PLTserie or
# PLTKSerie, return a GNUplotfile named after gnufilename which contains all
# those series and the specified title if any is given. In case a png filename
# is given, specific commands are added to generate the png file
# -----------------------------------------------------------------------------
def create_gnuplotfile(series: list, gnufilename: str, title: str, png: str) -> pltGNUfile.PLTGNUfile:
    """Given a list of series represented as instances of either PLTserie or
       PLTKSerie, return a GNUplotfile named after gnufilename which contains
       all those series and the specified title if any is given. In case a png
       filename is given, specific commands are added to generate the png file

    """

    # --initialization
    gnustream = None

    # in case no serie has been created, then return immediately
    if len(series) > 0:

        # get the x- and y- legends. It is assumed that all series have been
        # created with the same values for the x- and y- axis
        (xname, yname) = (series[0].get_xtitle(), series[0].get_ytitle())

        # create a GNUplot file with the information of all the given series, in
        # case any has been given. Note the titles for the x and y axis are the same
        # for all series
        if gnufilename is not None and len(gnufilename) > 0:
            gnustream = pltGNUfile.PLTGNUfile(gnufilename, xname, yname)

            # and add all series
            for iserie in series:

                # in case this is a kserie, then apply the operator and add the
                # resulting serie to the gnuplot file
                if isinstance(iserie, pltkserie.PLTKSerie):
                    iserie.exec(numpy.average)
                    gnustream += iserie
                else:

                    # otherwise, add the serie straight away
                    gnustream += iserie

            # create a png image, in case it was requested
            if png is not None and len(png) > 0:
                gnustream.set_png(png)

            # give the plot file a title, if any was given
            if title is not None and len(title) > 0:
                gnustream.set_title(title)

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
    spreadsheets = []
    for ifile in params.file:
        spreadsheet = utils.get_filename(ifile, ".xlsx")

        # ensure each spreadsheet is readable
        readable, err = utils.check_file_readable(spreadsheet)
        if not readable:
            LOGGER.critical(err)
            raise ValueError(err)

        # finally, add it to the list of spreadsheets to process
        spreadsheets.append(spreadsheet)

    # importantly, the series requested by the user have to be provided always
    # as a list. Moreover, if no serie is requested, then one accepting all data
    # (i.e., with condition True) has to be used instead. In this case the serie
    # is named after a concatenation of the names of all spreadsheets
    user_series = ["{}:True".format("/".join(spreadsheets))] if params.series is None else params.series

    # in case any serie is produced from the given spreadsheet using the
    # variables x and y
    series = get_data(spreadsheets, user_series, params.x, params.y)
    if series is not None and len(series) > 0:
        LOGGER.info(INFO_NUMBER_DATAPOINTS)
        for iserie in series:
            LOGGER.info(INFO_NUMBER_DATAPOINTS_SERIE.format(iserie.get_legend(), len(iserie)))

        # in case data was generated, but no output file was given, then issue a
        # warning
        if params.output is None or len(params.output) == 0:
            LOGGER.warning(WARNING_NO_GNUPLOT_FILE)
        else:

            # generate the gnuplot file with all the specified options including a
            # title if any was provided by the user
            gnufile = create_gnuplotfile(series, params.output, params.title, params.png)
            gnufile.write_gnuplot()

    else:
         LOGGER.warning(WARNING_NO_DATA)


# -----------------------------------------------------------------------------
# do_ktime
#
# Execute the ktime command with the given parameters
# -----------------------------------------------------------------------------
def do_ky(params: argparse.Namespace):
    """Execute the ktime command with the given parameters"""

    # spreadsheets have to be given as .xlsx files
    spreadsheets = []
    for ifile in params.file:
        spreadsheet = utils.get_filename(ifile, ".xlsx")

        # ensure each spreadsheet is readable
        readable, err = utils.check_file_readable(spreadsheet)
        if not readable:
            LOGGER.critical(err)
            raise ValueError(err)

        # finally, add it to the list of spreadsheets to process
        spreadsheets.append(spreadsheet)

    # importantly, the series requested by the user have to be provided always
    # as a list. Moreover, if no serie is requested then one accepting all data
    # (i.e., with condition True) has to be used instead. In this case the serie
    # is named after a concatenation of the names of all spreadhseets
    user_series = ["{}:True".format("/".join(spreadsheets))] if params.series is None else params.series

    # in case any serie si produced from the given spreadsheet using the
    # variables k and the given y
    series = get_k_data(spreadsheets, user_series, "k", params.y)
    if series is not None and len(series) > 0:
        LOGGER.info(INFO_NUMBER_DATAPOINTS)
        for iserie in series:
            LOGGER.info(INFO_NUMBER_DATAPOINTS_SERIE.format(iserie.get_legend(), len(iserie)))

        # in case data was generated, but no output file was given, then issue a
        # warning
        if params.output is None or len(params.output) == 0:
            LOGGER.warning(WARNING_NO_GNUPLOT_FILE)
        else:
            gnufile = create_gnuplotfile(series, params.output, params.title, params.png)
            gnufile.write_gnuplot()

    else:
        LOGGER.warning(WARNING_NO_DATA)

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
        "ky": do_ky
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
