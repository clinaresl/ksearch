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

    protected:

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
        // with an empty solution, and only one in spite of the number of
        // solutions requested
        if (_start == _goal) {

            // create then a single solution with no path (and no expansions!)
            std::vector<T> path;
            ksolution += generate_solution (path, 0, signature ());

            // and return
            return ksolution;
        }

        // if the start and goal nodes are different, then create an open list
        // and add the start state to it with a f-value equal to its g-value, 0
        bucket_t<backnode_t<T>> open;
        open.insert (_start, 0);

        // also create a closed list for storing expanded nodes
        closed_t<backnode_t<T>> closed;

        std::vector<T> path;
        ksolution += generate_solution (path, 0, signature ());
        return ksolution;
    }

} // namespace khs


#endif // _KHSMA_H_



// Local Variables:
// mode:cpp
// fill-column:80
// End:
