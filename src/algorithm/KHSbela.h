// -*- coding: utf-8 -*-
// KHSbela.h
// -----------------------------------------------------------------------------
//
// Started on <vie 11-08-2023 20:54:15.074907774 (1691780055)>
// Carlos Linares López <carlos.linares@uc3m.es>
//

//
// Implementation of Bidirectional Edge-Labeled A* (BELA*)
//

#ifndef _KHSBELA_H_
#define _KHSBELA_H_

#include<chrono>
#include<iostream>
#include<limits>
#include<memory>
#include<set>
#include<tuple>
#include<utility>
#include<vector>

#include "../KHSdefs.h"
#include "../structs/KHSbucket_t.h"
#include "../structs/KHSclosed_t.h"
#include "../structs/KHSlabelednode_t.h"
#include "../structs/KHSsolution_t.h"
#include "../structs/KHSksolution_t.h"

namespace khs {

    // Centroids play a central role in the implementation of BELA*. A centroid
    // is defined as the association between an edge (that exists in CLOSED) and
    // the cost of set of paths. Because the edge has to exist in CLOSED, the
    // pointers to it are stored in the centroid.
    struct centroid_t {

        // INVARIANT: a centroid is defined by an edge and an overall cost. The
        // edge is represented with a couple of pointers of its vertices into
        // CLOSED
        size_t _start;
        size_t _end;
        int _cost;

        // Constructor
        centroid_t (size_t start, size_t end, int cost) :
            _start { start },
            _end { end },
            _cost { cost } {}

        // operators
        bool operator==(const centroid_t& other) const {
            return _start == other._start && _end == other._end &&
                _cost == other._cost;
        }

        // getters
        const size_t& get_start () const { return _start; }
        const size_t& get_end () const { return _end; }
        const int& get_cost () const { return _cost; }
    }; // centroid_t

    template <typename T>
    class bela {

    private:

        // INVARIANT: a solver of the k-shortest path problem computes k paths
        // from a start to a goal. As a result, a number of stats such as the
        // heuristic estimate of the start state, the number of expanded nodes
        // the and CPU running time are reported for every single solution found
        int _k;                                     // number of paths to find
        labelednode_t<T> _start;                                  // start node
        labelednode_t<T> _goal;                                    // goal node
        int _h0;                       // heuristic distance of the start state
        std::size_t _expansions;                        // number of expansions

        // high-precision measuring time
        std::chrono::time_point<std::chrono::system_clock> _tstart, _tend;

    public:

        // Default constructors are forbidden
        bela () = delete;

        // Explicit constructor
        bela (const int k, const T& start, const T& goal) :
            _k { k },
            _start { start },
            _goal { goal },
            _expansions { 0 }
        {

            // compute the heuristic estimate of the start state to the goal
            _h0 = start.h (goal);

            // This implements BELA* with no heuristics and thus, the start node
            // is not updated with the h value
        }

        // getters
        const int get_k () const { return _k; }
        const T& get_start () const { return _start.get_state (); }
        const T& get_goal () const { return _goal.get_state (); }
        const int get_h0 () const { return _h0; }
        const std::size_t get_expansions () const { return _expansions; }
        const double get_cpu_time () const {
            return 1e-9*chrono::duration_cast<chrono::nanoseconds>(_tend - _tstart).count(); }

        // methods

        // The following method provides a convenient wrapping to generate
        // solutions more comfortably
        const solution_t<T> generate_solution (const vector<T>& path,
                                               const int g,
                                               const string& signature) {

            // return a solution with this information
            return solution_t<T> (path,
                                  _start.get_state (),
                                  _goal.get_state (),
                                  _h0,
                                  g,
                                  _expansions,
                                  get_cpu_time (),
                                  signature);
        }

        // every solver must be uniquely identified by a signature
        const string signature () const {
            return "mA";
        }

        // The following service computes all the prefixes of a given centroid.
        // Because all the necessary information is in CLOSED, it has to be
        // passed as an argument also. Note that the CLOSED list might be
        // updated during the process and thus it is not passed as a const
        // reference.
        //
        // The result is given as a vector of paths
        // which, in turn, are defined as a vector of nodes
        std::vector<std::vector<T>> get_prefixes (closed_t<labelednode_t<T>>& closed,
                                                  const centroid_t& centroid);

    }; // class bela<T>

// The following service computes all the prefixes of a given centroid. Because
// all the necessary information is in CLOSED, it has to be passed as an
// argument also. Note that the CLOSED list might be updated during the process
// and thus it is not passed as a const reference.
//
// The result is given as a vector of paths which, in turn, are defined as a
// vector of nodes
template <typename T>
std::vector<std::vector<T>> bela<T>::get_prefixes (closed_t<labelednode_t<T>>& closed,
                                                   const centroid_t& centroid) {

    std::vector<std::vector<T>> prefixes;

    return prefixes;
}



} // namespace khs

#endif // _KHSBELA_H_

// Local Variables:
// mode:cpp
// fill-column:80
// End:
