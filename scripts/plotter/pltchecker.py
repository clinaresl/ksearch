#!/usr/bin/env python3
# -*- coding: utf-8 -*-
#
# pltchecker.py
# Description:
# -----------------------------------------------------------------------------
#
# Started on <mié 11-10-2023 16:34:13.844295356 (1697034853)>
# Carlos Linares López <carlos.linares@uc3m.es>
#

"""
Verifies that a given data line represented with a dictionary satisfies any
of a given series
"""

# imports
# -----------------------------------------------------------------------------
import copy

# globals
# -----------------------------------------------------------------------------
COMMAND = """import re
output_value = {}"""


# -----------------------------------------------------------------------------
# PLTChecker
#
# Verifies that a given data line represented with a dictionary satisfies any
# given series
# -----------------------------------------------------------------------------
class PLTChecker:
    """Verifies that a given data line represented with a dictionary satisfies
       any given series

    """

    def __init__(self, data: dict, series: list):
        """A checker is defined with data given as a dictionary and an arbitrary
           number of series given as a list of strings.

           Every string represents a valid Python expression which can use
           variables whose names must be given as keys in the data dictionary.

        """

        # copy the attributes
        (self._data, self._series) = (data, series)


    def check(self) -> list:
        """returns a list of booleans where the i-th position is True if and
           only if the data line verifies the condition given in the i-th serie
           and False otherwise

        """

        # -- initialization
        output = []

        # for all given series
        for iserie in self._series:

            # first of all, make a copy of the data line
            context = copy.deepcopy(self._data)

            # to verify whether the data satisfies this serie or not, the serie
            # is executed as ordinary Python code within a context which is the
            # dictionary itself. The result of the evaluation is retrieved in a
            # variable named 'output_value'
            exec(COMMAND.format(iserie), context)

            # and write the result of this evaluation in the output list
            output.append(context["output_value"])

        # and return the result of evaluating each serie
        return output



# Local Variables:
# mode:python
# fill-column:80
# End:
