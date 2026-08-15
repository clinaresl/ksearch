#!/usr/bin/env python3
# -*- coding: utf-8 -*-
#
# pltpgfplot.py
# Description:
# -----------------------------------------------------------------------------
#
# Started on <sáb 26-07-2025 19:14:11.395655308 (1753550051)>
# Carlos Linares López <carlos.linares@uc3m.es>
#

"""
Generation of TikZ/pgfplots from data given in different series
"""

# imports
# -----------------------------------------------------------------------------
from datetime import datetime
from string import Template
import re

import pltconf
import pltkserie
import pltserie
import pltuntemplate
import random
import utils
import version

# globals
# -----------------------------------------------------------------------------

# default logger
LOGGER = utils.LOGGER

# constants

# debug messages
DEBUG_SUBSTITUTION = "\t{}: {}"
DEBUG_PLOT_PROPERTY_DATA = " Data {}:\n{}"

# info messages
INFO_REVERSING = "Reversing the template used to create {}"
INFO_DATA_SERIE_LEGEND = "\tSerie: {}"
INFO_DATA_SERIE_NBLINES = "\t\t# Lines: {}"

INFO_GROUP_PROPERTIES = "Group properties:"
INFO_GROUP_PROPERTY_XTITLE = "\txtitle: {}"
INFO_GROUP_PROPERTY_YTITLE = "\tytitle: {}"
INFO_GROUP_PROPERTY_SERIES = "\tseries: {}"

INFO_PLOT_PROPERTIES = "Plot properties:"
INFO_PLOT_PROPERTY_CAPTION = "\tCaption: {}"

INFO_SERIES_PROPERTIES = "Series properties:"
INFO_SERIES_MARK_KEY = "\t{}:"
INFO_SERIES_MARK_VALUE = "\t\tmark: {}"
INFO_SERIES_COLOR_VALUE = "\t\tcolor: {}"

# critical messages
CRITICAL_INVALID_DATA = "Series have to be given as instances of PLTSerie"
CRITICAL_INVALID_XTITLE = "An axis is given with xtitle={} whereas the xtitle was set to {}"
CRITICAL_INVALID_YTITLE = "An axis is given with ytitle={} whereas the ytitle was set to {}"
CRITICAL_DUPLICATED_SERIE = "Duplicated serie {} found in the plot {}"

# -----------------------------------------------------------------------------
# get_value
#
# Given a dictionary which maps regular expressions to values, return the value
# of the first key matching the given legend
# -----------------------------------------------------------------------------
def get_value(legend: str, selections: dict[str, str]) -> str:
    """Given a dictionary which maps regular expressions to values,
    return the value of the first key matching the given legend

    """

    for ikey in selections:
        if re.match(ikey, legend):
            return selections[ikey]

    # if none is found, return None
    return None

# -----------------------------------------------------------------------------
# PLTpgfplotfile
#
# Generation of TikZ/pgfplot images from data given in different series
# -----------------------------------------------------------------------------
class PLTpgfplotfile:
    """Generation of TikZ/pgfplot images from data given in different series

    """

    def __init__(self, filename: str, xtitle: str, ytitle: str):
        """A TikZ/pgfplot file is created with a filename and the titles for the
           x and y axis. Data is added with the += operator and the title is
           added with set_title

        """

        # copy the attributes and initialize the title of the plot to None
        (self._filename, self._xtitle, self._ytitle) = (filename, xtitle, ytitle)

        # initialize the data
        self._data: list = []

        # and also the counter used for iterating
        self._idx = 0

        # other data members that are initialized by other services are listed
        # below
        self._title = ""


    def __iadd__(self, data):
        """adds a new serie to the data stored in this TikZ/pgfplot file"""

        # verify the data is properly given as an instance of a PLTSerie or PLTKSerie
        if not isinstance(data, pltserie.PLTSerie) and \
           not isinstance(data, pltkserie.PLTKSerie):
            LOGGER.critical(CRITICAL_INVALID_DATA)
            raise ValueError(CRITICAL_INVALID_DATA)

        # add the data to the list of series
        self._data.append(data)

        # and return this TikZ/pltpgfplot file
        return self


    def __len__(self) -> int:
        """returns the number of series stored in this TikZ/pgfplot file"""

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

    def write(self):
        """Creates TikZ/pgfplot LaTeX code in file with the data of all series"""

        # Compute the data to show on the TikZ/pgfplot file
        coordinates = ""

        # First, compute the coordinates of each serie along with its legend
        # entry
        for idx, iserie in enumerate(self._data):

            # check whether there are colors pre-defined for this legend
            color = get_value(iserie.get_legend(), pltconf.PGFPLOT_COLORS_SELECTION)
            if color is None:
                color = pltconf.PGFPLOT_COLORS[random.randint(0, len(pltconf.PGFPLOT_COLORS)-1)]

            # likewise, check whether there are markers pre-defined for this
            # legend
            mark = get_value(iserie.get_legend(), pltconf.PGFPLOT_MARKERS_SELECTION)
            if mark is None:
                mark = pltconf.PGFPLOT_MARKERS[random.randint(0, len(pltconf.PGFPLOT_MARKERS)-1)]

            # compute the coordinates of this serie
            serie_template = Template(pltconf.TEMPLATE_PGFPLOT_SERIE)
            coordinates += serie_template.substitute(serie=f"{iserie:pgfplot}",
                                                     legend=iserie.get_legend(),
                                                     mark=mark,
                                                     color=color,
                                                     markercolor=color)

        # Now, embed all the series within a tikzpicture environment to show
        # data. Choose the appropriate template
        data_template = Template(pltconf.TEMPLATE_PGFPLOT_ENVIRONMENT)
        body = data_template.substitute(xtitle=self._xtitle,
                                        ytitle=self._ytitle,
                                        title=self._title,
                                        nblegends=len(self._data),
                                        data=coordinates)

        # open the file
        with open(self._filename, "w") as ostream:

            # show the preamble
            preamble_template=Template(pltconf.TEMPLATE_PGFPLOT_PREAMBLE)
            preamble = preamble_template.substitute(filename=self._filename,
                                                    datetime=datetime.now().strftime("%m/%d/%Y %H:%M:%S"),
                                                    author=version.__author__)
            ostream.write(preamble)

            # and now write all the data
            ostream.write(body)


# -----------------------------------------------------------------------------
# PLTpgfplotgroup
#
# Generation of a group of TikZ/pgfplot images from files containing
# TikZ/pgfplot images
# -----------------------------------------------------------------------------
class PLTpgfplotgroup:
    """Generation of a group of TikZ/pgfplot images from files containing
       TikZ/pgfplot images

    """

    def __init__(self, filenames: list):
        """A group of TikZ/pgfplot images is created from a non-empty collection
           of files containing each a TikZ/pgfplot image

        """

        # copy the attributes
        self._filenames = filenames

        # create specific containers for storing information about the group and
        # also each serie to show in every image
        #
        # The group properties are specifically three:
        #
        #    1. xtitle. All images should have the same xtitle. Otherwise an
        #       exception is raised
        #    2. ytitle. All images should have the same ytitle. Otherwise an
        #       exception is raised
        #    3. name_series: Names of the series to show in the group. They are
        #       stored to ensure that they all use the same look&feel. This
        #       shall be stored as a list instead of a string
        self._group_properties: dict[str, str] = {}

        # All images are stored in a list to ensure that they are shown in the
        # group in the same order given in the creation of this instance. Each
        # element of the list is a dictionary with the following information:
        #
        #    1. caption: Title of the image to be shown in the caption of the
        #       subfigure
        #    2. data: it is another dictionary indexed by the name series found
        #       in this image, and whose value is the whole set of coordinates
        #       to plot
        self._plot_properties: list[dict] = []

        # Information about each serie is given as a dictionary whose keys are
        # the serie name (which can be retrieved from _group_properties[name]),
        # and the value is another dictionary which records the following keys:
        #
        #    1. mark: mark used to generate the serie
        #    2. color: color used to draw the serie and to show the marks
        self._serie_properties: dict[str, dict[str, str]] = {}


    def process(self):
        """Process all files given in the creation of this instance to determine
           the key values stored in them, necessary for creating a group.

           This process is not general and follows the design of the
           TikZ/pgfplot images created by this script

        """

        # For all files
        for ifile in self._filenames:

            with open(ifile, 'r') as istream:

                # tell the user
                LOGGER.info(INFO_REVERSING.format(ifile))

                text = istream.read()

                # Every file starts with a preamble, reverse it, though no
                # variables are taken from here
                reverser_preamble = pltuntemplate.PLTuntemplate(pltconf.TEMPLATE_PGFPLOT_PREAMBLE, text)
                reverser_preamble.reverse_template()

                # show the values of the variables in DEBUG mode only
                substitutions = reverser_preamble.get_substitutions()
                for isubstitution in substitutions:
                    LOGGER.debug(DEBUG_SUBSTITUTION.format(isubstitution, substitutions[isubstitution]))

                # Remove the chunk of the text already processed
                text = text[reverser_preamble.get_cursor():]

                # Next, get the full definition of the entire TikZ/pgfplot picture.
                reverser_axis = pltuntemplate.PLTuntemplate(pltconf.TEMPLATE_PGFPLOT_ENVIRONMENT, text)
                reverser_axis.reverse_template()

                # show the values of the variables in DEBUG mode only
                substitutions = reverser_axis.get_substitutions()
                for isubstitution in substitutions:

                    # but the data, which might be too large
                    if isubstitution != "data":
                        LOGGER.debug(DEBUG_SUBSTITUTION.format(isubstitution, substitutions[isubstitution]))

                # store information of the group as retrieved from the axis
                # definition. Ensure the x- and y-title of each axis is always
                # the same. Otherwise, issue a critical error
                if "xtitle" not in self._group_properties:
                    self._group_properties["xtitle"] = substitutions["xtitle"]
                elif self._group_properties["xtitle"] != substitutions["xtitle"]:
                    LOGGER.critical(CRITICAL_INVALID_XTITLE.format(substitutions["xtitle"], self._group_properties["xtitle"]))
                    raise ValueError(CRITICAL_INVALID_XTITLE.format(substitutions["xtitle"], self._group_properties["xtitle"]))

                if "ytitle" not in self._group_properties:
                    self._group_properties["ytitle"] = substitutions["ytitle"]
                elif self._group_properties["ytitle"] != substitutions["ytitle"]:
                    LOGGER.critical(CRITICAL_INVALID_YTITLE.format(substitutions["ytitle"], self._group_properties["ytitle"]))
                    raise ValueError(CRITICAL_INVALID_YTITLE.format(substitutions["ytitle"], self._group_properties["ytitle"]))

                # Also, add information about this image to be shown in the group
                self._plot_properties.append({"caption": substitutions["title"]})

                # process the data of this axis. Note that it might contain
                # several series
                text = substitutions["data"]
                while(len(text) > 0):

                    reverser_data = pltuntemplate.PLTuntemplate(pltconf.TEMPLATE_PGFPLOT_SERIE, text)
                    reverser_data.reverse_template()
                    # print(reverser_data.get_substitutions())

                    # report the user about this serie
                    data_substitutions = reverser_data.get_substitutions()
                    LOGGER.info(INFO_DATA_SERIE_LEGEND.format(data_substitutions["legend"]))
                    LOGGER.info(INFO_DATA_SERIE_NBLINES.format(len(data_substitutions["serie"].splitlines())))

                    # and now for each serie show the substitutions in debug mode
                    for isubstitution in data_substitutions:
                        if isubstitution != "serie":
                            LOGGER.debug("\t\t" + DEBUG_SUBSTITUTION.format(isubstitution, data_substitutions[isubstitution]))

                    # store the name of this serie to be shown in the shared
                    # legend
                    if "name_series" not in self._group_properties:
                        self._group_properties["name_series"] = [data_substitutions["legend"]]
                    elif data_substitutions["legend"] not in self._group_properties["name_series"]:
                        self._group_properties["name_series"].append(data_substitutions["legend"])

                    # store the mark used to plot this serie, unless a marks has
                    # been already used in a previous plot, so that the same
                    # serie is plot with the same mark in all images of the
                    # group.
                    if data_substitutions["legend"] not in self._serie_properties:
                        self._serie_properties[data_substitutions["legend"]]={}
                    if "mark" not in self._serie_properties[data_substitutions["legend"]]:
                        self._serie_properties[data_substitutions["legend"]]["mark"] = data_substitutions["mark"]

                    # and also the color
                    if "color" not in self._serie_properties[data_substitutions["legend"]]:
                        self._serie_properties[data_substitutions["legend"]]["color"] = data_substitutions["color"]

                    # store the data segment of this serie of this plot and add
                    # it to the last dictionary in the plot properties
                    if data_substitutions["legend"] in self._plot_properties[-1]:
                        LOGGER.critical(CRITICAL_DUPLICATED_SERIE.format(data_substitutions["legend"], self._plot_properties[-1]["caption"]))
                        raise ValueError(CRITICAL_DUPLICATED_SERIE.format(data_substitutions["legend"], self._plot_properties[-1]["caption"]))
                    else:
                        self._plot_properties[-1][data_substitutions["legend"]] = data_substitutions["serie"]

                    # and now, move to the next serie
                    text = text[reverser_data.get_cursor():]


    def show_properties(self):
        """Show the properties on the logger in info mode"""

        LOGGER.info(INFO_GROUP_PROPERTIES)
        LOGGER.info(INFO_GROUP_PROPERTY_XTITLE.format(self._group_properties["xtitle"]))
        LOGGER.info(INFO_GROUP_PROPERTY_YTITLE.format(self._group_properties["ytitle"]))
        LOGGER.info(INFO_GROUP_PROPERTY_SERIES.format(",".join(self._group_properties["name_series"])))

        LOGGER.info(INFO_PLOT_PROPERTIES)
        for iplot in self._plot_properties:
            LOGGER.info(INFO_PLOT_PROPERTY_CAPTION.format(iplot["caption"]))
            for ilegend in iplot:
                if ilegend == "caption":
                    continue
                LOGGER.debug(DEBUG_PLOT_PROPERTY_DATA.format(ilegend, iplot[ilegend]))

        LOGGER.info(INFO_SERIES_PROPERTIES)
        for iserie in self._serie_properties:
            LOGGER.info(INFO_SERIES_MARK_KEY.format(iserie))
            LOGGER.info(INFO_SERIES_MARK_VALUE.format(self._serie_properties[iserie]["mark"]))
            LOGGER.info(INFO_SERIES_COLOR_VALUE.format(self._serie_properties[iserie]["color"]))


    def write(self, nbcolumns: int, caption: str, output: str):
        """Creates the TikZ/pgfplot group with all plots in the filenames given
           to the creation of this instance. The plots are arranged over a grid
           with the given number of columns and as many rows as necessary.

           Every plot is given the caption found in the input file, and the
           whole group is assigned the specified caption.

           The resulting code is shown in the specified output file.

        """

        # compute the widht of each plot. Note that 0.01 is substracted to make
        # sure that they all fit in one row
        width = (1.0 / nbcolumns) - 0.01

        # --init
        #
        # body contains the full text to write in the output file
        body = ""

        # First, go over all plots and create the code for each one of them
        for iplot in self._plot_properties:

            # collect the data from all series for this specific plot
            coordinates = ""
            for  ilegend in iplot:
                if ilegend == "caption":
                    continue

                # check whether there are colors pre-defined for this legend
                color = get_value(ilegend, pltconf.PGFPLOT_COLORS_SELECTION)
                if color is None:
                    color = self._serie_properties[ilegend]["color"]

                # likewise, check whether there are markers pre-defined for this
                # legend
                mark = get_value(ilegend, pltconf.PGFPLOT_MARKERS_SELECTION)
                if mark is None:
                    mark = self._serie_properties[ilegend]["mark"]

                # process the template which generates all data
                coordinates_template = Template(pltconf.TEMPLATE_PGFGROUP_COORDINATES)
                coordinates += coordinates_template.substitute(mark=mark,
                                                               color=color,
                                                               markercolor=color,
                                                               coordinates=iplot[ilegend],
                                                               legend=ilegend)

            # Next, create the tikzpicture environment with all plots
            environment_template = Template(pltconf.TEMPLATE_PGFGROUP_ENVIRONMENT)
            environment = environment_template.substitute(xtitle=self._group_properties["xtitle"],
                                                          ytitle=self._group_properties["ytitle"],
                                                          nblegends=len(self._group_properties["name_series"]),
                                                          data=coordinates)

            # And now, each should go into its own subfigure
            subfigure_template = Template(pltconf.TEMPLATE_PGFGROUP_SUBFIGURE)
            subfigure = subfigure_template.substitute(width=f"{width:.3}",
                                                      plot=environment,
                                                      caption=iplot["caption"])

            body += subfigure

        # Finally, put everything into a single figure* environment
        figure_template = Template(pltconf.TEMPLATE_PGFGROUP_FIGURE)
        figure = figure_template.substitute(body=body,
                                            caption=caption)

        # and create the whole figure
        with open(output, 'w') as ostream:
            ostream.write(figure)


# Local Variables:
# mode:python
# fill-column:80
# End:
