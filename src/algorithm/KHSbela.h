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
    // the overall cost of set of paths it represents. Because the edge has to
    // exist in CLOSED, pointers to it are stored in the centroid.
    struct centroid_t {

        // INVARIANT: a centroid is defined by an edge (qualifed by its start
        // and end vertices) and an overall cost. The edge is represented with a
        // couple of pointers to its vertices onto CLOSED
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

    private:

        // return all optimal paths to the designated node (which is identified
        // by its pointer to the closed list) from the start state. For doing
        // this, use the information stored in the given closed list. In the
        // process of looking for optimal paths, also update the information
        // about centroids, if any is found, and store the new backward
        // g-values. The cost given last is used for this purpose indeed.
        std::vector<std::vector<T>> _get_prefixes (const size_t ptr,
                                                   closed_t<labelednode_t<T>>& closed,
                                                   int cost,
                                                   bucket_t<centroid_t>& centroids,
                                                   std::vector<T> path) {

            // The enumeration of optimal paths is performed in depth-first
            // order, i.e., recursively

            // base case - node is the start state. The start state is
            // recognized in closed because it has a null labeled backpointer
            // stored in the first location
            auto bps = closed[ptr].get_backpointers ();
            if (bps.size () > 0 && bps[0].get_pointer () == std::string::npos) {

                // in case the start has not this backward g-value, then add it
                if (!closed[ptr].find_gb (cost)) {
                    closed[ptr] += cost;
                }

                // add this node to the path and reverse it so that it correctly
                // starts from the start state
                path.push_back (closed[ptr].get_state ());
                std::reverse (path.begin (), path.end ());

                // and return this solution
                return std::vector<std::vector<T>> { path };
            }

            // general case - this node is not the start state, then follow only
            // the backpointers that lead to optimal paths
            std::vector<std::vector<T>> prefixes;

            // in case this node has not this backward g-value, then add it, and
            // remember you did because there is an opportunity then to discover
            // new centroids. Note we are sure that the closed list has to be
            // updated because there should be at least one optimal path to this
            // node so that a backward g-value has to be added
            bool new_gb = false;
            if (!closed[ptr].find_gb (cost)) {
                closed[ptr] += cost;
                new_gb = true;
            }

            // add this node to the path
            path.push_back (closed[ptr].get_state ());

            // and consider all labeled backpointers
            for (auto& ibp: bps) {

                // if this backpointer leads to a parent on the optimal path
                labelednode_t<T> parent = closed[ibp.get_pointer ()];
                if (parent.get_g () + ibp.get_cost () == closed[ptr].get_g ()) {

                    // and recursively look for the optimal paths to the parent
                    // node incrementing the backward g-value accordingly
                    auto subprefixes = _get_prefixes (ibp.get_pointer (),
                                                      closed, cost + ibp.get_cost (),
                                                      centroids, path);

                    // add the optimal paths to the prefixes
                    prefixes.insert (prefixes.end (),
                                     subprefixes.begin (),
                                     subprefixes.end ());
                }
                else if (new_gb) {

                    // in case this backpointer does not lead to a parent on the
                    // optimal path, then a bridge has been discovered.
                    // Moreover, this bridge becomes a centroid if and only if a
                    // new backward g-value was added, which represents all
                    // paths with a cost equal to the g*-value of the parent,
                    // plus the cost of this operator and the backward g-value
                    // we just set in this node
                    centroid_t z { ibp.get_pointer (), ptr,
                                   parent.get_g () + ibp.get_cost () + cost};

                    // and add it to the collection of centroids to process in
                    // case it is necessary to discover new paths
                    centroids.insert (z, parent.get_g () + ibp.get_cost () + cost);
                }
            }

            // and return all prefixes found
            return prefixes;
        }

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

        // The following service computes all prefixes of a given centroid.
        // Because all the necessary information is in CLOSED, it has to be
        // passed as an argument also. Note that the CLOSED list might be
        // updated during the process and thus it is not passed as a const
        // reference. In the process, new centroids might be discovered and
        // added to the centroids bucket.
        //
        // The result is given as a vector of paths which, in turn, are defined
        // as a vector of nodes
        std::vector<std::vector<T>> get_prefixes (closed_t<labelednode_t<T>>& closed,
                                                  const centroid_t& centroid,
                                                  bucket_t<centroid_t>& centroids);

    }; // class bela<T>

    // The following service computes all the prefixes of a given centroid.
    // Because all the necessary information is in CLOSED, it has to be passed
    // as an argument also. Note that the CLOSED list might be updated during
    // the process and thus it is not passed as a const reference.
    //
    // The result is given as a vector of paths which, in turn, are defined as a
    // vector of nodes
    template <typename T>
    std::vector<std::vector<T>> bela<T>::get_prefixes (closed_t<labelednode_t<T>>& closed,
                                                       const centroid_t& centroid,
                                                       bucket_t<centroid_t>& centroids) {

        // Prefixes are defined as all *optimal* paths getting to the start
        // vertex of the centroid. The backward g-values of all nodes in the
        // prefix, including the start vertex of the centroid are updated
        // starting with a cost derived from the cost of the edge of the
        // centroid and the overall cost the centroid refers to
        int gstart = closed[centroid.get_start ()].get_g ();
        auto prefixes = _get_prefixes (centroid.get_start(),
                                       closed,
                                       centroid.get_cost () - gstart,
                                       centroids, {});

        return prefixes;
    }

} // namespace khs

#endif // _KHSBELA_H_

// Local Variables:
// mode:cpp
// fill-column:80
// End:
