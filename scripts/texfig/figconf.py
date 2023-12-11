#!/usr/bin/env python3
# -*- coding: utf-8 -*-
#
# figconf.py
# Description:
# -----------------------------------------------------------------------------
#
# Started on <vie 08-12-2023 18:41:29.928285754 (1702057289)>
# Carlos Linares López <carlos.linares@uc3m.es>
#

"""
Configuration file
"""

# globals
# -----------------------------------------------------------------------------

# the allowed domains are
VALID_DOMAINS = ['maps', 'n-pancake', 'n-puzzle', 'roadmap']

# the allowed parameters are
VALID_PARAMS = ['runtime', 'mem', 'expansions', 'nbcentroids']

# the allowed search variants are
VALID_SEARCH = ['brute-force', 'heuristic', 'mixed']

# png filenames are indexed by the domain name and given as regexps for
# extracting its components
RE_PNGFILENAME = {
    "maps": r'.*/random512-\d+.(?P<search>[a-zA-Z-]+).(?P<variant>[a-zA-Z-]+).(?P<param>[a-zA-Z-]+).png',
    "n-pancake": r'.*/\d+pancake.(?P<search>[a-zA-Z-]+).(?P<param>[a-zA-Z-]+).png',
    "n-puzzle": r'.*/\d+puzzle.(?P<search>[a-zA-Z-]+).(?P<param>[a-zA-Z-]+).png',
    "roadmap": r'.*/USA-road-d.[a-zA-Z-]{1,3}\.(?P<search>[a-zA-Z-]+).(?P<param>[a-zA-Z-]+).png'
}

# The entire LaTeX figure is generated with the following template which
# requires the code for each subfigure
LATEX_FIGURE = """\\begin{figure*}
  \\centering${subfigures}
  \\caption{$caption}
  \\label{fig:$domain:$variant:$search:$param}
\\end{figure*}
  """

# The code for each subfigure is generated with the following template
LATEX_SUBFIGURE = """
  \\begin{subfigure}{0.3\\textwidth}
    \\begin{center}
        \\includegraphics[width=\\textwidth]{${pngfilename}}
    \\end{center}
    \\caption{}
    \\label{fig:$domain:$variant:$search:$param:$counter}
  \\end{subfigure}"""

# The caption of a full figure is a combination of domain, variant, search and
# param. It requires a verbatim description of a param which is given first as a
# dictionary
VERBATIM_PARAM = {
    "mem": "Memory usage (in Mbytes)",
    "expansions": "Number of expansions",
    "nbcentroids": "Number of centroids",
    "runtime": "Runtime (in seconds)"
}
CAPTION_FIGURE = "${param} in the ${domain} (${variant}) domain with ${search} search algorithms"

# Local Variables:
# mode:python
# fill-column:80
# End:
