#!/usr/bin/env python3
# -*- coding: utf-8 -*-
#
# pltseries.py
# Description:
# -----------------------------------------------------------------------------
#
# Started on <sáb 15-11-2025 20:19:35.151046952 (1763234375)>
# Carlos Linares López <carlos.linares@uc3m.es>
#

"""Definition of a collection of series represented as instances of either
PLTSerie or PLTKSerie

"""

# imports
# -----------------------------------------------------------------------------
import numpy

import pltGNUfile
import pltkserie
import pltpgfplot
import pltserie
import plttable
import utils

# globals
# -----------------------------------------------------------------------------

# default logger
LOGGER = utils.LOGGER

# error messages
CRITICAL_INVALID_SERIE_TYPE = "The serie {} is not an instance of either PLTSerie or PLTKSerie"
CRITICAL_OUT_OF_BOUNDS = "Index {} out of bounds"
CRITICAL_EMPTY_SERIE = "Empty serie: {}"
CRITICAL_NO_SERIES = "No series have been generated"

# -----------------------------------------------------------------------------
# PLTSeries
#
# Definition of a collection of series represented as instances of either
# PLTSerie or PLTKSerie
# -----------------------------------------------------------------------------
class PLTSeries:
    """Definition of a collection of series represented as instances of either
       PLTSerie or PLTKSerie

    """

    def __init__(self):
        """A container is always initialized empty. Importantly, it can hold
           only instances of either PLTSerie or PLTKSerie.

        """

        # initialize the container
        self._data = []

        # and also the counter used for iterating
        self._idx = 0


    def __iadd__(self, serie):
        """add a new serie to this container. Only instances of PLTSerie or
           PLTKSerie are honoured

        """

        # Only instances of pltserie and pltkserie are acknowledged
        if not isinstance(serie, pltserie.PLTSerie) and not isinstance(serie, pltkserie.PLTKSerie):
            LOGGER.critical(CRITICAL_INVALID_SERIE_TYPE.format(serie))
            raise ValueError(CRITICAL_INVALID_SERIE_TYPE.format(serie))

        # then, just simply add it to the collection
        self._data.append(serie)

        # return the object
        return self

    def __len__(self) -> int:
        """Return the number of items in this container"""

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

    def __getitem__(self, key: int):
        """Return the serie at the given location. In case the request is out of
           bounds, an error is immediately raised

        """

        if key < 0 or key >= len(self._data):
            raise ValueError(CRITICAL_OUT_OF_BOUNDS.format(key))

        return self._data[key]

    def create_canvas(self, stype: str, filename: str, **kwargs) -> pltGNUfile.PLTGNUfile|None:
        """write all series in this instance in the specified filename as either
           an image (gnuplot/pgfplot) or a table, depending upon the value of
           'stype' which has to be one among:

              + 'table'
              + 'gnuplot'
              + 'pgfplot'

           Depending upon the type of resource to be created, different
           parameters can be passed:

              + title: str
              + pngfile: str

           If one specific resource requires any argument which has not been
           given, a warning is issued

        """

        # --initialization
        ostream = None

        # in case at least one serie has been created
        if len(self._data) > 0:

            # get the x- and y- titles. It is assumed that all series have been
            # created with the same values for the x- and y- axis
            (xname, yname) = (self._data[0].get_xtitle(), self._data[0].get_ytitle())

            # create a GNUplot file with the information of all the given series, in
            # case any has been given. Note the titles for the x and y axis are the same
            # for all series
            if filename is not None and len(filename) > 0:
                ostream = {
                    'table': plttable.PLTTable(filename, xname, yname),
                    'gnuplot': pltGNUfile.PLTGNUfile(filename, xname, yname),
                    'pgfplot': pltpgfplot.PLTpgfplotfile(filename, xname, yname),
                }[stype]

                # and add all series
                for iserie in self._data:

                    # if this serie contains no data then issue an error
                    if len(iserie) == 0:
                        LOGGER.error(CRITICAL_EMPTY_SERIE.format(iserie.get_legend()))
                        raise ValueError(CRITICAL_EMPTY_SERIE.format(iserie.get_legend()))

                    # in case this is a kserie, then apply the operator and add the
                    # resulting serie to the gnuplot file
                    if isinstance(iserie, pltkserie.PLTKSerie):
                        iserie.exec(numpy.average)
                        ostream += iserie
                    else:

                        # otherwise, add the serie straight away
                        ostream += iserie

                # qualify the canvas just created
                #
                # in case this is a gnuplot, add the png file if any is given
                if stype == 'gnuplot' and kwargs['png'] is not None and len(kwargs['png']) > 0:
                    ostream.set_png(kwargs['png'])

                # set the title, provided that any was given
                if kwargs['title'] is not None and len(kwargs['title']) > 0:
                    ostream.set_title(kwargs['title'])

        else:
            LOGGER.critical(CRITICAL_NO_SERIES)
            raise ValueError(CRITICAL_NO_SERIES)
            
        # finally, return the GNUplot file with all series extracted from the
        # spreadsheet
        return ostream
    
    
        
# Local Variables:
# mode:pytho    n
# fill-column:80
# End:
