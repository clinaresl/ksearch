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

import figconf
import version

# -----------------------------------------------------------------------------
# create a command parser to parse all params passed to the main script
# -----------------------------------------------------------------------------
def createPrgArgParser():
    """create a command parser to parse all params passed to the main script"""

    # initialize a parser
    parser = argparse.ArgumentParser(description="DESCRIPTION: " + version.__description__)

    # Group of mandatory arguments
    parser_mandatory = parser.add_argument_group("Mandatory arguments", "The following arguments are required")
    parser_mandatory.add_argument('-d', '--domain',
                                  required=True,
                                  type=str,
                                  choices=figconf.VALID_DOMAINS,
                                  help="Name of the domain to be used.")
    parser_mandatory.add_argument('-v', '--variant',
                                  required=True,
                                  type=str,
                                  help="Name of the variant to be used under the selected domain. It must be a valid variant for the selected domain")
    parser_mandatory.add_argument('-p', '--param',
                                  required=True,
                                  type=str,
                                  choices=figconf.VALID_PARAMS,
                                  help="Name of the parameter to show in the images")
    parser_mandatory.add_argument('-s', '--search',
                                  required=True,
                                  type=str,
                                  choices=figconf.VALID_SEARCH,
                                  help="Set of search algorithms to show in the figures")
    parser_mandatory.add_argument('-f', '--file',
                                  required=True,
                                  type=str,
                                  help="Name of the file to be generated with the LaTeX code to show all images. By default, the standard output is used")
    parser_mandatory.add_argument('-r', '--root',
                                  required=True,
                                  type=str,
                                  help="Path of results from which the given domain can be accessed, i.e., '../plotter/results'")

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
