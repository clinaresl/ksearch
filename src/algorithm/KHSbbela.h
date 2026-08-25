// -*- coding: utf-8 -*-
// KHSbbela.h
// -----------------------------------------------------------------------------
//
// Started on <mar 01-07-2025 15:00:11.859406069 (1751374811)>
// Carlos Linares López <carlos.linares@uc3m.es>
//

//
// Baseline simplistic BELA* for solving the K simple shortest-path problem
//

#ifndef _KHSBBELA_H_
#define _KHSBBELA_H_

#include<algorithm>
#include<cmath>
#include<stack>

#include "../ksearch.h"
#include "KHSbsolver.h"
#include "KHSbela.h"
#include "../structs/KHSlabelednode_t.h"

namespace khs {

    // baseline simplistic BELA* is almost identical to BELA* with the only
    // exception that it rejects paths which are non-simple
    template <typename T>
    class bbela : public bela<T> {

    private:

        // bBELA0/bBELA* has to process all prefixes and suffixes to preserve
        // completeness. However, this might slow down the algorithm
        // significantly. Hence, a parameter n is used so that k^n is set as an
        // upper bound on the number of prefixes or suffixes to generate. In
        // case this bound is reached and there are still simple paths to find,
        // execution is halted. In case n is 0, then a full exploration of
        // prefixes and suffixes is enforced which is the default behaviour.
        int _n = 0;

    public:

        // Default constructors are forbidden
        bbela () = delete;

        // Explicit constructor. Note that the brute-force search variant is
        // used by default. To enable heuristic search use brute_force=false
        bbela (const int k, const T& start, const T& goal,
              const bool brute_force = true) :
            bela<T> { k, start, goal, brute_force }
        {}

        // Destructor
        virtual ~bbela () = default;

        // methods
        void set_n (int value) {
            _n = value;
        }

        // The following method provides a convenient wrapper to generate
        // solutions more comfortably. Note that the difference with regular
        // BELA* is that bbela is required to return simple paths
        const solution_t<T, std::vector> generate_solution (const std::vector<T>& path,
                                                            const int g) {

            // return a solution with this information
            return solution_t<T, std::vector> (bsolver<T>::_k,
                                               path,
                                               bela<T>::_start.get_state (),
                                               bela<T>::_goal.get_state (),
                                               bsolver<T>::_z_exp,
                                               bsolver<T>::_z_gen,
                                               bsolver<T>::_h0,
                                               g,
                                               bsolver<T>::_expansions,
                                               bsolver<T>::get_cpu_time (),
                                               bsolver<T>::_nbpaths,
                                               signature (),
                                               true);
        }

        // every solver must be uniquely identified by a signature
        const std::string signature () const {
            if (bela<T>::_brute_force) {
                return "bBELA0<" + std::to_string (_n) + ">";
            }
            return "bBELA*<" + std::to_string (_n) + ">";
        }

        // Return whether the concatenation of the prefix and the suffix yields
        // a simple path. If so, path contains the concatenation of both
        // subpaths, and otherwise, its contents can be dismissed
        //
        // Because the final path contains true states, the closed list is
        // required to extract them
        bool simple_path (const std::vector<size_t>& prefix,
                          const std::vector<size_t>& suffix,
                          closed_t<labelednode_t<T>>& closed,
                          std::vector<T>& path) const;

        // Given a centroid, return all paths it represents as a solution of
        // the k-shortest path problem. In case a bound is given, no more than
        // bound paths are returned
        //
        // The process: first, needs to access the closed list; second, new
        // centroids might be discovered. This is why the closed list and the sorted
        // bucket of centroids must be specified as well
        ksolution_t<T, std::vector> get_paths (const centroid_t& centroid,
                                               closed_t<labelednode_t<T>>& closed,
                                               bucket_t<centroid_t>& centroids,
                                               size_t bound = std::numeric_limits<size_t>::max ());

        // the main service of this class computes a solution of the k-shortest
        // simple path problem from the start to the goal using one variant of
        // BELA* which overwrites the generation of paths:
        //
        //    * If brute_force is true, then BELA0*, i.e., with no heuristics is
        //      used
        //      
        //    * If brute_force is false, then the heuristic variant of BELA*
        //      with consistent heuristics is employed
        //
        // Importantly, the solutions shall be returned in the same order they
        // are generated!
        ksolution_t<T, std::vector> solve ();

    }; // class bbela<T>

    // Return whether the concatenation of the prefix and the suffix yields a
    // simple path. If so, path contains the concatenation of both subpaths, and
    // otherwise, its contents can be dismissed
    //
    // Because the final path contains true states, the closed list is
    // required to extract them
    template<typename T>
    bool bbela<T>::simple_path (const std::vector<size_t>& prefix,
                                const std::vector<size_t>& suffix,
                                closed_t<labelednode_t<T>>& closed,
                                std::vector<T>& path) const {

        // use an unordered set of integers with amortized constant time for
        // checking whether the concatenation of the prefix and the suffix
        // yields a loopless path
        std::unordered_set<size_t> nodes;

        // the prefix is simple by definition, but it is necessary to add all
        // its nodes to the unordered set to check nodes in the suffix agains
        // them as well
        nodes.insert (prefix.begin (), prefix.end ());

        // because it is simple, add it to the path to return in case the final
        // path is proven to be simple
        for (const auto iprefix: prefix) {
            path.push_back (closed[iprefix].get_state ());
        }

        // next, process all items in the suffix
        for (const auto isuffix : suffix) {

            // first, check whether this node has been visited before or not
            if (nodes.find (isuffix) == nodes.end ()) {

                // then add it to the path
                path.push_back (closed[isuffix].get_state ());

                // and also in the set of visited nodes
                nodes.insert (isuffix);
            } else {

                // otherwise, a repetition has been found!
                return false;
            }
        }

        // at this point, the entire path has been built and it has been proven
        // to be simple
        return true;
    }

    // Given a centroid, return all paths it represeents as a solution of the
    // k-shortest path problem. In case a bound is given, no more than bound
    // paths are returned
    //
    // The process: first, needs to access the closed list; second, new
    // centroids might be discovered. This is why the closed list and the sorted
    // bucket of centroids must be specified as well
    template <typename T>
    ksolution_t<T, std::vector> bbela<T>::get_paths (const centroid_t& centroid,
                                                     closed_t<labelednode_t<T>>& closed,
                                                     bucket_t<centroid_t>& centroids,
                                                     size_t bound) {

        // Update the number of centroids that are used to compute solution
        // paths
        bsolver<T>::_z_exp++;

        // Note that BELA* can not estimate the number of prefixes and suffixes
        // that have to be discored to generate bound paths. The reason is that
        // what paths are non-simple can not be known in advance. Setting the
        // number of prefixes to infinity would render the algorithm too slow.
        //
        // By default, bBELA computes all prefixes and suffixes because it can
        // not know in advance how many paths have to be discarded to get k
        // simple shortest paths. However, this behaviour slows down the
        // algorithm significantly. Instead, bBELA accepts a parameter _n which
        // is used to compute an upper bound on the number of prefixes or
        // suffixes to compute and it is equal to k^n. However, this might
        // result in the loss of paths. In case this bound is reached and still
        // there are paths to find, execution is halted.
        size_t nbsubpaths = (_n == 0) ? std::numeric_limits<size_t>::max () : static_cast<int>(std::pow(bsolver<T>::_k, _n));

        // Every centroid is the representative of a class of paths that get
        // from s to t through it. Their computation is just the cross product
        // of all its prefixes with all its suffixes
        std::vector<std::vector<size_t>> prefixes;
        std::vector<int> prefixMask;
        bela<T>::get_prefixes(closed, centroid, centroids, prefixes, prefixMask, nbsubpaths);

        std::vector<std::vector<size_t>> suffixes;
        std::vector<int> suffixMask;
        bela<T>::get_suffixes(closed, centroid, suffixes, suffixMask, nbsubpaths);

        ksolution_t<T, std::vector> solutions { bsolver<T>::_k, bela<T>::_start.get_state (), bela<T>::_goal.get_state () };
        for (auto i : prefixMask) {
            for (auto j : suffixMask) {

                // check if the concatenation of this prefix and suffix yield a
                // simple path
                std::vector<T> path;
                if (simple_path (prefixes[i], suffixes[j], closed, path)) {

                    // create a single solution with this path and add it to the
                    // collection of solutions to return. Note the cost of every
                    // path is equal to the overall cost of the centroid indeed!
                    bsolver<T>::_tend = std::chrono::system_clock::now ();
                    solutions += generate_solution (path, centroid.get_cost ());

                    // in case the bound has been reached, return the current
                    // solutions
                    if (solutions.size () >= bound) {
                        return solutions;
                    }
                } else {

                    // count the extra number of paths that had to be consider
                    // to get k simple paths
                    bsolver<T>::_nbpaths++;
                }
            }
        }

        // In case that bound prefixes or bound suffixes were generated but the
        // number of required solutions was not found, then throw an exception.
        // Halt execution! The reason is because we might have skipped some
        // paths, too serious to pass unnoticed!!
        if (_n > 0 and
            (prefixMask.size () == nbsubpaths or suffixMask.size () == nbsubpaths)) {
            std::cerr << " # Prefixes: " << prefixMask.size () << std::endl;
            std::cerr << " # Suffixes: " << suffixMask.size () << std::endl;
            throw std::runtime_error{" [bbela<T>::get_paths] Maximum number of prefixes or suffixes reached. Aborting ..."};
        }

        // return the collection of solutions computed so far
        return solutions;
    }

    // the main service of this class computes a solution of the k-shortest
    // simple path problem from the start to the goal using one variant of BELA*
    // which overwrites the generation of paths:
    //
    //    * If brute_force is true, then BELA0*, i.e., with no heuristics is
    //      used
    //      
    //    * If brute_force is false, then the heuristic variant of BELA* with
    //      consistent heuristics is employed
    //
    // Importantly, the solutions shall be returned in the same order they are
    // generated!
    template<typename T>
    ksolution_t<T, std::vector> bbela<T>::solve () {

        // in case _brute_force is given, use the brute-force variant of BELA*,
        // otherwise use the heuristic variant
        if (bela<T>::_brute_force) {
            return bela<T>::solve0 ();
        }
        return bela<T>::solveStar ();
    }

} // namespace khs

#endif // _KHSBBELA_H_

// Local Variables:
// mode:cpp
// fill-column:80
// End:
