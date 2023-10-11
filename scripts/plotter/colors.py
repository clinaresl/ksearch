#!/usr/bin/env python3
# -*- coding: utf-8 -*-
#
# colors.py
# Description: term color support
# -----------------------------------------------------------------------------
#
# Login   <carlos.linares@uc3m.es>
#

"""
term color support
"""

# imports
# -----------------------------------------------------------------------------
import io
import os
import re

# constants
# -----------------------------------------------------------------------------
PREFIX = "\033["
FOREGROUND_PREFIX = "38;2;"
BACKGROUND_PREFIX = "48;2;"
BOLD_PREFIX = "1"
DIM_PREFIX = "2"
ITALIC_PREFIX = "3"
UNDERLINE_PREFIX = "4"
BLINK_PREFIX = "5"
CROSSED_OUT_PREFIX = "9"
SUFFIX = "\033[0m"

# -----------------------------------------------------------------------------
# truecolor_enabled
#
# return true if true color is supported by the current terminal. Arguably, the
# method consists of just checking whether the env variable COLORTERM is
# 'truecolor' or not. It is known however that some terminals set it though they
# map queries to a restricted collection of colors. Other terminals, supporting
# true color however, do not set it
# -----------------------------------------------------------------------------
def truecolor_enabled():
    """return true if true color is supported by the current terminal. Arguably, the
       method consists of just checking whether the env variable COLORTERM is
       'truecolor' or not. It is known however that some terminals set it though
       they map queries to a restricted collection of colors. Other terminals,
       supporting true color however, do not set it

    """

    return os.getenv("COLORTERM") == 'truecolor'


# -----------------------------------------------------------------------------
# extract_components
#
# return the red, green and blue components from the color mask which should be
# given as a string with the format "#rrggbb". If it was not possible to
# properly parse the string, then None is returned instead
# -----------------------------------------------------------------------------
def extract_components(mask):
    """return the red, green and blue components from the color mask which should be
       given as a string with the format "#rrggbb". If it was not possible to
       properly parse the string, then None is returned instead

    """

    pattern = r'#(?P<red>[a-fA-F0-9]{2})(?P<green>[a-fA-F0-9]{2})(?P<blue>[a-fA-F0-9]{2})'

    # if the color mask does not conform to the notation then return none
    match = re.match(pattern, mask)
    if not match:
        return None

    # otherwise, return a triplet with the three components given as integers in
    # the range [0, 256)
    return (int(match.group('red'), 16),
            int(match.group('green'), 16),
            int(match.group('blue'), 16))


# -----------------------------------------------------------------------------
# insert_color
#
# return the magical ascii control codes for representing the given color mask
# -----------------------------------------------------------------------------
def insert_color(colormask):
    """return the magical ascii control codes for representing the given color mask

    """

    # -- initialization
    output = ""

    # only if a color mask has been effectively given
    if colormask:

        # if the colormask is not correct, the return the empty string
        components = extract_components(colormask)
        if components:

            # otherwise, compute the string representing the required combination
            output = str(components[0]) + ';' + \
                str(components[1]) + ';' + \
                str(components[2])

    # if no color mask has been given return the empty string
    return output


# -----------------------------------------------------------------------------
# insert_effect
#
# if and only if the value of effect is true, then the prefix corresponding to
# that effect is added to the current prefix and the result is returned
# -----------------------------------------------------------------------------
def insert_effect(effect, effect_prefix, prefix):
    """if and only if the value of effect is true, then the prefix corresponding to
       that effect is added to the current prefix and the result is returned

    """

    # only if the effect is enabled
    if effect:

        # then add the main prefix in case there is none yet; otherwise insert a
        # semicolon to distinguish the previous part of the new one
        prefix = PREFIX if not prefix else prefix + ';'
        prefix += effect_prefix

    # if the effect was not enabled, then the string passes untouched
    return prefix


# -----------------------------------------------------------------------------
# insert_prefix
#
# returns the combination of magical ascii control codes necessary to reproduce
# the combination of colors given in the foreground and the background along
# with the specified effects
#
# Both the foreground and background color masks should be given in the format
# "#rrggbb" where rr/gg/bb are two hexadecimal digits each with the red, green
# and blue components
#
# To enable effects it just suffices to give them any non-false value
# -----------------------------------------------------------------------------
def insert_prefix(foreground=None, background=None,
                  bold=False, dim=False, italic=False,
                  underline=False, blink=False, crossed_out=False):
    """returns the combination of magical ascii control codes necessary to reproduce
       the combination of colors given in the foreground and the background
       along with the specified effects

       Both the foreground and background color masks should be given in the
       format "#rrggbb" where rr/gg/bb are two hexadecimal digits each with the
       red, green and blue components

       To enable effects it just suffices to give them any non-false value

    """

    # -- initialization
    prefix = ""

    # -- foreground color
    if foreground:

        # if the foreground could be effectively processed, then add it;
        # otherwise, ignore it
        fore_str = insert_color(foreground)
        if fore_str:
            prefix = PREFIX + FOREGROUND_PREFIX + fore_str

    # -- background color
    if background:

        # likewise, if the background could be effectively processed, then add
        # it; otherwise, ignore it
        back_str = insert_color(background)
        if back_str:

            # if no foreground color was given, then start the prefix with the
            # magic heading ascii characters, otherwise, distinguish the
            # previous part of the next one with a semicolon
            prefix = PREFIX if not prefix else prefix + ';'

            # and in any case insert the specification for the background
            prefix += BACKGROUND_PREFIX + back_str

    # -- effects
    prefix = insert_effect(bold, BOLD_PREFIX, prefix)
    prefix = insert_effect(dim, DIM_PREFIX, prefix)
    prefix = insert_effect(italic, ITALIC_PREFIX, prefix)
    prefix = insert_effect(underline, UNDERLINE_PREFIX, prefix)
    prefix = insert_effect(blink, BLINK_PREFIX, prefix)
    prefix = insert_effect(crossed_out, CROSSED_OUT_PREFIX, prefix)

    # in spite of the options given by the user, the prefix is ended with 'm',
    # if any has been generated
    if prefix:
        prefix += 'm'

    # and return the prefix computed so far
    return prefix


# -----------------------------------------------------------------------------
# insert_suffix
#
# returns the suffix used to terminate any combination of magical ascii control
# codes
# -----------------------------------------------------------------------------
def insert_suffix():
    """returns the suffix used to terminate any combination of magical ascii control
       codes

    """

    # the suffix consists only of the reset command
    return SUFFIX


# -----------------------------------------------------------------------------
# cprint
#
# print extended with color support and various effects
#
# objects, sep and end have the same purpose and meaning than in the
# built-in function print. Note that the file redirection has been removed
# as this overloaded version is intended to work only on terminals
#
# Both the foreground and background color masks should be given in the
# format "#rrggbb" where rr/gg/bb are two hexadecimal digits each with the
# red, green and blue components
#
# To enable effects it just suffices to give them any non-false value
# -----------------------------------------------------------------------------
def cprint(*objects, sep=' ', end='\n',
           foreground=None, background=None,
           bold=False, dim=False, italic=False, underline=False, blink=False, crossed_out=False):
    """print extended with color support and various effects

       objects, sep and end have the same purpose and meaning than in the
       built-in function print. Note that the file redirection has been removed
       as this overloaded version is intended to work only on terminals

       Both the foreground and background color masks should be given in the
       format "#rrggbb" where rr/gg/bb are two hexadecimal digits each with the
       red, green and blue components

       To enable effects it just suffices to give them any non-false value

    """

    # -- initialization
    prefix = ""
    suffix = ""

    # First create the string that results after printing the objects with the
    # given parameters ignoring colors by now. Note that the end is inhibited,
    # while the separator is used, this is because we are interested only in get
    # the text to print
    buffer = io.StringIO()
    print(*objects, sep=sep, end='', file=buffer)

    # compute the preffix and, if any is generated, then add the suffix
    prefix = insert_prefix(foreground, background,
                           bold, dim, italic, underline, blink, crossed_out)
    if prefix:
        suffix = insert_suffix()

    # finally, add the prefix to the output and suffix. Notice that sep is now
    # inhibited because it was already considered, while we add any end
    # character given
    print(prefix + buffer.getvalue() + suffix, sep='', end=end)


# -----------------------------------------------------------------------------
# test
#
# test true color on the terminal
# -----------------------------------------------------------------------------
def test():
    """test true color on the terminal"""

    # first, show solid utf-8 blocks with different colors

    # from red to green, in steps to 3 with the hope that the bar fits the
    # terminal width
    for index in range(0, 256, 3):
        foreground = "#%0.2x" % (255 - index) + "%0.2x" % index + "00"
        cprint('█', end='', foreground=foreground)

    # from green to blue
    for index in range(0, 256, 3):
        foreground = "#00%0.2x" % (255 - index) + "%0.2x" % index
        cprint('█', end='', foreground=foreground)

    # write a text with different effects, but using only one
    text = "last words? git push -f"
    cprint()
    cprint(text, bold=True)
    cprint(text, dim=True)
    cprint(text, italic=True)
    cprint(text, underline=True)
    cprint(text, blink=True)
    cprint(text, crossed_out=True)

    # show various combinations of two effects
    cprint(text, bold=True, italic=True)
    cprint(text, bold=True, underline=True)
    cprint(text, bold=True, blink=True)
    cprint(text, bold=True, crossed_out=True)

    cprint(text, italic=True, dim=True)
    cprint(text, italic=True, underline=True)
    cprint(text, italic=True, blink=True)
    cprint(text, italic=True, crossed_out=True)

    # -- foreground
    quijote1 = "  En un lugar de la Mancha, de cuyo nombre no quiero acordarme, no ha mucho tiempo que vivía un hidalgo de los de lanza en astillero, adarga antigua, rocín flaco y galgo corredor."
    for index, _ in enumerate(quijote1):
        offset = int(255*index / len(quijote1))
        foreground = "#%0.2x" % (255 - offset) + "%0.2x" % offset + "00"
        cprint(quijote1[index], end='', foreground=foreground, bold=True)
    cprint()

    quijote2 = "Una olla de algo más vaca que carnero, salpicón las más noches, duelos y quebrantos los sábados, lentejas los viernes, algún palomino de añadidura los domingos, consumían las ... "
    for index, _ in enumerate(quijote2):
        offset = int(255*index / len(quijote2))
        foreground = "#00" + "%0.2x" % (255 - offset) + "%0.2x" % offset
        cprint(quijote2[index], end='', foreground=foreground, underline=True)
    cprint()

    quijote3 = "tres partes de su hacienda. El resto della concluían sayo de velarte, calzas de velludo para las fiestas con sus pantuflos de lo mismo, los días de entre semana se honraba con ..."
    for index, _ in enumerate(quijote3):
        offset = int(255*index / len(quijote3))
        foreground = "#%0.2x" % offset + "00" + "%0.2x" % (255 - offset)
        cprint(quijote3[index], end='', foreground=foreground, italic=True)
    cprint()
    cprint()

    # -- background
    for index, _ in enumerate(quijote1):
        offset = int(255*index / len(quijote1))
        background = "#%0.2x" % (255 - offset) + "%0.2x" % offset + "00"
        cprint(quijote1[index], end='', background=background, bold=True)
    cprint()

    for index, _ in enumerate(quijote2):
        offset = int(255*index / len(quijote2))
        background = "#00" + "%0.2x" % (255 - offset) + "%0.2x" % offset
        cprint(quijote2[index], end='', background=background, underline=True)
    cprint()

    for index, _ in enumerate(quijote3):
        offset = int(255*index / len(quijote3))
        background = "#%0.2x" % offset + "00" + "%0.2x" % (255 - offset)
        cprint(quijote3[index], end='', background=background, italic=True)
    cprint()
    cprint()

    # -- foreground & background
    for index, _ in enumerate(quijote1):
        offset = int(255*index / len(quijote1))
        foreground = "#%0.2x" % (255 - offset) + "%0.2x" % offset + "00"
        background = "#00" + "%0.2x" % (255 - offset) + "%0.2x" % offset
        cprint(quijote1[index], end='', foreground=foreground, background=background,
               bold=True)
    cprint()

    for index, _ in enumerate(quijote2):
        offset = int(255*index / len(quijote2))
        foreground = "#00" + "%0.2x" % (255 - offset) + "%0.2x" % offset
        background = "#%0.2x" % offset + "00" + "%0.2x" % (255 - offset)
        cprint(quijote2[index], end='', foreground=foreground, background=background,
               underline=True)
    cprint()

    for index, _ in enumerate(quijote3):
        offset = int(255*index / len(quijote3))
        foreground = "#%0.2x" % offset + "00" + "%0.2x" % (255 - offset)
        background = "#%0.2x" % (255 - offset) + "%0.2x" % offset + "00"
        cprint(quijote3[index], end='', foreground=foreground, background=background,
               italic=True)
