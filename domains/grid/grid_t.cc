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

using namespace std;

// Static vars
int grid_t::_n = 10;
string grid_t::_variant = "unit";

// return the children of this state which is known to have the given heuristic
// value as a vector of tuples, each containing: first, the cost of the
// operator; secondly, its heuristic value; thirdly, the successor state
void grid_t::children (int h, const grid_t& goal,
                       std::vector<std::tuple<int, int, grid_t>>& successors)
{

    // first, consider the moves of the unit variant where the only different is
    // the cost of the operators

    // --west
    if (_x > 0) {
        grid_t successor {_x-1, _y};
        successors.push_back (tuple<int, int, grid_t>{
            (_variant == "unit") ? 1 : 10,
            successor.h (goal),
            successor});
    }

    // --east
    if (_x < _n-1) {
        grid_t successor {_x+1, _y};
        successors.push_back (tuple<int, int, grid_t>{
            (_variant == "unit") ? 1 : 10,
            successor.h (goal),
            successor});
    }

    // --south
    if (_y > 0) {
        grid_t successor {_x, _y-1};
        successors.push_back (tuple<int, int, grid_t>{
            (_variant == "unit") ? 1 : 10,
            successor.h (goal),
            successor});
    }

    // --north
    if (_y < _n-1) {
        grid_t successor {_x, _y+1};
        successors.push_back (tuple<int, int, grid_t>{
            (_variant == "unit") ? 1 : 10,
            successor.h (goal),
            successor});
    }

    // now, in case the octile variant is being used, consider also the diagonal
    // moves
    if (_variant == "octile") {

        // --southwest
        if (_x > 0 && _y > 0) {
            grid_t successor {_x-1, _y-1};
            successors.push_back (tuple<int, int, grid_t>{
                14,
                successor.h (goal),
                successor});
        }

        // --southeast
        if (_x < _n-1 && _y > 0) {
            grid_t successor {_x+1, _y-1};
            successors.push_back (tuple<int, int, grid_t>{
                14,
                successor.h (goal),
                successor});
        }

        // --northeast
        if (_x < _n-1 && _y < _n-1) {
            grid_t successor {_x+1, _y+1};
            successors.push_back (tuple<int, int, grid_t>{
                14,
                successor.h (goal),
                successor});
        }

        // --northwest
        if (_x > 0 && _y < _n-1) {
            grid_t successor {_x-1, _y+1};
            successors.push_back (tuple<int, int, grid_t>{
                14,
                successor.h (goal),
                successor});
        }
    }
}

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
        return 10*max(dx, dy) + 4*min(dx, dy);
    } else {
        throw runtime_error ("Unknown variant: " + _variant);
    }
}

// Local Variables:
// mode:cpp
// fill-column:80
// End:
