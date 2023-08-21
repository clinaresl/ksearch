// -*- coding: utf-8 -*-
// KHSmA.h
// -----------------------------------------------------------------------------
//
// Started on <jue 10-08-2023 16:19:58.329084386 (1691677198)>
// Carlos Linares López <carlos.linares@uc3m.es>
//

//
// Implementation of mA* with and without heuristics
//

#ifndef _KHSMA_H_
#define _KHSMA_H_

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
#include "../structs/KHSbacknode_t.h"
#include "../structs/KHSsolution_t.h"
#include "../structs/KHSksolution_t.h"

namespace khs {

    template<typename T>
    class mA {

    private:

        // INVARIANT: a solver of the k-shortest path problem computes k paths
        // from a start to a goal. As a result, a number of stats such as the
        // heuristic estimate of the start state, the number of expanded nodes
        // and the CPU running time are reported for every single solution found
        int _k;                                     // number of paths to find
        backnode_t<T> _start;                                     // start node
        backnode_t<T> _goal;                                       // goal node
        int _h0;                       // heuristic distance of the start state
        std::size_t _expansions;                        // number of expansions

        // high-precision measuring time
        std::chrono::time_point<std::chrono::system_clock> _tstart, _tend;

    private:

        // return a path of states from the given pointer in closed until the
        // start state (which is recognized because it has a null backwards
        // pointer), and add the specified goal state at the end
        const vector<T> _reconstruct_path (closed_t<backnode_t<T>>& closed,
                                           const backpointer_t& bp,
                                           const T& goal) const {

            // create a vector to store the solution path and also a back
            // pointer to use
            vector<T> path;
            auto backpointer = bp;

            // starting from the backpointer move backwards adding nodes from
            // closed in the solution path until the start state is found
            while (backpointer.get_pointer () != string::npos) {

                // add this node to the solution path
                auto node = closed[backpointer.get_pointer ()];
                path.push_back (node.get_state ());

                // and move backwards in closed carefully selecting the
                // backpointer at the right index
                backpointer = node.get_backpointer (backpointer.get_index ());
            }

            // because the path is reconstructed backwards, reverse it
            reverse (path.begin (), path.end ());

            // and finally add the goal at the end
            path.push_back (goal);
            return path;
        }

    public:

        // Default constructors are forbidden
        mA () = delete;

        // Explicit constructor.
        mA (const int k, const T& start, const T& goal) :
            _k          { k},
            _start      {backnode_t<T> {start}},
            _goal       {backnode_t<T> {goal }},
            _expansions {    0}
            {

                // compute the heuristic distance of the start state to the goal
                _h0 = start.h (goal);

                // This implements mA* with no heuristics and thus, the start
                // node is not updated with its h value
            }

        // getters
        const int get_k () const
            { return _k; }
        const T& get_start () const
            { return _start.get_state (); }
        const T& get_goal () const
            { return _goal.get_state (); }
        const int get_h0 () const
            { return _h0; }
        const size_t get_expansions () const
            { return _expansions; }
        const double get_cpu_time () const
            { return 1e-9*chrono::duration_cast<chrono::nanoseconds>(_tend - _tstart).count(); }

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

        // the main service of this class computes a solution of the k-shortest
        // path problem from the start to the goal
        const ksolution_t<T> solve ();

    }; // class mA<T>

    // the main service of this class computes a solution of the k-shortest path
    // problem from the start to the goal
    template<typename T>
    const ksolution_t<T> mA<T>::solve () {

        // First things first, create a container to store all solutions found
        ksolution_t<T> ksolution{_k, _start.get_state (), _goal.get_state ()};

        // In case the start and the goal nodes are the same, return immediately
        // with a single empty solution, and only one in spite of the number of
        // solutions requested. Even if the main loop of mA* can recognize this
        // case, it would try to find (k-1) additional solution paths, but this
        // is impossible as no solution path should contain the goal state more
        // than once!
        if (_start == _goal) {

            // create then a single solution with no path (and no expansions!)
            std::vector<T> path;
            ksolution += generate_solution (path, 0, signature ());

            // and return
            return ksolution;
        }

        // if the start and goal nodes are different, then create an open list
        // and add the start state to it with a f-value equal to its g-value, 0
        // and no backpointer
        _start += backpointer_t{string::npos, 0};
        bucket_t<backnode_t<T>> open;
        open.insert (_start, 0);

        // also create a closed list for storing expanded nodes
        closed_t<backnode_t<T>> closed;

        // iterate until the k-shortest path problem has been fully solved
        while (true) {

            // take the first node from OPEN
            auto node = open.pop_front ();

            // in case this is the goal state
            if (node.get_state () == _goal.get_state ()) {

                // create a solution and add it to the solution of the
                // k-shortest path problem. Note that nodes in OPEN only have
                // one backpointer, so that is the one used for reconstructing
                // the path
                std::vector<T> path;
                ksolution += generate_solution (_reconstruct_path (closed,
                                                                   node.get_backpointer (0),
                                                                   _goal.get_state ()),
                                                node.get_g (),
                                                signature ());

                // in case we already found k solutions then return
                if (ksolution.size () == _k) {
                    return ksolution;
                }

                // otherwise, proceed with the next one in OPEN
                continue;
            }

            // Check whether this node has been expanded before or not
            auto ptr = closed.find (node);

            // In case it has never been expanded
            if (ptr == string::npos) {

                // Then add it to CLOSED for the first time. Note that the new
                // node in CLOSED contains only one backpointer, the one stored
                // in OPEN
                ptr = closed.insert (node);
            } else {

                // Otherwise, if the node already exists in CLOSED, verify first
                // it has not been expanded more than k times
                backnode_t<T> cnode = closed[ptr];
                if (cnode.get_backpointers ().size () >= _k) {

                    // then skip the expansion of this node
                    continue;
                }

                // otherwise, update its backpointers in CLOSED
                closed[ptr] += node.get_backpointer (0);
            }

            // expand this node. Note that the heuristic value is dismissed
            vector<tuple<int, int, T>> successors;
            const_cast<T&>(node.get_state ()).children (0, _goal.get_state (), successors);

            // and insert them all in OPEN adding the right backpointers
            for (auto& successor : successors) {

                // create a backnode with this successor. Note that the h value
                // is dismissed
                backnode_t<T> onode{get<2>(successor), 0, node.get_g () + get<0>(successor)};

                // set the backpointer to the location of its parent in CLOSED
                // at the last index
                auto bps = closed[ptr].get_backpointers ();
                onode += backpointer_t{ptr, bps.size ()-1};

                // and add it to OPEN using f=g
                open.insert (onode, onode.get_g ());
            }
        }
    }

} // namespace khs


#endif // _KHSMA_H_



// Local Variables:
// mode:cpp
// fill-column:80
// End:
