#!/usr/bin/env python3
# -*- coding: utf-8 -*-
#
# texfig.py
# Description:
# -----------------------------------------------------------------------------
#
# Started on <vie 08-12-2023 18:35:56.016025770 (1702056956)>
# Carlos Linares López <carlos.linares@uc3m.es>
#

"""
Creates LaTeX code for generating the images with all data from the experiments
"""

# imports
# -----------------------------------------------------------------------------
import os
import re
import time

from pathlib import Path
from string import Template

import argparser
import figconf
import utils

# globals
# -----------------------------------------------------------------------------
LOGGER = utils.LOGGER

# debug messages
DEBUG_EXAMINING_FOLDER = "Examining folder '{}'"
DEBUG_ADDING_SUBFIGURE = "\t Adding subfigure from file '{}'"

# critical messages
CRITICAL_INVALID_VARIANT = "The variant '{}' is not valid for the domain '{}'"
CRITICAL_MAP_NOT_GIVEN = "The map name must be given for the domain 'maps' or 'roadmap'"
CRITICAL_SIZE_NOT_GIVEN = "The size of the instance must be given for the domain 'n-pancake' or 'n-puzzle'"
CRITICAL_INVALID_SEARCH = "The selection of search algorithms in the set 'mixed' is not valid for the domain '{}'"

# functions
# -----------------------------------------------------------------------------

# -----------------------------------------------------------------------------
# validate_variant
#
# Return true if and only if the given variant exists in the specified domain
# and false otherwise
# -----------------------------------------------------------------------------
def validate_variant(domain: str, variant: str):
    """Return true if and only if the given variant exists in the specified
       domain and false otherwise"""

    # specification of valid variants as a dictionary of domains
    valid_variants = {
        "maps": ["unit", "octile"],
        "n-pancake": ["unit", "heavy-cost"],
        "n-puzzle": ["unit", "heavy-cost"],
        "roadmap": ["unit", "dimacs"]
        }

    # return whether the given variant is correct or not
    return variant in valid_variants[domain]


# -----------------------------------------------------------------------------
# generate_tex_code
#
# Generate the LaTeX code for generating the images with all data for the given
# selection of input parameters
# -----------------------------------------------------------------------------
def generate_tex_code(domain: str, variant: str, param: str, search: str,
                      filename: str, root:str):
    """Generate the LaTeX code for generating the images with all data for the
         given selection of input parameters

    """

    # move to the right directory where all the results can be found
    folder = os.path.join(root, domain, variant)

    # determine all the different cases for this combination of domain and
    # variant and store all png files with the requested information, i.e.,
    # corresonding to the given search variant and param selection
    files = []
    for isubdir in Path(folder).iterdir():
        if isubdir.is_dir():
            LOGGER.debug(DEBUG_EXAMINING_FOLDER.format(isubdir))
            for jsubdir in Path(isubdir).iterdir():
                if jsubdir.is_file():

                    # check whether this file matches the filename corresponding
                    # to the combination specified by the user
                    if (m := re.match(figconf.RE_PNGFILENAME[domain], str(jsubdir))):

                        # verify the search and param values given by the user
                        if (m.group('search') == search and
                            m.group('param') == param):

                            # and add this file to the list of files to add in
                            # the LaTeX figure
                            files.append(str(jsubdir))

                            LOGGER.debug(DEBUG_ADDING_SUBFIGURE.format(jsubdir))

    # now, create the LaTeX figure with a subfigure for each png file
    counter = 0                # used to refer to the subfigures: (a), (b), ...
    latex_subfigures = ""
    for ifile in sorted(files):

        # instantiate the template to generate information for this specific
        # image and add it to the LaTeX commands necessary for generating all
        # subfigures
        template = Template(figconf.LATEX_SUBFIGURE)
        latex_subfigures += template.substitute(domain=domain,
                                                variant=variant,
                                                search=search,
                                                param=param,
                                                counter=chr(ord('a')+counter),
                                                pngfilename=ifile)

        # and increment the counter
        counter += 1

    # Finally, instantiate the general template with the code of all subfigures,
    # using the correct caption
    tmpl_caption = Template(figconf.CAPTION_FIGURE)
    tmpl_figure = Template(figconf.LATEX_FIGURE)
    return tmpl_figure.substitute(domain=domain,
                                  variant=variant,
                                  search=search,
                                  param=param,
                                  subfigures=latex_subfigures,
                                  caption=tmpl_caption.substitute(param=figconf.VERBATIM_PARAM[param],
                                                                  domain=domain,
                                                                  variant=variant,
                                                                  search=search))


# -----------------------------------------------------------------------------
# main body
# -----------------------------------------------------------------------------
def main():
    """Main body

    """

    # -------------------------------------------------------------------------
    # --initialization

    # get the current time
    start_time = time.time()

    # invoke the parser and parse all commands
    params = argparser.createPrgArgParser().parse_args()

    # set the requested logging level
    LOGGER.setLevel(utils.get_logging_level(params.level))

    # determine whether the given variant is correct or not
    if not validate_variant(params.domain, params.variant):
        LOGGER.critical(CRITICAL_INVALID_VARIANT.format(params.variant, params.domain))
        raise ValueError(CRITICAL_INVALID_VARIANT.format(params.variant, params.domain))

    # check that the set of algorithms chosen makes sense in the given domain.
    # Actually, "mixed" is allowed only in the roadmap domain
    if (params.search == "mixed" and params.domain != "roadmap"):
        LOGGER.critical(CRITICAL_INVALID_SEARCH.format(params.domain))
        raise ValueError(CRITICAL_INVALID_SEARCH.format(params.domain))

    # and generate the code
    code =generate_tex_code(params.domain, params.variant, params.param, params.search,
                            params.file, params.root)

    # and save the LaTeX commands in the specified file
    with open(params.file, 'w') as stream:
        stream.write(code)

# main
# -----------------------------------------------------------------------------
if __name__ == '__main__':
    main()



# Local Variables:
# mode:python
# fill-column:80
# End:
