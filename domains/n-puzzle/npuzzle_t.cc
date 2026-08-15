// -*- coding: utf-8 -*-
// npuzzle_t.cc
// -----------------------------------------------------------------------------
//
// Started on <sáb 04-12-2021 13:42:12.830773173 (1638621732)>
// Carlos Linares López <carlos.linares@uc3m.es>
//

//
// Definition of a state of the N-puzzle
//

#include<cmath>

#include "npuzzle_t.h"

// Static vars
int npuzzle_t::_n = 0;
std::string npuzzle_t::_variant = "unit";
std::vector<std::vector<int>> npuzzle_t::_oprs;
std::vector<std::vector<std::vector<int>>> npuzzle_t::_increment;

// A puzzle can be constructed from a vector of integers. This constructor
// assumes the sliding-tile puzzle is squared and also correct, i.e., there are
// no duplicates and all values are in the range [0, n^2)
npuzzle_t::npuzzle_t (std::vector<int> perm) {

    // traverse the given permutation to get the length of the side and also the
    // location of the blank
    for (auto i = 0 ; i < perm.size () ; i++) {

        // is this the blank location?
        if (!perm[i]) {
            _blank = i;
        }

        // and push this content into the permutation
        _perm.push_back (perm[i]);
    }

    // finally, take the length of this sliding-tile puzzle to be square root.
    // Again note this constructor assumes the permutation to be correct
    _n = sqrt (perm.size ());
}

// swaps the contents of locations i and j. It updates other internal data
// if necessary
void npuzzle_t::_swap (int i, int j)
{
    // update the location of the blank if necessary
    if (_blank == i) {
        _blank = j;
    } else if (_blank == j) {
        _blank = i;
    }

    // next, update the contents of the permutation
    std::swap (_perm[i], _perm[j]);
}

// return the heuristic distance to get from this permutation to the identity
// permutation, i.e., the given goal is ignored, under either the unit or the
// heavy-cost variants
int npuzzle_t::h (const npuzzle_t& goal) const {

    // -- initialization
    int manhattan = 0;

    // and process all tiles in this permutation
    for (auto i = 0 ; i < _perm.size () ; i++) {

        // do not count blanks
        if (_perm[i]) {
            if (_variant == "unit") {
                manhattan += abs (_perm[i]/_n - i/_n) +
                    abs (_perm[i]%_n - i%_n);
            } else if (_variant == "heavy-cost") {
                manhattan += _perm[i] * (
                    abs (_perm[i]/_n - i/_n) +
                    abs (_perm[i]%_n - i%_n));
            } else {
                throw std::invalid_argument ("[npuzzle_t::h] Unknown variant!");
            }
        }
    }

    return manhattan;
}


// Local Variables:
// mode:cpp
// fill-column:80
// End:
