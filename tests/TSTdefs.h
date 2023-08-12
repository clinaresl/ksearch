 // -*- coding: utf-8 -*-
// TSTdefs.h
// -----------------------------------------------------------------------------
//
// Started on <sáb 07-08-2021 00:58:59.720149236 (1628290739)>
// Carlos Linares López <carlos.linares@uc3m.es>
//

//
// Definitions
//

#ifndef _TSTDEFS_H_
#define _TSTDEFS_H_

#include <cmath>
#include <iostream>
#include <limits>
#include <tuple>
#include <utility>
#include <vector>

// predefined number of executions of each unit test
constexpr int NB_TESTS = 10'000;

// predefined maximum value
constexpr int MAX_VALUE = std::numeric_limits<int>::max ();

// predefined number of values
constexpr int MAX_VALUES = 100;

// maximum number of buckets
constexpr int MAX_NB_BUCKETS = 1'000'000;

// minimum and maximum number of characters in strings randomly generated
constexpr int MIN_NB_CHARS = 40;
constexpr int MAX_NB_CHARS = 80;

// number of discs used in the simultation of the pancake puzzle
constexpr int NB_DISCS = 20;

// maximum length of paths
constexpr int MAX_PATH_LENGTH = 100;

// length of the simple grid used in the tests
constexpr int SIMPLE_GRID_LENGTH = 10;

// Definition of a simple domain for testing search algorithms
class simplegrid_t {

private:

    // INVARIANT: a simple grid consists of a grid with two horizontal lines.
    // The lower line gets from 0 to N, and the second line (y=1) gets from 1 to
    // N-1. There are straight horizontal edges (cost 1) between adjacent
    // vertices with the same y. There are also diagonal edges (cost 2) between
    // vertices with the same y and adjacent values of x. Every node is
    // distinguised by a couple of coordinates and the length of the lower line
    int _n;                                        // length of the simple grid
    int _x;                                 // x coordinate of the current node
    int _y;                                 // y coordinate of the current node

public:

    // Default constructors are forbidden by default
    simplegrid_t () = delete;

    // Explicit constructor
    simplegrid_t (int n, int x, int y) : _n(n), _x(x), _y(y) {}

    // getters
    int get_n () const { return _n; }
    int get_x () const { return _x; }
    int get_y () const { return _y; }

    // operators
    bool operator<(const simplegrid_t& other) const {
        return std::tie (_n, _x, _y) < std::tie (other._n, other._x, other._y);
    }
    bool operator==(const simplegrid_t& other) const {
        return std::tie (_n, _x, _y) == std::tie (other._n, other._x, other._y);
    }

    friend std::ostream& operator<<(std::ostream& stream, const simplegrid_t& right) {

        // Show the location of the node given in right along with the dimension
        // of the grid
        stream << "(" << right.get_x () << ", " << right.get_y () << ") [" << right.get_n () << "]";
        return stream;
    }

    // return the children of this state which is known to have the given
    // heuristic value as a vector of tuples, each containing: first, the cost
    // of the operator; secondly, its heuristic value; thirdly, the successor
    // state.
    void children (int h, const simplegrid_t& goal,
                   std::vector<std::tuple<int, int, simplegrid_t>>& successors) {

        // Heuristic values are ignored!

        // Note there are no back edges, and that children are generated forward
        // only

        // if this is the lower left corner, i.e., (0, 0) then it has two
        // children with the same cost, 1
        if (_x == 0 && _y == 0) {
            successors.push_back (std::make_tuple (1, 0, simplegrid_t (_n, 1, 0)));
            successors.push_back (std::make_tuple (1, 0, simplegrid_t (_n, 1, 1)));
        } else if (_x!=_n || _y!=0) {

            // in case this is not the lower right corner, i.e., (N, 0) then
            // compute its children. Nodes in the lower line have only one child
            if (_y == 0) {
                successors.push_back (std::make_tuple (1, 0, simplegrid_t (_n, _x+1, _y)));
            } else {

                // nodes in the upper line have two children unless _x = _n-1 in
                // which case, it only has the child (N, 0)
                if (_x < _n-1) {
                    successors.push_back (std::make_tuple (1, 0, simplegrid_t (_n, _x+1, _y)));
                }
                successors.push_back (std::make_tuple (2, 0, simplegrid_t (_n, _x+1, _y-1)));
            }
        }

        // and return. Note that the goal state, i.e, the lower right corner,
        // (N, 0) has no children.
    }

    // return the heuristic distance to get from this permutation to the
    // location (N, 0), i.e., the given goal is ignored
    int h (const simplegrid_t& goal) const {

        // currently no heuristics are implemented
        return 0;
    }


}; // class simplegrid_t

namespace std {

    // Definition of a hash function for instances of the simplegrid. The
    // definition is included in the std namespace so that it can be used
    // directly by the functions implemented in that namespace
    template<>
    struct hash<::simplegrid_t> {

        // hash function
        size_t operator() (const simplegrid_t& right) const {

            // return a unique index to the location given in right
            return right.get_x () + right.get_y () * right.get_n ();
        }
    }; // struct hash<simplegrid_t>
}


#endif // _TSTDEFS_H_

// Local Variables:
// mode:cpp
// fill-column:80
// End:
