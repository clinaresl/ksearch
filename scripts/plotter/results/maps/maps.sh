#!/usr/bin/env sh

# Maps random512 instance #0
# -----------------------------------------------------------------------------

# Creating subdirectories
# -----------------------------------------------------------------------------
function create_subdirectories() {
    for SIZE in 10 20 30 40
    do
        for VARIANT in "unit" "octile"
        do
            mkdir -p results/maps/${VARIANT}/${SIZE}
        done
    done
}


# Clearing images
# -----------------------------------------------------------------------------
function clear_images() {
    for VARIANT in "unit" "octile"
    do
        rm -f results/maps/${VARIANT}/*/*.png
        rm -f results/maps/${VARIANT}/*/*.gnuplot
    done
}

# Clearing tables
# -----------------------------------------------------------------------------
function clear_tables() {
    for VARIANT in "unit" "octile"
    do
        rm -f results/maps/${VARIANT}/*/*.tex
    done
}

# Creating images
# -----------------------------------------------------------------------------
# Brute-force variants
function create_brute_force_images() {
    for RATIO in 10 15 20 25 30 35
    do
        for VARIANT in "unit" "octile"
        do
            export MEASURE="runtime"
            ./plotter.py ky --file data/maps/${VARIANT}/${RATIO}/random512-${RATIO}-0.belA0.${VARIANT}.1-10000.csv \
                data/maps/${VARIANT}/${RATIO}/random512-${RATIO}-0.K0.${VARIANT}.1-1000.csv \
                data/maps/${VARIANT}/${RATIO}/random512-${RATIO}-0.mDijkstra.${VARIANT}.1-100.csv \
                --y      ${MEASURE} \
                --series "BELA0:re.match('BELA0', solver)" "K0:re.match('K0', solver)" "mDijkstra:re.match('mDijkstra', solver) and k < 5" \
                --title  "Runtime (seconds) - Maps ${RATIO} ${VARIANT}" \
                --output results/maps/${VARIANT}/${RATIO}/random512-${RATIO}.brute-force.${VARIANT}.${MEASURE}.gnuplot \
                --png    random512-${RATIO}.brute-force.${VARIANT}.${MEASURE}.png
            gnuplot results/maps/${VARIANT}/${RATIO}/random512-${RATIO}.brute-force.${VARIANT}.${MEASURE}.gnuplot
            mv random512-${RATIO}.brute-force.${VARIANT}.${MEASURE}.png results/maps/${VARIANT}/${RATIO}/random512-${RATIO}.brute-force.${VARIANT}.${MEASURE}.png

            export MEASURE="mem"
            ./plotter.py ky --file data/maps/${VARIANT}/${RATIO}/random512-${RATIO}-0.belA0.${VARIANT}.1-10000.csv \
                data/maps/${VARIANT}/${RATIO}/random512-${RATIO}-0.K0.${VARIANT}.1-1000.csv \
                data/maps/${VARIANT}/${RATIO}/random512-${RATIO}-0.mDijkstra.${VARIANT}.1-100.csv \
                --y      ${MEASURE} \
                --series "BELA0:re.match('BELA0', solver)" "K0:re.match('K0', solver)" "mDijkstra:re.match('mDijkstra', solver) and k < 5" \
                --title  "Memory (Mbytes) - Maps ${RATIO} ${VARIANT}" \
                --output results/maps/${VARIANT}/${RATIO}/random512-${RATIO}.brute-force.${VARIANT}.${MEASURE}.gnuplot \
                --png    random512-${RATIO}.brute-force.${VARIANT}.${MEASURE}.png
            gnuplot results/maps/${VARIANT}/${RATIO}/random512-${RATIO}.brute-force.${VARIANT}.${MEASURE}.gnuplot
            mv random512-${RATIO}.brute-force.${VARIANT}.${MEASURE}.png results/maps/${VARIANT}/${RATIO}/random512-${RATIO}.brute-force.${VARIANT}.${MEASURE}.png

            export MEASURE="expansions"
            ./plotter.py ky --file data/maps/${VARIANT}/${RATIO}/random512-${RATIO}-0.belA0.${VARIANT}.1-10000.csv \
                data/maps/${VARIANT}/${RATIO}/random512-${RATIO}-0.K0.${VARIANT}.1-1000.csv \
                data/maps/${VARIANT}/${RATIO}/random512-${RATIO}-0.mDijkstra.${VARIANT}.1-100.csv \
                --y      ${MEASURE} \
                --series "BELA0:re.match('BELA0', solver)" "K0:re.match('K0', solver)" "mDijkstra:re.match('mDijkstra', solver) and k < 2" \
                --title  "#Expansions - Maps ${RATIO} ${VARIANT}" \
                --output results/maps/${VARIANT}/${RATIO}/random512-${RATIO}.brute-force.${VARIANT}.${MEASURE}.gnuplot \
                --png    random512-${RATIO}.brute-force.${VARIANT}.${MEASURE}.png
            gnuplot results/maps/${VARIANT}/${RATIO}/random512-${RATIO}.brute-force.${VARIANT}.${MEASURE}.gnuplot
            mv random512-${RATIO}.brute-force.${VARIANT}.${MEASURE}.png results/maps/${VARIANT}/${RATIO}/random512-${RATIO}.brute-force.${VARIANT}.${MEASURE}.png

            export MEASURE="nbcentroids"
            ./plotter.py ky --file data/maps/${VARIANT}/${RATIO}/random512-${RATIO}-0.belA0.${VARIANT}.1-10000.csv \
                data/maps/${VARIANT}/${RATIO}/random512-${RATIO}-0.K0.${VARIANT}.1-1000.csv \
                data/maps/${VARIANT}/${RATIO}/random512-${RATIO}-0.mDijkstra.${VARIANT}.1-100.csv \
                --y      ${MEASURE} \
                --series "BELA0:re.match('BELA0', solver)" "K0:re.match('K0', solver)" "mDijkstra:re.match('mDijkstra', solver) and k < 5" \
                --title  "#Centroids - Maps ${RATIO} ${VARIANT}" \
                --output results/maps/${VARIANT}/${RATIO}/random512-${RATIO}.brute-force.${VARIANT}.${MEASURE}.gnuplot \
                --png    random512-${RATIO}.brute-force.${VARIANT}.${MEASURE}.png
            gnuplot results/maps/${VARIANT}/${RATIO}/random512-${RATIO}.brute-force.${VARIANT}.${MEASURE}.gnuplot
            mv random512-${RATIO}.brute-force.${VARIANT}.${MEASURE}.png results/maps/${VARIANT}/${RATIO}/random512-${RATIO}.brute-force.${VARIANT}.${MEASURE}.png
        done
    done
}

# Heuristic variants
function create_heuristic_images() {
    for RATIO in 10 15 20 25 30 35
    do
        for VARIANT in "unit" "octile"
        do
            export MEASURE="runtime"
            ./plotter.py ky --file data/maps/${VARIANT}/${RATIO}/random512-${RATIO}-0.belAs.${VARIANT}.1-10000.csv \
                data/maps/${VARIANT}/${RATIO}/random512-${RATIO}-0.Ks.${VARIANT}.1-1000.csv \
                data/maps/${VARIANT}/${RATIO}/random512-${RATIO}-0.mAs.${VARIANT}.1-100.csv \
                --y      ${MEASURE} \
                --series "BELA*:re.match('BELA\*', solver)" "K*:re.match('K\*', solver)" "mA*:re.match('mA\*', solver)" \
                --title  "Runtime (seconds) - Maps ${RATIO} ${VARIANT}" \
                --output results/maps/${VARIANT}/${RATIO}/random512-${RATIO}.heuristic.${VARIANT}.${MEASURE}.gnuplot \
                --png    random512-${RATIO}.heuristic.${VARIANT}.${MEASURE}.png
            gnuplot results/maps/${VARIANT}/${RATIO}/random512-${RATIO}.heuristic.${VARIANT}.${MEASURE}.gnuplot
            mv random512-${RATIO}.heuristic.${VARIANT}.${MEASURE}.png results/maps/${VARIANT}/${RATIO}/random512-${RATIO}.heuristic.${VARIANT}.${MEASURE}.png

            export MEASURE="mem"
            ./plotter.py ky --file data/maps/${VARIANT}/${RATIO}/random512-${RATIO}-0.belAs.${VARIANT}.1-10000.csv \
                data/maps/${VARIANT}/${RATIO}/random512-${RATIO}-0.Ks.${VARIANT}.1-1000.csv \
                data/maps/${VARIANT}/${RATIO}/random512-${RATIO}-0.mAs.${VARIANT}.1-100.csv \
                --y      ${MEASURE} \
                --series "BELA*:re.match('BELA\*', solver)" "K*:re.match('K\*', solver)" "mA*:re.match('mA\*', solver)" \
                --title  "Memory (Mbytes) - Maps ${RATIO} ${VARIANT}" \
                --output results/maps/${VARIANT}/${RATIO}/random512-${RATIO}.heuristic.${VARIANT}.${MEASURE}.gnuplot \
                --png    random512-${RATIO}.heuristic.${VARIANT}.${MEASURE}.png
            gnuplot results/maps/${VARIANT}/${RATIO}/random512-${RATIO}.heuristic.${VARIANT}.${MEASURE}.gnuplot
            mv random512-${RATIO}.heuristic.${VARIANT}.${MEASURE}.png results/maps/${VARIANT}/${RATIO}/random512-${RATIO}.heuristic.${VARIANT}.${MEASURE}.png

            export MEASURE="expansions"
            ./plotter.py ky --file data/maps/${VARIANT}/${RATIO}/random512-${RATIO}-0.belAs.${VARIANT}.1-10000.csv \
                data/maps/${VARIANT}/${RATIO}/random512-${RATIO}-0.Ks.${VARIANT}.1-1000.csv \
                data/maps/${VARIANT}/${RATIO}/random512-${RATIO}-0.mAs.${VARIANT}.1-100.csv \
                --y      ${MEASURE} \
                --series "BELA*:re.match('BELA\*', solver)" "K*:re.match('K\*', solver)" "mA*:re.match('mA\*', solver)" \
                --title  "#Expansions - Maps ${RATIO} ${VARIANT}" \
                --output results/maps/${VARIANT}/${RATIO}/random512-${RATIO}.heuristic.${VARIANT}.${MEASURE}.gnuplot \
                --png    random512-${RATIO}.heuristic.${VARIANT}.${MEASURE}.png
            gnuplot results/maps/${VARIANT}/${RATIO}/random512-${RATIO}.heuristic.${VARIANT}.${MEASURE}.gnuplot
            mv random512-${RATIO}.heuristic.${VARIANT}.${MEASURE}.png results/maps/${VARIANT}/${RATIO}/random512-${RATIO}.heuristic.${VARIANT}.${MEASURE}.png

            export MEASURE="nbcentroids"
            ./plotter.py ky --file data/maps/${VARIANT}/${RATIO}/random512-${RATIO}-0.belAs.${VARIANT}.1-10000.csv \
                data/maps/${VARIANT}/${RATIO}/random512-${RATIO}-0.Ks.${VARIANT}.1-1000.csv \
                data/maps/${VARIANT}/${RATIO}/random512-${RATIO}-0.mAs.${VARIANT}.1-100.csv \
                --y      ${MEASURE} \
                --series "BELA*:re.match('BELA\*', solver)" "K*:re.match('K\*', solver)" "mA*:re.match('mA\*', solver)" \
                --title  "#Centroids - Maps ${RATIO} ${VARIANT}" \
                --output results/maps/${VARIANT}/${RATIO}/random512-${RATIO}.heuristic.${VARIANT}.${MEASURE}.gnuplot \
                --png    random512-${RATIO}.heuristic.${VARIANT}.${MEASURE}.png
            gnuplot results/maps/${VARIANT}/${RATIO}/random512-${RATIO}.heuristic.${VARIANT}.${MEASURE}.gnuplot
            mv random512-${RATIO}.heuristic.${VARIANT}.${MEASURE}.png results/maps/${VARIANT}/${RATIO}/random512-${RATIO}.heuristic.${VARIANT}.${MEASURE}.png
        done
    done
}

# Creating tables
# -----------------------------------------------------------------------------
# Brute-force variants
function create_brute_force_tables() {
    for RATIO in 10 15 20 25 30 35
    do
        for VARIANT in "unit" "octile"
        do
            export MEASURE="runtime"
            ./plotter.py ky --file data/maps/${VARIANT}/${RATIO}/random512-${RATIO}-0.belA0.${VARIANT}.1-10000.csv \
                data/maps/${VARIANT}/${RATIO}/random512-${RATIO}-0.K0.${VARIANT}.1-1000.csv \
                data/maps/${VARIANT}/${RATIO}/random512-${RATIO}-0.mDijkstra.${VARIANT}.1-100.csv \
                --y      ${MEASURE} \
                --series "BELA\$_0\$:re.match('BELA0', solver)" "K\$_0\$:re.match('K0', solver)" "mDijkstra:re.match('mDijkstra', solver) and k < 5" \
                --title  "Runtime (seconds) - Maps ${RATIO} ${VARIANT}" \
                --k "1;2;3;4;5;10;50;100;500;1000;5000;10000" \
                --output results/maps/${VARIANT}/${RATIO}/random512-${RATIO}.brute-force.${VARIANT}.${MEASURE} \
                --table

            export MEASURE="mem"
            ./plotter.py ky --file data/maps/${VARIANT}/${RATIO}/random512-${RATIO}-0.belA0.${VARIANT}.1-10000.csv \
                data/maps/${VARIANT}/${RATIO}/random512-${RATIO}-0.K0.${VARIANT}.1-1000.csv \
                data/maps/${VARIANT}/${RATIO}/random512-${RATIO}-0.mDijkstra.${VARIANT}.1-100.csv \
                --y      ${MEASURE} \
                --series "BELA\$_0\$:re.match('BELA0', solver)" "K\$_0\$:re.match('K0', solver)" "mDijkstra:re.match('mDijkstra', solver) and k < 5" \
                --title  "Memory (Mbytes) - Maps ${RATIO} ${VARIANT}" \
                --k "1;2;3;4;5;10;50;100;500;1000;5000;10000" \
                --output results/maps/${VARIANT}/${RATIO}/random512-${RATIO}.brute-force.${VARIANT}.${MEASURE} \
                --table

            export MEASURE="expansions"
            ./plotter.py ky --file data/maps/${VARIANT}/${RATIO}/random512-${RATIO}-0.belA0.${VARIANT}.1-10000.csv \
                data/maps/${VARIANT}/${RATIO}/random512-${RATIO}-0.K0.${VARIANT}.1-1000.csv \
                data/maps/${VARIANT}/${RATIO}/random512-${RATIO}-0.mDijkstra.${VARIANT}.1-100.csv \
                --y      ${MEASURE} \
                --series "BELA\$_0\$:re.match('BELA0', solver)" "K\$_0\$:re.match('K0', solver)" "mDijkstra:re.match('mDijkstra', solver) and k < 5" \
                --title  "#Expansions - Maps ${RATIO} ${VARIANT}" \
                --k "1;2;3;4;5;10;50;100;500;1000;5000;10000" \
                --output results/maps/${VARIANT}/${RATIO}/random512-${RATIO}.brute-force.${VARIANT}.${MEASURE} \
                --table

            export MEASURE="nbcentroids"
            ./plotter.py ky --file data/maps/${VARIANT}/${RATIO}/random512-${RATIO}-0.belA0.${VARIANT}.1-10000.csv \
                data/maps/${VARIANT}/${RATIO}/random512-${RATIO}-0.K0.${VARIANT}.1-1000.csv \
                data/maps/${VARIANT}/${RATIO}/random512-${RATIO}-0.mDijkstra.${VARIANT}.1-100.csv \
                --y      ${MEASURE} \
                --series "BELA\$_0\$:re.match('BELA0', solver)" "K\$_0\$:re.match('K0', solver)" "mDijkstra:re.match('mDijkstra', solver) and k < 5" \
                --title  "#Centroids - Maps ${RATIO} ${VARIANT}" \
                --k "1;2;3;4;5;10;50;100;500;1000;5000;10000" \
                --output results/maps/${VARIANT}/${RATIO}/random512-${RATIO}.brute-force.${VARIANT}.${MEASURE} \
                --table
        done
    done
}

# Heuristic variants
function create_heuristic_tables() {
    for RATIO in 10 15 20 25 30 35
    do
        for VARIANT in "unit" "octile"
        do
            export MEASURE="runtime"
            ./plotter.py ky --file data/maps/${VARIANT}/${RATIO}/random512-${RATIO}-0.belAs.${VARIANT}.1-10000.csv \
                data/maps/${VARIANT}/${RATIO}/random512-${RATIO}-0.Ks.${VARIANT}.1-1000.csv \
                data/maps/${VARIANT}/${RATIO}/random512-${RATIO}-0.mAs.${VARIANT}.1-100.csv \
                --y      ${MEASURE} \
                --series "BELA\$^*\$:re.match('BELA\*', solver)" "K\$^*\$:re.match('K\*', solver)" "mA\$^*\$:re.match('mA\*', solver)" \
                --title  "Runtime (seconds) - Maps ${RATIO} ${VARIANT}" \
                --k "1;2;3;4;5;10;50;100;500;1000;5000;10000" \
                --output results/maps/${VARIANT}/${RATIO}/random512-${RATIO}.heuristic.${VARIANT}.${MEASURE} \
                --table

            export MEASURE="mem"
            ./plotter.py ky --file data/maps/${VARIANT}/${RATIO}/random512-${RATIO}-0.belAs.${VARIANT}.1-10000.csv \
                data/maps/${VARIANT}/${RATIO}/random512-${RATIO}-0.Ks.${VARIANT}.1-1000.csv \
                data/maps/${VARIANT}/${RATIO}/random512-${RATIO}-0.mAs.${VARIANT}.1-100.csv \
                --y      ${MEASURE} \
                --series "BELA\$^*\$:re.match('BELA\*', solver)" "K\$^*\$:re.match('K\*', solver)" "mA\$^*\$:re.match('mA\*', solver)" \
                --title  "Memory (Mbytes) - Maps ${RATIO} ${VARIANT}" \
                --k "1;2;3;4;5;10;50;100;500;1000;5000;10000" \
                --output results/maps/${VARIANT}/${RATIO}/random512-${RATIO}.heuristic.${VARIANT}.${MEASURE} \
                --table

            export MEASURE="expansions"
            ./plotter.py ky --file data/maps/${VARIANT}/${RATIO}/random512-${RATIO}-0.belAs.${VARIANT}.1-10000.csv \
                data/maps/${VARIANT}/${RATIO}/random512-${RATIO}-0.Ks.${VARIANT}.1-1000.csv \
                data/maps/${VARIANT}/${RATIO}/random512-${RATIO}-0.mAs.${VARIANT}.1-100.csv \
                --y      ${MEASURE} \
                --series "BELA\$^*\$:re.match('BELA\*', solver)" "K\$^*\$:re.match('K\*', solver)" "mA\$^*\$:re.match('mA\*', solver)" \
                --title  "#Expansions - Maps ${RATIO} ${VARIANT}" \
                --k "1;2;3;4;5;10;50;100;500;1000;5000;10000" \
                --output results/maps/${VARIANT}/${RATIO}/random512-${RATIO}.heuristic.${VARIANT}.${MEASURE} \
                --table

            export MEASURE="nbcentroids"
            ./plotter.py ky --file data/maps/${VARIANT}/${RATIO}/random512-${RATIO}-0.belAs.${VARIANT}.1-10000.csv \
                data/maps/${VARIANT}/${RATIO}/random512-${RATIO}-0.Ks.${VARIANT}.1-1000.csv \
                data/maps/${VARIANT}/${RATIO}/random512-${RATIO}-0.mAs.${VARIANT}.1-100.csv \
                --y      ${MEASURE} \
                --series "BELA\$^*\$:re.match('BELA\*', solver)" "K\$^*\$:re.match('K\*', solver)" "mA\$^*\$:re.match('mA\*', solver)" \
                --title  "#Centroids - Maps ${RATIO} ${VARIANT}" \
                --k "1;2;3;4;5;10;50;100;500;1000;5000;10000" \
                --output results/maps/${VARIANT}/${RATIO}/random512-${RATIO}.heuristic.${VARIANT}.${MEASURE} \
                --table
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
