// -*- coding: utf-8 -*-
// roadmap_t.h
// -----------------------------------------------------------------------------
//
// Started on <jue 12-10-2023 21:18:45.416751021 (1697138325)>
// Carlos Linares López <carlos.linares@uc3m.es>
// Ian Herman <iankherman@gmail.com>   Ian Herman <iankherman@gmail.com>

//
// Definition of a single state in the roadmap doamin
//

#ifndef _ROADMAP_T_H_
#define _ROADMAP_T_H_

#include <cmath>
#include <cstdlib>
#include <iomanip>
#include <map>
#include <string>
#include <tuple>
#include <vector>

#include "graph_t.h"
#include "../instance.h"

// Class definition
//
// Definition of a single state in the roadmap domain
class roadmap_t {

private:

    // INVARIANT: every state of the roadmap domain consists of a single index
    // to a static graph denoting its current location
    static graph_t _graph;                                  // underlying graph
    size_t _index;                                          // current location

    // This domain can be used under two different variants: unit and dimacs. In
    // the unit domain the cost of every edge is 1, while in the dimacs domain
    // the cost of every edge is read from the definition file
    static std::string _variant;

    // The heuristic used in the roadmap is very time consuming. Thus, in case
    // brute-force search variants are going to be used, the computation of the
    // heuristic function must be skipped for the sake of efficiency
    static bool _brute_force;

public:

    // Explicit constructor
    roadmap_t (std::size_t index):
        _index {index}
        {}

    // getters
    [[nodiscard]] static const graph_t& get_graph () {
        return _graph;
    }
    [[nodiscard]] const size_t get_index () const {
        return _index;
    }
    [[nodiscard]] static const std::string get_variant () {
        return _variant;
    }

    // setters
    static void set_variant (std::string variant) {
        if (variant != "unit" && variant != "dimacs") {
            throw std::invalid_argument ("[roadmap_t::set_variant] Invalid variant");
        }
        _variant = variant;
    }
    static void set_brute_force (bool brute_force) {
        _brute_force = brute_force;
    }

    // operator overloading

    // operator overloading
    std::strong_ordering operator<=>(roadmap_t const& other) const {
        return _index <=> other.get_index ();
    }
    
    // this instance is equal to another if and only if its indices are the same
    bool operator== (const roadmap_t& other) const {
        return _index == other.get_index ();
    }

    friend std::ostream& operator<< (std::ostream& stream, roadmap_t const& right) {
        stream << std::setfill (' ') << std::setw (10) << right.get_index ();
        return stream;
    }

    // methods

    // load the roadmap graph given in the first file. It also loads the
    // coordinates of every vertex whose filename is derived from the given
    // filename. The files with the contents of the graph and coordinates should
    // define a graph in the format of the 9th DIMACS competition.
    //
    // In case a binary file with the same information is available (whose name
    // follows the filename given), it is used instead. If not, after loading
    // the information, a binary file is created.
    static void init (const std::string& filename,
                      const std::string& variant = "unit") {

        // set the given variant
        roadmap_t::_variant = variant;

        // next, check whether a binary file exists already containing the
        // information of the graph. get_bin_filename is a free function
        // provided along the definition of a graph
        std::filesystem::path bin{get_bin_filename (filename)};
        if (std::filesystem::exists (bin)) {

            // then prefer loading the binary file because is much faster.
            // Prefer verbose output to show information about the binary file
            // being processed
            roadmap_t::_graph.read_binary (filename, true);
        } else {
        
            // otherwise, load the graph. Loading the file will automatically
            // create the binary file in the same directory where the file used
            // for defining the graph resides so that in the future, the binary
            // file will be preferred
            roadmap_t::_graph.load (filename);
        }
    }

    // process each child separately through the use of a callable that has to
    // receive exactly three arguments: cost_t g, cost_t h and the successor
    // state. It is the responsibility of the caller to provide the right
    // callable at the calling site.
    template<typename F>
    requires std::invocable<F&, int, int, roadmap_t&&>
    void children (int h, roadmap_t const& goal, F&& callable) const {

        // for all edges issued from this vertex
        for (edge_t const& iedge: roadmap_t::_graph.get_edges (_index)) {

            // create the successor
            roadmap_t successor { iedge.get_to () };

            // and invoke the callable with the right arguments: g, h and the
            // successor
            if (roadmap_t::_variant == "unit") {
                callable ( 1, 0, std::move (successor) );
            } else {
                callable ( iedge.get_weight (),
                           (roadmap_t::_brute_force) ? 0 : successor.h (goal),
                           std::move (successor) );
            }
        }        
    }

   // return the heuristic distance to get from this state to the given goal state.
   // The heuristic function implemented is the air distance according to the
   // cosine law using a value for the earth radius equal to 6530 kms
    int h (const roadmap_t& goal) const;

}; // class roadmap_t

namespace std {

    // Definition of a hash function for instances of the roadmap. The
    // definition is included in the std namespace so that it can be used
    // directly by the functions implemented in that namespace
    template<>
    struct hash<::roadmap_t> {

        // hash function
        size_t operator() (const roadmap_t& right) const {

            // indices are unique and thus use it as the hash value
            return right.get_index ();
        }

    }; // struct hash<npancake_t>
}

#endif // _ROADMAP_T_H_

// Local Variables:
// mode:cpp
// fill-column:80
// End:
