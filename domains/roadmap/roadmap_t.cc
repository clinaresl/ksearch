// -*- coding: utf-8 -*-
// roadmap_t.cc
// -----------------------------------------------------------------------------
//
// Started on <jue 12-10-2023 21:31:03.656116743 (1697139063)>
// Carlos Linares López <carlos.linares@uc3m.es>
// Ian Herman <iankherman@gmail.com>   Ian Herman <iankherman@gmail.com>

//
// Definition of a single state in the roadmap doamin
//

#include "roadmap_t.h"

// Static vars
graph_t roadmap_t::_graph;
bool roadmap_t::_brute_force = false;
std::string roadmap_t::_variant = "unit";

// return the heuristic distance to get from this state to the given goal state.
// The heuristic function implemented is the air distance according to the
// cosine law using a value for the earth radius equal to 6530 kms
int roadmap_t::h (const roadmap_t& goal) const {

    // in case a brute force search algorithm is being used, then ignore the
    // computation of the heuristic function
    if (roadmap_t::_brute_force) {
        return 0;
    }

    // if this is the goal, then return 0 immediately
    if (_index == goal.get_index ()) {
        return 0;
    }

    // otherwise, compute the great-circle distance between this state and the
    // goal state
    auto ivertex = roadmap_t::_graph.get_vertex (_index);
    auto jvertex = roadmap_t::_graph.get_vertex (goal.get_index ());

    // It is not expected to get values out of bounds with the following
    // expression. Note that the floor is returned to be conservative
    return int (floor (ivertex.haversine (jvertex)));
}


// Local Variables:
// mode:cpp
// fill-column:80
// End:
