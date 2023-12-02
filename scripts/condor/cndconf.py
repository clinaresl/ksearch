#!/usr/bin/env python3
# -*- coding: utf-8 -*-
#
# cndconf.py
# Description:
# -----------------------------------------------------------------------------
#
# Started on <sáb 02-12-2023 19:10:41.971115483 (1701540641)>
# Carlos Linares López <carlos.linares@uc3m.es>
#

"""
Global definitions of filenames and its contents
"""

# globals
# -----------------------------------------------------------------------------

# the allowed domains are
VALID_DOMAINS = ['maps', 'n-pancake', 'n-puzzle', 'roadmap']

# the allowed search algorithms are
VALID_ALGORITHMS = ['belA0', 'K0', 'mDijkstra', 'belA*', 'K*', 'mA*']

# the executables of each domain are defined next:
EXECUTABLE = {
    "maps": "map",
    "n-pancake": "npancake",
    "n-puzzle": "npuzzle",
    "roadmap": "roadmap"
}

# the arguments of every executable are given next
ARGS = {
    "maps": "--map domains/$domain/benchmark/$testfile.map --solver '$algorithm' --file domains/$domain/benchmark/$testfile.test --variant '$variant' --k '$kspec' --summary --no-doctor --csv $filename.csv",
    "n-pancake": "--solver '$algorithm' --file domains/$domain/benchmark/$testfile.test --variant '$variant' --k '$kspec' --summary --no-doctor --csv $filename.csv",
    "n-puzzle": "--solver '$algorithm' --file domains/$domain/benchmark/$testfile.test --variant '$variant' --k '$kspec' --summary --no-doctor --csv $filename.csv",
    "roadmap": "--graph domains/$domain/benchmark/$testfile.map --solver '$algorithm' --file domains/$domain/benchmark/$testfile.test --variant '$variant' --k '$kspec' --summary --no-doctor --csv $filename.csv",
}


# filenames are indexed by the domain name
FILENAME = {
    "maps": "$map.$solver.$variant.$mink-$maxk",
    "n-pancake": "$size.$solver.$variant.$mink-$maxk",
    "n-puzzle": "${nbtiles}puzzle.$solver.$variant.$mink-$maxk",
    "roadmap": "$map.$solver.$variant.$mink-$maxk"
}

# template for the condor job submission files
CONDOR_FILE = """#
# Libksearch experiments
#
# Domain: $domain
# Variant: $variant
# Solver: $algorithm
#
nice_user = True
notify_user = $user
notification = Always
getenv = True

requirements = MyMachine=="32GB"

Executable     = $filename.sh
Universe       = vanilla

output = $filename.$$(Cluster).out
log = $filename.$$(Cluster).log
error = $filename.$$(Cluster).err

should_transfer_files = yes
transfer_input_files = ../libksearch
transfer_output_files = $filename.csv

queue
"""

# template used for generating the shell script to execute in the backend
SHELL_FILE = """#!/usr/bin/sh
cd libksearch

# compile the library, remember to disable the unit tests as GTest might not be
# available in the backend node
cmake . -DCMAKE_BUILD_TYPE=Release  -DDISABLE_TESTS=ON
make ksearch

# Compile the executable and run it writing the output in a csv file
make $executable
domains/$domain/$executable $args
"""

# Local Variables:
# mode:python
# fill-column:80
# End:
