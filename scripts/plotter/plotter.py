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

import argparser
import pltchecker
import pltkserie
import pltpgfplot
import pltserie
import pltseries
import spsreader
import utils

# globals
# -----------------------------------------------------------------------------

# default logger
LOGGER = utils.LOGGER

# regular expressions
RE_KAPPA = ";"
RE_SERIES = r'\s*(?P<legend>[^:]+)\s*:(?P<condition>.*)\s*'
RE_PROBLEM_ID = r'^\s*(?P<id>\d+)/(?P<k>\d+)\s*$'

# debug messsages
DEBUG_DATALINE_REMOVED = "Line #{}: {} removed because {}"
DEBUG_DATALINE = "Data accepted in serie '{}': {}"
DEBUG_OUTPUT_TABLE = "LaTeX table selected. No images will be generated"
DEBUG_OUTPUT_IMAGE = "Image generation selected. No tables will be generated"

# info messages
INFO_ACCESSING_SPREADSHEET = "Opening spreadsheet {} ..."
INFO_NUMBER_DATAPOINTS = "Number of datapoints:"
INFO_NUMBER_DATAPOINTS_SERIE = "\tSerie {}: {}"
INFO_NUMBER_DATALINES = "Number of data lines: {}"
INFO_ELAPSED_TIME = "Elapsed time: {0}"
INFO_OUTPUT_FORMAT = "Output format: {}"

# warning message
WARNING_NO_DATA = "No data met the given criteria in the series"
WARNING_NO_TITLE = "No title has been given"
WARNING_UNNECESSARY_PNG = "No .pgn file is necessary when generating LaTeX code (either TikZ/pgfplot file or tables)"

# error messages
ERROR_NO_PNG = "It was requested to generate a gnuplot file, but no png file has been given. Provide one with --png"

# critical messages
CRITICAL_UNKNOWN_HEADER = "The {}-name does not exist in the current line: {}"
CRITICAL_DUPLICATED_HEADER = "Duplicated header {}"
CRITICAL_INVALID_SERIE = "The serie {} can not be parsed. Type '--help' to get additional information"


# -----------------------------------------------------------------------------
# validate_params
#
# Check that the values given are compliant with the options given by the parser
# of this script. In case an error is found, a message is shown and execution is
# halted
# -----------------------------------------------------------------------------
def validate_params(params: argparse.Namespace):
    """Check that the values given are compliant with the options given by the
       parser of this script. In case an error is found, a message is shown and
       execution is halted

    """

    # in case the subcommand 'group' is used, all parameters are assumed to be
    # correct since that command specifically takes pgfplots and produces a
    # pgfgroup
    if params.command != "group":
   
        if params.oformat == 'table':
            LOGGER.debug(DEBUG_OUTPUT_TABLE)

            if params.png is not None:
                LOGGER.warning(WARNING_UNNECESSARY_PNG)

        else:

            LOGGER.debug(DEBUG_OUTPUT_IMAGE)

            if params.title is None:
                LOGGER.warning(WARNING_NO_TITLE)

            if params.oformat == "gnuplot" and params.png is None:
                LOGGER.error(ERROR_NO_PNG)
                raise ValueError(ERROR_NO_PNG)

            if params.oformat == "pgfplot" and params.png is not None:
                LOGGER.warning(WARNING_UNNECESSARY_PNG)

# -----------------------------------------------------------------------------
# filter_data
#
# Given a dictionary whose keys are the header names of a spreadsheet and whose
# values are the cell contents retrieved from the same line, update data (which
# must be given as a list of instances of either pltserie or pltkserie) with
# those tuples (x, y) that match each condition respectively. The names of the
# variables x and y are given in xname and yname.
#
# In case no serie (i.e., condition) is provided update data right away with the
# corresponding tuple from the given line
# -----------------------------------------------------------------------------
def filter_data(data: list, line: dict,
                conditions: list, xname: str, yname: str) -> list:
    """Given a dictionary whose keys are the header names of a spreadsheet and
       whose values are the cell contents retrieved from the same line, update
       data (which must be given as a list of instances of pltserie) with those
       tuples (x, y) that match each condition respectively. The names of the
       variables x and y are given in xname and yname.

       In case no serie (i.e., condition) is provided update data right away
       with the corresponding tuple from the given line

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
# return an instance of PLTSeries with all series of data accepted from a list
# of spreadsheets, each represented as an instance of either PLTSerie or
# PLTKSerie, according to the value of stype.
#
# series consist of a list of series specification. Each serie is defined as a
# string with a legend and a condition separated by a colon, e.g., "k=1:k==1"
# where the condition is any valid Python boolean expression (including matching
# regular expressions, e.g., ""Problem #0: re.match('00/\d+', id)"") which can
# use variables that have to be found among the header names.
#
# remove_if consist of a list of conditions, each being a valid Python boolean
# expression which can also use variables that have to be found among the header
# names. Every data line which satisfies any of the conditions given in
# remove_if is removed.
#
# Every datapoint of each serie consists of a tuple (x, y) whose values are
# given by the contents of the headers xname and yname respectively.
# -----------------------------------------------------------------------------
def get_data(stype: str, spreadsheets: list, delimiter,
             series: list, remove_if: list, xname: str, yname: str) -> pltseries.PLTSeries:
    """return an instance of PLTSeries with all series of data accepted from a
       list of spreadsheets, each represented as an instance of either PLTSerie
       or PLTKSerie, according to the value of stype.

       series consist of a list of series specification. Each serie is defined
       as a string with a legend and a condition separated by a colon, e.g.,
       "k=1:k==1" where the condition is any valid Python boolean expression
       (including matching regular expressions, e.g., ""Problem #0:
       re.match('00/\\d+', id)"") which can use variables that have to be found
       among the header names.

       remove_if consist of a list of conditions, each being a valid Python
       boolean expression which can also use variables that have to be found
       among the header names. Every data line which satisfies any of the
       conditions given in remove_if is removed.

       Every datapoint of each serie consists of a tuple (x, y) whose values are
       given by the contents of the headers xname and yname respectively.

    """

    # depending upon the serie_type given, instances of PLTSerie or PLTKSerie
    # are created
    serie_type = {
        "plot": pltserie.PLTSerie,
        "ky": pltkserie.PLTKSerie,        
    }
    
    # create a list of strings with the legends and conditions of each serie,
    # and also, a container for each serie to reteurn
    data = []
    legends = []
    conditions = []
    for iserie in series:

        # extract the legend and condition of this serie
        if (m := re.match(RE_SERIES, iserie)):
            legend = m.group('legend').strip()
            legends.append(legend)
            conditions.append(m.group('condition').strip())

            # and also add a sample of the corresponding type
            data.append(serie_type[stype](legend, xname, yname))

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
        reader = spsreader.SPSReader(spreadsheet, delimiter=";")
        for irecord in reader:

            # create an ordinary dictionary to represent the information of this line
            line = {}
            for ikey in irecord:

                # check this header is not duplicated
                if ikey in line:
                    LOGGER.critical(CRITICAL_DUPLICATED_HEADER.format(ikey))

                # add this key to the dictionary
                line[ikey] = irecord[ikey]

            # once the entire line has been retrieved, ensure that there are headers
            # named after the x and y names
            if xname not in line:
                LOGGER.error(CRITICAL_UNKNOWN_HEADER.format("x", line))
                raise ValueError(CRITICAL_UNKNOWN_HEADER.format("x", line))
            if yname not in line:
                LOGGER.error(CRITICAL_UNKNOWN_HEADER.format("y", line))
                raise ValueError(CRITICAL_UNKNOWN_HEADER.format("y", line))

            # remove this line in case it matches any of the conditions in
            # remove_if
            if remove_if and len(remove_if) > 0:
                checker = pltchecker.PLTChecker(line, remove_if)
                results = checker.check()
                if any(results):

                    # get the first condition in remove_if verified and show it on
                    # the debug line
                    idx = next(i for i, v in enumerate(results) if v)
                    LOGGER.debug(DEBUG_DATALINE_REMOVED.format(1+nblines, line, remove_if[idx]))
                    continue
            
            # once the entire line has been retrieved in an ordinary dictionary,
            # check what series are verified, in case any has been given
            filter_data(data, line, conditions, xname, yname)

            # and increment the number of processed lines
            nblines += 1

        # show the number of lines processed
        LOGGER.info(INFO_NUMBER_DATALINES.format(nblines))

    # before leaving, remove all series which contain no data
    output = pltseries.PLTSeries()
    for iserie in data:
        if len(iserie) > 0:
            output += iserie

    # and return the data computed with all series
    return output


# -----------------------------------------------------------------------------
# do_cmd
#
# Execute the specified command in params with the given arguments
# -----------------------------------------------------------------------------
def do_cmd(params: argparse.Namespace):
    """Execute the specified command in params with the given arguments"""

    # get the full list of spreadsheets to process ---which can be of different
    # types
    spreadsheets = []
    for ifile in params.file:

        # ensure each spreadsheet is readable
        readable, err = utils.check_file_readable(ifile)
        if not readable:
            LOGGER.critical(err)
            raise ValueError(err)

        # finally, add it to the list of spreadsheets to process
        spreadsheets.append(ifile)

    # importantly, the series requested by the user have to be provided always
    # as a list. Moreover, if no serie is requested then one accepting all data
    # (i.e., with condition True) has to be used instead. In this case the serie
    # is named after a concatenation of the names of all spreadhseets
    user_series = ["{}:True".format("/".join(spreadsheets))] if params.series is None else params.series

    # get all series of data according to the user parameters
    series = get_data(params.command, spreadsheets, params.delimiter, user_series, params.remove_if, params.x, params.y)

    # show information about the format chosen
    LOGGER.info(INFO_OUTPUT_FORMAT.format(params.oformat))

    # and now process all series retrieved from the spreadsheets
    if len(series) > 0:
        LOGGER.info(INFO_NUMBER_DATAPOINTS)
        for iserie in series:
            LOGGER.info(INFO_NUMBER_DATAPOINTS_SERIE.format(iserie.get_legend(), len(iserie)))

        # guess the extension of the file to generate
        ext = 'gnuplot' if params.oformat == 'gnuplot' else 'tex'

        # create the resource with the information of all series and write its
        # contents into the output file
        if canvas := series.create_canvas(params.oformat,
                                          utils.get_filename(params.output, ext),
                                          title=params.title,
                                          png=utils.get_filename(params.png, "png") if params.png else None): 
            canvas.write()
                    
    else:
        LOGGER.warning(WARNING_NO_DATA)


# -----------------------------------------------------------------------------
# do_group
#
# Execute the group command with the given parameters
# -----------------------------------------------------------------------------
def do_group(params: argparse.Namespace):
    """Execute the group command with the given parameters"""

    # create an instance of a pltpgfgroup
    group = pltpgfplot.PLTpgfplotgroup(params.file)

    # and process the contents of all files to get all the substitutions
    # performed to create precisely those contents
    group.process()
    group.show_properties()
    group.write(params.nbcolumns, params.caption, params.output)

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

    # validate the params
    validate_params (params)

    # execute the specified command
    {
        "plot": do_cmd,
        "ky": do_cmd,
        "group": do_group,
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
