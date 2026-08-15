// -*- coding: utf-8 -*-
// map_t.cc
// -----------------------------------------------------------------------------
//
// Started on <mié 22-11-2023 19:02:18.526929844 (1700676138)>
// Carlos Linares López <carlos.linares@uc3m.es>
//

//
// Definition of a single state in the map domain
//

#include "map_t.h"

// Static vars
int map_t::_width = 0;
int map_t::_height = 0;
std::vector<bool> map_t::_map = std::vector<bool>();
std::string map_t::_variant = "";

// return the heuristic distance to get from this state to the given goal state.
// The heuristic function implemented takes into account the variant considered:
// The unit variant returns the Manhattan distance while the octile variant
// returns the octile heuristic
int map_t::h (const map_t& goal) const {

    // if this is the goal, then return 0 immediately
    if (*this == goal) {
        return 0;
    }

    // compute the difference across both axis between this state and the goal
    // state
    int dx = abs (_x - goal.get_x ());
    int dy = abs (_y - goal.get_y ());

    // Now, if the unit variant is being used, then just return the manhattan
    // distance
    if (_variant == "unit") {
        return dx + dy;
    } else if (_variant == "octile") {

        // if the octile variant is being used, then compute the diagonal
        // distance and add the difference between the diagonal and the
        // horizontal/vertical distances
        return 10 * std::max(dx, dy) + 4 * std::min (dx, dy);
    }

    // otherwise, raise an exception
    throw std::runtime_error ("[map_t::h] Unknown variant!");
}


// Local Variables:
// mode:cpp
// fill-column:80
// End:
