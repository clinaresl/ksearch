#!/usr/bin/env python3
# -*- coding: utf-8 -*-
#
# pltserie.py
# Description:
# -----------------------------------------------------------------------------
#
# Started on <mié 11-10-2023 17:17:59.300668590 (1697037479)>
# Carlos Linares López <carlos.linares@uc3m.es>
#

"""Represents a given serie of bidimensional data which is given a legend along
with x and y titles

"""

# imports
# -----------------------------------------------------------------------------
import utils

# globals
# -----------------------------------------------------------------------------

# default logger
LOGGER = utils.LOGGER

# critical messages
CRITICAL_INVALID_DATA = "The given data is not a bidimensional point: {}"

# -----------------------------------------------------------------------------
# PLTSerie
#
# Represents a given serie of bidimensional data which is given a legend along
# with x and y titles
# -----------------------------------------------------------------------------
class PLTSerie:
    """Represents a given serie of bidimensional data which is given a legend
       along with x and y titles

    """

    def __init__(self, legend: str, xtitle: str, ytitle: str):
        """A serie is defined by a legend with titles for the x and y axis. Data
        is added with the += operator

        """

        # copy the attributes
        (self._legend, self._xtitle, self._ytitle) = (legend, xtitle, ytitle)

        # initialize the data
        self._data = []

        # and also the counter used for iterating
        self._idx = 0


    def __iadd__(self, data: tuple):
        """adds a new bidimensional point to the data stored in this serie"""

        # first, verify that data is given in two dimensions only
        if len(data) != 2:
            LOGGER.critical(CRITICAL_INVALID_DATA.format(data))

        # add the data
        self._data.append(data)

        # return the object
        return self

    def __len__(self) -> int:
        """returns the number of data points stored in this serie"""

        return len(self._data)

    def __iter__(self):
        """Defines the simplest case for iterators"""

        self._idx = 0
        return self

    def __next__(self):
        '''returns the next datapoint in this container
        '''

        # if we did not reach the limit
        if self._idx < len(self._data):

            # return the item in the current position (after incrementing)
            item = self._data[self._idx]
            self._idx += 1
            return item

        # restart the iterator for subsequent invocations of it
        self._idx = 0

        # and stop the current iteration
        raise StopIteration()


    def __str__(self) -> str:
        """returns a string representation of the data stored in this serie"""

        # initialize the output
        output = ""

        # for all data
        for (x, y) in self._data:
            output += "\t{} {}\n".format(x, y)

        # return the output
        return output

    def get_legend(self) -> str:
        """returns the legend of this serie"""

        # return the legend
        return self._legend

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
