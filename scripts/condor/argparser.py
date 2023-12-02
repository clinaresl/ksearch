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

import cndconf
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
                                  choices=cndconf.VALID_DOMAINS,
                                  help="Name of the domain to be used.")
    parser_mandatory.add_argument('-v', '--variant',
                                  required=True,
                                  type=str,
                                  help="Name of the variant to be used under the selected domain. It must be a valid variant for the selected domain")
    parser_mandatory.add_argument('-a', '--algorithm',
                                  required=True,
                                  type=str,
                                  choices=cndconf.VALID_ALGORITHMS,
                                  help="Name of the algorithm to use for solving instances under the given domains with the selected variant.")
    parser_mandatory.add_argument('-k', '--k',
                                  required=True,
                                  type=str,
                                  help="Number of paths to find. The specification of k follows the specification of k for any solver implemented in this library, i.e., it consists of a semicolon separated specification of values of k. Each part might consist of up to three different numbers: <first-k> <last-k> <step>. If only one value is given, only that value of k is considered; if two values are given, the step is assumed by default equal to one.")

    # Group of optional arguments
    parser_optional = parser.add_argument_group('Optional', 'The following arguments are optional')
    parser_optional.add_argument('-m', '--map',
                                 type=str,
                                 default=None,
                                 help="name of the map to be used in the domains 'map' or 'roadmap' without the suffix, e.g., 'USA-road-d.BAY' or 'random512-15-0'")
    parser_optional.add_argument('-s', '--size',
                                 type=int,
                                 default=0,
                                 help="Length of the permutations in the N-Pancake or the side of the square N-puzzle.")
    parser_optional.add_argument('-u', '--user',
                                 type=str,
                                 default="carlos.linares@uc3m.es",
                                 help="e-mail address to notify upon completion of the condor job. To be included in the condor job submission file")

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
