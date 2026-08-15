#!/usr/bin/env sh

# Creation of the TikZ/pgfplots used in the supplemental material

# Creation of the independent pgfplots
for variant in "unit" "octile"
do
    for size in 10 15 20 25 30 35
    do
        ./plotter.py ky --file data/roadmap-maps/$variant/$size/random512-$size-0.$variant.euclidean.kssp-SBs.dimacs.1-1000.csv \
            data/roadmap-maps/$variant/$size/random512-$size-0.$variant.euclidean.libksearch-bBELA0.1.dimacs.1-1000.csv \
            data/roadmap-maps/$variant/$size/random512-$size-0.$variant.euclidean.libksearch-sBELA0.dimacs.1-1000.csv \
            --y runtime \
            --series 'SB*:re.match("SB*", solver)' \
                     'bBELA$_0$:re.match("bBELA0", solver)' \
                     'sBELA$_0$:re.match("sBELA0", solver)' \
            --title "$size Map $variant" \
            --output $size-$variant.runtime \
            --format latex
    done
done

# Creation of groups, one per variant
for variant in "unit" "octile"
do
    ./plotter.py group --file 10-$variant.runtime.tex 15-$variant.runtime.tex 20-$variant.runtime.tex 25-$variant.runtime.tex 30-$variant.runtime.tex 35-$variant.runtime.tex \
        --output random.$variant.runtime.tex \
        --nbcolumns 2 \
        --caption "Experiments in random maps -- $variant variant"
done

# Finally, remove the individual pgfplots
for variant in "unit" "octile"
do
    for size in 10 15 20 25 30 35
    do
        rm $size-$variant.runtime.tex
    done
done
