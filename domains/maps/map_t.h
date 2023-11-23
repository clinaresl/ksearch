// -*- coding: utf-8 -*-
// map_t.h
// -----------------------------------------------------------------------------
//
// Started on <mié 22-11-2023 19:02:25.723793022 (1700676145)>
// Carlos Linares López <carlos.linares@uc3m.es>
//

//
// Definition of a single state in the map domain
//

#ifndef _MAP_T_H_
#define _MAP_T_H_

#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

class map_t {

private:

    // INVARIANT: a state in the map domain is just a pair of coordinates (x, y)
    // over the bidimensional map
    int _x;                                                     // x-coordinate
    int _y;                                                     // y-coordinate

    // information about the map (width, height and contents) are shared by all
    // instances. Note that the only information stored of a map is whether a
    // location within the bounds of the map is passable or not
    static int _width;                                        // width of the map
    static int _height;                                      // height of the map
    static std::vector<bool> _map;        // map (static: shared by all instances)

    // This domain can be played in two different variants: unit, octile. The
    // variant is also shared among all instances
    static std::string _variant;

private:

    // methods

    // returns whether a position is blocked or not
    bool _blocked (int x,  int y)
        { return !_map [y*_width+x]; }

public:

    // Default constructors are forbidden by default
    map_t () = delete;

    // Explicit constructor
    map_t (int x, int y) :
        _x {x},
        _y {y}
        { }

    // getters
    int get_x () const
        { return _x; }
    int get_y () const
        { return _y; }
    static int get_width ()
        { return _width; }
    static int get_height ()
        { return _height; }
    static const std::string get_variant ()
        { return _variant; }

    // setters
    void set_x (const int x)
        { _x = x; }
    void set_y (const  int y)
        { _y = y; }
    static void set_width ( int width)
        { _width = width; }
    static void set_height ( int height)
        { _height = height; }
    static void set_map (const std::vector<bool>& map)
        { _map = map; }
    static void set_variant (const std::string variant) {
        if (variant != "unit" && variant != "octile") {
            throw std::invalid_argument ("[map_t::set_variant] Invalid variant!");
        }
        _variant = variant;
    }

    // operator overloading

    // this instance is less than another if and only if it precedes the other
    // according to the lexicographic ordering
    bool operator<(const map_t& other) const {
        return _x < other.get_x () ||
            (_x == other.get_x () && _y < other.get_y ());
    }

    // this instance is equal to another if and only if they are in the same
    // location
    bool operator==(const map_t& other) const {
        return _x == other.get_x () && _y == other.get_y ();
    }

    // this instance is not equal to another if and only if they are not in the
    // same position
    bool operator!=(const map_t& other) const {
        return _x != other.get_x () || _y != other.get_y ();
    }

    friend std::ostream& operator<< (std::ostream& stream, map_t state) {
        stream << "(" << state.get_x () << ", " << state.get_y () << ")";
        return stream;
    }

    // methods

    // load the map given in the specified file and store the given variant. The
    // file should define a map according to the File Format of movingai
    static void init (const std::string& filename,
                      const std::string& variant = "unit") {

        // next, load the map given in the specified filename
        std::string line;                                      // line contents
        int width, height;                       // width and height of the map
        std::ifstream mapstream (filename, std::ios::in);       // input stream

        // veriy I/O operations are available
        if (!mapstream.good ()) {          // make sure it was correctly opened
            throw std::runtime_error ("[map_t::init] Error opening file!");
        }

        // now, process the header of the map
        getline (mapstream, line);                      // ignore the type map!
        getline (mapstream, line); sscanf (line.c_str (), "height %i", &height);
        getline (mapstream, line); sscanf (line.c_str (), "width %i", &width);
        getline (mapstream, line);     // get height, width and ignore the rest

        // process the contents of the map
        int lineno = 4;               // keep track of the number of lines read
        int offset = 0;              // initialize the next position in the map
        std::vector<bool> content(width*height, false);          // reserve mem
        while (getline (mapstream, line)) {

            // update the number of lines processed
            lineno++;

            // process the contents of the map storing for each location whether
            // it is passable or not
            auto i = 0;
            while (i < line.size ()) {

                // skip the newline character. F**k pseudo-operating systems
                // which use buggy unuseful characters. As Tanembaum said,
                // programmers of such (1/2)Os should be condemmed to use them
                // for the rest of their lives :(
                if (line[i] == '\n' || line[i] == '\r') {
                    break;
                }

                // update the content of this cell. If the cells is occupied
                // then false is stored and true otherwise
                content [offset + i] = (line [i] == '.' ||
                                        line [i] == 'G' ||
                                        line [i] == 'S' ||
                                        line [i] == 'T');

                // and move forward, counting also the number of cells processed
                // in this line
                i++;
            }

            // verify the width of the map is correct
            if (i != width) {
                throw std::runtime_error ("[map_t::init] Invalid map width! Aborting ...");
            }

            // update the offset
            offset += i;
        }

        // check the height of the map is correct (after substracting the first
        // four lines taken by the header)
        if (lineno - 4 != height) {
            throw std::runtime_error ("[map_t::init] Invalid map height! Aborting ...");
        }

        // close the stream - we are done reading the file
        mapstream.close ();

        // and update all the static attributes
        map_t::set_width (width);
        map_t::set_height (height);
        map_t::set_map (content);

        // finally, store the specified variant
        map_t::set_variant (variant);
    }

    // return the children of this state as a vector of tuples, each containing:
    // first, the cost of the operator, secondly, its heuristic value; thirdly,
    // the successor state.
    void children (int h, const map_t& goal,
                   std::vector<std::tuple<int, int, map_t>>& successors);

    // return the heuristic distance to get from this state to the given goal
    // state. The heuristic function implemented takes into account the variant
    // considered: The unit variant returns the Manhattan distance while the
    // octile variant returns the octile heuristic
    int h (const map_t& goal) const;

}; // class map_t

namespace std {

    // Definition of a hash function for instances of the map. The definition is
    // included in the std namespace so that it can be used directly by the
    // functions implemented in that namespace
    template<>
    struct hash<::map_t> {

        // hash function
        size_t operator() (const map_t& right) const {

            // The hash function is the location within the map
            return right.get_y () * map_t::get_width () + right.get_x ();
        }

    }; // struct hash<npancake_t>
}

#endif // _MAP_T_H_

// Local Variables:
// mode:cpp
// fill-column:80
// End:
