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
from pathlib import Path

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
    parser_mandatory.add_argument('-p', '--project',
                                  required=True,
                                  type=str,
                                  choices=cndconf.VALID_PROJECTS,
                                  help="Name of the project to use. They refer to the name of github projects which are used for automating the experiments. The choice affects the type of .sh file to generate, and thus to the condor configuration file as well.")
    parser_mandatory.add_argument('-d', '--domain',
                                  required=True,
                                  type=str,
                                  choices=cndconf.VALID_DOMAINS,
                                  help="Name of the domain to use.")
    parser_mandatory.add_argument('-v', '--variant',
                                  required=True,
                                  type=str,
                                  help="Name of the variant to use under the selected domain. It must be a valid variant for the selected domain")
    parser_mandatory.add_argument('-a', '--algorithm',
                                  required=True,
                                  type=str,
                                  choices=cndconf.VALID_ALGORITHMS,
                                  help="Name of the algorithm to use for solving instances under the given domains with the selected variant.")
    parser_mandatory.add_argument('-k', '--k',
                                  required=True,
                                  type=str,
                                  help="Number of paths to find. The specification of k follows the specification of k for any solver implemented in this library, i.e., it consists of a semicolon separated specification of values of k. Each part might consist of up to three different numbers: <first-k> <last-k> <step>. If only one value is given, only that value of k is considered; if two values are given, the step is assumed by default equal to one.")
    parser_mandatory.add_argument('-t', '--testfile',
                                  required=True,
                                  type=str,
                                  help="Filename with the test cases to solve. In case it is given with a path to it, only the basename is used")

    # Group of optional arguments
    parser_optional = parser.add_argument_group('Optional', 'The following arguments are optional')
    parser_optional.add_argument('-P', '--parameters',
                                 type=str,
                                 default=None,
                                 help="If given, they are attached to the algorithm given with --algorithm between parenthesis, e.g., \"--algorithm 'bBELA0' --params '0'\" yields \"bBELA(0)\".")
    parser_optional.add_argument('-m', '--map',
                                 type=str,
                                 default=None,
                                 help="name of the map to be used in the domains 'map', 'roadmap' or 'roadmap-maps' without the suffix, e.g., 'USA-road-d.BAY' or 'random512-15-0'. In case it is given with a path to it, only the basename is used. It serves also to identify the test file with the instances to solve, and thus it is not necessary in the domains 'n-pancake' and 'n-puzzle'")
    parser_optional.add_argument('-s', '--size',
                                 type=int,
                                 default=0,
                                 help="Length of the permutations in the N-Pancake or the side of the square N-puzzle.")
    parser_optional.add_argument('-r', '--requirements',
                                 type=str,
                                 default=None,
                                 help="In case it is given it is added verbatim in the requirements field of the condor job submission file.")
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


# -----------------------------------------------------------------------------
# create a command parser to parse all params passed to files.py
# -----------------------------------------------------------------------------
def createFilesArgParser():
    """create a command parser to parse all params passed to files.py"""

    # initialize a parser
    parser = argparse.ArgumentParser(description="DESCRIPTION: move all files generated by condor to their respective directories, or remove a selection of them")
    subparser = parser.add_subparsers(title="commands", dest="command")

    # add the commands
    list = subparser.add_parser("ls",
                                help="Show the bundles that match the given criteria",
                                description="DESCRIPTION: Show the bundles that match the given criteria")
    move = subparser.add_parser("mv",
                                help="Move both valid and complete bundles of files to their destination directory which is created in case it does not exist",
                                description="DESCRIPTION: Move both valid and complete bundles of files to their destination directory which is created in case it does not exist")
    remove = subparser.add_parser("rm",
                                  help="Remove a selection of files from each bundle in the current directory",
                                  description="DESCRIPTION: Remove a selection of files from each bundle in the current directory")
    tar = subparser.add_parser("tar",
                               help="Compress all files of all bundles that match the given criteria",
                               description="DESCRIPTION: Compress all files of all bundles that match the given criteria")

    # mv
    # -------------------------------------------------------------------------
    # Group of optional arguments
    mv_optional = move.add_argument_group('Optional', 'The following arguments are optional')
    mv_optional.add_argument('-d', '--dry-run',
                             action="store_true",
                             help="If given, the action to be executed over each file is explicitly shown and no action is taken")
    mv_optional.add_argument('-f', '--force',
                             action="store_true",
                             help="If given, files are overwritten when being moved if necessary")

    # rm
    # -------------------------------------------------------------------------
    # Group of optional arguments
    rm_optional = remove.add_argument_group('Optional', 'The following arguments are optional')
    rm_optional.add_argument('-d', '--dry-run',
                             action="store_true",
                             help="If given, the action to be executed over each file is explicitly shown and no action is taken.")
    rm_optional.add_argument('-a', '--remove-all',
                             action="store_true",
                             help="removes all files of every bundle found in the current directory")
    rm_optional.add_argument('-v', '--remove-valid',
                             action="store_true",
                             help="removes all files from each valid bundle")
    rm_optional.add_argument('-e', '--remove-invalid',
                             action="store_true",
                             help="removes all files from each invalid bundle")
    rm_optional.add_argument('-c', '--remove-complete',
                             action="store_true",
                             help="removes all files from each complete bundle")
    rm_optional.add_argument('-f', '--remove-incomplete',
                             action="store_true",
                             help="removes all files from each incomplete bundle")
    rm_optional.add_argument('-x', '--remove-complete-and-valid',
                             action="store_true",
                             help="removes all files from each complete and valid bundle")
    rm_optional.add_argument('-g', '--remove-incomplete-and-valid',
                             action="store_true",
                             help="removes all files from each incomplete and valid bundle")
    rm_optional.add_argument('-i', '--remove-complete-and-invalid',
                             action="store_true",
                             help="removes all files from each complete and invalid bundle")
    rm_optional.add_argument('-j', '--remove-incomplete-and-invalid',
                             action="store_true",
                             help="removes all files from each incomplete and invalid bundle")
    rm_optional.add_argument('-k', '--domain',
                             type=str,
                             help="lists all bundles of the given domain")
    rm_optional.add_argument('-l', '--project',
                             type=str,
                             help="lists all bundles of the given project")
    rm_optional.add_argument('-m', '--algorithm',
                             type=str,
                             help="lists all bundles of the given algorithm")
    rm_optional.add_argument('-n', '--variant',
                             type=str,
                             help="lists all bundles of the given variant")
    rm_optional.add_argument('-o', '--size',
                             type=str,
                             help="lists all bundles of the given size")

    # ls
    # -------------------------------------------------------------------------
    # Group of optional arguments
    ls_optional = list.add_argument_group('Optional', 'The following arguments are optional')
    ls_optional.add_argument('-a', '--all',
                             action="store_true",
                             help="lists all bundles of every bundle found in the current directory")
    ls_optional.add_argument('-v', '--valid',
                             action="store_true",
                             help="lists all bundles from each valid bundle")
    ls_optional.add_argument('-e', '--invalid',
                             action="store_true",
                             help="lists all bundles from each invalid bundle")
    ls_optional.add_argument('-c', '--complete',
                             action="store_true",
                             help="lists all bundles from each complete bundle")
    ls_optional.add_argument('-f', '--incomplete',
                             action="store_true",
                             help="lists all bundles from each incomplete bundle")
    ls_optional.add_argument('-x', '--complete-and-valid',
                             action="store_true",
                             help="lists all bundles from each complete and valid bundle")
    ls_optional.add_argument('-g', '--incomplete-and-valid',
                             action="store_true",
                             help="lists all bundles from each incomplete and valid bundle")
    ls_optional.add_argument('-i', '--complete-and-invalid',
                             action="store_true",
                             help="lists all bundles from each complete and invalid bundle")
    ls_optional.add_argument('-j', '--incomplete-and-invalid',
                             action="store_true",
                             help="lists all bundles from each incomplete and invalid bundle")
    ls_optional.add_argument('-k', '--domain',
                             type=str,
                             help="lists all bundles of the given domain")
    ls_optional.add_argument('-l', '--project',
                             type=str,
                             help="lists all bundles of the given project")
    ls_optional.add_argument('-m', '--algorithm',
                             type=str,
                             help="lists all bundles of the given algorithm")
    ls_optional.add_argument('-n', '--variant',
                             type=str,
                             help="lists all bundles of the given variant")
    ls_optional.add_argument('-o', '--size',
                             type=str,
                             help="lists all bundles of the given size")

    # tar
    # -------------------------------------------------------------------------
    # Group of positional arguments
    tar.add_argument('archive',
                     type=Path,
                     help="Name of the archive to create")

    # Group of optional arguments
    tar_optional = tar.add_argument_group('Optional', 'The following arguments are optional')
    tar_optional.add_argument('-a', '--all',
                              action="store_true",
                              help="lists all bundles of every bundle found in the current directory")
    tar_optional.add_argument('-v', '--valid',
                              action="store_true",
                              help="lists all bundles from each valid bundle")
    tar_optional.add_argument('-e', '--invalid',
                              action="store_true",
                              help="lists all bundles from each invalid bundle")
    tar_optional.add_argument('-c', '--complete',
                              action="store_true",
                              help="lists all bundles from each complete bundle")
    tar_optional.add_argument('-f', '--incomplete',
                              action="store_true",
                              help="lists all bundles from each incomplete bundle")
    tar_optional.add_argument('-x', '--complete-and-valid',
                              action="store_true",
                              help="lists all bundles from each complete and valid bundle")
    tar_optional.add_argument('-g', '--incomplete-and-valid',
                              action="store_true",
                              help="lists all bundles from each incomplete and valid bundle")
    tar_optional.add_argument('-i', '--complete-and-invalid',
                              action="store_true",
                              help="lists all bundles from each complete and invalid bundle")
    tar_optional.add_argument('-j', '--incomplete-and-invalid',
                              action="store_true",
                              help="lists all bundles from each incomplete and invalid bundle")
    tar_optional.add_argument('-k', '--domain',
                              type=str,
                              help="lists all bundles of the given domain")
    tar_optional.add_argument('-l', '--project',
                              type=str,
                              help="lists all bundles of the given project")
    tar_optional.add_argument('-m', '--algorithm',
                              type=str,
                              help="lists all bundles of the given algorithm")
    tar_optional.add_argument('-n', '--variant',
                              type=str,
                              help="lists all bundles of the given variant")
    tar_optional.add_argument('-o', '--size',
                              type=str,
                              help="lists all bundles of the given size")

    # parser
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
