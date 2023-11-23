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

using namespace std;

// Static vars
int map_t::_width = 0;
int map_t::_height = 0;
vector<bool> map_t::_map = vector<bool>();
string map_t::_variant = "";

// return the children of this state as a vector of tuples, each containing:
// first, the cost of the operator, secondly, its heuristic value; thirdly,
// the successor state.
void map_t::children (int h, const map_t& goal,
                      std::vector<std::tuple<int, int, map_t>>& successors) {

    // first, consider the moves of the unit variant where the only different is
    // the cost of the operators. In the unit variant only the four adjacent
    // locations are considered children and they all have the same cost. In the
    // octile variant, nodes diagonally adjacent are also considered but this
    // time the cost differs and while horizontal and vertical moves have a cost
    // equal to 10 units, diagonal moves are 14 units worth ---roughly
    // approximating the square root of 2

    // --west
    if (_x > 0 && !_blocked (_x-1, _y)) {
        map_t successor {_x-1, _y};
        successors.push_back (tuple<int, int, map_t>{
                (_variant == "unit") ? 1 : 10,
                successor.h (goal),
                successor});
    }

    // --east
    if (_x < _width-1 && !_blocked (_x+1, _y)) {
        map_t successor {_x+1, _y};
        successors.push_back (tuple<int, int, map_t>{
                (_variant == "unit") ? 1 : 10,
                successor.h (goal),
                successor});
    }

    // --south
    if (_y > 0 && !_blocked (_x, _y-1)) {
        map_t successor {_x, _y-1};
        successors.push_back (tuple<int, int, map_t>{
                (_variant == "unit") ? 1 : 10,
                successor.h (goal),
                successor});
    }

    // --north
    if (_y < _height-1 && !_blocked (_x, _y+1)) {
        map_t successor {_x, _y+1};
        successors.push_back (tuple<int, int, map_t>{
                (_variant == "unit") ? 1 : 10,
                successor.h (goal),
                successor});
    }

    // now, in case the octile variant is being used, consider also the diagonal
    // moves
    if (_variant == "octile") {

        // --southwest
        if (_x > 0 && _y > 0 && !_blocked (_x-1, _y-1)) {
            map_t successor {_x-1, _y-1};
            successors.push_back (tuple<int, int, map_t>{
                    14,
                    successor.h (goal),
                    successor});
        }

        // --southeast
        if (_x < _width-1 && _y > 0 && !_blocked (_x+1, _y-1)) {
            map_t successor {_x+1, _y-1};
            successors.push_back (tuple<int, int, map_t>{
                    14,
                    successor.h (goal),
                    successor});
        }

        // --northeast
        if (_x < _width-1 && _y < _height-1 && !_blocked (_x+1, _y+1)) {
            map_t successor {_x+1, _y+1};
            successors.push_back (tuple<int, int, map_t>{
                    14,
                    successor.h (goal),
                    successor});
        }

        // --northwest
        if (_x > 0 && _y < _height-1 && !_blocked (_x-1, _y+1)) {
            map_t successor {_x-1, _y+1};
            successors.push_back (tuple<int, int, map_t>{
                    14,
                    successor.h (goal),
                    successor});
        }
    }
}

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
        return 10 * max(dx, dy) + 4 * min (dx, dy);
    }

    // otherwise, raise an exception
    throw runtime_error ("[map_t::h] Unknown variant!");
}


// Local Variables:
// mode:cpp
// fill-column:80
// End:
