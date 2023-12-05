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

# critical messages
CRITICAL_INVALID_VARIANT = "The variant '{}' is not valid for the domain '{}'"
CRITICAL_MAP_NOT_GIVEN = "The map name must be given for the domain 'maps' or 'roadmap'"
CRITICAL_SIZE_NOT_GIVEN = "The size of the instance must be given for the domain 'n-pancake' or 'n-puzzle'"
CRITICAL_TESTFILE_NOT_GIVEN = "The testfile is mandatory for the domain '{}'"

# -----------------------------------------------------------------------------
# validate_variant
#
# Return true if and only if the given variant exists in the specified domain
# and false otherwise
# -----------------------------------------------------------------------------
def validate_variant(domain: str, variant: str):
    """Return true if and only if the given variant exists in the specified
       domain and false otherwise"""

    # specification of valid variants as a dictionary of domains
    valid_variants = {
        "maps": ["unit", "octile"],
        "n-pancake": ["unit", "heavy-cost"],
        "n-puzzle": ["unit", "heavy-cost"],
        "roadmap": ["unit", "dimacs"]
        }

    # return whether the given variant is correct or not
    return variant in valid_variants[domain]

# -----------------------------------------------------------------------------
# generate_condor_file
#
# generates the condor file from the values given
# -----------------------------------------------------------------------------
def generate_condor_file(domain: str, variant: str, algorithm: str, user:str,
                         mink: int, maxk: int,
                         mapname: str,
                         n: int, nbtiles: int):
    """generates the condor file from the values given"""

    # in case the algorithm contains a star, replace it by a 's'
    algorithm_nostar = algorithm.replace('*', 's')

    # first, determine the filename of the condor file. For this, instantiate
    # the template corresponding to the given domain with the given values
    template = Template(cndconf.FILENAME[domain])
    filename = template.substitute(map=mapname,
                                   solver=algorithm_nostar,
                                   variant=variant,
                                   mink=mink,
                                   maxk=maxk,
                                   size=n,
                                   nbtiles=nbtiles)

    # next, create the contents of the condor submission file
    template = Template(cndconf.CONDOR_FILE)
    contents = template.substitute(domain=domain,
                                   variant=variant,
                                   algorithm=algorithm,
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
def generate_shell_file(domain: str, variant: str, algorithm: str,
                        testfile: str, kspec: str,
                        user:str,
                        mink: int, maxk: int,
                        mapname: str,
                        n: int, nbtiles: int):
    """generates the shell script to execute in the backend node"""

    # in case the algorithm contains a star, replace it by a 's'
    algorithm_nostar = algorithm.replace('*', 's')

    # first, determine the filename of the shell script. For this, instantiate
    # the template corresponding to the given domain with the given values
    template = Template(cndconf.FILENAME[domain])
    filename = template.substitute(map=mapname,
                                   solver=algorithm_nostar,
                                   variant=variant,
                                   mink=mink,
                                   maxk=maxk,
                                   size=n,
                                   nbtiles=nbtiles)

    # prior to the creation of the shell file, compute the arguments to give to
    # the solver
    template = Template(cndconf.ARGS[domain])
    args = template.substitute(domain=domain,
                               testfile=testfile,
                               algorithm=algorithm,
                               variant=variant,
                               kspec=kspec,
                               filename=filename)

    # next, create the contents of the condor submission file
    template = Template(cndconf.SHELL_FILE)
    contents = template.substitute(domain=domain,
                                   variant=variant,
                                   algorithm=algorithm,
                                   user=user,
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
    if (params.domain in ['maps', 'roadmap']) and (params.map is None):
        LOGGER.critical(CRITICAL_MAP_NOT_GIVEN)
        raise ValueError(CRITICAL_MAP_NOT_GIVEN)
    if (params.domain in ['n-pancake', 'n-puzzle']) and (params.size == 0):
        LOGGER.critical(CRITICAL_SIZE_NOT_GIVEN)
        raise ValueError(CRITICAL_SIZE_NOT_GIVEN)

    # Compute the name of the testfile. If the domain is the 'maps' or the
    # 'roadmap' it should be indeed equal to the map filename and thus, it must
    # not be given 'n-pancake' and 'n-puzzle'.
    testfile=None
    if (params.domain in ['maps', 'roadmap']):

        # In case it is given in the 'maps' or 'roadmap' domain a warning
        # message is issued because it will be ignored (and it is a better idea
        # to ignore it and to force the test suite to be named after the
        # map/roadmap file instead of overwritting it)
        if  (params.testfile is not None):
            LOGGER.warning(WARNING_UNNECESSARY_TESTFILE.format(params.domain))

        # and update the name of the testfile
        testfile = params.map

    # If the domain selected is either the n-pancake or the n-puzzle then it is
    # mandatory
    if (params.domain in ['n-pancake', 'n-puzzle']):

        if (params.testfile is None):
            LOGGER.critical(CRITICAL_TESTFILE_NOT_GIVEN.format(params.domain))
            raise ValueError(CRITICAL_TESTFILE_NOT_GIVEN.format(params.domain))

        # and copy the name of the testfile
        testfile = params.testfile

    # generate the condor file
    generate_condor_file(params.domain, params.variant, params.algorithm, params.user,
                         k.min(), k.max(),
                         params.map,
                         params.size, params.size*params.size-1)

    # and also the shell file
    generate_shell_file(params.domain, params.variant, params.algorithm,
                        testfile, params.k,
                        params.user,
                        k.min(), k.max(),
                        params.map,
                        params.size, params.size*params.size-1)

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
