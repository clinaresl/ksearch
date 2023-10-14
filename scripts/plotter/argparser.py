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
                                help="Generate a GNUplot file from spreadsheet data without transformations",
                                description="DESCRIPTION: Generate a GNUplot file from spreadsheet data")
    ktime = subparser.add_parser("ktime",
                                 help="Generate a GNUplot file with k versus the average running time",
                                 description="DESCRIPTION: Generate a GNUplot file with k versus the average running time")

    # plot
    # -------------------------------------------------------------------------
    # Group of mandatory arguments
    plot_mandatory = plot.add_argument_group("Mandatory arguments", "The following arguments are required")
    plot_mandatory.add_argument('-f', '--file',
                                required=True,
                                type=str,
                                help="name of the file containing the .xlsx spreadsheet")
    plot_mandatory.add_argument('-x', '--x',
                                required=True,
                                type=str,
                                help="name of the column containing the x values")
    plot_mandatory.add_argument('-y', '--y',
                                required=True,
                                type=str,
                                help="name of the column containing the y values")

    # Group of optional arguments
    plot_optional = plot.add_argument_group('Optional', 'The following arguments are optional')
    plot_optional.add_argument('-s', '--series',
                               type=str,
                               nargs='*',
                               help="criteria used for filtering data lines. An arbitrary number of them can be given. Each serie has the format 'legend:condition'. If none is given, all data lines are accepted to generate a single plot with no specific legend unless 'legend:True' is given which assigns the specific legend to a unique series with all data. The 'condition' consists of a valid boolean expression in Python (i.e., 'and', 'or' and 'not' can be used, and also any relational operator, parenthesis, etc.) including also regular expressions. Each serie creates a different plot with the legend given before the colon.")
    plot_optional.add_argument('-t', '--title',
                               type=str,
                               default=None,
                               help="Provides a title for the resulting plot")
    plot_optional.add_argument('-w', '--output',
                               type=str,
                               default=None,
                               help="Name of the gnuplot file to be generated. If none is given, no output file is generated")

    # ktime
    # -------------------------------------------------------------------------
    # Group of mandatory arguments
    ktime_mandatory = ktime.add_argument_group("Mandatory arguments", "The following arguments are required")
    ktime_mandatory.add_argument('-f', '--file',
                                 required=True,
                                 type=str,
                                 help="name of the file containing the .xlsx spreadsheet")

    # Group of optional arguments
    ktime_optional = ktime.add_argument_group('Optional', 'The following arguments are optional')
    ktime_optional.add_argument('-s', '--series',
                                type=str,
                                nargs='*',
                                help="criteria used for filtering data lines. An arbitrary number of them can be given. Each serie has the format 'legend:condition'. If none is given, all data lines are accepted to generate a single plot with no specific legend unless 'legend:True' is given which assigns the specific legend to a unique series with all data. The 'condition' consists of a valid boolean expression in Python (i.e., 'and', 'or' and 'not' can be used, and also any relational operator, parenthesis, etc.) including also regular expressions. Each serie creates a different plot with the legend given before the colon.")
    ktime_optional.add_argument('-t', '--title',
                                type=str,
                                default=None,
                                help="Provides a title for the resulting plot")
    ktime_optional.add_argument('-w', '--output',
                                type=str,
                                default=None,
                                help="Name of the gnuplot file to be generated. If none is given, no output file is generated")

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
