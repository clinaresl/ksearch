#!/usr/bin/env python3
# -*- coding: utf-8 -*-
#
# spsreader.py
# Description: A generic reader of spreadsheets
# -----------------------------------------------------------------------------
#
# Started on <jue 18-08-2022 19:50:55.195725497 (1660845055)>
# Carlos Linares López <carlos.linares@uc3m.es>
#

"""
A generic reader of spreadsheets
"""

# imports
# -----------------------------------------------------------------------------
import os

import pyexcel


# globals
# -----------------------------------------------------------------------------
ERROR_FILE_NOT_FOUND = "the file '{0}' does not exist or is unreachable"
ERROR_UNKNOWN_HEADER = "Unknown header '{0}' found in the spreadsheet '{1}"
ERROR_UNKNOWN_HEADER_NAME = "Unknown header '{0}'"

# -----------------------------------------------------------------------------
# SPSReader
#
# A generic reader of spreadsheets
#
# This class accesses data in spreadsheets in any format. It assumes that the
# headers are given in the first non-empty line and that data is arranged
# vertically. It skips all lines and columns which are empty
# -----------------------------------------------------------------------------
class SPSReader():
    """A generic reader of spreadsheets

       This class accesses data in spreadsheets in any format. It assumes that
       the headers are given in the first non-empty line and that data is
       arranged vertically. It skips all lines and columns which are empty
    """

    def __init__(self, spsfilename, spssheet=None):
        """the only information required to create a generic reader is the filename. In
           case the spreadsheet contains more than one sheet, then a second
           argument can be provided to access them individually. Otherwise, it
           processes only the first one

           the filename can be given with a path either relative or absolute

        """

        # copy the attributes only in case this is a valid file
        if os.path.isfile(spsfilename):
            self._spsfilename = spsfilename

        # otherwise raise an exception
        else:
            raise FileNotFoundError(ERROR_FILE_NOT_FOUND.format(spsfilename))

        # access the only sheet in the spreadsheet
        self._spssheet = spssheet
        if self._spssheet:
            self._sheet = pyexcel.get_sheet(file_name=self._spsfilename, sheet_name=self._spssheet)
        else:
            self._sheet = pyexcel.get_sheet(file_name=self._spsfilename)

        # look for the first non-empty row, which is assumed to contain the
        # headers
        self._yoffset = 0
        while self._yoffset < len(self._sheet) and \
              not any(self._sheet.row[self._yoffset]):
            self._yoffset += 1

        # likewise, look for the first non-empty column
        self._xoffset = 0
        while self._xoffset < len(self._sheet.row[self._yoffset]) and \
              not any(self._sheet.column[self._xoffset]):
            self._xoffset += 1

        # get the headers. The following data member registers the location of
        # each header so that they could be given in the spreadsheet in any
        # order
        self._header = {}
        for idx, icolumn in zip(range(len(self._sheet.row[self._yoffset])),
                                self._sheet.row[self._yoffset]):

            # while looking for the headers skip the empty columns
            if any(self._sheet.column[idx]):
                self._header[icolumn] = idx
                idx += 1

        # by default, locate at the first row of data
        self._yoffset += 1
        self._row = 0

        # finally, mark the length (or number of rows) of this spreadsheet
        # unknown
        self._length = -1


    def __call__(self, key):
        """return all values (even if they are repeated) of a header identified by its
           key

        """

        # first things first, verify that the given key is a valid symbolic name
        if key not in self._header:
            raise ValueError(ERROR_UNKNOWN_HEADER_NAME.format(key))

        # for all entries with data from the categories spreadsheet
        result = list()
        for irow in range(len(self._sheet) - self._yoffset):

            # if and only if this row is non-empty, add the corresponding field
            # to the result
            if any(self._sheet.row[irow + self._yoffset]):
                result.append(self._sheet.row[irow + self._yoffset][self._header[key]])

        return result


    def __getitem__(self, key):
        """returns all distinct values of a header identified by its key"""

        # first things first, verify that the given key is a valid symbolic name
        if key not in self._header:
            raise ValueError(ERROR_UNKNOWN_HEADER_NAME.format(key))

        # use a set to record different values
        result = set()

        # for all entries with data from the categories spreadsheet
        for irow in range(len(self._sheet) - self._yoffset):

            # if and only if this row is non-empty, add the corresponding field
            # to the set
            if any(self._sheet.row[irow + self._yoffset]):
                result.add(self._sheet.row[irow + self._yoffset][self._header[key]])

        return list(result)


    def __len__(self):
        """return the number of rows with data, i.e., skipping all empty lines"""

        # if and only if this computation was not done before
        if self._length < 0:

            # -- initialization
            self._length = 0

            # then go over all rows of the spreadsheet and count all non-empty
            # lines
            irow = self._yoffset
            while irow < len(self._sheet):

                # if this is a non-empty line then count it
                if any(self._sheet.row[irow]):
                    self._length += 1

                # and move to the next line
                irow += 1

        # note that the length of the spreadsheet is cached. This is become it
        # is assumed that the contents of the spreadsheet do never change
        return self._length


    def __iter__(self):
        '''defines the simplest case for iterators'''

        return self


    def __next__(self):
        '''returns the next data row

        '''

        # if we did not reach the limit
        if self._row < len(self._sheet) - 1:

            # move to the next row of data
            while self._row + self._yoffset < len(self._sheet) and \
                  not any(self._sheet.row[self._row + self._yoffset]):

                # if we are still within the area of the spreadsheet but this
                # line is empty, then skip it
                self._row += 1

            # if we reached the bounds of the data region, then stop the
            # iteration
            if self._row + self._yoffset == len(self._sheet):

                # restart the counter of the row before stopping the current
                # iteration
                self._row = 0
                raise StopIteration()

            # otherwise, return the current line of data encapsulated as a
            # dictionary indexed by the headers and move to the next line
            result = dict()
            for iheader in self._header:
                result[iheader] = self._sheet.row[self._row + self._yoffset][self._header[iheader]]
            self._row += 1

            return result

        # restart the iterator from the first line of data, i.e., skip the
        # headers
        self._row = 0

        # and stop the current iteration
        raise StopIteration()



# Local Variables:
# mode:python
# fill-column:80
# End:
