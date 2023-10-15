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
from datetime import datetime

import pltkserie
import pltserie
import utils
import version

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

    def __init__(self, filename: str, xtitle: str, ytitle: str):
        """A GNUplot file is created with a filename and the titles for the x
           and y axis. Data is added with the += operator and the title is added
           with set_title

        """

        # copy the attributes and initialize the title of the plot to None
        (self._filename, self._xtitle, self._ytitle, self._title) = (filename, xtitle, ytitle, None)

        # initialize the data
        self._data = []

        # and also the counter used for iterating
        self._idx = 0


    def __iadd__(self, data):
        """adds a new serie to the data stored in this GNUplot file"""

        # verify the data is properly given as an instance of a PLTSerie or PLTKSerie
        if not isinstance(data, pltserie.PLTSerie) and \
           not isinstance(data, pltkserie.PLTKSerie):
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

    def get_xtitle(self) -> str:
        """returns the x title of this serie"""

        # return the x title
        return self._xtitle

    def get_ytitle(self) -> str:
        """returns the y title of this serie"""

        # return the y title
        return self._ytitle

    def set_title(self, value: str):
        """Set the title of this plot"""

        self._title = value

    def write_gnuplot(self):
        """Creates a gnuplot file with the data of all series"""

        # open the file
        gnustream = open(self._filename, "w")

        # write the header
        gnustream.write("#!/usr/bin/gnuplot\n")
        gnustream.write("# -*- coding: utf-8 -*-\n")
        gnustream.write("#\n")
        gnustream.write("# " + self._filename + "\n")
        gnustream.write("#\n")
        gnustream.write("# Started on " + datetime.now().strftime("%m/%d/%Y %H:%M:%S") + "\n")
        gnustream.write("# Author: " + version.__author__ + "\n")

        # write the options
        gnustream.write("set grid\n")
        gnustream.write('set xlabel "{}"\n'.format(self._xtitle))
        gnustream.write('set ylabel "{}"\n'.format(self._ytitle))
        gnustream.write("\n")

        # in case a title was given add it
        if self._title is not None and len(self._title) > 0:
            gnustream.write('set title "{}"\n\n'.format(self._title))

        # next, add the gnuplot commands to plot the series
        gnustream.write("plot ")
        for idx, iserie in enumerate(self._data):

            if idx < len(self._data)-1:
                gnustream.write('"-" title "{}"      with linesp, '.format(iserie.get_legend()))
            else:
                gnustream.write('"-" title "{}"      with linesp\n\n'.format(iserie.get_legend()))

        # add the datapoints of each serie
        for iserie in self._data:
            gnustream.write("{}".format(iserie))
            gnustream.write("end\n")

        # Finally add a pause command to see the iamge
        gnustream.write('pause -1 "Press ENTER to exit ..."')

# Local Variables:
# mode:python
# fill-column:80
# End:
