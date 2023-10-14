#!/usr/bin/env python3
# -*- coding: utf-8 -*-
#
# utils.py
# Description: Helper functions
# -----------------------------------------------------------------------------
#
# Started on <jue 04-02-2021 21:28:18.019659815 (1612470498)>
# Carlos Linares López <carlos.linares@uc3m.es>
#

"""
Helper functions
"""

# imports
# -----------------------------------------------------------------------------
import datetime
import errno
import logging
import os
import re

import colors

# constants
# -----------------------------------------------------------------------------

# logging

LOG_FORMAT = '[%(color_lvlname_prefix)s %(levelname)-8s:%(color_suffix)s %(color_ascitime_prefix)s %(asctime)s | %(color_suffix)s %(color_name_prefix)s %(name)s%(color_suffix)s]: %(color_prefix)s %(message)s %(color_suffix)s'
LOG_COLOR_PREFIX = {
    "ASCITIME" : colors.insert_prefix(foreground="#008080"),
    "NAME" : colors.insert_prefix(foreground="#00a0a0", italic=True),
    "DEBUG" : colors.insert_prefix(foreground="#99ccff"),
    "INFO" : colors.insert_prefix(foreground="#a0a020"),
    "WARNING" : colors.insert_prefix(foreground="#20aa20", bold=True),
    "ERROR" : colors.insert_prefix(foreground="#ff2020", bold=True),
    "CRITICAL" : colors.insert_prefix(foreground="#ff0000", bold=True)
}

#

# functions
# -----------------------------------------------------------------------------

# -----------------------------------------------------------------------------
# seconds_to_str
#
# return a string with the number of hours, minutes and seconds corresponding to
# the given number of seconds
# -----------------------------------------------------------------------------
def seconds_to_str(seconds: int) -> str:
    """return a string with the number of hours, minutes and seconds
       corresponding to the given number of seconds

    """

    # -- initialization
    hours = 0
    minutes = 0
    seconds = int(seconds)

    # -- compute the number of hours, minutes and seconds
    hours = seconds // 3600
    seconds = seconds % 3600
    minutes = seconds // 60
    seconds = seconds % 60

    # -- return the result
    return "{0:02d}:{1:02d}:{2:02d}".format(hours, minutes, seconds)


# -----------------------------------------------------------------------------
# string_to_date
#
# casts the given value which should be a string to an instace of datetime.date.
# The date could be given in any of the formats YYYY-MM-DD, DD-MM-YY,
# DD-MM-YYYY, YYYY/MM/DD, DD/MM/YY or DD/MM/YYYY . If only two digits are given
# for the year "20" is prepended
# -----------------------------------------------------------------------------
def string_to_date(value: str):
    '''casts the given value which should be a string to an instace of
       datetime.date. The date could be given in any of the formats YYYY-MM-DD,
       DD-MM-YY, DD-MM-YYYY, YYYY/MM/DD, DD/MM/YY or DD/MM/YYYY . If only two
       digits are given for the year "20" is prepended

    '''

    # check list
    #
    # sometimes reading strings in python automatically produces datetime.date
    # objects. If this happened, return it directly
    if (isinstance(value, datetime.date)):
        return value

    # -- initialization
    result = None

    # first, get a datetime.datetime from the given argument

    # check if the format used was YYYY, mm, dd with either dashes or slashes
    if re.match(r'^\d{4}-\d{1,2}-\d{1,2}$', value):
        result = datetime.datetime.strptime(value, "%Y-%m-%d")
    elif re.match(r'^\d{4}/\d{1,2}/\d{1,2}$', value):
        result = datetime.datetime.strptime(value, "%Y/%m/%d")

    # check if the format used was dd, mm, YYYY with either dashes or slashes
    elif re.match(r'^\d{1,2}-\d{1,2}-\d{4}$', value):
        result = datetime.datetime.strptime(value, "%d-%m-%Y")
    elif re.match(r'^\d{1,2}/\d{1,2}/\d{4}$', value):
        result = datetime.datetime.strptime(value, "%d/%m/%Y")

    # finally, check whether the format used was dd, mm, yy with either dashes
    # or slashes
    elif re.match(r'^\d{1,2}-\d{1,2}-\d{2}$', value):
        result = datetime.datetime.strptime(value, "%d-%m-%y")
    elif re.match(r'^\d{1,2}/\d{1,2}/\d{2}$', value):
        result = datetime.datetime.strptime(value, "%d/%m/%y")

    # if and only if the given string could be parsed
    if result:

        # then return a datetime.date object
        return datetime.date (result.year, result.month, result.day)

    # otherwise return None as an indication of error
    return None


# -----------------------------------------------------------------------------
# return a time object from a string
# -----------------------------------------------------------------------------
def string_to_time(value: str):
    """return a time object from a string

    """

    return datetime.time.fromisoformat(value)


# -----------------------------------------------------------------------------
# return whether the given file can be read or not, in spite of it existing or
# not. If the file is not readable it returns in addition an error message which
# is empty if the file is readable
# -----------------------------------------------------------------------------
def check_file_readable(fnm):
    """return whether the given file can be read or not, in spite of it
        existing or not. If the file is not readable it returns in addition an
        error message which is empty if the file is readable

    """

    try:
        with open(fnm, encoding="utf-8") as f:
            s = f.read()
            return True, ""
    except IOError as x:
        if x.errno == errno.ENOENT:
            return False, "'{0}' does not exist".format(fnm)
        elif x.errno == errno.EACCES:
            return False, "'{0}' cannot be read".format(fnm)
        else:
            return False, "'{0}' unknown error!".format(fnm)
    except UnicodeDecodeError as x:

        # in case of this error, the file has proven itself to be readable
        return True, ""

# -----------------------------------------------------------------------------
# return whether the given file can be written or not, in spite of it existing
# or not
# -----------------------------------------------------------------------------
def check_file_writable(fnm):
    """return whether the given file can be written or not, in spite of it
       existing # or not

    """

    # first case, the file already exists
    if os.path.exists(fnm):
        if os.path.isfile(fnm):
            return os.access(fnm, os.W_OK)
        else:
            return False

    # second case, the target does not exist, check perms on parent dir
    pdir = os.path.dirname(fnm)
    if not pdir:
        pdir = '.'

    # target is creatable if parent dir is writable
    return os.access(pdir, os.W_OK)


# -----------------------------------------------------------------------------
# get_basename
# return the basename of a file, i.e., the whole string but the contents after
# the *last* dot
# -----------------------------------------------------------------------------
def get_basename(filename: str):
    """return the basename of a file, i.e., the whole string but the contents after
       the *last* dot

    """

    # verify there is at least one dot
    match = re.match(r'(?P<filename>.*)\..*', filename)

    # if none exists, reeturn the whole string
    if not match:
        return filename

    # otherwise, just return everything before the last dot ---note this
    # function strongly relies on the greedy behaviour of the package re
    return match.group("filename")


# -----------------------------------------------------------------------------
# get_full_path
#
# return the absolute path of the path given in its argument which is expected
# to be a relative path which might use ~. This function does not implement any
# error checking. If the given string is badly-formed the results are undefined
# -----------------------------------------------------------------------------
def get_full_path(pathname: str):
    """return the absolute path of the path given in its argument which is expected
       to be a relative path which might use ~. This function does not implement
       any # error checking. If the given string is badly-formed the results are
       undefined

    """

    # remove unnecessary blanks
    pathname = pathname.strip()

    # in case the pathname is an absolute path. 'abspath' is used again to
    # ensure that the same convention is used regarding the inclusion of the
    # trailing os separator '/
    if os.path.isabs(pathname):
        return os.path.abspath(pathname)

    # in case this is a path starting with the $home. In this case, the first
    # component should be strictly equal to ~
    if pathname[0] == "~":

        # split the whole pathname into its components
        components = pathname.split(os.sep)

        # join all components again after expanding the first one
        pathname = os.path.join(os.path.expanduser("~"), *components[1:])

    # otherwise, this is assumed to be a relative path name so that return its
    # absolute path
    return os.path.abspath(pathname)

# -----------------------------------------------------------------------------
# get_filename
#
# return the right name of a file. If the given filename already finishes with
# the given suffix, then it is readily used; otherwise, the given suffix is
# added
# -----------------------------------------------------------------------------
def get_filename(filename: str, suffix: str):
    """return the right name of a file. If the given filename already finishes with
       the given suffix, then it is readily used; otherwise, the given suffix is
       added

    """

    # trivial case - no suffix is given
    if not suffix:
        return filename

    # break the filename into its different components
    split = os.path.splitext(filename)

    # before moving one make sure the specified suffix starts with a dot after
    # removing whitespaces at the beginning and end if any is given
    suffix = suffix.strip()
    suffix = '.' + suffix if suffix[0] != '.' else suffix

    # if the given suffix is already in use, then return the given filename
    # straight ahead
    if split[-1] == suffix:
        return filename

    # in any other case (either if no extension was given, or an extension
    # different than the specified suffix) was given, then add the given suffix
    return filename + suffix

# -----------------------------------------------------------------------------
# get_logging_level
#
# returns the logging level corresonding to the given string which has to be
# defined in logger. Otherwise, an exception is raised
# -----------------------------------------------------------------------------
def get_logging_level(level: str):
    """returns the logging level corresponding to the given string which has to
       be defined in logger. Otherwise, as exception is raised

    """

    if level.upper() == "CRITICAL":
        return logging.CRITICAL
    elif level.upper() == "ERROR":
        return logging.ERROR
    elif level.upper() == "WARNING":
        return logging.WARNING
    elif level.upper() == "INFO":
        return logging.INFO
    elif level.upper() == "DEBUG":
        return logging.DEBUG
    elif level.upper() == "NOTSET":
        return logging.NOTSET
    else:
        raise ValueError("Unknown logging level '{0}".format(level))

# -----------------------------------------------------------------------------
# setup_logger
#
# setup and configure a logger
# -----------------------------------------------------------------------------
def setup_logger(verbose = False):
    """setup and configure a logger"""

    logger = logging.getLogger('plotter')
    logger.addFilter(LoggerContextFilter())
    if verbose:
        logger.setLevel(logging.DEBUG)
    else:
        logger.setLevel(logging.INFO)
    handler = logging.StreamHandler()
    formatter = logging.Formatter(LOG_FORMAT)
    handler.setFormatter(formatter)
    logger.addHandler(handler)

    # and return the logger
    return logger


# -----------------------------------------------------------------------------
# LoggerContextFilter
#
# Creation of a context filter for the logger that adds color support
# -----------------------------------------------------------------------------
class LoggerContextFilter(logging.Filter):
    """
    Creation of a context filter for the logger that adds color support
    """

    def filter(self, record):

        # first inject the colors for all fields in the header
        record.color_lvlname_prefix = LOG_COLOR_PREFIX[record.levelname]
        record.color_ascitime_prefix = LOG_COLOR_PREFIX['ASCITIME']
        record.color_name_prefix = LOG_COLOR_PREFIX['NAME']

        # choose the color as a function of the level of the log message
        record.color_prefix = LOG_COLOR_PREFIX[record.levelname]
        record.color_suffix = colors.insert_suffix()

        return True

# globals
# -----------------------------------------------------------------------------

# default logger
LOGGER = setup_logger()


# Local Variables:
# mode:python
# fill-column:80
# End:
