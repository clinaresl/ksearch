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
from string import Template

import pltconf
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
        (self._filename, self._xtitle, self._ytitle) = (filename, xtitle, ytitle)

        # initialize the data
        self._data: list = []

        # and also the counter used for iterating
        self._idx = 0

        # other data members that are initialized by other services are listed
        # below
        self._png = ""
        self._title = ""


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

    def set_png(self, value: str):
        """Set the value of a png file to be generated"""

        self._png = value

    def set_title(self, value: str):
        """Set the title of this plot"""

        self._title = value

    def write(self):
        """Creates a gnuplot file with the data of all series"""

        # create a template with the contents of GNUplot files
        tpl = Template(pltconf.TEMPLATE_GNUPLOT)

        # compute the titles of all series and the data to be shown in gnuplot
        # format
        data = ""
        title_series = ""

        # and also the marker to use, if there are any instance of PLTSerie then
        # use points; otherwise use linesp
        marker = "points" if any(map(lambda x: isinstance(x, pltserie.PLTSerie), self._data)) else "linesp"
        for idx, iserie in enumerate(self._data):

            # add the title of this serie
            if idx < len(self._data)-1:
                title_series += '"-" using 1:2:xtic(1) title "{}", '.format(iserie.get_legend())
            else:
                title_series += '"-" using 1:2:xtic(1) title "{}"'.format(iserie.get_legend())

            # and the data column in the same order
            data += f"{iserie:gnuplot}end\n"
                
        
        # and now process to substitute the template
        output = tpl.substitute(xlabel=self._xtitle,
                                ylabel=self._ytitle,
                                title=self._title,
                                output=self._png,
                                style=marker,
                                title_series=title_series,
                                data=data)

        # open the file and write the contents
        gnustream = open(self._filename, "w")
        gnustream.write(output)


# Local Variables:
# mode:python
# fill-column:80
# End:
