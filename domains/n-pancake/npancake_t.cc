// -*- coding: utf-8 -*-
// npancake_t.cc
// -----------------------------------------------------------------------------
//
// Started on <sáb 26-02-2022 18:10:41.081648137 (1645895441)>
// Carlos Linares López <carlos.linares@uc3m.es>
// Ian Herman <iankherman@gmail.com>   Ian Herman <iankherman@gmail.com>

//
// Definition of a state of the N-pancake
//

#include "npancake_t.h"

using namespace std;

// Static vars
int npancake_t::_n = 0;
string npancake_t::_variant = "unit";
vector<vector<vector<int>>> npancake_t::_increment;

// flip the first k positions of this permutation
std::vector<int> npancake_t::_flip(int k) {

    vector<int> perm = _perm;
    for (auto i=0 ; i<=k/2 ; i++) {     // for all locations in [0,floor (k/2)]
        swap (perm [i], perm [k-i]);
    }
    return perm;
}

// return the children of this state which is known to have the given heuristic
// value as a vector of tuples, each containing: first, the cost of the
// operator; secondly, its heuristic value; thirdly, the successor state. Note
// the goal is also given in case that incremental policies of the computation
// of the heuristic value are not used
void npancake_t::children (int h, const npancake_t& goal,
                           vector<tuple<int, int, npancake_t>>& successors) {

    for (auto i=1; i < _n; i++) {                          // for all locations

        // retrieve the net change in the computation of the heuristic value.
        // There are two cases, either we are flipping the entire permutation in
        // which case the last position has to be taken directly, or we are
        // changing a segment of it, in which case, the content of the last
        // position (is considered)
        int delta_h = (i == _n - 1)
            ? _increment[_perm[0]][_perm[i]][_n]
            : _increment[_perm[0]][_perm[i]][_perm[i+1]];

        // create the successor and insert it into the deque. Note that the net
        // change in the gap heuristic is retrieved from the _increment lookup
        // table. In the unit variant, the cost of every operator is always
        // equal to 1, whereas in the heavy cost variant, the cost of the
        // operator is the disc radius that is moved to the head of the
        // permutation.
        successors.push_back (tuple<int, int, npancake_t>{
            (_variant == "unit") ? 1 : _perm[i],
            h + delta_h,
            npancake_t (_flip (i))});
    }
}

// return the heuristic distance to get from this permutation to the identity
// permutation, i.e., the given goal is ignored, under either the unit or the
// heavy-cost variants
int npancake_t::h (const npancake_t& goal) const {

    int h=0;                    // heuristic value computed as the gap function
    for (auto i=0 ; i <_n -1 ; i++) {    // sum up gaps between consecutive pos
        if (_variant == "unit") {

            // in the unit variant, just count the number of gaps
            h += (int) (abs ((_perm   [i]) -
                             (_perm [i+1])) != 1);
        } else if (_variant == "heavy-cost") {

            // in the heavy-cost variant, weight each gap with the minimum disc
            // radius. Taking the minimum is necessary to derive an admissible
            // heuristic function because even if the gap looks now like 7|1
            // (for example), it is not clear if the operator moving 7 to the
            // head will be applied or if another operator will be applied
            // transforming the previous gap into 1|7 and then only moving the
            // disc with radius 1
            h += min(_perm[i], _perm[i+1]) * (
                (int) (abs ((_perm   [i]) -
                            (_perm [i+1])) != 1));
        } else {
            throw std::invalid_argument ("[npancake_t::h] Unknown variant!");
        }
    }

    // Additionally, sum one gap in case the last position is not N-1
    if (_perm [_n - 1] != _n - 1) {
        if (_variant == "unit") {
            h += 1;
        } else if (_variant == "heavy-cost") {
            h += _perm[_n-1];
        } else {
            throw std::invalid_argument ("[npancake_t::h] Unknown variant!");
        }
    }

    // and return the heuristic value
    return h;
}


// Local Variables:
// mode:cpp
// fill-column:80
// End:
