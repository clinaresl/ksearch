#!/usr/bin/env python3
# -*- coding: utf-8 -*-
#
# plttable.py
# Description:
# -----------------------------------------------------------------------------
#
# Started on <lun 01-04-2024 17:51:06.021009729 (1711986666)>
# Carlos Linares López <carlos.linares@uc3m.es>
#

"""
Automated generation of the data body of LaTeX tables
"""

# imports
# -----------------------------------------------------------------------------
from string import Template

import pltserie
import pltkserie
import utils
import version

# globals
# -----------------------------------------------------------------------------

# default logger
LOGGER = utils.LOGGER

# critical messages
CRITICAL_INVALID_DATA = "Series have to be given as instances of PLTSerie"
CRITICAL_DUPLICATED_SERIE = "Duplicated serie! A serie with legend {} was already inserted"

# (half-)templates
TMPL_TITLE = Template(r'\multicolumn{$nbcolumns}{c}{$title}\\')
TMPL_BEST_ONE = Template(r'\textbf{$value}')

# -----------------------------------------------------------------------------
# PLTTable
#
# Generation of the data body of LaTeX tables from data given in different
# series
# -----------------------------------------------------------------------------
class PLTTable:
    """Generation of the data body of LaTeX tables from data given in different
       series

    """

    def __init__(self, filename: str, xtitle: str, ytitle: str):
        """A Table is created with a filename and the titles for the x and y
           axis. Data is added with the += operator and the title is added with
           set_title

           Every row in the table is the data of a specific serie to be added.

           Columns are then named as follows: first column is the legend of the
           serie; all the other columns are named as 'variable=value' where
           variable is the given xtitle and value is the value retrieved from
           the data series.

        """

        # copy the attributes and initialize the title of the plot to None
        (self._filename, self._xtitle, self._ytitle) = (filename, xtitle, ytitle)

        # initialize the data both as a list of series and also a dictionary of
        # them indexed by their legend
        self._data: list = []
        self._serie: dict = {}

        # and also the counter used for iterating
        self._idx = 0

        # Additionally, a title can be given to be shown on the table
        self._title = ""


    def __iadd__(self, data):
        """adds a new serie to the data stored in this instance"""

        # verify the data is properly given as an instance of a PLTSerie or PLTKSerie
        if not isinstance(data, pltserie.PLTSerie) and \
           not isinstance(data, pltkserie.PLTKSerie):
            LOGGER.critical(CRITICAL_INVALID_DATA)
            raise ValueError(CRITICAL_INVALID_DATA)

        # add the data to the list of series
        self._data.append(data)

        # and also the dictionary
        if data.get_legend() in self._serie.keys():
            LOGGER.critical(CRITICAL_DUPLICATED_SERIE.format(data.get_legend()))
            raise ValueError(CRITICAL_DUPLICATED_SERIE.format(data.get_legend()))
        self._serie[data.get_legend()] = data

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

    def write_table(self):
        """Creates the data body of a LaTeX table from the data given in all series"""

        # First, compute the name of each row as the legend of each serie
        row_names = []
        for iserie in self._data:
            row_names.append(iserie.get_legend())

        # Second, get the different values of the xtitle, which is assumed
        # to be always the same! For this, retrieve the keys of each serie and
        # compute the distinct values of it
        keys = []
        for iserie in self._data:
            keys += iserie.get_data().keys()
        xtitles = list(set(keys))
        xtitles.sort()

        # compute the total number of columns to show in the table which is one
        # for the name of the row plus the number of different values of the x
        # parameter
        nbcolumns = 1 + len(xtitles)

        # Thirdly, create a dictionary which indexes every value to be shown in
        # a cell of the table
        cells = {}

        # data in this dictionary is indexed first by the row name, next by the
        # value of the column. In passing, compute the best value for each
        # column which is assumed to be the minimum value across all non-null
        # cells
        best_one = {}
        for iname in row_names:
            cells[iname] = {}
            for ivalue in xtitles:

                # in case this serie contains a value for this column add it!
                if self._serie[iname][ivalue]:
                    cells[iname][ivalue] = "{:.2f}".format(round(self._serie[iname][ivalue], 2))

                    # in case no best value is known for this column, store it
                    # as an ancilliary value
                    if ivalue not in best_one:
                        best_one[ivalue] = self._serie[iname][ivalue]
                    else:

                        # otherwise, update the best solution found if this one
                        # is better
                        best_one[ivalue] = min(best_one[ivalue],
                                               self._serie[iname][ivalue])

                else:

                    # if no value has been found, then store a blank cell
                    cells[iname][ivalue] = "--"

        # and write data into the specified file in the construction of this
        # instance.
        with open(self._filename, 'w') as tablestream:

            # Headers of all columns.

            # In case a title was given, then show it first on a single line
            if self._title:
                tablestream.write(TMPL_TITLE.substitute({'nbcolumns': nbcolumns,
                                                         'title': self._title}))
                tablestream.write("\n")

            # next, show the header names
            tablestream.write("Algorithm & ")
            for idx, ivalue in enumerate(xtitles):
                tablestream.write("{0}={1}".format(self._xtitle, ivalue))

                # unless this is the last column, add an ampersand
                if idx < len(xtitles)-1:
                    tablestream.write(" & ")
                else:
                    tablestream.write(" ")
            tablestream.write("\\\\\n")

            # and now, write each serie in a row
            for iserie in row_names:
                tablestream.write("{0} & ".format(iserie))

                # and all values of the y parameter for every value of the x
                # parameter
                for idx, ivalue in enumerate(xtitles):

                    # if this value is the best one across all values in the
                    # same column then distinguish def __iter__(self):
                    if best_one[ivalue] == self._serie[iserie][ivalue]:
                        tablestream.write(TMPL_BEST_ONE.substitute({'value': "{:.2f}".format(round(best_one[ivalue], 2))}))

                    else:

                        # otherwise show in standard font
                        tablestream.write("{0}".format(cells[iserie][ivalue]))

                    # unless this is the last column, add an ampersand
                    if idx < len(xtitles)-1:
                        tablestream.write(" & ")
                    else:
                        tablestream.write(" ")

                # and start now a new line
                tablestream.write("\\\\\n")

# Local Variables:
# mode:python
# fill-column:80
# End:
