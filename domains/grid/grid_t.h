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

    // process each child separately through the use of a callable that has to
    // receive exactly three arguments: cost_t g, cost_t h and the successor
    // state. It is the responsibility of the caller to provide the right
    // callable at the calling site.
    template<typename F>
    requires std::invocable<F&, int, int, grid_t&&>
    void children (int h, grid_t const& goal, F&& callable) const {

        // first, consider the moves of the unit variant where the only different is
        // the cost of the operators

        // --west
        if (_x > 0) {
            grid_t successor {_x-1, _y};
            callable ((_variant == "unit") ? 1 : 10,
                      successor.h (goal),
                      std::move (successor));
        }

        // --east
        if (_x < _n-1) {
            grid_t successor {_x+1, _y};
            callable ((_variant == "unit") ? 1 : 10,
                      successor.h (goal),
                      std::move (successor));
        }

        // --south
        if (_y > 0) {
            grid_t successor {_x, _y-1};
            callable ((_variant == "unit") ? 1 : 10,
                      successor.h (goal),
                      std::move (successor));
        }

        // --north
        if (_y < _n-1) {
            grid_t successor {_x, _y+1};
            callable ((_variant == "unit") ? 1 : 10,
                      successor.h (goal),
                      std::move (successor));
        }

        // now, in case the octile variant is being used, consider also the diagonal
        // moves
        if (_variant == "octile") {

            // --southwest
            if (_x > 0 && _y > 0) {
                grid_t successor {_x-1, _y-1};
                callable (14,
                          successor.h (goal),
                          std::move (successor));
            }

            // --southeast
            if (_x < _n-1 && _y > 0) {
                grid_t successor {_x+1, _y-1};
                callable (14,
                          successor.h (goal),
                          std::move (successor));
            }

            // --northeast
            if (_x < _n-1 && _y < _n-1) {
                grid_t successor {_x+1, _y+1};
                callable (14,
                          successor.h (goal),
                          std::move (successor));
            }

            // --northwest
            if (_x > 0 && _y < _n-1) {
                grid_t successor {_x-1, _y+1};
                callable (14,
                          successor.h (goal),
                          std::move (successor));
            }
        }
    }
    
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
