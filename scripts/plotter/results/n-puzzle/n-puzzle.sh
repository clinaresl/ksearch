#!/usr/bin/env sh

# N-Puzzle
# -----------------------------------------------------------------------------

# Creating subdirectories
# -----------------------------------------------------------------------------
function create_subdirectories() {
        for SIZE in 8 15
        do
                mkdir -p results/n-puzzle/unit/${SIZE}
                mkdir -p results/n-puzzle/heavy-cost/${SIZE}
        done
}


# Clearing images
# -----------------------------------------------------------------------------
function clear_images() {
        for VARIANT in "unit" "heavy-cost"
        do
                rm -f results/n-puzzle/${VARIANT}/*/*.png
                rm -f results/n-puzzle/${VARIANT}/*/*.gnuplot
        done
}

# Creating images
# -----------------------------------------------------------------------------
# Brute-force variants
function create_brute_force_images() {

        # brute-force variants are tried only in the 8-Puzzle/unit variant
        SIZE=8
        VARIANT="unit"

        ./plotter.py ky --file data/n-puzzle/${VARIANT}/${SIZE}/${SIZE}puzzle.*${VARIANT}*csv \
            --y runtime \
            --series "BELA0:re.match('BELA0', solver)" "K0:re.match('K0', solver)" "mDijkstra:re.match('mDijkstra', solver)" \
            --title "Runtime (seconds) - ${SIZE}-Puzzle ${VARIANT}" \
            --output results/n-puzzle/${VARIANT}/${SIZE}/${SIZE}puzzle.brute-force.runtime.gnuplot \
            --png ${SIZE}puzzle.brute-force.runtime.png
        gnuplot results/n-puzzle/${VARIANT}/${SIZE}/${SIZE}puzzle.brute-force.runtime.gnuplot
        mv ${SIZE}puzzle.brute-force.runtime.png results/n-puzzle/${VARIANT}/${SIZE}/${SIZE}puzzle.brute-force.runtime.png

        ./plotter.py ky --file data/n-puzzle/${VARIANT}/${SIZE}/${SIZE}puzzle.* \
            --y mem \
            --series "BELA0:re.match('BELA0', solver)" "K0:re.match('K0', solver)" "mDijkstra:re.match('mDijkstra', solver)" \
            --title "Memory (Mbytes) - ${SIZE}-Puzzle ${VARIANT}" \
            --output results/n-puzzle/${VARIANT}/${SIZE}/${SIZE}puzzle.brute-force.mem.gnuplot \
            --png ${SIZE}puzzle.brute-force.mem.png
        gnuplot results/n-puzzle/${VARIANT}/${SIZE}/${SIZE}puzzle.brute-force.mem.gnuplot
        mv ${SIZE}puzzle.brute-force.mem.png results/n-puzzle/${VARIANT}/${SIZE}/${SIZE}puzzle.brute-force.mem.png

        ./plotter.py ky --file data/n-puzzle/${VARIANT}/${SIZE}/${SIZE}puzzle.* \
            --y expansions \
            --series "BELA0:re.match('BELA0', solver)" "K0:re.match('K0', solver)" "mDijkstra:re.match('mDijkstra', solver)" \
            --title "#Expansions - ${SIZE}-Puzzle ${VARIANT}" \
            --output results/n-puzzle/${VARIANT}/${SIZE}/${SIZE}puzzle.brute-force.expansions.gnuplot \
            --png ${SIZE}puzzle.brute-force.expansions.png
        gnuplot results/n-puzzle/${VARIANT}/${SIZE}/${SIZE}puzzle.brute-force.expansions.gnuplot
        mv ${SIZE}puzzle.brute-force.expansions.png results/n-puzzle/${VARIANT}/${SIZE}/${SIZE}puzzle.brute-force.expansions.png

        ./plotter.py ky --file data/n-puzzle/${VARIANT}/${SIZE}/${SIZE}puzzle.* \
            --y nbcentroids \
            --series "BELA0:re.match('BELA0', solver)" "K0:re.match('K0', solver)" "mDijkstra:re.match('mDijkstra', solver)" \
            --title "#Centroids - ${SIZE}-Puzzle ${VARIANT}" \
            --output results/n-puzzle/${VARIANT}/${SIZE}/${SIZE}puzzle.brute-force.nbcentroids.gnuplot \
            --png ${SIZE}puzzle.brute-force.nbcentroids.png
        gnuplot results/n-puzzle/${VARIANT}/${SIZE}/${SIZE}puzzle.brute-force.nbcentroids.gnuplot
        mv ${SIZE}puzzle.brute-force.nbcentroids.png results/n-puzzle/${VARIANT}/${SIZE}/${SIZE}puzzle.brute-force.nbcentroids.png
}

# Heuristic variants
function create_heuristic_images() {

    for VARIANT in "unit" "heavy-cost"
    do
        for SIZE in 8 15
        do
            ./plotter.py ky --file data/n-puzzle/${VARIANT}/${SIZE}/${SIZE}puzzle.*${VARIANT}*csv \
                --y runtime \
                --series "BELA*:re.match('BELA\*', solver)" "K*:re.match('K\*', solver)" "mA*:re.match('mA\*', solver)" \
                --title "Runtime (seconds) - ${SIZE}-Puzzle ${VARIANT}" \
                --output results/n-puzzle/${VARIANT}/${SIZE}/${SIZE}puzzle.heuristic.runtime.gnuplot \
                --png ${SIZE}puzzle.heuristic.runtime.png
            gnuplot results/n-puzzle/${VARIANT}/${SIZE}/${SIZE}puzzle.heuristic.runtime.gnuplot
            mv ${SIZE}puzzle.heuristic.runtime.png results/n-puzzle/${VARIANT}/${SIZE}/${SIZE}puzzle.heuristic.runtime.png

            ./plotter.py ky --file data/n-puzzle/${VARIANT}/${SIZE}/${SIZE}puzzle.* \
                --y mem \
                --series "BELA*:re.match('BELA\*', solver)" "K*:re.match('K\*', solver)" "mA*:re.match('mA\*', solver)" \
                --title "Memory (Mbytes) - ${SIZE}-Puzzle ${VARIANT}" \
                --output results/n-puzzle/${VARIANT}/${SIZE}/${SIZE}puzzle.heuristic.mem.gnuplot \
                --png ${SIZE}puzzle.heuristic.mem.png
            gnuplot results/n-puzzle/${VARIANT}/${SIZE}/${SIZE}puzzle.heuristic.mem.gnuplot
            mv ${SIZE}puzzle.heuristic.mem.png results/n-puzzle/${VARIANT}/${SIZE}/${SIZE}puzzle.heuristic.mem.png

            ./plotter.py ky --file data/n-puzzle/${VARIANT}/${SIZE}/${SIZE}puzzle.* \
                --y expansions \
                --series "BELA*:re.match('BELA\*', solver)" "K*:re.match('K\*', solver)" "mA*:re.match('mA\*', solver)" \
                --title "#Expansions - ${SIZE}-Puzzle ${VARIANT}" \
                --output results/n-puzzle/${VARIANT}/${SIZE}/${SIZE}puzzle.heuristic.expansions.gnuplot \
                --png ${SIZE}puzzle.heuristic.expansions.png
            gnuplot results/n-puzzle/${VARIANT}/${SIZE}/${SIZE}puzzle.heuristic.expansions.gnuplot
            mv ${SIZE}puzzle.heuristic.expansions.png results/n-puzzle/${VARIANT}/${SIZE}/${SIZE}puzzle.heuristic.expansions.png

            ./plotter.py ky --file data/n-puzzle/${VARIANT}/${SIZE}/${SIZE}puzzle.* \
                --y nbcentroids \
                --series "BELA*:re.match('BELA\*', solver)" "K*:re.match('K\*', solver)" "mA*:re.match('mA\*', solver)" \
                --title "#Centroids - ${SIZE}-Puzzle ${VARIANT}" \
                --output results/n-puzzle/${VARIANT}/${SIZE}/${SIZE}puzzle.heuristic.nbcentroids.gnuplot \
                --png ${SIZE}puzzle.heuristic.nbcentroids.png
            gnuplot results/n-puzzle/${VARIANT}/${SIZE}/${SIZE}puzzle.heuristic.nbcentroids.gnuplot
            mv ${SIZE}puzzle.heuristic.nbcentroids.png results/n-puzzle/${VARIANT}/${SIZE}/${SIZE}puzzle.heuristic.nbcentroids.png
        done
    done
}




# Main body
while [[ $# -gt 0 ]]; do
        case $1 in
                -c|--create)
                        create_subdirectories
                        exit 0
                        ;;
                -x|--clear)
                        clear_images
                        exit 0
                        ;;
                -i|--images)
                        create_brute_force_images
                        create_heuristic_images
                        exit 0
                        ;;
                -h|--help)
                        echo " Use:"
                        echo "    -c|--create   to create the subdirectories under the results directory"
                        echo "    -x|--clear    to clear all png and gnuplot files under the results directory"
                        echo "    -i|--images   to create the png images under the results directory"
                        exit 0
                        ;;
                -*|--*)
                        echo " Unknown option $1"
                        echo " Use -h|--help for help"
                        exit 1
                        ;;
        esac
done