#!/usr/bin/env python3
# -*- coding: utf-8 -*-
#
# pltGNUfile.py
# Description:
# -----------------------------------------------------------------------------
#
# Started on <mié 11-10-2023 19:23:35.383738591 (1697045015)>
# Carlos Linares López <carlos.linares@uc3m.es>
#

"""
Generation of GNUplot images from data given in different series
"""

# imports
# -----------------------------------------------------------------------------
import pltserie
import utils

# globals
# -----------------------------------------------------------------------------

# default logger
LOGGER = utils.LOGGER

# critical messages
CRITICAL_INVALID_DATA = "Series have to be given as instances of PLTSerie"

# -----------------------------------------------------------------------------
# PLTGNUfile
#
# Generation of GNUplot images from data given in different series
# -----------------------------------------------------------------------------
class PLTGNUfile:
    """Generation of GNUplot images from data given in different series

    """

    def __init__(self, filename: str):
        """A GNUplot file is created with a filename. Data is added with the +=
        operator

        """

        # copy the attributes
        self._filename = filename

        # initialize the data
        self._data = []

        # and also the counter used for iterating
        self._idx = 0


    def __iadd__(self, data: pltserie.PLTSerie):
        """adds a new serie to the data stored in this GNUplot file"""

        # verify the data is properly given as an instance of a PLTSerie
        if not isinstance(data, pltserie.PLTSerie):
            LOGGER.critical(CRITICAL_INVALID_DATA)
            raise ValueError(CRITICAL_INVALID_DATA)

        # add the data to the list of series
        self._data.append(data)

        # and return the GNUplot file itself
        return self


    def __len__(self) -> int:
        """returns the number of series stored in this GNUplot file"""

        return len(self._data)

    def __iter__(self):
        """Defines the simplest case for iterators"""

        self._idx = 0
        return self

    def __next__(self):
        '''returns the next serie in this container
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



# Local Variables:
# mode:python
# fill-column:80
# End:
