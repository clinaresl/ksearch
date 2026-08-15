#!/usr/bin/env python3
# -*- coding: utf-8 -*-
#
# argparser.py
# Description:
# -----------------------------------------------------------------------------
#
# Started on <dom 04-09-2022 20:35:30.413409616 (1662316530)>
# Carlos Linares López <carlos.linares@uc3m.es>
#

"""
Command parser
"""

# imports
# -----------------------------------------------------------------------------
import argparse
import datetime
import os
import sys

import version

# -----------------------------------------------------------------------------
# create a command parser to parse all params passed to the main script
# -----------------------------------------------------------------------------
def createPrgArgParser():
    """create a command parser to parse all params passed to the main script"""

    # initialize a parser
    parser = argparse.ArgumentParser(description="DESCRIPTION: " + version.__description__)
    subparser = parser.add_subparsers(title="commands", dest="command")

    # add the commands
    plot = subparser.add_parser("plot",
                                help="Generate a table/gnuplot/pgfplot showing all datapoints extracted for an arbitrary combination of x and y columns",
                                description="DESCRIPTION: Generate a table/gnuplot/pgfplot showing all datapoints extracted for an arbitrary combination of x and y columns")
    ky = subparser.add_parser("ky",
                              help="Generate a table/gnuplot/pgfplot where the y-value of each datapoint is the average among all samples with the same x-value",
                              description="DESCRIPTION: Generate a table/gnuplot/pgfplot where the y-value of each datapoint is the average among all samples with the same x-value")
    group = subparser.add_parser("group",
                                 help="Takes an arbitrary number of .tex files with pgfplots created by this script and groups them into the same environment so that they all share the same legends.",
                                 description="DESCRIPTION: Takes an arbitrary number of .tex files with pgfplots created by this script and groups them into the same environment so that they all share the same legends.")

    # plot
    # -------------------------------------------------------------------------
    # Group of mandatory arguments
    plot_mandatory = plot.add_argument_group("Mandatory arguments", "The following arguments are required")
    plot_mandatory.add_argument('-f', '--file',
                                required=True,
                                nargs="+",
                                type=str,
                                help="name of the file(s) containing the spreadsheet(s) to process. It is possible to provide an arbitrary number of them to generate a single plot with all data")
    plot_mandatory.add_argument('-x', '--x',
                                required=True,
                                type=str,
                                help="name of the column containing the x values")
    plot_mandatory.add_argument('-y', '--y',
                                required=True,
                                type=str,
                                help="name of the column containing the y values")
    plot_mandatory.add_argument('-w', '--output',
                                type=str,
                                default=None,
                                help="Name of the output file to generate")

    # Group of optional arguments
    plot_optional = plot.add_argument_group('Optional', 'The following arguments are optional')
    plot_optional.add_argument('-d', '--delimiter',
                               type=str,
                               default=';',
                               help="in case a csv file is given, this argument specifies the delimiter used to separate columns. By default ';'")
    plot_optional.add_argument('-s', '--series',
                               type=str,
                               nargs='*',
                               help="criteria used for automatically creating series. An arbitrary number of them can be given. Each serie has the format 'legend:condition'. If none is given, all data lines are accepted to generate a single plot with no specific legend unless 'legend:True' is given which assigns the specific legend to a unique series with all data. The 'condition' consists of a valid boolean expression in Python (i.e., 'and', 'or' and 'not' can be used, and also any relational operator, parenthesis, etc. including also regular expressions). Each serie creates a different plot with the legend given before the colon.")
    plot_optional.add_argument('-X', '--remove-if',
                               type=str,
                               nargs='*',
                               help="criteria used for filtering data lines. An arbitrary number of criteria can be given. Each must consist of a valid boolean expression in Python  (i.e., 'and', 'or' and 'not' can be used, and also any relational operator, parenthesis, etc. including also regular expressions). Those lines from the input csv files matching the given expression are automatically discarded.")
    plot_optional.add_argument('-F', '--format',
                               dest='oformat',
                               choices=['gnuplot', 'pgfplot', 'table'],
                               default='gnuplot',
                               help="Format of the output. Valid values are 'gnuplot', 'pgfplot' and 'table'. The first generates a file with the name given with --output (and suffix 'gnuplot') that when being processed with 'gnuplot' produces a png with the name given with --png; the second produces a file (with suffix .tex) that when being processed with 'pdflatex' (TikZ/pgfplots 1.18) generates a pdf with the image; and the last one generates a LaTeX table with the information of all series. By default, 'gnuplot'")
    plot_optional.add_argument('-t', '--title',
                               type=str,
                               default=None,
                               help="Provides a title for the resulting plot")
    plot_optional.add_argument('-p', '--png',
                               type=str,
                               default=None,
                               help="Name of the png file to be generated. If none is given, no png file is generated")

    # ky
    # -------------------------------------------------------------------------
    # Group of mandatory arguments
    ky_mandatory = ky.add_argument_group("Mandatory arguments", "The following arguments are required")
    ky_mandatory.add_argument('-f', '--file',
                              required=True,
                              nargs="+",
                              type=str,
                              help="name of the file(s) containing the spreadsheet(s) to process. It is possible to provide an arbitrary number of them to generate a single plot with all data")
    ky_mandatory.add_argument('-x', '--x',
                              required=True,
                              type=str,
                              help="provide a column name for grouping all samples with the same value of this column. The data point will be given an x-value equal to the common value of the group, and the y-value will be set to the average of all values  observed in the y column (provided with --y)")
    ky_mandatory.add_argument('-y', '--y',
                              required=True,
                              type=str,
                              help="name of the column containing the y values")
    ky_mandatory.add_argument('-w', '--output',
                              type=str,
                              default=None,
                              help="Name of the output file to generate")

    # Group of optional arguments
    ky_optional = ky.add_argument_group('Optional', 'The following arguments are optional')
    ky_optional.add_argument('-d', '--delimiter',
                               type=str,
                               default=';',
                               help="in case a csv file is given, this argument specifies the delimiter used to separate columns. By default ';'")
    ky_optional.add_argument('-s', '--series',
                             type=str,
                             nargs='*',
                             help="criteria used for automatically creating series. An arbitrary number of them can be given. Each serie has the format 'legend:condition'. If none is given, all data lines are accepted to generate a single plot with no specific legend unless 'legend:True' is given which assigns the specific legend to a unique series with all data. The 'condition' consists of a valid boolean expression in Python (i.e., 'and', 'or' and 'not' can be used, and also any relational operator, parenthesis, etc. including also regular expressions). Each serie creates a different plot with the legend given before the colon.")
    ky_optional.add_argument('-X', '--remove-if',
                             type=str,
                             nargs='*',
                             help="criteria used for filtering data lines. An arbitrary number of criteria can be given. Each must consist of a valid boolean expression in Python  (i.e., 'and', 'or' and 'not' can be used, and also any relational operator, parenthesis, etc. including also regular expressions). Those lines from the input csv files matching the given expression are automatically discarded.")
    ky_optional.add_argument('-F', '--format',
                             dest='oformat',
                             choices=['gnuplot', 'pgfplot', 'table'],
                             default='gnuplot',
                             help="Format of the output. Valid values are 'gnuplot', 'pgfplot' and 'table'. The first generates a file with the name given with --output (and suffix 'gnuplot') that when being processed with 'gnuplot' produces a png with the name given with --png; the second produces a file (with suffix .tex) that when being processed with 'pdflatex' (TikZ/pgfplots 1.18) generates a pdf with the image; and the last one generates a LaTeX table with the information of all series. By default, 'gnuplot'")
    ky_optional.add_argument('-t', '--title',
                             type=str,
                             default=None,
                             help="Provides a title for the resulting plot")
    ky_optional.add_argument('-p', '--png',
                             type=str,
                             default=None,
                             help="Name of the png file to be generated only when creating gnuplot files, i.e., it is of no use when generating the body data of LaTeX tables. If none is given, no png file is generated")

    # group
    # -------------------------------------------------------------------------
    # Group of mandatory arguments
    group_mandatory = group.add_argument_group("Mandatory arguments", "The following arguments are required")
    group_mandatory.add_argument('-f', '--file',
                                 required=True,
                                 nargs="+",
                                 type=str,
                                 help="name of the file(s) containing the TikZ/pgfplot views to group. Each file represents a different pgfplot.")
    group_mandatory.add_argument('-w', '--output',
                                 type=str,
                                 default=None,
                                 help="Name of the file to be generated with the TikZ/pgfplot description of a group with all figures given in the input.")
    group_mandatory.add_argument('-n', '--nbcolumns',
                                 type=int,
                                 default=2,
                                 help="Number of columns of the group. Plots will be arranged on a grid with the number of colummns specified and as many rows as necessary")

    # Group of optional arguments
    group_optional = group.add_argument_group("Optional", "The following arguments are optional")
    group_optional.add_argument('-c', '--caption',
                                type=str,
                                default="Caption",
                                help="Caption of the group to be shown in the resulting TikZ/pgfplot plot")

    # Parser
    # -------------------------------------------------------------------------
    # Group of logging services
    logging = parser.add_argument_group('Logging', 'The following arguments specify logging settings')
    logging.add_argument('-L', '--level',
                         choices=['DEBUG', 'INFO', 'WARNING', 'ERROR', 'CRITICAL'],
                         default='INFO',
                         help="level of log messages. Messages of the same level or above are shown. By default, INFO, i.e., all messages are shown")

    # Group of miscellaneous arguments
    misc = parser.add_argument_group('Miscellaneous')
    misc.add_argument('-V', '--version',
                      action='version',
                      version=" %s %s" % (sys.argv [0], version.__version__),
                      help="output version information and exit")

    # and return the parser
    return parser



# Local Variables:
# mode:python
# fill-column:80
# End:
