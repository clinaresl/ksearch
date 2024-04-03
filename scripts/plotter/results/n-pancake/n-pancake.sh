#!/usr/bin/env sh

# N-Pancake
# -----------------------------------------------------------------------------

# Creating subdirectories
# -----------------------------------------------------------------------------
function create_subdirectories() {
        for SIZE in 10 20 30 40
        do
                mkdir -p results/n-pancake/unit/${SIZE}
        done

        # the heavy-cost domain has been tested only on the 10-Pancake
        mkdir -p results/n-pancake/heavy-cost/10
}


# Clearing images
# -----------------------------------------------------------------------------
function clear_images() {
        for VARIANT in "unit" "heavy-cost"
        do
                rm -f results/n-pancake/${VARIANT}/*/*.png
                rm -f results/n-pancake/${VARIANT}/*/*.gnuplot
        done
}

# Clearing tables
# -----------------------------------------------------------------------------
function clear_tables() {
    for VARIANT in "unit" "heavy-cost"
    do
        rm -f results/n-pancake/${VARIANT}/*/*.tex
    done
}

# Creating images
# -----------------------------------------------------------------------------
# Brute-force variants
function create_brute_force_images() {

        # brute-force variants are tried only in the 10-Pancake
        SIZE=10

        for VARIANT in "unit" "heavy-cost"
        do
                ./plotter.py ky --file data/n-pancake/${VARIANT}/${SIZE}/${SIZE}pancake.*${VARIANT}*csv \
                        --y runtime \
                        --series "BELA0:re.match('BELA0', solver)" "K0:re.match('K0', solver)" "mDijkstra:re.match('mDijkstra', solver)" \
                        --title "Runtime (seconds) - ${SIZE}-Pancake ${VARIANT}" \
                        --output results/n-pancake/${VARIANT}/${SIZE}/${SIZE}pancake.brute-force.runtime.gnuplot \
                        --png ${SIZE}pancake.brute-force.runtime.png
                gnuplot results/n-pancake/${VARIANT}/${SIZE}/${SIZE}pancake.brute-force.runtime.gnuplot
                mv ${SIZE}pancake.brute-force.runtime.png results/n-pancake/${VARIANT}/${SIZE}/${SIZE}pancake.brute-force.runtime.png

                ./plotter.py ky --file data/n-pancake/${VARIANT}/${SIZE}/${SIZE}pancake.* \
                        --y mem \
                        --series "BELA0:re.match('BELA0', solver)" "K0:re.match('K0', solver)" "mDijkstra:re.match('mDijkstra', solver)" \
                        --title "Memory (Mbytes) - ${SIZE}-Pancake ${VARIANT}" \
                        --output results/n-pancake/${VARIANT}/${SIZE}/${SIZE}pancake.brute-force.mem.gnuplot \
                        --png ${SIZE}pancake.brute-force.mem.png
                gnuplot results/n-pancake/${VARIANT}/${SIZE}/${SIZE}pancake.brute-force.mem.gnuplot
                mv ${SIZE}pancake.brute-force.mem.png results/n-pancake/${VARIANT}/${SIZE}/${SIZE}pancake.brute-force.mem.png

                ./plotter.py ky --file data/n-pancake/${VARIANT}/${SIZE}/${SIZE}pancake.* \
                        --y expansions \
                        --series "BELA0:re.match('BELA0', solver)" "K0:re.match('K0', solver)" "mDijkstra:re.match('mDijkstra', solver)" \
                        --title "#Expansions - ${SIZE}-Pancake ${VARIANT}" \
                        --output results/n-pancake/${VARIANT}/${SIZE}/${SIZE}pancake.brute-force.expansions.gnuplot \
                        --png ${SIZE}pancake.brute-force.expansions.png
                gnuplot results/n-pancake/${VARIANT}/${SIZE}/${SIZE}pancake.brute-force.expansions.gnuplot
                mv ${SIZE}pancake.brute-force.expansions.png results/n-pancake/${VARIANT}/${SIZE}/${SIZE}pancake.brute-force.expansions.png

                ./plotter.py ky --file data/n-pancake/${VARIANT}/${SIZE}/${SIZE}pancake.* \
                        --y nbcentroids \
                        --series "BELA0:re.match('BELA0', solver)" "K0:re.match('K0', solver)" "mDijkstra:re.match('mDijkstra', solver)" \
                        --title "#Centroids - ${SIZE}-Pancake ${VARIANT}" \
                        --output results/n-pancake/${VARIANT}/${SIZE}/${SIZE}pancake.brute-force.nbcentroids.gnuplot \
                        --png ${SIZE}pancake.brute-force.nbcentroids.png
                gnuplot results/n-pancake/${VARIANT}/${SIZE}/${SIZE}pancake.brute-force.nbcentroids.gnuplot
                mv ${SIZE}pancake.brute-force.nbcentroids.png results/n-pancake/${VARIANT}/${SIZE}/${SIZE}pancake.brute-force.nbcentroids.png
        done
}

# Heuristic variants
function create_heuristic_images() {

        # In the unit variant, heuristic algorithms are tested in the 20-, 30- and 40-Pancake
        for SIZE in 20 30 40
        do
                for VARIANT in "unit"
                do
                        ./plotter.py ky --file data/n-pancake/${VARIANT}/${SIZE}/${SIZE}pancake.* \
                                --y runtime \
                                --series "BELA*:re.match('BELA\*', solver)" "K*:re.match('K\*', solver)" "mA*:re.match('mA\*', solver)" \
                                --title "Runtime (seconds) - ${SIZE}-Pancake ${VARIANT}" \
                                --output results/n-pancake/${VARIANT}/${SIZE}/${SIZE}pancake.heuristic.runtime.gnuplot \
                                --png ${SIZE}pancake.heuristic.runtime.png
                        gnuplot results/n-pancake/${VARIANT}/${SIZE}/${SIZE}pancake.heuristic.runtime.gnuplot
                        mv ${SIZE}pancake.heuristic.runtime.png results/n-pancake/${VARIANT}/${SIZE}/${SIZE}pancake.heuristic.runtime.png

                        ./plotter.py ky --file data/n-pancake/${VARIANT}/${SIZE}/${SIZE}pancake.* \
                                --y mem \
                                --series "BELA*:re.match('BELA\*', solver)" "K*:re.match('K\*', solver)" "mA*:re.match('mA\*', solver)" \
                                --title "Memory (Mbytes) - ${SIZE}-Pancake ${VARIANT}" \
                                --output results/n-pancake/${VARIANT}/${SIZE}/${SIZE}pancake.heuristic.mem.gnuplot \
                                --png ${SIZE}pancake.heuristic.mem.png
                        gnuplot results/n-pancake/${VARIANT}/${SIZE}/${SIZE}pancake.heuristic.mem.gnuplot
                        mv ${SIZE}pancake.heuristic.mem.png results/n-pancake/${VARIANT}/${SIZE}/${SIZE}pancake.heuristic.mem.png

                        ./plotter.py ky --file data/n-pancake/${VARIANT}/${SIZE}/${SIZE}pancake.* \
                                --y expansions \
                                --series "BELA*:re.match('BELA\*', solver)" "K*:re.match('K\*', solver)" "mA*:re.match('mA\*', solver)" \
                                --title "#Expansions - ${SIZE}-Pancake ${VARIANT}" \
                                --output results/n-pancake/${VARIANT}/${SIZE}/${SIZE}pancake.heuristic.expansions.gnuplot \
                                --png ${SIZE}pancake.heuristic.expansions.png
                        gnuplot results/n-pancake/${VARIANT}/${SIZE}/${SIZE}pancake.heuristic.expansions.gnuplot
                        mv ${SIZE}pancake.heuristic.expansions.png results/n-pancake/${VARIANT}/${SIZE}/${SIZE}pancake.heuristic.expansions.png

                        ./plotter.py ky --file data/n-pancake/${VARIANT}/${SIZE}/${SIZE}pancake.* \
                                --y nbcentroids \
                                --series "BELA*:re.match('BELA\*', solver)" "K*:re.match('K\*', solver)" "mA*:re.match('mA\*', solver)" \
                                --title "#Centroids - ${SIZE}-Pancake ${VARIANT}" \
                                --output results/n-pancake/${VARIANT}/${SIZE}/${SIZE}pancake.heuristic.nbcentroids.gnuplot \
                                --png ${SIZE}pancake.heuristic.nbcentroids.png
                        gnuplot results/n-pancake/${VARIANT}/${SIZE}/${SIZE}pancake.heuristic.nbcentroids.gnuplot
                        mv ${SIZE}pancake.heuristic.nbcentroids.png results/n-pancake/${VARIANT}/${SIZE}/${SIZE}pancake.heuristic.nbcentroids.png
                done
        done

        # in the heavy-cost variant, heuristic algorithms are tested only in the 10-Pancake
        SIZE=10
        VARIANT="heavy-cost"
        ./plotter.py ky --file data/n-pancake/${VARIANT}/${SIZE}/${SIZE}pancake.* \
                --y runtime \
                --series "BELA*:re.match('BELA\*', solver)" "K*:re.match('K\*', solver)" "mA*:re.match('mA\*', solver)" \
                --title "Runtime (seconds) - ${SIZE}-Pancake ${VARIANT}" \
                --output results/n-pancake/${VARIANT}/${SIZE}/${SIZE}pancake.heuristic.runtime.gnuplot \
                --png ${SIZE}pancake.heuristic.runtime.png
        gnuplot results/n-pancake/${VARIANT}/${SIZE}/${SIZE}pancake.heuristic.runtime.gnuplot
        mv ${SIZE}pancake.heuristic.runtime.png results/n-pancake/${VARIANT}/${SIZE}/${SIZE}pancake.heuristic.runtime.png

        ./plotter.py ky --file data/n-pancake/${VARIANT}/${SIZE}/${SIZE}pancake.* \
                --y mem \
                --series "BELA*:re.match('BELA\*', solver)" "K*:re.match('K\*', solver)" "mA*:re.match('mA\*', solver)" \
                --title "Memory (Mbytes) - ${SIZE}-Pancake ${VARIANT}" \
                --output results/n-pancake/${VARIANT}/${SIZE}/${SIZE}pancake.heuristic.mem.gnuplot \
                --png ${SIZE}pancake.heuristic.mem.png
        gnuplot results/n-pancake/${VARIANT}/${SIZE}/${SIZE}pancake.heuristic.mem.gnuplot
        mv ${SIZE}pancake.heuristic.mem.png results/n-pancake/${VARIANT}/${SIZE}/${SIZE}pancake.heuristic.mem.png

        ./plotter.py ky --file data/n-pancake/${VARIANT}/${SIZE}/${SIZE}pancake.* \
                --y expansions \
                --series "BELA*:re.match('BELA\*', solver)" "K*:re.match('K\*', solver)" "mA*:re.match('mA\*', solver)" \
                --title "#Expansions - ${SIZE}-Pancake ${VARIANT}" \
                --output results/n-pancake/${VARIANT}/${SIZE}/${SIZE}pancake.heuristic.expansions.gnuplot \
                --png ${SIZE}pancake.heuristic.expansions.png
        gnuplot results/n-pancake/${VARIANT}/${SIZE}/${SIZE}pancake.heuristic.expansions.gnuplot
        mv ${SIZE}pancake.heuristic.expansions.png results/n-pancake/${VARIANT}/${SIZE}/${SIZE}pancake.heuristic.expansions.png

        ./plotter.py ky --file data/n-pancake/${VARIANT}/${SIZE}/${SIZE}pancake.* \
                --y nbcentroids \
                --series "BELA*:re.match('BELA\*', solver)" "K*:re.match('K\*', solver)" "mA*:re.match('mA\*', solver)" \
                --title "#Centroids - ${SIZE}-Pancake ${VARIANT}" \
                --output results/n-pancake/${VARIANT}/${SIZE}/${SIZE}pancake.heuristic.nbcentroids.gnuplot \
                --png ${SIZE}pancake.heuristic.nbcentroids.png
        gnuplot results/n-pancake/${VARIANT}/${SIZE}/${SIZE}pancake.heuristic.nbcentroids.gnuplot
        mv ${SIZE}pancake.heuristic.nbcentroids.png results/n-pancake/${VARIANT}/${SIZE}/${SIZE}pancake.heuristic.nbcentroids.png
}

# Creating tables
# -----------------------------------------------------------------------------
# Brute-force variants
function create_brute_force_tables() {

        # brute-force variants are tried only in the 10-Pancake
        SIZE=10

        for VARIANT in "unit" "heavy-cost"
        do
                ./plotter.py ky --file data/n-pancake/${VARIANT}/${SIZE}/${SIZE}pancake.*${VARIANT}*csv \
                        --y runtime \
                        --series "BELA\$_0\$:re.match('BELA0', solver)" "K\$_0\$:re.match('K0', solver)" "mDijkstra:re.match('mDijkstra', solver)" \
                        --title "Runtime (seconds) - ${SIZE}-Pancake ${VARIANT}" \
                        --k "1;2;3;4;5;10;40;50;100;500;900;1000;5000;10000" \
                        --output results/n-pancake/${VARIANT}/${SIZE}/${SIZE}pancake.brute-force.runtime \
                        --table

                ./plotter.py ky --file data/n-pancake/${VARIANT}/${SIZE}/${SIZE}pancake.* \
                        --y mem \
                        --series "BELA\$_0\$:re.match('BELA0', solver)" "K\$_0\$:re.match('K0', solver)" "mDijkstra:re.match('mDijkstra', solver)" \
                        --title "Memory (Mbytes) - ${SIZE}-Pancake ${VARIANT}" \
                        --k "1;2;3;4;5;10;40;50;100;500;900;1000;5000;10000" \
                        --output results/n-pancake/${VARIANT}/${SIZE}/${SIZE}pancake.brute-force.mem \
                        --table

                ./plotter.py ky --file data/n-pancake/${VARIANT}/${SIZE}/${SIZE}pancake.* \
                        --y expansions \
                        --series "BELA\$_0\$:re.match('BELA0', solver)" "K\$_0\$:re.match('K0', solver)" "mDijkstra:re.match('mDijkstra', solver)" \
                        --title "#Expansions - ${SIZE}-Pancake ${VARIANT}" \
                        --k "1;2;3;4;5;10;40;50;100;500;900;1000;5000;10000" \
                        --output results/n-pancake/${VARIANT}/${SIZE}/${SIZE}pancake.brute-force.expansions \
                        --table

                ./plotter.py ky --file data/n-pancake/${VARIANT}/${SIZE}/${SIZE}pancake.* \
                        --y nbcentroids \
                        --series "BELA\$_0\$:re.match('BELA0', solver)" "K\$_0\$:re.match('K0', solver)" "mDijkstra:re.match('mDijkstra', solver)" \
                        --title "#Centroids - ${SIZE}-Pancake ${VARIANT}" \
                        --k "1;2;3;4;5;10;40;50;100;500;900;1000;5000;10000" \
                        --output results/n-pancake/${VARIANT}/${SIZE}/${SIZE}pancake.brute-force.nbcentroids \
                        --table
        done
}

# Heuristic variants
function create_heuristic_tables() {

        # In the unit variant, heuristic algorithms are tested in the 20-, 30- and 40-Pancake
        for SIZE in 20 30 40
        do
                for VARIANT in "unit"
                do
                        ./plotter.py ky --file data/n-pancake/${VARIANT}/${SIZE}/${SIZE}pancake.* \
                                --y runtime \
                                --series "BELA\$^*\$:re.match('BELA\*', solver)" "K\$^*\$:re.match('K\*', solver)" "mA\$^*\$:re.match('mA\*', solver)" \
                                --title "Runtime (seconds) - ${SIZE}-Pancake ${VARIANT}" \
                                --k "1;2;3;4;5;10;40;50;100;500;900;1000;5000;10000" \
                                --output results/n-pancake/${VARIANT}/${SIZE}/${SIZE}pancake.heuristic.runtime \
                                --table

                        ./plotter.py ky --file data/n-pancake/${VARIANT}/${SIZE}/${SIZE}pancake.* \
                                --y mem \
                                --series "BELA\$^*\$:re.match('BELA\*', solver)" "K\$^*\$:re.match('K\*', solver)" "mA\$^*\$:re.match('mA\*', solver)" \
                                --title "Memory (Mbytes) - ${SIZE}-Pancake ${VARIANT}" \
                                --k "1;2;3;4;5;10;40;50;100;500;900;1000;5000;10000" \
                                --output results/n-pancake/${VARIANT}/${SIZE}/${SIZE}pancake.heuristic.mem \
                                --table

                        ./plotter.py ky --file data/n-pancake/${VARIANT}/${SIZE}/${SIZE}pancake.* \
                                --y expansions \
                                --series "BELA\$^*\$:re.match('BELA\*', solver)" "K\$^*\$:re.match('K\*', solver)" "mA\$^*\$:re.match('mA\*', solver)" \
                                --title "#Expansions - ${SIZE}-Pancake ${VARIANT}" \
                                --k "1;2;3;4;5;10;40;50;100;500;900;1000;5000;10000" \
                                --output results/n-pancake/${VARIANT}/${SIZE}/${SIZE}pancake.heuristic.expansions \
                                --table

                        ./plotter.py ky --file data/n-pancake/${VARIANT}/${SIZE}/${SIZE}pancake.* \
                                --y nbcentroids \
                                --series "BELA\$^*\$:re.match('BELA\*', solver)" "K\$^*\$:re.match('K\*', solver)" "mA\$^*\$:re.match('mA\*', solver)" \
                                --title "#Centroids - ${SIZE}-Pancake ${VARIANT}" \
                                --k "1;2;3;4;5;10;40;50;100;500;900;1000;5000;10000" \
                                --output results/n-pancake/${VARIANT}/${SIZE}/${SIZE}pancake.heuristic.nbcentroids \
                                --table
                done
        done

        # in the heavy-cost variant, heuristic algorithms are tested only in the 10-Pancake
        SIZE=10
        VARIANT="heavy-cost"
        ./plotter.py ky --file data/n-pancake/${VARIANT}/${SIZE}/${SIZE}pancake.* \
                --y runtime \
                --series "BELA\$^*\$:re.match('BELA\*', solver)" "K\$^*\$:re.match('K\*', solver)" "mA\$^*\$:re.match('mA\*', solver)" \
                --title "Runtime (seconds) - ${SIZE}-Pancake ${VARIANT}" \
                --k "1;2;3;4;5;10;40;50;100;500;900;1000;5000;10000" \
                --output results/n-pancake/${VARIANT}/${SIZE}/${SIZE}pancake.heuristic.runtime \
                --table

        ./plotter.py ky --file data/n-pancake/${VARIANT}/${SIZE}/${SIZE}pancake.* \
                --y mem \
                --series "BELA\$^*\$:re.match('BELA\*', solver)" "K\$^*\$:re.match('K\*', solver)" "mA\$^*\$:re.match('mA\*', solver)" \
                --title "Memory (Mbytes) - ${SIZE}-Pancake ${VARIANT}" \
                --k "1;2;3;4;5;10;40;50;100;500;900;1000;5000;10000" \
                --output results/n-pancake/${VARIANT}/${SIZE}/${SIZE}pancake.heuristic.mem \
                --table

        ./plotter.py ky --file data/n-pancake/${VARIANT}/${SIZE}/${SIZE}pancake.* \
                --y expansions \
                --series "BELA\$^*\$:re.match('BELA\*', solver)" "K\$^*\$:re.match('K\*', solver)" "mA\$^*\$:re.match('mA\*', solver)" \
                --title "#Expansions - ${SIZE}-Pancake ${VARIANT}" \
                --k "1;2;3;4;5;10;40;50;100;500;900;1000;5000;10000" \
                --output results/n-pancake/${VARIANT}/${SIZE}/${SIZE}pancake.heuristic.expansions \
                --table

        ./plotter.py ky --file data/n-pancake/${VARIANT}/${SIZE}/${SIZE}pancake.* \
                --y nbcentroids \
                --series "BELA\$^*\$:re.match('BELA\*', solver)" "K\$^*\$:re.match('K\*', solver)" "mA\$^*\$:re.match('mA\*', solver)" \
                --title "#Centroids - ${SIZE}-Pancake ${VARIANT}" \
                --k "1;2;3;4;5;10;40;50;100;500;900;1000;5000;10000" \
                --output results/n-pancake/${VARIANT}/${SIZE}/${SIZE}pancake.heuristic.nbcentroids \
                --table
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
                        exit 0
                        ;;
                -t|--tables)
                        create_brute_force_tables
                        create_heuristic_tables
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
