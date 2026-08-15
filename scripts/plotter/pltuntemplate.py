#!/usr/bin/env python3
# -*- coding: utf-8 -*-
#
# pltuntemplate.py
# Description:
# -----------------------------------------------------------------------------
#
# Started on <dom 27-07-2025 12:09:27.248388505 (1753610967)>
# Carlos Linares López <carlos.linares@uc3m.es>
#

"""
Template reversing
"""

# imports
# -----------------------------------------------------------------------------
import re

import utils


# globals
# -----------------------------------------------------------------------------

# default logger
LOGGER = utils.LOGGER

# regular expressions
RE_UNTEMPLATE = r"\$(?:(?P<braced>\{[_a-zA-Z][_a-zA-Z0-9]*\})|(?P<plain>[_a-zA-Z][_a-zA-Z0-9]*))"
RE_TOKEN = re.compile(RE_UNTEMPLATE)

# critical messages
CRITICAL_NON_FEASIBLE_REVERSE = "It was not possible to unreverse a template!"

# -----------------------------------------------------------------------------
# PLTuntemplate
#
# Given a template and the text that resulted from its execution, reverse the
# templating process to get a dictionary with the contents of the variables
# originally given in the template
# -----------------------------------------------------------------------------
class PLTuntemplate:
    """Given a template and the text that resulted from its execution, reverse
       the templating process to get a dictionary with the contents of the
       variables originally given in the template

    """

    def __init__(self, template_string: str, result_text: str):
        """A process for reversing needs access to the original template and
           also the resulting string

        """

        # copy the attributes
        (self._template, self._text) = (template_string, result_text)

        # create a dictionary to store the results. The keys of the dictionary
        # are the names of the variables and their value is equal to the first
        # value they were given (this applies when the same template is applied
        # repeatedly for generating the resulting text).
        self._substitutions: dict[str, str] = {}

        # To ease reusing the same text with different templates, the last
        # location of the cursor is stored so that the next chunk of text to
        # process with a different template starts from it
        self._cursor: int = 0

    def get_template(self):
        """Return the template of this instance"""

        return self._template

    def get_result(self):
        """Return the text that resulted from the substitution in the template"""

        return self._text

    def get_substitutions(self):
        """Return the substitutions performed in the template given to this
           instance to get the text used in the creation of this instance

           Invoke this service after invoking reverse_template

        """

        return self._substitutions

    def get_cursor(self):
        """Return the offset in the text given to this instance after computing
           all the necessary substitutions to the given template

           Invoke this service after invoking reverse_template

        """

        return self._cursor

    def reverse_template(self):
        """Compute and store a dictionary whose keys are the variables found in
           the original template and whose values are the strings used to create
           the resulting text from the given template

        """

        # -- init
        regex_parts = []
        cursor = 0

        # for all occurrences of the regexp
        for match in RE_TOKEN.finditer(self._template):

            # get the start and end locations of the matched text in the
            # template
            start, end = match.span()

            # and create a regex to match the chunk in the resulting text before
            # the variable
            fixed_text = re.escape(self._template[cursor:start])
            regex_parts.append (fixed_text)

            # extract the variable name used in the original template (which
            # might have been given either as '$varname' or '${varname}')
            if match.group('braced'):
                varname = match.group('braced')[1:-1]
            else:
                varname = match.group('plain')

            # and add a regex to match the contents in the resulting text that
            # match this specific variable
            regex_parts.append(f"(?P<{varname}>.*?)")
            cursor = end

        # Add any remaining text after the last variable
        regex_parts.append(re.escape(self._template[cursor:]))

        # and put all parts together in a single regex and compile it.
        # Importantly, match a whole chunk of text, including newlines, so use
        # DOTALL
        re_full_spec = "".join(regex_parts)
        re_full = re.compile(re_full_spec, re.DOTALL)

        # and now match the contents in the resulting text
        if (match := re_full.match(self._text)):

            # and store the substitutions and the last location of the cursor
            _, self._cursor = match.span()
            self._substitutions = match.groupdict()
            return

        # In case no match happened then return a serious and deep error
        LOGGER.critical(CRITICAL_NON_FEASIBLE_REVERSE)
        raise ValueError(CRITICAL_NON_FEASIBLE_REVERSE)


# Local Variables:
# mode:python
# fill-column:80
# End:
