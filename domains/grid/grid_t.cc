// -*- coding: utf-8 -*-
// grid_t.cc
// -----------------------------------------------------------------------------
//
// Started on <mar 29-08-2023 20:01:59.413160737 (1693332119)>
// Carlos Linares López <carlos.linares@uc3m.es>
// Ian Herman <iankherman@gmail.com>   Ian Herman <iankherman@gmail.com>

//
// Definition of a state of the grid domain (with no obstacles)
//

#include "grid_t.h"

// Static vars
int grid_t::_n = 10;
std::string grid_t::_variant = "unit";

// return the heuristic distance to get from this state to the given goal state
int grid_t::h (const grid_t& goal) const
{

    // the heuristic distance in the unit variant is just the manhattan distance
    // to the goal
    if (_variant == "unit") {
        return abs (_x - goal.get_x ()) + abs (_y - goal.get_y ());
    } else if (_variant == "octile") {

        // In the octile variant, it is necessary also to take into account two
        // facts: on one hand, the cost of horizontal and vertical moves is 10,
        // whereas the cost of the diagonal moves is 14. Also, it is allowed to
        // travel diagonally a distance equal to the minimum difference. In any
        // case, the maximum difference has to be traveled
        int dx = abs (_x - goal.get_x ());
        int dy = abs (_y - goal.get_y ());
        return 10*std::max(dx, dy) + 4*std::min(dx, dy);
    } else {
        throw std::runtime_error ("Unknown variant: " + _variant);
    }
}

// Local Variables:
// mode:cpp
// fill-column:80
// End:
