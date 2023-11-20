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

using namespace std;

// Static vars
graph_t roadmap_t::_graph;
string roadmap_t::_variant = "unit";

// return the children of this state as a vector of tuples, each containing:
// firest, the cost of the operator, secondly, tis heuristic value; thirdly, the
// successor state. No heuristics are used (at least initially) in this domain,
// so that the following signature is used only for compatibility with the
// ksearch library
void roadmap_t::children (int h, const roadmap_t& goal,
                          vector<tuple<int, int, roadmap_t>>& successors) {

    // for all edges issued from this vertex
    for (const auto& edge: roadmap_t::_graph.get_edges (_index)) {

        // add a new successor to the list of successors
        if (roadmap_t::_variant == "unit") {
            successors.push_back (make_tuple (1, 0, roadmap_t {edge.get_to ()}));
        } else {
            successors.push_back (make_tuple (edge.get_weight (), 0, roadmap_t {edge.get_to ()}));
        }
    }
}

// return the heuristic distance to get from this state to the given goal state.
// Because heuristics are not used (at least initially) it always returns zero
int roadmap_t::h (const roadmap_t& goal) const {

    return 0;
}


// Local Variables:
// mode:cpp
// fill-column:80
// End:
