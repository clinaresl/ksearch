// -*- coding: utf-8 -*-
// grid_t.h
// -----------------------------------------------------------------------------
//
// Started on <mar 29-08-2023 18:52:30.996052108 (1693327950)>
// Carlos Linares López <carlos.linares@uc3m.es>
// Ian Herman <iankherman@gmail.com>   Ian Herman <iankherman@gmail.com>

//
// Definition of a state of the grid domain (with no obstacles)
//

#ifndef _GRID_T_H_
#define _GRID_T_H_

#include <algorithm>
#include <iostream>
#include <string>
#include <tuple>
#include <vector>

// Class definition
//
// Definition of a state of the Grid domain (with no obstacles)
class grid_t {

private:

    // INVARIANT: every state of the grid domain consists of a pair of x and y
    // coordinates over a cartesian plan. The size of the grid is stored as a
    // static attribute
    static int _n;                                   // size of the square grid
    int _x;                                                     // x coordinate
    int _y;                                                     // y coordinate

    // The domain can be used under two different variants: unit and octile. In
    // the first one, the only allowed transitions are either horizontal or
    // vertical; in the latter, it is also allowed to move diagonally, resulting
    // in a branching factor equal to 8.
    static std::string _variant;

public:

    // Default constructors are forbidden by default
    grid_t () = delete;

    // Explicit constructor
    grid_t (int x, int y):
    _x {x},
    _y {y}
        {}

    // getters
    static const int get_n () { return _n; }
    const int get_x () const { return _x; }
    const int get_y () const { return _y; }
    static const std::string get_variant () { return _variant; }

    // setters
    static void set_n (int n) { _n = n; }
    static void set_variant (std::string variant) {
        if (variant != "unit" and variant != "octile") {
            throw std::invalid_argument ("[grid_t::set_variant] Invalid variant");
        }
        _variant = variant;
    }

    // operator overloading

    // this instance is less than another if its x coordinate is less than the
    // other's x coordinate, or if they are equal and its y coordinate is less
    // than the other's y coordinate
    bool operator< (const grid_t& other) const {
        return (_x < other._x) or ((_x == other._x) and (_y < other._y));
    }

    // this instance is equal to another if both coordinates are equal
    bool operator== (const grid_t& other) const {
        return (_x == other._x) and (_y == other._y);
    }

    // this instance is not equal to another if either coordinate is not equal
    bool operator!= (const grid_t& other) const {
        return (_x != other._x) or (_y != other._y);
    }

    friend std::ostream& operator<< (std::ostream& stream, const grid_t& right) {

        // Show the coordinates
        stream << "(" << right.get_x () << ", " << right.get_y () << ")";

        return stream;
    }

    // return the children of this state which is known to have the given
    // heuristic value as a vector of tuples, each containing: first, the cost
    // of the operator; secondly, its heuristic value; thirdly, the successor
    // state
    void children (int h, const grid_t& goal,
                   std::vector<std::tuple<int, int, grid_t>>& successors);

    // return the heuristic distance to get from this state to the given goal
    // state
    int h (const grid_t& goal) const;

}; // class grid_t

namespace std {

    // Definition of a hash function for instances of the grid. The definition
    // is included in the std namespace so that it can be used directly by the
    // functions implemented in that namespace
    template<>
    struct hash<::grid_t> {

        // hash function
        size_t operator() (const grid_t& right) const {

            // The hash function uses the size of the square grid to return a
            // perfect hash function
            return right.get_y () * grid_t::get_n () + right.get_x ();
        }

    }; // struct hash<npancake_t>
}

#endif // _GRID_T_H_

// Local Variables:
// mode:cpp
// fill-column:80
// End:
