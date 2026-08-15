#!/usr/bin/env python3
# -*- coding: utf-8 -*-
#
# condor.py
# Description:
# -----------------------------------------------------------------------------
#
# Started on <sáb 02-12-2023 17:34:57.694411833 (1701534897)>
# Carlos Linares López <carlos.linares@uc3m.es>
#

"""
Generation of job configuration and shell scripts for the Condor system
"""

# imports
# -----------------------------------------------------------------------------
import time
from pathlib import Path
from string import Template

import argparser
import cndconf
import cndk
import utils

# globals
# -----------------------------------------------------------------------------

# default logger
LOGGER = utils.LOGGER

# suffixes
CONDOR_SUFFIX = ".condor"
SHELL_SUFFIX = ".sh"

# info messages
INFO_ELAPSED_TIME = "Elapsed time: {0}"
INFO_K_INTERVAL = "The values of K are in the interval [{0}, {1}]"
INFO_CONDOR_FILE_GENERATED = "The condor file '{0}' has been generated"
INFO_SHELL_FILE_GENERATED = "The shell file '{0}' has been generated"

# warning messages
WARNING_UNNECESSARY_TESTFILE = "The testfile is not necessary for the domain '{}' and it will be ignored. Make sure to name it after the data file!"
WARNING_ENTRY_NOT_FOUND = "Entry '{}' in '{}' not found in VALID_PROJECTS"

# critical messages
CRITICAL_INVALID_VARIANT = "The variant '{}' is not valid for the domain '{}'"
CRITICAL_MAP_NOT_GIVEN = "The map name must be given for the domain 'maps' or 'roadmap'"
CRITICAL_SIZE_NOT_GIVEN = "The size of the instance must be given for the domain 'n-pancake' or 'n-puzzle'"
CRITICAL_TESTFILE_NOT_GIVEN = "The testfile is mandatory for the domain '{}'"
CRITICAL_NO_VALID_PROJECTS = "No valid projects were found in cndconf.py"
CRITICAL_PROJECT_NOT_FOUND = "Project '{}' not found in '{}'"


# -----------------------------------------------------------------------------
# return the name of the file given in a string which might qualify the file
# with either a full or relative path
# -----------------------------------------------------------------------------
def get_basename(s: str) -> str:
    """return the name of the file given in a string which might qualify the
    file with either a full or relative path

    """

    return Path(s).name if s is not None else None

# -----------------------------------------------------------------------------
# projects are created by hand and the entries in the CONDOR_FILE and the
# SHELL_FILE are also created by hand. Just verify that all entries in both
# dictionaries refer to valid projects and that there is an entry in these
# dictionaries for every valid project
# -----------------------------------------------------------------------------
def validate_project() -> bool:
    """projects are created by hand and the entries in the CONDOR_FILE and the
       SHELL_FILE are also created by hand. Just verify that all entries in both
       dictionaries refer to valid projects and that there is an entry in these
       dictionaries for every valid project

    """

    # check that the number of valid projects is non-null
    if len(cndconf.VALID_PROJECTS) == 0:
            LOGGER.critical (CRITICAL_NO_VALID_PROJECTS)
            raise ValueError(CRITICAL_NO_VALID_PROJECTS)

    # Now, check that every valid project has contents in both CONDOR_FILE and
    # SHELL_FILE:
    for iproject in cndconf.VALID_PROJECTS:

        if iproject not in cndconf.CONDOR_FILE:
            LOGGER.critical(CRITICAL_PROJECT_NOT_FOUND.format(iproject, "CONDOR_FILE"))
            raise ValueError(CRITICAL_PROJECT_NOT_FOUND.format(iproject, "CONDOR_FILE"))

        if iproject not in cndconf.SHELL_FILE:
            LOGGER.critical (CRITICAL_PROJECT_NOT_FOUND.format(iproject, "SHELL_FILE"))
            raise ValueError(CRITICAL_PROJECT_NOT_FOUND.format(iproject, "SHELL_FILE"))

    # Issue a warning in case there are entries CONDOR_FILE which are not
    # recognized as a valid project
    for iproject in cndconf.CONDOR_FILE:

        if iproject not in cndconf.VALID_PROJECTS:
            LOGGER.warning(WARNING_ENTRY_NOT_FOUND.format(iproject, "CONDOR_FILE"))

    # And also check the entries in SHELL_FILE
    for iproject in cndconf.SHELL_FILE:

        if iproject not in cndconf.VALID_PROJECTS:
            LOGGER.warning(WARNING_ENTRY_NOT_FOUND.format(iproject, "SHELL_FILE"))


# -----------------------------------------------------------------------------
# validate_variant
#
# Return true if and only if the given variant exists in the specified domain
# and false otherwise
# -----------------------------------------------------------------------------
def validate_variant(domain: str, variant: str):
    """Return true if and only if the given variant exists in the specified
       domain and false otherwise"""

    # return whether the given variant is correct or not
    return variant in cndconf.VALID_VARIANTS[domain]

# -----------------------------------------------------------------------------
# generate_condor_file
#
# generates the condor file from the values given
# -----------------------------------------------------------------------------
def generate_condor_file(project: str, domain: str, variant: str, algorithm: str,
                         params: str, requirements: str, user: str,
                         mink: int, maxk: int,
                         mapname: str,
                         n: int, nbtiles: int):
    """generates the condor file from the values given"""

    # in case the algorithm contains a star, replace it by a 's'
    algorithm_nostar = algorithm.replace('*', 's')

    # and in case any parameters have been given, attach them to the algorithm
    # name between dots
    if params is not None:
        algorithm_nostar = algorithm_nostar + "." + params

    # first, determine the filename of the condor file. For this, instantiate
    # the template corresponding to the given domain with the given values. Note
    # that acronyms are used to identify the project since these names might be
    # too long
    template = Template(cndconf.FILENAME[domain])
    filename = template.substitute(project=cndconf.ACRONYM_PROJECTS[project],
                                   map=mapname,
                                   solver=algorithm_nostar,
                                   variant=variant,
                                   mink=mink,
                                   maxk=maxk,
                                   size=n,
                                   nbtiles=nbtiles)

    # next, create the contents of the condor submission file using the
    # corresponding project. Note that if requirements is not given, then
    # nothing should be written, otherwise "requirements = " should be prefixing
    # the value given
    if requirements is not None and len(requirements) > 0:
        requirements = "requirements = " + requirements
    else:
        requirements = ""
    template = Template(cndconf.CONDOR_FILE[project])
    contents = template.substitute(project=project,
                                   domain=domain,
                                   variant=variant,
                                   algorithm=algorithm,
                                   requirements=requirements,
                                   user=user,
                                   filename=filename)

    # and write the contents in the condor job submission configuration file
    with open(filename + CONDOR_SUFFIX, "w") as condor:
        condor.write(contents)

    # tell the user the CONDOR file has been properly generated
    LOGGER.info(INFO_CONDOR_FILE_GENERATED.format(filename+CONDOR_SUFFIX))

# -----------------------------------------------------------------------------
# generate_shell_file
#
# generates the shell script to execute in the backend node
# -----------------------------------------------------------------------------
def generate_shell_file(project: str, domain: str, variant: str, algorithm: str,
                        params: str, testfile: str, kspec: str,
                        user:str,
                        mink: int, maxk: int,
                        mapname: str,
                        n: int, nbtiles: int):
    """generates the shell script to execute in the backend node"""

    # in case the algorithm contains a star, replace it by a 's'
    algorithm_nostar = algorithm.replace('*', 's')

    # and in case any parameters have been given, attach them to the filename to
    # generate between dots
    if params is not None:
        algorithm_nostar = algorithm_nostar + "." + params

    # Note that for all domains, their name is part of the path to follow to get
    # to the binary, but this is not true in case of the roadmap-maps. Hence, a
    # variable called "domainexec" records a part of the path to the binary
    # which is updated accordingly next
    execdomain = "roadmap" if domain == "roadmap-maps" else domain

    # In case that some parameters were given, this have to be enclosed between
    # angles immediately after the solver name
    paramalgorithm = algorithm
    if params is not None:
        paramalgorithm = algorithm + "<" + params + ">"

    # first, determine the filename of the shell script. For this, instantiate
    # the template corresponding to the given domain with the given values
    template = Template(cndconf.FILENAME[domain])
    filename = template.substitute(project=cndconf.ACRONYM_PROJECTS[project],
                                   map=mapname,
                                   solver=algorithm_nostar,
                                   variant=variant,
                                   mink=mink,
                                   maxk=maxk,
                                   size=n,
                                   nbtiles=nbtiles)

    # prior to the creation of the shell file, compute the arguments to give to
    # the solver which depend also on the project chosen
    template = Template(cndconf.ARGS[project][domain])
    args = template.substitute(domain=domain,
                               execdomain=execdomain,
                               testfile=testfile,
                               paramalgorithm=paramalgorithm,
                               variant=variant,
                               kspec=kspec,
                               filename=filename)

    # next, create the contents of the condor submission file which depend upon
    # the chosen project.
    template = Template(cndconf.SHELL_FILE[project])
    contents = template.substitute(project=project,
                                   execdomain=execdomain,
                                   executable=cndconf.EXECUTABLE[domain],
                                   args=args)

    # and write the contents in the condor job submission configuration file
    with open(filename + SHELL_SUFFIX, "w") as condor:
        condor.write(contents)

    # tell the user the CONDOR file has been properly generated
    LOGGER.info(INFO_SHELL_FILE_GENERATED.format(filename+SHELL_SUFFIX))

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

    # projects are created by hand and the entries in the CONDOR_FILE and the
    # SHELL_FILE are also created by hand. Just verify that all entries in both
    # dictionaries refer to valid projects and that there is an entry in these
    # dictionaries for every valid project
    validate_project()

    # determine whether the given variant is correct or not
    if not validate_variant(params.domain, params.variant):
        LOGGER.critical(CRITICAL_INVALID_VARIANT.format(params.variant, params.domain))
        raise ValueError(CRITICAL_INVALID_VARIANT.format(params.variant, params.domain))

    # parse the specification of k values, and report the minimum and maximum
    # values of k
    k = cndk.CNDk(params.k)
    LOGGER.info(INFO_K_INTERVAL.format(k.min(), k.max()))

    # check that the map name and the size are given in case those values are
    # demanded by the specified domain
    if (params.domain in ['maps', 'roadmap', 'roadmap-maps']) and (params.map is None):
        LOGGER.critical(CRITICAL_MAP_NOT_GIVEN)
        raise ValueError(CRITICAL_MAP_NOT_GIVEN)
    if (params.domain in ['n-pancake', 'n-puzzle']) and (params.size == 0):
        LOGGER.critical(CRITICAL_SIZE_NOT_GIVEN)
        raise ValueError(CRITICAL_SIZE_NOT_GIVEN)

    # Compute the name of the testfile. If the domain is 'maps', 'roadmap' or
    # 'roadmap-maps' it should then be equal to the map filename and thus, it
    # must not be given as in the 'n-pancake' and 'n-puzzle'
    testfile = None
    if (params.domain in ['maps', 'roadmap', 'roadmap-maps']):

        # In case it is given in the 'maps' or 'roadmap' domain a warning
        # message is issued because it will be ignored (and it is a better idea
        # to ignore it and to force the test suite to be named after the
        # map/roadmap file instead of overwritting it)
        if  (params.testfile is not None):
            LOGGER.warning(WARNING_UNNECESSARY_TESTFILE.format(params.domain))

        # and copy the name of the testfile. Look at the trick, we ensure that
        # ".test" is added to the test filename, and then we remove it using
        # stem from the pathlib
        testfile = Path(utils.get_filename(get_basename(params.testfile), ".test")).stem

    # If the domain selected is either n-pancake or n-puzzle then it is
    # mandatory
    if (params.domain in ['n-pancake', 'n-puzzle']):

        if (params.testfile is None):
            LOGGER.critical(CRITICAL_TESTFILE_NOT_GIVEN.format(params.domain))
            raise ValueError(CRITICAL_TESTFILE_NOT_GIVEN.format(params.domain))

        # and copy the name of the testfile. Look at the trick, we ensure that
        # ".test" is added to the test filename, and then we remove it using
        # stem from the pathlib
        testfile = Path(utils.get_filename(get_basename(params.testfile), ".test")).stem

    # generate the condor file
    generate_condor_file(project=params.project,
                         domain=params.domain,
                         variant=params.variant,
                         algorithm=params.algorithm,
                         params=params.parameters,
                         requirements=params.requirements,
                         user=params.user,
                         mink=k.min(),
                         maxk=k.max(),
                         mapname=get_basename(params.map),
                         n=params.size,
                         nbtiles=params.size*params.size-1)

    # and also the shell file
    generate_shell_file(project=params.project,
                        domain=params.domain,
                        variant=params.variant,
                        algorithm=params.algorithm,
                        params=params.parameters,
                        testfile=testfile,
                        kspec=params.k,
                        user=params.user,
                        mink=k.min(),
                        maxk=k.max(),
                        mapname=get_basename(params.map),
                        n=params.size,
                        nbtiles=params.size*params.size-1)

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
