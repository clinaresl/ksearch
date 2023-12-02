#!/usr/bin/env python3
# -*- coding: utf-8 -*-
#
# CNDk.py
# Description:
# -----------------------------------------------------------------------------
#
# Started on <sáb 02-12-2023 17:54:20.671245746 (1701536060)>
# Carlos Linares López <carlos.linares@uc3m.es>
#

"""
Specification of the k values
"""

# imports
# -----------------------------------------------------------------------------
import re
import sys

import utils

# globals
# -----------------------------------------------------------------------------
LOGGER = utils.LOGGER

# regular expressions
RE_K_SPEC = r'^\s*(?P<k0>\d+)\s*(?P<kn>\d+)?\s*(?P<step>\d+)?\s*$'
RE_K_SPECS =r'(?P<spec>[^;]+)'

# debug messages
DEBUG_K_SPEC = "The k spec '{}' yielded the following values: ({}, {}, {})"

# critical messages
CRITICAL_WRONG_K_SPEC = "The k specification '{}' is not correct"
CRITICAL_WRONG_KN = "The k specification '{}' resulted in a value of the last-k {} which is inferior to the first value of k, {}"

# -----------------------------------------------------------------------------
# CNDkspec
#
# Process a single specification of k values to ensure it is correct
# -----------------------------------------------------------------------------
class CNDkspec:
    """Process a specification of k values to ensure it is correct and provide
       services to derive the minimum and maximum k value

    """

    def __init__(self, spec: str):
        """A single specification of k values can contain up to three different
           integers: <first-k> <last-k> <step>. If only one is given, the value
           of k is taken to be precisely that; if two are given, the step is
           assumed by default to be equal to one

        """

        # copy the attributes
        self._spec = spec

        # and initialize the attributes of this class
        (self._k0, self._kn, self._step) = (None, None, 1)

    def get_k0(self):
        """return the minimum value of this k specification"""

        return self._k0

    def get_kn(self):
        """return the maximum value of this k specification"""

        return self._kn

    def get_step(self):
        """return the step between successive values of k"""

        return self._step

    def parse(self):
        """Parses the contents of this specification, and update its private
           attributes

           If the specification is incorrect, or wrong, a critical message is
           issued and execution is terminated

        """

        # check the given specification matches the regular expression for a
        # single specification of k values
        if ( (m:=re.match(RE_K_SPEC, self._spec)) is not None):

            # then update the private attributes
            self._k0 = int(m.group('k0'))

            # in case a second argument was given
            if m.group('kn') is not None:

                self._kn = int(m.group('kn'))

                # and only in case a third value was given
                if m.group('step') is not None:
                    self._step = int(m.group('step'))

                else:

                    # if no step is given, assume one by default
                    self._step = 1

            else:

                # if only one value was given, assume the last to be equal to
                # the first and the step to be equal to one
                self._kn = self._k0
                self._step = 1

            # and show the values parsed as debug messages
            LOGGER.debug(DEBUG_K_SPEC.format(self._spec, self._k0, self._kn, self._step))

        else:

            # if the given specification is not correct, halt execution with a
            # critical error message
            LOGGER.critical(CRITICAL_WRONG_K_SPEC.format(self._spec))
            raise ValueError(CRITICAL_WRONG_K_SPEC.format(self._spec))

        # before leaving, ensure the values given make sense
        if (self._kn < self._k0):

            LOGGER.critical(CRITICAL_WRONG_KN.format(self._spec, self._kn, self._k0))
            raise ValueError(CRITICAL_WRONG_KN.format(self._spec, self._kn, self._k0))

    def __str__(self) -> str:
        """return a human readable version of the contents of this instance"""

        return "({0} {1} {2})".format(self._k0, self._kn, self._step)

# -----------------------------------------------------------------------------
# CNDk
#
# Process a specification of k values to ensure it is correct and provide
# services to derive the minimum and maximum k value
# -----------------------------------------------------------------------------
class CNDk:
    """Process a specification of k values to ensure it is correct and provide
       services to derive the minimum and maximum k value

    """

    def __init__(self, spec: str):
        """A valid specification of k values consist of a semicolon separated
           list of specifications.

           Each specification can contain up to three different integers:
           <first-k> <last-k> <step>. If only one is given, the value of k is
           taken to be precisely that; if two are given, the step is assumed by
           default to be equal to one

        """

        # copy the attributes
        self._spec = spec

        # populate a list with all the different single specifications of k
        # values as instances of CNDkspec, and parse them separately
        self._ks = []
        for ikspec in re.findall(RE_K_SPECS, self._spec):

            # parse this specification and, if valid, add it to the list of k
            # values
            cndkspec = CNDkspec(ikspec)
            cndkspec.parse()
            self._ks.append(cndkspec)

    def min(self):
        """return the minimum value of k among all of those stored in this
           instance"""

        # --initialization
        mink = sys.maxsize

        # for all k specifications
        for ikspec in self._ks:
            mink = min(ikspec.get_k0(), mink)

        return mink

    def max(self):
        """return the maximum value of k among all of those stored in this
           instance"""

        # --initialization
        maxk = 0

        # for all k specifications
        for ikspec in self._ks:
            maxk = max(ikspec.get_kn(), maxk)

        return maxk


# Local Variables:
# mode:python
# fill-column:80
# End:
