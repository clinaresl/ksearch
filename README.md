K optimal paths -- May, 2016


# Introduction #

This library implements a number of search algorithms to compute *K*
optimal paths. It provides both *brute-force* and *heuristic* search
algorithms.

All algorithms are implemented in the method `solve` of different
classes:

1. *m-Dijkstra* and *m-A* are implemented in the `msolver` and
`mhsolver` respectively. This algorithm expands each node up to *K*
times, guaranteeing that the best *K* solutions are found. To force
the algorithms to compute simple paths, the descendants of each node
are compared with all the nodes in the path that led to its parent.


# Install #

To compile the libksearch library you can use the following command:

    $ ./configure; make

The first command, `./configure`, will go through a number of tests
and, if it does not find any error, it will create the `Makefile` that
is executed with the second command.

To install the library just type:

    $ make install

By default, the library is installed in `/usr/local/lib` and the
header files are installed in
`/usr/local/include/libksearch-MAJOR.MINOR` (where *MAJOR* and *MINOR*
are substituted by the version numbers of this library) To change the
include and lib dirs use `--prefix` with `./configure`:

    $ ./configure --prefix=/home/myuser/local; make

It will automatically create a `Makefile` which is specifically
instructed to copy all the binaries and headers in
`/home/myuser/local`

To uninstall the binaries and headers of the libksearch library just
execute:

    $ make uninstall

Finally, be aware that it might be necessary to prepend both `make
install` and `make uninstall` with `sudo` in case you are installing
in su-protected directories such as `/usr/local`


# License #

libksearch is free software: you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the
Free Software Foundation, either version 3 of the License, or (at your
option) any later version.

libksearch is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
General Public License for more details.

You should have received a copy of the GNU General Public License
along with libksearch.  If not, see <http://www.gnu.org/licenses/>.


# Author #

Carlos Linares Lopez <carlos.linares@uc3m.es>

