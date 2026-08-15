#!/usr/bin/env python3
# -*- coding: utf-8 -*-
#
# pltconf.py
# Description:
# -----------------------------------------------------------------------------
#
# Started on <dom 27-07-2025 12:50:43.621195514 (1753613443)>
# Carlos Linares López <carlos.linares@uc3m.es>
#

"""
Configuration file
"""

# constants
# -----------------------------------------------------------------------------

# gnuplot

TEMPLATE_GNUPLOT = """#!/usr/bin/gnuplot
# -*- coding: utf-8 -*-
# File automatically created by libksearch/plotter
set grid
set xlabel "${xlabel}"
set ylabel "${ylabel}"

set title "${title}"

set terminal png enhanced font "Ariel,10"
set output '${output}'

# add the following line if the xlabels are strings
# set xtics rotate by -60
set style data ${style}

# remove the clause 'using 1:2:xtic(1)' if xlabels overlap and they are not
# strings
plot ${title_series}  
${data}
"""



# pgfplots

# constants
PGFPLOT_MARKERS = [
    '*',
    'x',
    '+',
    '-',
    'square',
    'square*',
    'triangle',
    'triangle*',
    'diamond',
    'diamond*',
    'halfdiamond*',
    'halfsquare*',
    'halfsquare right*',
    'halfsquare left*',
    'pentagon',
    'pentagon*',
    'star',
    'asterisk',
    '10-pointed star',
    'oplus',
    'oplus*',
    'otimes',
    'otimes*',
    'Mercedes star',
    'Mercedes star flipped',
    'halfcircle halfcircle*',
    'heart',
]

# colors

# the full list of svgnames colors is given next
PGFPLOT_COLORS = [
    'AliceBlue',
    'AntiqueWhite',
    'Aqua',
    'Aquamarine',
    'Azure',
    'Beige',
    'Bisque',
    'Black',
    'BlanchedAlmond',
    'Blue',
    'BlueViolet',
    'Brown',
    'BurlyWood',
    'CadetBlue',
    'Chartreuse',
    'Chocolate',
    'Coral',
    'CornflowerBlue',
    'Cornsilk',
    'Crimson',
    'Cyan',
    'DarkBlue',
    'DarkCyan',
    'DarkGoldenrod',
    'DarkGray',
    'DarkGreen',
    'DarkGrey',
    'DarkKhaki',
    'DarkMagenta',
    'DarkOliveGreen',
    'DarkOrange',
    'DarkOrchid',
    'DarkRed',
    'DarkSalmon',
    'DarkSeaGreen',
    'DarkSlateBlue',
    'DarkSlateGray',
    'DarkSlateGrey',
    'DarkTurquoise',
    'DarkViolet',
    'DeepPink',
    'DeepSkyBlue',
    'DimGray',
    'DimGrey',
    'DodgerBlue',
    'FireBrick',
    'FloralWhite',
    'ForestGreen',
    'Fuchsia',
    'Gainsboro',
    'GhostWhite',
    'Gold',
    'Goldenrod',
    'Gray',
    'Green',
    'GreenYellow',
    'Grey',
    'Honeydew',
    'HotPink',
    'IndianRed',
    'Indigo',
    'Ivory',
    'Khaki',
    'Lavender',
    'LavenderBlush',
    'LawnGreen',
    'LemonChiffon',
    'LightBlue',
    'LightCoral',
    'LightCyan',
    'LightGoldenrod',
    'LightGoldenrodYellow',
    'LightGray',
    'LightGreen',
    'LightGrey',
    'LightPink',
    'LightSalmon',
    'LightSeaGreen',
    'LightSkyBlue',
    'LightSlateBlue',
    'LightSlateGray',
    'LightSlateGrey',
    'LightSteelBlue',
    'LightYellow',
    'Lime',
    'LimeGreen',
    'Linen',
    'Magenta',
    'Maroon',
    'MediumAquamarine',
    'MediumBlue',
    'MediumOrchid',
    'MediumPurple',
    'MediumSeaGreen',
    'MediumSlateBlue',
    'MediumSpringGreen',
    'MediumTurquoise',
    'MediumVioletRed',
    'MidnightBlue',
    'MintCream',
    'MistyRose',
    'Moccasin',
    'NavajoWhite',
    'Navy',
    'NavyBlue',
    'OldLace',
    'Olive',
    'OliveDrab',
    'Orange',
    'OrangeRed',
    'Orchid',
    'PaleGoldenrod',
    'PaleGreen',
    'PaleTurquoise',
    'PaleVioletRed',
    'PapayaWhip',
    'PeachPuff',
    'Peru',
    'Pink',
    'Plum',
    'PowderBlue',
    'Purple',
    'Red',
    'RosyBrown',
    'RoyalBlue',
    'SaddleBrown',
    'Salmon',
    'SandyBrown',
    'SeaGreen',
    'Seashell',
    'Sienna',
    'Silver',
    'SkyBlue',
    'SlateBlue',
    'SlateGray',
    'SlateGrey',
    'Snow',
    'SpringGreen',
    'SteelBlue',
    'Tan',
    'Teal',
    'Thistle',
    'Tomato',
    'Turquoise',
    'Violet',
    'VioletRed',
    'Wheat',
    'White',
    'WhiteSmoke',
    'Yellow',
    'YellowGreen'
]

# instead of providing means for selecting colors and markers for specific
# series, a pre-selection is given next with regard to the algorithms typically
# used with plotter.
#
# In case an unknown algorithm is used, the color is selected randomly. Also,
# the creation of groups takes care of the case of the same serie being shown
# with different colors and/or markers and making the same selection for the
# same series in the same group
PGFPLOT_MARKERS_SELECTION = {
    r"^SB$": 'x',
    r"^SB\*$": 'x',
    r"^bBELA0.*$": 'oplus',
    r"^sBELA0.*$": 'halfdiamond',
    r"^bBELA\$.*$": 'oplus*',
    r"^sBELA\$.*$": 'diamond*',
}

PGFPLOT_COLORS_SELECTION = {
    r"^Yen$": 'Violet',
    r"^Feng$": 'OliveDrab',
    r"^SB$": 'SeaGreen',
    r"^SB\*$": 'SeaGreen',
    r"^bBELA\$_0.*$": 'Peru',
    r"^sBELA\$_0.*$": 'NavyBlue',
    r"^bBELA\$\^.*$": 'Teal',
    r"^sBELA\$\^.*$": 'FireBrick',
}


# templates
TEMPLATE_PGFPLOT_PREAMBLE = r"""%% ${filename}
%%
%% Started on ${datetime}
%% Author ${author}
%%
%% File automatically created by libksearch/plotter

%% The contents of this file can be embedded into a figure/figure* environment.
%% For it to be correctly processed by pdflatex, add the following to the
%% preamble of your .tex file
%%
%% \usepackage{pgfplots}
%% \pgfplotsset{compat=1.18}
%% \usepgflibrary{plotmarks}
"""

TEMPLATE_PGFPLOT_ENVIRONMENT = r"""    \begin{tikzpicture}
      \begin{axis}[
        width=\linewidth,
        % height=0.66\linewidth,
        xlabel={${xtitle}},
        ylabel={${ytitle}},
        ymode=log,
        grid=both,
        legend style={at={(0.5,-0.25)}, anchor=north, legend columns=${nblegends}},
        title={${title}},
      ]
        % Data
${data}
    \end{axis}
    \end{tikzpicture}
"""

TEMPLATE_PGFPLOT_SERIE = r"""      \addplot+[mark=${mark}, mark size=2.0pt, thick, color=${color}, mark options={fill=${markercolor}}] coordinates {
${serie}      };
      \addlegendentry{${legend}}
"""

TEMPLATE_PGFGROUP_SUBFIGURE = r"""  \begin{subfigure}{${width}\textwidth}
${plot}
    \caption{${caption}}
  \end{subfigure}
"""

TEMPLATE_PGFGROUP_ENVIRONMENT = r"""    \begin{tikzpicture}
      \begin{axis}[
        width=\linewidth,
        % height=0.66\linewidth,
        xlabel={${xtitle}},
        ylabel={${ytitle}},
        ymode=log,
        grid=both,
        legend to name=mysharedlegend,
        legend columns=${nblegends},
        legend style={at={(0.5,-0.2)}, anchor=north},
      ]
        % Data
${data}
      \end{axis}
    \end{tikzpicture}
"""

TEMPLATE_PGFGROUP_COORDINATES = r"""      \addplot+[mark=${mark}, mark size=2.0pt, thick, color=${color}, mark options={fill=${markercolor}}] coordinates {
${coordinates}
      };
      \addlegendentry{${legend}}
"""

TEMPLATE_PGFGROUP_FIGURE = r"""\begin{figure*}
  \centering
${body}
  \vspace{1em}
  \centering
  \begin{minipage}{\textwidth}
    \centering
    \ref{mysharedlegend}
  \end{minipage}
  \caption{${caption}}
\end{figure*}
"""

# Local Variables:
# mode:python
# fill-column:80
# End:
