#!/usr/bin/env sh

# 9th DIMACS Implementation Challenge: Shortest Paths
# -----------------------------------------------------------------------------

# Creating subdirectories
# -----------------------------------------------------------------------------
function create_subdirectories() {
        for VARIANT in "unit" "dimacs"
        do
                for MAP in "BAY" "CAL" "COL" "E" "FLA" "LKS" "NE" "NW" "NY"
                do
                        mkdir -p results/roadmap/${VARIANT}/${MAP}
                done
        done
}


# Clearing images
# -----------------------------------------------------------------------------
function clear_images() {
        for VARIANT in "unit" "dimacs"
        do
                for MAP in "BAY" "CAL" "COL" "E" "FLA" "LKS" "NE" "NW" "NY"
                do
                        rm -f results/roadmap/${VARIANT}/${MAP}/*.png
                        rm -f results/roadmap/${VARIANT}/${MAP}/*.gnuplot
                done
        done
}

# Clearing tables
# -----------------------------------------------------------------------------
function clear_tables() {
    for VARIANT in "unit" "dimacs"
    do
                for MAP in "BAY" "CAL" "COL" "E" "FLA" "LKS" "NE" "NW" "NY"
                do
                        rm -f results/roadmap/${VARIANT}/${MAP}/*.tex
                done
    done
}


# Creating images
# -----------------------------------------------------------------------------
# Brute-force variants
function create_brute_force_images() {

        # In the unit domain all search algorithms are brute-force variants
        VARIANT="unit"
        for MAP in "BAY" "CAL" "COL" "E" "FLA" "LKS" "NE" "NW" "NY"
        do
                ./plotter.py ky --file data/roadmap/${VARIANT}/${MAP}/USA-road-d.*.csv \
                        --y runtime \
                        --series "BELA0:re.match('BELA0', solver)" "K0:re.match('K0', solver)" "mDijkstra:re.match('mDijkstra', solver)" \
                        --title "Runtime (seconds) - ${MAP} Roadmap ${VARIANT}" \
                        --output results/roadmap/${VARIANT}/${MAP}/USA-road-d.${MAP}.brute-force.runtime.gnuplot \
                        --png USA-road-d.${MAP}.brute-force.runtime.png
                gnuplot results/roadmap/${VARIANT}/${MAP}/USA-road-d.${MAP}.brute-force.runtime.gnuplot
                mv USA-road-d.${MAP}.brute-force.runtime.png results/roadmap/${VARIANT}/${MAP}/

                ./plotter.py ky --file data/roadmap/${VARIANT}/${MAP}/USA-road-d.*.csv \
                        --y mem \
                        --series "BELA0:re.match('BELA0', solver)" "K0:re.match('K0', solver)" "mDijkstra:re.match('mDijkstra', solver)" \
                        --title "Memory (Mbytes) - ${MAP} Roadmap ${VARIANT}" \
                        --output results/roadmap/${VARIANT}/${MAP}/USA-road-d.${MAP}.brute-force.mem.gnuplot \
                        --png USA-road-d.${MAP}.brute-force.mem.png
                gnuplot results/roadmap/${VARIANT}/${MAP}/USA-road-d.${MAP}.brute-force.mem.gnuplot
                mv USA-road-d.${MAP}.brute-force.mem.png results/roadmap/${VARIANT}/${MAP}/

                ./plotter.py ky --file data/roadmap/${VARIANT}/${MAP}/USA-road-d.*.csv \
                        --y expansions \
                        --series "BELA0:re.match('BELA0', solver)" "K0:re.match('K0', solver)" "mDijkstra:re.match('mDijkstra', solver)" \
                        --title "#Expansions - ${MAP} Roadmap ${VARIANT}" \
                        --output results/roadmap/${VARIANT}/${MAP}/USA-road-d.${MAP}.brute-force.expansions.gnuplot \
                        --png USA-road-d.${MAP}.brute-force.expansions.png
                gnuplot results/roadmap/${VARIANT}/${MAP}/USA-road-d.${MAP}.brute-force.expansions.gnuplot
                mv USA-road-d.${MAP}.brute-force.expansions.png results/roadmap/${VARIANT}/${MAP}/

                ./plotter.py ky --file data/roadmap/${VARIANT}/${MAP}/USA-road-d.*.csv \
                        --y nbcentroids \
                        --series "BELA0:re.match('BELA0', solver)" "K0:re.match('K0', solver)" "mDijkstra:re.match('mDijkstra', solver)" \
                        --title "#Centroids - ${MAP} Roadmap ${VARIANT}" \
                        --output results/roadmap/${VARIANT}/${MAP}/USA-road-d.${MAP}.brute-force.nbcentroids.gnuplot \
                        --png USA-road-d.${MAP}.brute-force.nbcentroids.png
                gnuplot results/roadmap/${VARIANT}/${MAP}/USA-road-d.${MAP}.brute-force.nbcentroids.gnuplot
                mv USA-road-d.${MAP}.brute-force.nbcentroids.png results/roadmap/${VARIANT}/${MAP}/
        done

        # Brute-force variants are also applied to the dimacs variant
        VARIANT="dimacs"
        for MAP in "BAY" "CAL" "COL" "CTR" "E" "FLA" "LKS" "NE" "NW" "NY" "W" "USA"
        do
                ./plotter.py ky --file data/roadmap/${VARIANT}/${MAP}/USA-road-d.*.csv \
                        --y runtime \
                        --series "BELA0:re.match('BELA0', solver)" "K0:re.match('K0', solver)" "mDijkstra:re.match('mDijkstra', solver)" \
                        --title "Runtime (seconds) - ${MAP} Roadmap ${VARIANT}" \
                        --output results/roadmap/${VARIANT}/${MAP}/USA-road-d.${MAP}.brute-force.runtime.gnuplot \
                        --png USA-road-d.${MAP}.brute-force.runtime.png
                gnuplot results/roadmap/${VARIANT}/${MAP}/USA-road-d.${MAP}.brute-force.runtime.gnuplot
                mv USA-road-d.${MAP}.brute-force.runtime.png results/roadmap/${VARIANT}/${MAP}/

                ./plotter.py ky --file data/roadmap/${VARIANT}/${MAP}/USA-road-d.*.csv \
                        --y mem \
                        --series "BELA0:re.match('BELA0', solver)" "K0:re.match('K0', solver)" "mDijkstra:re.match('mDijkstra', solver)" \
                        --title "Memory (Mbytes) - ${MAP} Roadmap ${VARIANT}" \
                        --output results/roadmap/${VARIANT}/${MAP}/USA-road-d.${MAP}.brute-force.mem.gnuplot \
                        --png USA-road-d.${MAP}.brute-force.mem.png
                gnuplot results/roadmap/${VARIANT}/${MAP}/USA-road-d.${MAP}.brute-force.mem.gnuplot
                mv USA-road-d.${MAP}.brute-force.mem.png results/roadmap/${VARIANT}/${MAP}/

                ./plotter.py ky --file data/roadmap/${VARIANT}/${MAP}/USA-road-d.*.csv \
                        --y expansions \
                        --series "BELA0:re.match('BELA0', solver)" "K0:re.match('K0', solver)" "mDijkstra:re.match('mDijkstra', solver)" \
                        --title "#Expansions - ${MAP} Roadmap ${VARIANT}" \
                        --output results/roadmap/${VARIANT}/${MAP}/USA-road-d.${MAP}.brute-force.expansions.gnuplot \
                        --png USA-road-d.${MAP}.brute-force.expansions.png
                gnuplot results/roadmap/${VARIANT}/${MAP}/USA-road-d.${MAP}.brute-force.expansions.gnuplot
                mv USA-road-d.${MAP}.brute-force.expansions.png results/roadmap/${VARIANT}/${MAP}/

                ./plotter.py ky --file data/roadmap/${VARIANT}/${MAP}/USA-road-d.*.csv \
                        --y nbcentroids \
                        --series "BELA0:re.match('BELA0', solver)" "K0:re.match('K0', solver)" "mDijkstra:re.match('mDijkstra', solver)" \
                        --title "#Centroids - ${MAP} Roadmap ${VARIANT}" \
                        --output results/roadmap/${VARIANT}/${MAP}/USA-road-d.${MAP}.brute-force.nbcentroids.gnuplot \
                        --png USA-road-d.${MAP}.brute-force.nbcentroids.png
                gnuplot results/roadmap/${VARIANT}/${MAP}/USA-road-d.${MAP}.brute-force.nbcentroids.gnuplot
                mv USA-road-d.${MAP}.brute-force.nbcentroids.png results/roadmap/${VARIANT}/${MAP}/
        done
}

# Heuristic variants
function create_heuristic_images() {
        # Heuristic variants are only applied to the dimacs variant
        VARIANT="dimacs"
        for MAP in "BAY" "CAL" "COL" "CTR" "E" "FLA" "LKS" "NE" "NW" "NY" "W" "USA"
        do
                ./plotter.py ky --file data/roadmap/${VARIANT}/${MAP}/USA-road-d.*.csv \
                        --y runtime \
                        --series "BELA*:re.match('BELA\*', solver)" "K*:re.match('K\*', solver)" "mA*:re.match('mA\*', solver)" \
                        --title "Runtime (seconds) - ${MAP} Roadmap ${VARIANT}" \
                        --output results/roadmap/${VARIANT}/${MAP}/USA-road-d.${MAP}.heuristic.runtime.gnuplot \
                        --png USA-road-d.${MAP}.heuristic.runtime.png
                gnuplot results/roadmap/${VARIANT}/${MAP}/USA-road-d.${MAP}.heuristic.runtime.gnuplot
                mv USA-road-d.${MAP}.heuristic.runtime.png results/roadmap/${VARIANT}/${MAP}/

                ./plotter.py ky --file data/roadmap/${VARIANT}/${MAP}/USA-road-d.*.csv \
                        --y mem \
                        --series "BELA*:re.match('BELA\*', solver)" "K*:re.match('K\*', solver)" "mA*:re.match('mA\*', solver)" \
                        --title "Memory (Mbytes) - ${MAP} Roadmap ${VARIANT}" \
                        --output results/roadmap/${VARIANT}/${MAP}/USA-road-d.${MAP}.heuristic.mem.gnuplot \
                        --png USA-road-d.${MAP}.heuristic.mem.png
                gnuplot results/roadmap/${VARIANT}/${MAP}/USA-road-d.${MAP}.heuristic.mem.gnuplot
                mv USA-road-d.${MAP}.heuristic.mem.png results/roadmap/${VARIANT}/${MAP}/

                ./plotter.py ky --file data/roadmap/${VARIANT}/${MAP}/USA-road-d.*.csv \
                        --y expansions \
                        --series "BELA*:re.match('BELA\*', solver)" "K*:re.match('K\*', solver)" "mA*:re.match('mA\*', solver)" \
                        --title "#Expansions - ${MAP} Roadmap ${VARIANT}" \
                        --output results/roadmap/${VARIANT}/${MAP}/USA-road-d.${MAP}.heuristic.expansions.gnuplot \
                        --png USA-road-d.${MAP}.heuristic.expansions.png
                gnuplot results/roadmap/${VARIANT}/${MAP}/USA-road-d.${MAP}.heuristic.expansions.gnuplot
                mv USA-road-d.${MAP}.heuristic.expansions.png results/roadmap/${VARIANT}/${MAP}/

                ./plotter.py ky --file data/roadmap/${VARIANT}/${MAP}/USA-road-d.*.csv \
                        --y nbcentroids \
                        --series "BELA*:re.match('BELA\*', solver)" "K*:re.match('K\*', solver)" "mA*:re.match('mA\*', solver)" \
                        --title "#Centroids - ${MAP} Roadmap ${VARIANT}" \
                        --output results/roadmap/${VARIANT}/${MAP}/USA-road-d.${MAP}.heuristic.nbcentroids.gnuplot \
                        --png USA-road-d.${MAP}.heuristic.nbcentroids.png
                gnuplot results/roadmap/${VARIANT}/${MAP}/USA-road-d.${MAP}.heuristic.nbcentroids.gnuplot
                mv USA-road-d.${MAP}.heuristic.nbcentroids.png results/roadmap/${VARIANT}/${MAP}/
        done
}

# Both brute-force variants and heuristic variants but mDijkstra and mA*
function create_mixed_images() {
        # Heuristic variants are only applied to the dimacs variant
        VARIANT="dimacs"
        for MAP in "BAY" "CAL" "COL" "CTR" "E" "FLA" "LKS" "NE" "NW" "NY" "W" "USA"
        do
                ./plotter.py ky --file data/roadmap/${VARIANT}/${MAP}/USA-road-d.*.belA*csv \
                                       data/roadmap/${VARIANT}/${MAP}/USA-road-d.*.K*csv \
                        --y runtime \
                        --series "BELA0:re.match('BELA0', solver)" "K0:re.match('K0', solver)" "BELA*:re.match('BELA\*', solver)" "K*:re.match('K\*', solver)" \
                        --title "Runtime (seconds) - ${MAP} Roadmap ${VARIANT}" \
                        --output results/roadmap/${VARIANT}/${MAP}/USA-road-d.${MAP}.mixed.runtime.gnuplot \
                        --png USA-road-d.${MAP}.mixed.runtime.png
                gnuplot results/roadmap/${VARIANT}/${MAP}/USA-road-d.${MAP}.mixed.runtime.gnuplot
                mv USA-road-d.${MAP}.mixed.runtime.png results/roadmap/${VARIANT}/${MAP}/

                ./plotter.py ky --file data/roadmap/${VARIANT}/${MAP}/USA-road-d.*.belA*csv \
                                       data/roadmap/${VARIANT}/${MAP}/USA-road-d.*.K*csv \
                        --y mem \
                        --series "BELA0:re.match('BELA0', solver)" "K0:re.match('K0', solver)" "BELA*:re.match('BELA\*', solver)" "K*:re.match('K\*', solver)" \
                        --title "Memory (Mbytes) - ${MAP} Roadmap ${VARIANT}" \
                        --output results/roadmap/${VARIANT}/${MAP}/USA-road-d.${MAP}.mixed.mem.gnuplot \
                        --png USA-road-d.${MAP}.mixed.mem.png
                gnuplot results/roadmap/${VARIANT}/${MAP}/USA-road-d.${MAP}.mixed.mem.gnuplot
                mv USA-road-d.${MAP}.mixed.mem.png results/roadmap/${VARIANT}/${MAP}/

                ./plotter.py ky --file data/roadmap/${VARIANT}/${MAP}/USA-road-d.*.belA*csv \
                                       data/roadmap/${VARIANT}/${MAP}/USA-road-d.*.K*csv \
                        --y expansions \
                        --series "BELA0:re.match('BELA0', solver)" "K0:re.match('K0', solver)" "BELA*:re.match('BELA\*', solver)" "K*:re.match('K\*', solver)" \
                        --title "#Expansions - ${MAP} Roadmap ${VARIANT}" \
                        --output results/roadmap/${VARIANT}/${MAP}/USA-road-d.${MAP}.mixed.expansions.gnuplot \
                        --png USA-road-d.${MAP}.mixed.expansions.png
                gnuplot results/roadmap/${VARIANT}/${MAP}/USA-road-d.${MAP}.mixed.expansions.gnuplot
                mv USA-road-d.${MAP}.mixed.expansions.png results/roadmap/${VARIANT}/${MAP}/

                ./plotter.py ky --file data/roadmap/${VARIANT}/${MAP}/USA-road-d.*.belA*csv \
                                       data/roadmap/${VARIANT}/${MAP}/USA-road-d.*.K*csv \
                        --y nbcentroids \
                        --series "BELA0:re.match('BELA0', solver)" "K0:re.match('K0', solver)" "BELA*:re.match('BELA\*', solver)" "K*:re.match('K\*', solver)" \
                        --title "#Centroids - ${MAP} Roadmap ${VARIANT}" \
                        --output results/roadmap/${VARIANT}/${MAP}/USA-road-d.${MAP}.mixed.nbcentroids.gnuplot \
                        --png USA-road-d.${MAP}.mixed.nbcentroids.png
                gnuplot results/roadmap/${VARIANT}/${MAP}/USA-road-d.${MAP}.mixed.nbcentroids.gnuplot
                mv USA-road-d.${MAP}.mixed.nbcentroids.png results/roadmap/${VARIANT}/${MAP}/
        done
}

# Creating tables
# -----------------------------------------------------------------------------
# Brute-force variants
function create_brute_force_tables() {

        # In the unit domain all search algorithms are brute-force variants
        VARIANT="unit"
        for MAP in "BAY" "CAL" "COL" "E" "FLA" "LKS" "NE" "NW" "NY"
        do
                ./plotter.py ky --file data/roadmap/${VARIANT}/${MAP}/USA-road-d.*.csv \
                        --y runtime \
                        --series "BELA\$_0\$:re.match('BELA0', solver)" "K\$_0\$:re.match('K0', solver)" "mDijkstra:re.match('mDijkstra', solver)" \
                        --title "Runtime (seconds) - ${MAP} Roadmap ${VARIANT}" \
                        --k "1;10;50;100;500;1000;5000;10000" \
                        --output results/roadmap/${VARIANT}/${MAP}/USA-road-d.${MAP}.brute-force.runtime \
                        --table

                ./plotter.py ky --file data/roadmap/${VARIANT}/${MAP}/USA-road-d.*.csv \
                        --y mem \
                        --series "BELA\$_0\$:re.match('BELA0', solver)" "K\$_0\$:re.match('K0', solver)" "mDijkstra:re.match('mDijkstra', solver)" \
                        --title "Memory (Mbytes) - ${MAP} Roadmap ${VARIANT}" \
                        --k "1;10;50;100;500;1000;5000;10000" \
                        --output results/roadmap/${VARIANT}/${MAP}/USA-road-d.${MAP}.brute-force.mem \
                        --table

                ./plotter.py ky --file data/roadmap/${VARIANT}/${MAP}/USA-road-d.*.csv \
                        --y expansions \
                        --series "BELA\$_0\$:re.match('BELA0', solver)" "K\$_0\$:re.match('K0', solver)" "mDijkstra:re.match('mDijkstra', solver)" \
                        --title "#Expansions - ${MAP} Roadmap ${VARIANT}" \
                        --k "1;10;50;100;500;1000;5000;10000" \
                        --output results/roadmap/${VARIANT}/${MAP}/USA-road-d.${MAP}.brute-force.expansions \
                        --table

                ./plotter.py ky --file data/roadmap/${VARIANT}/${MAP}/USA-road-d.*.csv \
                        --y nbcentroids \
                        --series "BELA\$_0\$:re.match('BELA0', solver)" "K\$_0\$:re.match('K0', solver)" "mDijkstra:re.match('mDijkstra', solver)" \
                        --title "#Centroids - ${MAP} Roadmap ${VARIANT}" \
                        --k "1;10;50;100;500;1000;5000;10000" \
                        --output results/roadmap/${VARIANT}/${MAP}/USA-road-d.${MAP}.brute-force.nbcentroids \
                        --table
        done

        # Brute-force variants are also applied to the dimacs variant
        VARIANT="dimacs"
        for MAP in "BAY" "CAL" "COL" "CTR" "E" "FLA" "LKS" "NE" "NW" "NY" "W" "USA"
        do
                ./plotter.py ky --file data/roadmap/${VARIANT}/${MAP}/USA-road-d.*.csv \
                        --y runtime \
                        --series "BELA\$_0\$:re.match('BELA0', solver)" "K\$_0\$:re.match('K0', solver)" "mDijkstra:re.match('mDijkstra', solver)" \
                        --title "Runtime (seconds) - ${MAP} Roadmap ${VARIANT}" \
                        --k "1;10;50;100;500;1000;5000;10000" \
                        --output results/roadmap/${VARIANT}/${MAP}/USA-road-d.${MAP}.brute-force.runtime \
                        --table

                ./plotter.py ky --file data/roadmap/${VARIANT}/${MAP}/USA-road-d.*.csv \
                        --y mem \
                        --series "BELA\$_0\$:re.match('BELA0', solver)" "K\$_0\$:re.match('K0', solver)" "mDijkstra:re.match('mDijkstra', solver)" \
                        --title "Memory (Mbytes) - ${MAP} Roadmap ${VARIANT}" \
                        --k "1;10;50;100;500;1000;5000;10000" \
                        --output results/roadmap/${VARIANT}/${MAP}/USA-road-d.${MAP}.brute-force.mem \
                        --table

                ./plotter.py ky --file data/roadmap/${VARIANT}/${MAP}/USA-road-d.*.csv \
                        --y expansions \
                        --series "BELA\$_0\$:re.match('BELA0', solver)" "K\$_0\$:re.match('K0', solver)" "mDijkstra:re.match('mDijkstra', solver)" \
                        --title "#Expansions - ${MAP} Roadmap ${VARIANT}" \
                        --k "1;10;50;100;500;1000;5000;10000" \
                        --output results/roadmap/${VARIANT}/${MAP}/USA-road-d.${MAP}.brute-force.expansions \
                        --table

                ./plotter.py ky --file data/roadmap/${VARIANT}/${MAP}/USA-road-d.*.csv \
                        --y nbcentroids \
                        --series "BELA\$_0\$:re.match('BELA0', solver)" "K\$_0\$:re.match('K0', solver)" "mDijkstra:re.match('mDijkstra', solver)" \
                        --title "#Centroids - ${MAP} Roadmap ${VARIANT}" \
                        --k "1;10;50;100;500;1000;5000;10000" \
                        --output results/roadmap/${VARIANT}/${MAP}/USA-road-d.${MAP}.brute-force.nbcentroids \
                        --table
        done
}

# Heuristic variants
function create_heuristic_tables() {
        # Heuristic variants are only applied to the dimacs variant
        VARIANT="dimacs"
        for MAP in "BAY" "CAL" "COL" "CTR" "E" "FLA" "LKS" "NE" "NW" "NY" "W" "USA"
        do
                ./plotter.py ky --file data/roadmap/${VARIANT}/${MAP}/USA-road-d.*.csv \
                        --y runtime \
                        --series "BELA\$^*\$:re.match('BELA\*', solver)" "K\$^*\$:re.match('K\*', solver)" "mA\$^*\$:re.match('mA\*', solver)" \
                        --title "Runtime (seconds) - ${MAP} Roadmap ${VARIANT}" \
                        --k "1;10;50;100;500;1000;5000;10000" \
                        --output results/roadmap/${VARIANT}/${MAP}/USA-road-d.${MAP}.heuristic.runtime \
                        --table

                ./plotter.py ky --file data/roadmap/${VARIANT}/${MAP}/USA-road-d.*.csv \
                        --y mem \
                        --series "BELA\$^*\$:re.match('BELA\*', solver)" "K\$^*\$:re.match('K\*', solver)" "mA\$^*\$:re.match('mA\*', solver)" \
                        --title "Memory (Mbytes) - ${MAP} Roadmap ${VARIANT}" \
                        --k "1;10;50;100;500;1000;5000;10000" \
                        --output results/roadmap/${VARIANT}/${MAP}/USA-road-d.${MAP}.heuristic.mem \
                        --table

                ./plotter.py ky --file data/roadmap/${VARIANT}/${MAP}/USA-road-d.*.csv \
                        --y expansions \
                        --series "BELA\$^*\$:re.match('BELA\*', solver)" "K\$^*\$:re.match('K\*', solver)" "mA\$^*\$:re.match('mA\*', solver)" \
                        --title "#Expansions - ${MAP} Roadmap ${VARIANT}" \
                        --k "1;10;50;100;500;1000;5000;10000" \
                        --output results/roadmap/${VARIANT}/${MAP}/USA-road-d.${MAP}.heuristic.expansions \
                        --table

                ./plotter.py ky --file data/roadmap/${VARIANT}/${MAP}/USA-road-d.*.csv \
                        --y nbcentroids \
                        --series "BELA\$^*\$:re.match('BELA\*', solver)" "K\$^*\$:re.match('K\*', solver)" "mA\$^*\$:re.match('mA\*', solver)" \
                        --title "#Centroids - ${MAP} Roadmap ${VARIANT}" \
                        --k "1;10;50;100;500;1000;5000;10000" \
                        --output results/roadmap/${VARIANT}/${MAP}/USA-road-d.${MAP}.heuristic.nbcentroids \
                        --table
        done
}

# Both brute-force variants and heuristic variants but mDijkstra and mA*
function create_mixed_tables() {
        # Heuristic variants are only applied to the dimacs variant
        VARIANT="dimacs"
        for MAP in "BAY" "CAL" "COL" "CTR" "E" "FLA" "LKS" "NE" "NW" "NY" "W" "USA"
        do
                ./plotter.py ky --file data/roadmap/${VARIANT}/${MAP}/USA-road-d.*.belA*csv \
                                       data/roadmap/${VARIANT}/${MAP}/USA-road-d.*.K*csv \
                        --y runtime \
                        --series "BELA\$_0\$:re.match('BELA0', solver)" "K\$_0\$:re.match('K0', solver)" "BELA\$^*\$:re.match('BELA\*', solver)" "K\$^*\$:re.match('K\*', solver)" \
                        --title "Runtime (seconds) - ${MAP} Roadmap ${VARIANT}" \
                        --k "1;10;50;100;500;1000;5000;10000" \
                        --output results/roadmap/${VARIANT}/${MAP}/USA-road-d.${MAP}.mixed.runtime \
                        --table

                ./plotter.py ky --file data/roadmap/${VARIANT}/${MAP}/USA-road-d.*.belA*csv \
                                       data/roadmap/${VARIANT}/${MAP}/USA-road-d.*.K*csv \
                        --y mem \
                        --series "BELA\$_0\$:re.match('BELA0', solver)" "K\$_0\$:re.match('K0', solver)" "BELA\$^*\$:re.match('BELA\*', solver)" "K\$^*\$:re.match('K\*', solver)" \
                        --title "Memory (Mbytes) - ${MAP} Roadmap ${VARIANT}" \
                        --k "1;10;50;100;500;1000;5000;10000" \
                        --output results/roadmap/${VARIANT}/${MAP}/USA-road-d.${MAP}.mixed.mem \
                        --table

                ./plotter.py ky --file data/roadmap/${VARIANT}/${MAP}/USA-road-d.*.belA*csv \
                                       data/roadmap/${VARIANT}/${MAP}/USA-road-d.*.K*csv \
                        --y expansions \
                        --series "BELA\$_0\$:re.match('BELA0', solver)" "K\$_0\$:re.match('K0', solver)" "BELA\$^*\$:re.match('BELA\*', solver)" "K\$^*\$:re.match('K\*', solver)" \
                        --title "#Expansions - ${MAP} Roadmap ${VARIANT}" \
                        --k "1;10;50;100;500;1000;5000;10000" \
                        --output results/roadmap/${VARIANT}/${MAP}/USA-road-d.${MAP}.mixed.expansions \
                        --table

                ./plotter.py ky --file data/roadmap/${VARIANT}/${MAP}/USA-road-d.*.belA*csv \
                                       data/roadmap/${VARIANT}/${MAP}/USA-road-d.*.K*csv \
                        --y nbcentroids \
                        --series "BELA\$_0\$:re.match('BELA0', solver)" "K\$_0\$:re.match('K0', solver)" "BELA\$^*\$:re.match('BELA\*', solver)" "K\$^*\$:re.match('K\*', solver)" \
                        --title "#Centroids - ${MAP} Roadmap ${VARIANT}" \
                        --k "1;10;50;100;500;1000;5000;10000" \
                        --output results/roadmap/${VARIANT}/${MAP}/USA-road-d.${MAP}.mixed.nbcentroids \
                        --table
        done
}

# Main body
while [[ $# -gt 0 ]]; do
        case $1 in
                -c|--create)
                        create_subdirectories
                        exit 0
                        ;;
                -x|--clear-images)
                        clear_images
                        exit 0
                        ;;
                -x|--clear-tables)
                        clear_tables
                        exit 0
                        ;;
                -i|--images)
                        create_brute_force_images
                        create_heuristic_images
                        create_mixed_images
                        exit 0
                        ;;
                -t|--tables)
                        create_brute_force_tables
                        create_heuristic_tables
                        create_mixed_tables
                        exit 0
                        ;;
                -h|--help)
                        echo " Use:"
                        echo "    -c|--create          to create the subdirectories under the results directory"
                        echo "    -x|--clear-images    to clear all png and gnuplot files under the results directory"
                        echo "    -x|--clear-tables    to clear all tex files under the results directory"
                        echo "    -i|--images          to create the png images under the results directory"
                        echo "    -t|--tables          to create the tex files with the data body of LaTeX tables"
                        exit 0
                        ;;
                -*|--*)
                        echo " Unknown option $1"
                        echo " Use -h|--help for help"
                        exit 1
                        ;;
        esac
done
