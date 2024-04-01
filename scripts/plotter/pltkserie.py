#!/usr/bin/env python3
# -*- coding: utf-8 -*-
#
# pltkserie.py
# Description:
# -----------------------------------------------------------------------------
#
# Started on <dom 15-10-2023 15:07:18.605213599 (1697375238)>
# Carlos Linares López <carlos.linares@uc3m.es>
#

"""Represents a given serie of bidimensional data indexed by its first value
over which various operations can be performed

"""

# imports
# -----------------------------------------------------------------------------
from collections import defaultdict

import utils

# globals
# -----------------------------------------------------------------------------

# default logger
LOGGER = utils.LOGGER

# critical messages
CRITICAL_INVALID_DATA = "The given data is not a bidimensional point: {}"

# -----------------------------------------------------------------------------
# PLTKSerie
#
# Represents a given serie of bidimensional data indexed by its first value over
# which various operations can be performed
# -----------------------------------------------------------------------------
class PLTKSerie:
    """Represents a given serie of bidimensional data indexed by its first value
       over which various operations can be performed

    """

    def __init__(self, legend: str, xtitle: str, ytitle: str):
        """A serie is defined by a legend with titles for the x and y axis. Data
           is *grouped* with the += operator

        """

        # copy the attributes
        (self._legend, self._xtitle, self._ytitle) = (legend, xtitle, ytitle)

        # data to be stored in this serie consists of a dictionary which indexes
        # the second value by its first value, i.e., it groups the second value
        # under the first value so that various operations can be performed with
        # all values with the same first value
        self._data = defaultdict(list)

        # data, when being processed is stored as a list of tuples where the
        # first value is every index of the dictionary and the second value is
        # the result of applying an operation over all values of the dictionary
        # under the same key
        self._serie: list = []

        # counter of the number of values stored in the serie
        self._len = 0


    def __getitem__(self, key) -> float:
        """Return the value of the processed serie of this instance
           corresponding to the given key. If no value is found, it returns None

           This method works only when this serie has been processed with exec

        """

        # linearly search for the key
        for idata in self._serie:

            # every processed datapoint is stored as a tuple where the first
            # value is the index and the second value is the processed result
            if idata[0] == key:
                return idata[1]

        # if none has been found return so
        return None


    def __iadd__(self, data: tuple):
        """adds a new bidimensional point to the data stored in this serie"""

        # first, verify that data is given in two dimensions only
        if len(data) != 2:
            LOGGER.critical(CRITICAL_INVALID_DATA.format(data))

        # add the data and increment the number of values stored
        self._data[data[0]].append(data[1])
        self._len += 1

        # return the object
        return self

    def __len__(self) -> int:
        """returns the number of data points stored in this serie"""

        return self._len

    def __str__(self) -> str:
        """returns a string representation of the data stored in the private
           attribute self._serie, i.e., the serie has to be processed with exec
           before its contents can be shown

        """

        # initialize the output
        output = ""

        # for all data
        for (x, y) in self._serie:
            output += f"\t{x} {y}\n"

        # return the output
        return output

    def exec(self, op):
        """Applies the given operation to every item in the data dictionary. The
           function must receive a list and return a scalar, e.g., avg, std,
           min, max, etc.

           The result is privately stored in self._serie

        """

        # first, clear the serie where the results are stored
        self._serie = []

        # for every item in the data dictionary ---in ascending order of the first key
        for ikey in sorted(self._data.keys()):

            # store the value returned by the given operation for all values under this key
            self._serie.append((ikey, op(self._data[ikey])))

    def get_data(self) -> dict:
        """return the data of this instance"""

        return self._data

    def get_legend(self) -> str:
        """returns the legend of this serie"""

        # return the legend
        return self._legend

    def get_serie(self) -> list:
        """return the serie of this instance"""

        return self._serie

    def get_xtitle(self) -> str:
        """returns the x title of this serie"""

        # return the x title
        return self._xtitle

    def get_ytitle(self) -> str:
        """returns the y title of this serie"""

        # return the y title
        return self._ytitle


# Local Variables:
# mode:python
# fill-column:80
# End:
