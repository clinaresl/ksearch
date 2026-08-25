// -*- coding: utf-8 -*-
// KHSbsolver.h
// -----------------------------------------------------------------------------
//
// Started on <mar 22-08-2023 21:37:32.746298308 (1692733052)>
// Carlos Linares López <carlos.linares@uc3m.es>
// Ian Herman <iankherman@gmail.com>   Ian Herman <iankherman@gmail.com>

//
// Definition of a base solver
//

#ifndef _KHSBSOLVER_H_
#define _KHSBSOLVER_H_

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
#include "../structs/KHSsolution_t.h"
#include "../structs/KHSksolution_t.h"

namespace khs {

    template<typename T>
    class bsolver {

    protected:

        // INVARIANT: Any solver of the k shortest-path problem must be able to
        // find k solution paths from a start state with a given heuristic value
        // after expanding a number of nodes and, in case BELA* is used,
        // considering a number of centroids.
        int _k;                                      // number of paths to find
        int _h0;                       // heuristic distance of the start state
        std::size_t _expansions;                   // number of node expansions
        int _z_exp;                            // number of centroid expansions

        // Also, and only in the context of bBELA we are interested in computing
        // the *extra* number of paths that are necessary to get k paths, i.e.,
        // the number of non-simple paths generated while looking for k simple
        // paths. This parameter is reported with value 0 for all the other
        // "simple algorithms"
        int _nbpaths;

        // high-precision measuring time
        std::chrono::time_point<std::chrono::system_clock> _tstart, _tend;

    public:

        // Default constructors are forbidden
        bsolver () = delete;

        // Explicit constructor
        bsolver (const int k) :
            _k { k },
            _h0 { 0 },
            _expansions { 0 },
            _z_exp { 0 },
            _nbpaths { 0 }
            {}

        // Destructor
        virtual ~bsolver () = default;

        // getters
        int get_k () const
            { return _k; }
        int get_h0 () const
            { return _h0; }
        size_t get_expansions () const
            { return _expansions; }
        int nb_z_exp () const
            { return _z_exp; }
        int nb_paths () const
            { return _nbpaths; }
        double get_cpu_time () const
            { return 1e-9*std::chrono::duration_cast<std::chrono::nanoseconds>(_tend - _tstart).count(); }

        // Every solver must be uniquely identified by a signature used for
        // reporting purposes
        virtual const std::string signature () const = 0;

        // the main service of any solver consists of computing the solution of
        // the k-shortest path problem which is returned as a container of k
        // solutions. Importantly, the solutions shall be returned in the same
        // order they are generated!
        virtual ksolution_t<T, std::vector> solve () = 0;

    }; // class bsolver<T>

} // namespace khs

#endif // _KHSBSOLVER_H_

// Local Variables:
// mode:cpp
// fill-column:80
// End:
