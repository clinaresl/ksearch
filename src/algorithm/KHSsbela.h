// -*- coding: utf-8 -*-
// KHSsbela.cc
// -----------------------------------------------------------------------------
//
// Started on <lun 02-06-2025 12:51:39.647992556 (1748861499)>
// Carlos Linares López <carlos.linares@uc3m.es>
//

//
// Adaptation of Bidirectional Edge-Labeled A* (BELA*) for solving the K simple
// shortest-path problem
//

#ifndef _KHSSBELA_H_
#define _KHSSBELA_H_

#include <unordered_set>
#include <vector>

#include "KHSbela.h"
#include "../structs/KHSclosed_t.h"
#include "../structs/KHSidnode_t.h"
#include "../structs/KHSksolution_t.h"

namespace khs {

    template<typename T>
    class sbela: public bsolver<T> {

        // INVARIANT: a solver of the k simple shortest-path probllem computes k
        // simple (loopless) paths from a start to a goal. As a result, a number
        // of stats such as the heuristic estimate of the start state, the
        // number of expanded nodes, and the CPU running time are reported for
        // every single solution found along with the number of centroids used.
        idnode_t<T> _start;                                       // start node
        idnode_t<T> _goal;                                         // goal node

        // In addition, this solver can be executed as a brute-force search
        // algorithm, or using heuristics
        bool _brute_force;      // whether the brute-force variant is requested

        // Importantly, paths are generated in sequence from the computation of
        // suffixes, and every suffix is given a suffix id (which are stored in
        // 'idnode's). These suffix ids are generated in sequence from a
        // predefined value, initially zero
        size_t _sc;                                           // suffix counter

        // To detect whether some nodes still have suffix ids assigned from
        // previous computations, every computation of simple suffixes is
        // started with a specific suffix id
        size_t _sc0;                                  // initial suffix counter

        // in general, the prefix computation is invoked immediately after the
        // suffix computation, but this is not necessarily true (e.g., when
        // runing unit tests). The following flag signals whether the suffix
        // have been just computed or not
        bool _suffix_precomputed;

    private:

        // The following service computes all *simple* suffixes from the given node
        // which have the specified cost. In case a bound is given, no more than
        // bound suffixes are returned
        //
        // There are three important properties that must be verified by all paths
        // returned by this function:
        //
        // 1. Every suffix must be simple
        //
        // 2. Every suffix must be assigned a unique id
        //
        // 3. Every node belonging to one suffix must be identified with the suffix
        //    id.
        //
        // From the third point, a side effect of computing *simple* suffixes is
        // that the closed list is updated with the suffix ids each node belongs
        // to. In case a node visited by this procedure is found to have old
        // suffix ids, they are removed. This means that suffix ids are
        // permanent labels
        void _get_suffixes(const size_t& ptr,
                           const int cost,
                           closed_t<idnode_t<T>>& closed,
                           std::vector<std::vector<size_t>>& suffixes,
                           std::vector<int> &finishedMask,
                           const size_t bound);

    public:

        // Default constructors are forbidden
        sbela () = delete;

        // Explicit constructor. Note that the brute-force variant is used by
        // default. To enable heuristics use brute_force=false
        sbela (const int k, const T& start, const T& goal,
               const bool brute_force=true) :
            bsolver<T>(k),
            _start                  {                  start },
            _goal                   {                   goal },
            _brute_force            {             brute_force},
            _sc                     {                      0 },
            _sc0                    {                      0 },
            _suffix_precomputed     {                  false }
        {

            // compute the heuristic estimate of the start state to the goal
            bsolver<T>::_h0 = start.h (goal);
        }

        // Destructor
        virtual ~sbela () = default;

        // getters
        [[nodiscard]] const T& get_start () const { return _start.get_state (); }
        [[nodiscard]] const T& get_goal () const { return _goal.get_state (); }
        [[nodiscard]] int get_sc () const { return _sc; }
        [[nodiscard]] int get_sc0 () const { return _sc0; }

        // methods

        // show information of the given centroid on the given output stream.
        // This task can be performed only here because centroids consist of
        // ptrs to locations in closed, and only the solver has access to it
        std::ostream& show_centroid (std::ostream& stream,
                                     centroid_t& z,
                                     closed_t<idnode_t<T>>& closed) const {

            // show first the information of the centroid itself
            stream << "(" << z.get_start () << " -> " << z.get_end() << " / " << z.get_cost () << ")" << std::endl;

            // next, show the information of the start and end nodes
            stream << "\t[" << z.get_start () << "]: " << closed[z.get_start ()] << std::endl;
            stream << "\t[" << z.get_end () << "]: " << closed[z.get_end ()] << std::endl;

            return stream;
        }

        // The following method provides a convenient wrapper to generate
        // solutions more comfortably
        const solution_t<T, std::vector> generate_solution (const std::vector<T>& path,
                                                            const int g) {

            // return a solution (which must be a simple path) with this
            // information
            return solution_t<T, std::vector> (bsolver<T>::_k,
                                               path,
                                               _start.get_state (),
                                               _goal.get_state (),
                                               bsolver<T>::_z_exp,
                                               bsolver<T>::_h0,
                                               g,
                                               bsolver<T>::_expansions,
                                               bsolver<T>::get_cpu_time (),
                                               0,
                                               signature (),
                                               true);
        }

        // every solver must be uniquely identified by a signature
        const std::string signature () const {
            if (_brute_force) {
                return "sBELA0";
            }
            return "sBELA*";
        }

        // The following version of get_prefixes simply provides a wrapper for
        // the unit test cases and is not intended to be used by the search
        // algorithm
        std::vector<std::vector<size_t>> get_prefixes(closed_t<idnode_t<T>> &closed,
                                                      const centroid_t &centroid,
                                                      bucket_t<centroid_t> &centroids,
                                                      std::vector<std::unordered_set<size_t>> &pids,
                                                      const size_t bound = std::numeric_limits<size_t>::max());

        // The following service computes all prefixes of a given centroid. In
        // case a bound is given, no more than bound prefixes are returned.
        //
        // Because all the necessary information is in CLOSED, it has to be
        // passed as an argument also. Note that the CLOSED list might be
        // updated during the process and thus it is not passed as a const
        // reference. In the process, new centroids might be discovered and
        // added to the centroids bucket.
        //
        // The result is given as a vector of paths which, in turn, are defined
        // as a vector of pointers to CLOSED
        void get_prefixes(closed_t<idnode_t<T>> &closed,
                          const centroid_t &centroid,
                          bucket_t<centroid_t> &centroids,
                          std::vector<std::vector<size_t>> &prefixes,
                          std::vector<std::unordered_set<size_t>> &pids,
                          std::vector<int> &finishedMask,
                          const size_t bound = std::numeric_limits<size_t>::max());

        // The following version of get_suffixes simply provides a wrapper for
        // the unit test cases and is not intended to be used by the search
        // algorithm
        std::vector<std::vector<size_t>> get_suffixes(closed_t<idnode_t<T>>& closed,
                                                      const centroid_t& centroid,
                                                      const size_t bound = std::numeric_limits<size_t>::max ());

        // The following service computes all *simple* suffixes of the given
        // centroid. In case a bound is given, no more than bound suffixes are
        // returned
        //
        // Because all the necessary information is in closed, it has to be
        // passed as an argument also
        //
        // Each *simple* suffix is represented as a vector of pointers to closed
        //
        // A side effect of computing *simple* suffixes is that the closed list
        // is updated with the suffix ids each node belong to. In case a node
        // visited by this procedure is found to have old suffix ids, they are
        // removed. This means that suffix ids are permanent labels
        void get_suffixes(closed_t<idnode_t<T>>& closed,
                          const centroid_t& centroid,
                          std::vector<std::vector<size_t>>& suffixes,
                          const size_t bound = std::numeric_limits<size_t>::max ());


        // Given a centroid, return all paths it represeents as a solution of the
        // simple k-shortest path problem. In case a bound is given, no more than
        // bound paths are returned
        //
        // The process: first, needs to access the closed list; second, new
        // centroids might be discovered. This is why the closed list and the sorted
        // bucket of centroids must be specified as well
        ksolution_t<T, std::vector> get_paths (const centroid_t& centroid,
                                               closed_t<idnode_t<T>>& closed,
                                               bucket_t<centroid_t>& centroids,
                                               size_t bound = std::numeric_limits<size_t>::max ());

        // brute-force variant of sBELA*, the so-called sBELA0
        ksolution_t<T, std::vector> solve0 ();

        // heuristic implementation of sBELA* with consistent heuristics
        ksolution_t<T, std::vector> solveStar ();

        // the main service of this class computes a solution of the k simple
        // shortest-path problem from the start to the goal using one variant of
        // sBELA*:
        //
        //    * If brute_force is true, then sBELA0*, i.e., with no heuristics
        //      is used
        //
        //    * If brute_force is false then the heuristic variant of sBELA*
        //      with consistent heuristics is employed
        //
        // Importantly, the solutions shall be returned in the same order they
        // are generated!
        ksolution_t<T, std::vector> solve ();

    }; // class sbela<T>

    // The following version of get_prefixes simply provides a wrapper for
    // the unit test cases and is not intended to be used by the search
    // algorithm
    template <typename T>
    std::vector<std::vector<size_t>> sbela<T>::get_prefixes(closed_t<idnode_t<T>> &closed,
                                                            const centroid_t &centroid,
                                                            bucket_t<centroid_t> &centroids,
                                                            std::vector<std::unordered_set<size_t>> &pids,
                                                            const size_t bound) {

        std::vector<std::vector<size_t>> prefixes;
        std::vector<std::vector<size_t>> validPrefixes;
        std::vector<std::unordered_set<size_t>> prevpids;
        std::vector<int> mask;
        get_prefixes(closed, centroid, centroids, prefixes, prevpids, mask, bound);
        for (auto i : mask) {
            validPrefixes.push_back(prefixes[i]);
            pids.push_back (prevpids[i]);
        }

        return validPrefixes;
    }

    // The following service computes all prefixes of a given centroid. In
    // case a bound is given, no more than bound prefixes are returned.
    //
    // Because all the necessary information is in CLOSED, it has to be
    // passed as an argument also. Note that the CLOSED list might be
    // updated during the process and thus it is not passed as a const
    // reference. In the process, new centroids might be discovered and
    // added to the centroids bucket.
    //
    // The result is given as a vector of paths which, in turn, are defined
    // as a vector of pointers to CLOSED
    template <typename T>
    void sbela<T>::get_prefixes(closed_t<idnode_t<T>> &closed,
                                const centroid_t &centroid,
                                bucket_t<centroid_t> &centroids,
                                std::vector<std::vector<size_t>> &prefixes,
                                std::vector<std::unordered_set<size_t>> &pids,
                                std::vector<int> &finishedMask,
                                const size_t bound) {

        // Prefixes are defined as all *optimal* paths getting to the start
        // vertex of the centroid. The backward g-values of all nodes in the
        // prefix, including the start vertex of the centroid are updated
        // starting with a cost derived from the cost of the edge of the
        // centroid and the overall cost the centroid refers to
        struct params_t {
            int prefixIndex;
            size_t ptr;
            std::unordered_set<size_t> sids;
            int cost;
        };
        typedef struct params_t params_t;

        int gstart = closed[centroid.get_start ()].get_g ();
        std::stack<params_t> recursionStack;
        int numPaths = 0;
        recursionStack.push({0, centroid.get_start(), std::unordered_set<size_t>(), centroid.get_cost() - gstart});
        prefixes.emplace_back();
        pids.emplace_back ();

        while (not recursionStack.empty()) {

            auto [prefixIndex, ptr, sids, cost] = recursionStack.top ();
            recursionStack.pop();

            // The following verification guarantees the algorithm is complete,
            // i.e., that it stops when no more solutions can be found. Note it
            // can be applied only when suffixes have been computed before
            if (_suffix_precomputed and sids.size () == _sc - _sc0) {
                continue;
            }

            prefixes[prefixIndex].push_back (ptr);
            pids[prefixIndex] = sids;

            auto bps = closed[ptr].get_backpointers ();
            if (closed[ptr] == _start) {

                // in case the start has not this backward g-value, then add it
                if (!closed[ptr].find_gb (cost)) {
                    closed[ptr] += cost;

                    // In case it is necessary, add new centroids. Note that any
                    // edge getting to the start state is a centroid because the
                    // optimal cost of the start state is 0. All true edges
                    // start at location 1 because location 0 contains a null
                    // labeled backpointer
                    for (auto i = 1 ; i < bps.size () ; i++) {
                        centroid_t z { bps[i].get_pointer (),
                            ptr,
                            closed[bps[i].get_pointer ()].get_g () + bps[i].get_cost () + cost };
                        centroids.insert (z,
                                          closed[bps[i].get_pointer ()].get_g () + bps[i].get_cost () + cost );
                    }
                }

                // add this node to the path and reverse it so that it correctly
                // starts from the start state
                std::reverse (prefixes[prefixIndex].begin (), prefixes[prefixIndex].end());

                // annotate the index of this prefix and exit if the number of
                // *paths* to compute has been reached. Note that this prefix
                // knows how many paths it contributes to because (_sc - _sc0)
                // is the number of simple suffixes and sids.size () is the
                // number of simple suffixes intersecting with this prefix.
                finishedMask.push_back(prefixIndex);
                if (_suffix_precomputed) {
                    numPaths += (_sc - _sc0 - sids.size ());
                } else {
                    numPaths++;
                }
                if (numPaths >= bound) {
                    break;
                }

                // Haven't you reached it? Keep up the good job! You can do it!!
                continue;
            }

            // general case - this node is not the start state, then follow only
            // the backpointers that lead to optimal paths in case this node has
            // not this backward g-value, then add it, and remember you did
            // because there is an opportunity then to discover new centroids.
            // Note we are sure that the closed list has to be updated because
            // there should be at least one optimal path to this node so that a
            // backward g-value has to be added
            bool new_gb = false;
            if (!closed[ptr].find_gb (cost)) {

                // A very important property is that ptr is never the goal
                // state. Otherwise, a centroid arising from the goal state
                // might be created at this point. This *can not* happen anyway,
                // because the goal state should never be expanded
                closed[ptr] += cost;
                new_gb = true;
            }

            // and consider all labeled backpointers
            bool firstChild = true;
            for (auto& ibp: bps) {

                // Tempting as it is, it is not possible to skip the end vertex
                // of the centroid being expanded right away. The reason is
                // because if this node was given a gb-value (just above), then
                // it will not be verified whether new centroids must be
                // created. Yeah! It might seem that that's what we do not want
                // (i.e., to create a new centroid which just inverts the start
                // and end vertex of this centroid) because that is certainly a
                // non-simple path. And we certainly do not want that but it
                // turns out that the inverted centroid might lead to a simple
                // path: Consider that the computation of the prefixes of a
                // different centroid z' get to this node again (so that a
                // simple path might exist indeed!): Because this node was
                // already given the same gb-value, it will not be updated (and
                // new_gb will be set to false), so that it will not be checked
                // whether new centroids can be created, thus avoiding the
                // discovery of the new centroid.

                // get the parent that can be reached with this backpointer
                idnode_t<T> parent = closed[ibp.get_pointer ()];

                // and also compyute the sufix ids of any path that got to this
                // node
                auto next_sids = sids;
                auto parent_ids = parent.get_pids ();
                if (parent_ids.size () > 0 && parent_ids[0] >= _sc0) {
                    next_sids.insert (parent_ids.begin (), parent_ids.end ());
                }

                // In case this node has to be followed *and* given that it is
                // not the end vertex of the centroid
                if (parent.get_g () + ibp.get_cost () == closed[ptr].get_g () and ibp.get_pointer () != centroid.get_end ()) {
                    if (firstChild) { // Continue with same prefix
                        recursionStack.push(params_t{prefixIndex, ibp.get_pointer(), next_sids, cost + ibp.get_cost()});
                        firstChild = false;
                    } else { // Continue with new prefix
                        int nextPrefix = prefixes.size();
                        prefixes.emplace_back(prefixes[prefixIndex]);
                        pids.emplace_back (next_sids);
                        recursionStack.push(params_t{nextPrefix, ibp.get_pointer(), next_sids, cost + ibp.get_cost()});
                    }
                } else if (parent.get_g () + ibp.get_cost () > closed[ptr].get_g () and new_gb) {

                    // in case this backpointer does not lead to a parent on the
                    // optimal path, then a sidetrack has been discovered.
                    // Moreover, this sidetrack becomes a centroid if and only if a
                    // new backward g-value was added, which represents all
                    // paths with a cost equal to the g*-value of the parent,
                    // plus the cost of this operator and the backward g-value
                    // we just set in this node.
                    //
                    // Note that the start vertex of the centroid z to create
                    // next might be the end vertex of the centroid currently
                    // under consideration!
                    centroid_t z { ibp.get_pointer (), ptr,
                        parent.get_g () + ibp.get_cost () + cost};

                    // and add it to the collection of centroids to process in
                    // case it is necessary to discover new paths
                    centroids.insert (z, parent.get_g () + ibp.get_cost () + cost);
                }
            }
        }
    }

    // The following service computes all *simple* suffixes from the given node
    // which have the specified cost. In case a bound is given, no more than
    // bound suffixes are returned
    //
    // There are three important properties that must be verified by all paths
    // returned by this function:
    //
    // 1. Every suffix must be simple
    //
    // 2. Every suffix must be assigned a unique id
    //
    // 3. Every node belonging to one suffix must be identified with the suffix
    //    id.
    //
    // From the third point, a side effect of computing *simple* suffixes is
    // that the closed list is updated with the suffix ids each node belongs to.
    // In case a node visited by this procedure is found to have old suffix ids,
    // they are removed. This means that suffix ids are permanent labels
    template<typename T>
    void sbela<T>::_get_suffixes(const size_t& ptr,
                                 const int cost,
                                 closed_t<idnode_t<T>>& closed,
                                 std::vector<std::vector<size_t>>& suffixes,
                                 std::vector<int> &finishedMask,
                                 const size_t bound) {

        // definition of the stack that simulates the recursion stack with the
        // parameters of every *invocation*. 'posthoc' is used to simulate the
        // operations that are required by a recursive function *after* solving
        // the recursive call. In case no operations are required to be
        // performed after the *recursive invocation* the top of the stack has
        // to be removed manually.
        struct params_t {
            int suffixIndex;
            size_t ptr;
            int cost;
            bool posthoc;
        }; typedef struct params_t params_t;

        // Populate the stack with the information of the end vertex of the
        // centroid being expanded
        std::stack<params_t> recursionStack;
        int numFinishedSuffixes = 0;
        recursionStack.push({0, ptr, cost, false});
        suffixes.emplace_back();

        // Simulate all recursive calls, i.e., process all items from the
        // recrusion stack until it becomes empty
        while (not recursionStack.empty ()) {

            // Get the first item from the stack
            auto [suffixIndex, ptr, cost, posthoc] = recursionStack.top ();

            // has this node already being processed?
            if (posthoc) {

                // If so, then perform the operations that follow the recursive
                // invocation in a recursive function only then remove it from
                // the recursion stack
                closed[ptr].set_unwound (true);
                recursionStack.pop ();

                // and continue unwinding the recursion stack
                continue;
            } else {

                // Otherwise, remember this node has been already processed
                recursionStack.top ().posthoc = true;
            }

            // base case #0: A loop has been found!
            idnode_t<T> node = closed[ptr];
            if (not node.get_unwound ()) {

                // In this case we do not want the post-hoc operations to take
                // place, hence remove the top of the stack
                recursionStack.pop ();
                continue;
            }

            // Once observed that no loop exists (yet) then it is accepted to
            // process this node adding it to the vector of suffixes. It is
            // first verified whether it contains *old* suffix ids. Suffix ids
            // are old if and only if they are less than the first suffix id
            // used in the current suffix computation. And this is true even if
            // we reached the goal
            suffixes[suffixIndex].push_back(ptr);
            std::vector<size_t> pids = node.get_pids ();
            if (pids.size () > 0 and pids[0] < _sc0) {
                closed[ptr].clear_pids ();
            }

            // base case#1: node is the goal state
            if (node == _goal) {
                finishedMask.push_back(suffixIndex);
                numFinishedSuffixes++;

                // in case the requested number of suffixes has been found,
                // exit!
                if (numFinishedSuffixes >= bound)
                    break;

                // otherwise, keep on searching suffixes
                continue;
            }

            // In preparation to the general case, the current node is annotated
            // to be an ancestor of all nodes *recursively* enumerated from here
            closed[ptr].set_unwound (false);

            // general case: This node is not the goal state, then expand this
            // node and select only those children with backward g-values that
            // decrease accordingly.
            bool firstChild = true;
            T state = node.get_state ();
            state.children (
                0,
                state,
                [&] (int g, int h, T&& successor) {
                    
                    // look for this descendant in CLOSED and very importantly,
                    // verify that (ptr) has been previously expanded to generate
                    // (it) which has been also expanded. Avoiding this verification
                    // leads to errors if the heuristic is inconsistent!
                    auto it = closed.find (successor);
                    if (it != std::string::npos && closed[it].find_bp (ptr)) {

                        // then verify whether this node has a backward g-value
                        // which decreases accordingly
                        if (closed[it].find_gb (cost - g)) {
                            if (firstChild) {

                                // Reuse the same suffix index with the leftmost
                                // child
                                recursionStack.push(params_t{suffixIndex, it, cost - g, false});
                                firstChild = false;
                            } else {

                                // create a new suffix index for the remaining
                                // children
                                int nextPrefix = suffixes.size();
                                suffixes.emplace_back(suffixes[suffixIndex]);
                                recursionStack.push(params_t{nextPrefix, it, cost - g, false});
                            }
                        }
                    }
                });
        }

        // Before leaving, set the suffix ids of all nodes in all suffixes found
        _sc = _sc0;
        for (auto const index : finishedMask) {
            for (auto const& iptr : suffixes[index]) {
                closed[iptr].add_pid (_sc);
            }

            // and increment the suffix counter to enumerate all nodes in the
            // following suffix with it
            _sc++;
        }
    }

    // The following version of get_suffixes simply provides a wrapper for
    // the unit test cases and is not intended to be used by the search
    // algorithm
    template<typename T>
    std::vector<std::vector<size_t>> sbela<T>::get_suffixes(closed_t<idnode_t<T>>& closed,
                                                            const centroid_t& centroid,
                                                            const size_t bound) {

        // call the *official* version and return the suffixes computed
        std::vector<std::vector<size_t>> suffixes;
        get_suffixes (closed, centroid, suffixes, bound);

        return suffixes;
    }

    // The following service computes all *simple* suffixes of the given
    // centroid. In case a bound is given, no more than bound suffixes are
    // returned
    //
    // Because all the necessary information is in closed, it has to be passed
    // as an argument also
    //
    // Each *simple* suffix is represented as a vector of pointers to closed
    //
    // A side effect of computing *simple* suffixes is that the closed list is
    // updated with the suffix ids each node belong to. In case a node visited
    // by this procedure is found to have old suffix ids, they are removed. This
    // means that suffix ids are permanent labels
    template<typename T>
    void sbela<T>::get_suffixes (closed_t<idnode_t<T>>& closed,
                                 const centroid_t& centroid,
                                 std::vector<std::vector<size_t>>& suffixes,
                                 const size_t bound) {

        // suffixes are computed from the end vertex of the centroid. Look for
        // it in closed
        size_t ptr_end = centroid.get_end ();

        // the cost of the suffixes is equal to the (overall) cost of the
        // centroid minus the cost of the sidetrack of the centroid and the cost
        // of any optimal solution to get to the start vertex of the centroid

        // check all the labeled backpointers of the end vertex of the centroid
        // to get the one leading to the start vertex of the centroid. It is
        // assumed that multi-graphs are not in use and thus, there is only one
        // such backpointer
        int edgeCost = 0;
        for (auto& bp : closed[ptr_end].get_backpointers ()) {

            // both, the backpointers and the centroid use pointers to the
            // closed list. Thus, two nodes are equal if they have the same
            // location
            if (bp.get_pointer () == centroid.get_start ()) {
                edgeCost = bp.get_cost ();
                break;
            }
        }

        // compute the cost of any optimal path getting to the start vertex of
        // the centroid
        int gstart = closed[centroid.get_start ()].get_g ();

        // One simply trick, in the end, this manager has to produce simple
        // paths, meaning that no suffix should contain the start vertex of its
        // centroid because it will be used by all prefixes computed with the
        // same centroid. Flagging the start vertex of the centroid suffices to
        // avoid its consideration in the suffix computation
        size_t ptr_start = centroid.get_start ();
        closed[ptr_start].set_unwound (false);

        // Finally, before proceeding, remember the first suffix id that is
        // available for the suffixes to compute next. This is important because
        // any node in closed with parent ids strictly less than it will be
        // discarded by the suffix computation.
        _sc0 = _sc;

        // everything set! compute all suffixes
        std::vector<int> suffixMask;
        std::vector<std::vector<size_t>> new_suffixes;
        _get_suffixes (ptr_end,
                       centroid.get_cost () - gstart - edgeCost,
                       closed,
                       new_suffixes,
                       suffixMask,
                       bound);
        for (auto isuffix : suffixMask) {
            suffixes.push_back (new_suffixes[isuffix]);
        }

        // Do not forget to unflag the start vertex of this centroid
        closed[ptr_start].set_unwound (true);
    }

    // Given a centroid, return all paths it represeents as a solution of the
    // simple k-shortest path problem. In case a bound is given, no more than
    // bound paths are returned
    //
    // The process: first, needs to access the closed list; second, new
    // centroids might be discovered. This is why the closed list and the sorted
    // bucket of centroids must be specified as well
    template <typename T>
    ksolution_t<T, std::vector> sbela<T>::get_paths (const centroid_t& centroid,
                                                     closed_t<idnode_t<T>>& closed,
                                                     bucket_t<centroid_t>& centroids,
                                                     size_t bound) {

        // Update the number of centroids that are used to compute solution
        // paths
        bsolver<T>::_z_exp++;

        // Every centroid is the representative of a class of simple paths that
        // get from s to t through it. Their computation is the cross product of
        // all its prefixes with all its suffixes from which non-simple paths
        // are substracted.
        //
        // Suffixes are computed first, since they populate nodes in closed with
        // a path identifier which is then used by the prefix computation to
        // discard non-simple paths. Prior to the computation of suffixes, it is
        // signaled that suffixes have been just computed.
        _suffix_precomputed = true;
        std::vector<std::vector<size_t>> suffixes;
        get_suffixes(closed, centroid, suffixes);

        // It is tempting to check here whether any suffixes have been
        // generated, because if none has been found then there are certainly no
        // simple paths. But this is unnecessary and the prefix computation will
        // exit immediately when checking that any prefix intersects with *all*
        // suffixes (since there is none!)
        std::vector<std::vector<size_t>> prefixes;
        std::vector<std::unordered_set<size_t>> pids;
        std::vector<int> prefixMask;
        get_prefixes(closed, centroid, centroids, prefixes, pids, prefixMask, bound);

        ksolution_t<T, std::vector> solutions { bsolver<T>::_k, _start.get_state (), _goal.get_state () };
        for (auto i : prefixMask) {
            for (auto j = 0 ; j < int (suffixes.size ()) ; ++j) {

                // Check whether this prefix can be concatenated to this suffix.
                // This is true if and only if this suffix has not visited any
                // node with a suffix id equal to _sc0 + j (because suffixes are
                // computed in the same order they are numbered)
                if (pids[i].find (_sc0+j) == pids[i].end ()) {

                    // compute the concatenation of this prefix and suffix: the
                    // prefix is known to be simple by definition; the suffix is
                    // known to be simple by construction; and it has just been
                    // checked that the prefix and the suffix do not intersect,
                    // hence their concatenation is a simple path.
                    std::vector<T> path;

                    // both the prefix and the suffix are given as pointers to
                    // CLOSED, so that the states are retrieved now, while
                    // generating each solution path
                    for (auto& iprefix : prefixes[i]) {
                        path.push_back (closed[iprefix].get_state ());
                    }
                    for (auto& jsuffix : suffixes[j]) {
                        path.push_back (closed[jsuffix].get_state ());
                    }

                    // create a single solution with this path and add it to the
                    // collection of solutions to reeturn. Note the cost of every
                    // path is equal to the overall cost of the centroid indeed!
                    bsolver<T>::_tend = std::chrono::system_clock::now ();
                    solutions += generate_solution (path, centroid.get_cost ());

                    // in case the bound has been reached, return the current
                    // solutions
                    if (solutions.size () >= bound) {

                        _suffix_precomputed = false;
                        return solutions;
                    }
                }
            }
        }

        // return the collection of solutions computed so far after resetting
        // the suffix enumeration flag
        _suffix_precomputed = false;
        return solutions;
    }

    // brute-force variant of sBELA*, the so-called sBELA0
    template <typename T>
    ksolution_t<T, std::vector> sbela<T>::solve0 () {

        // Start the chrono!
        bsolver<T>::_tstart = std::chrono::system_clock::now ();

        // First things first, create a container to store all solutions found
        ksolution_t<T, std::vector> ksolution{bsolver<T>::_k, _start.get_state (), _goal.get_state ()};

        // In case the start and the goal nodes are the same, return immediately
        // with a single empty solution, and only one in spite of the number of
        // solutions requested. Even if the main loop of sBELA* can recognize
        // this case, it would try to find (k-1) additional solution paths, but
        // this is impossible as no solution path should contain the goal state
        // more than once!
        if (_start == _goal) {

            // create then a single solution with no path (and no expansions!)
            std::vector<T> path;
            bsolver<T>::_tend = std::chrono::system_clock::now ();
            ksolution += generate_solution (path, 0);

            // and return
            return ksolution;
        }

        // if the start and goal nodes are different, then create an open list
        // and add the start state to it with its f-value and no labeled
        // backpointer
        _start += labeledbackpointer_t{std::string::npos, 0};
        bucket_t<idnode_t<T>> open;
        open.insert (_start, 0);

        // also, create a closed list for storing expanded nodes
        closed_t<idnode_t<T>> closed;

        // finally, create a bucket of centroids for storing those that are
        // discovered during the search process
        bucket_t<centroid_t> centroids;

        // iterate utnil the k simple shortest-path problem has been fully
        // solved, or OPEN is exhausted
        while (open.size ()) {

            // take the first node from OPEN
            auto node = open.pop_front ();

            // before expanding any node from OPEN, check whether there are any
            // centroids with a cost less or equal than the current f-value
            auto minz = centroids.get_mini ();
            while (centroids.size () > 0 && minz <= node.get_f () && centroids.size (minz) > 0 ) {

                // add all paths represented by this centroid and add them to
                // the solution of the k shortest-path problem
                auto z = centroids.pop_front ();

                // get all paths represented by this centroid. Use here an upper
                // bound equal to the number of requested paths minus the number
                // of solution paths already found, so that achieving that
                // number it is possible to abort execution and exit with k
                // shortest-paths
                ksolution_t<T, std::vector> solutions = get_paths (z, closed, centroids, bsolver<T>::_k - ksolution.size ());

                // and add them to the solutions already found. In case the
                // requested number of solutions has been already found, then
                // stop the chrono and exit immediately. Job done!
                ksolution += solutions;
                if (ksolution.size () >= bsolver<T>::_k) {
                    return ksolution;
                }

                // and get the overall cost of the next available centroid
                minz = centroids.get_mini ();
            }

            // check whether this node has been expanded before or not
            auto ptr = closed.find (node);

            // in case this is the goal state
            if (node.get_state () == _goal.get_state ()) {

                // in case it has never been expanded
                if (ptr == std::string::npos) {

                    // then add it to the CLOSED list
                    ptr = closed.insert (node);
                } else {

                    // otherwise, update its labeled backpointers
                    closed[ptr] += node.get_backpointer (0);
                }

                // and make sure the goal state has a backward g-value equal to
                // zero
                closed[ptr] += 0;

                // next, add the edge from the parent to the goal as a centroid
                // ---in representation of direct paths from the start state to
                // the goal. Note that the backwards g-value of the goal state
                // must be zero and thus, only one centroid can be generated
                auto parent = closed[node.get_backpointer (0).get_pointer ()];
                centroid_t z { node.get_backpointer (0).get_pointer (),
                    ptr,
                    parent.get_g () + node.get_backpointer (0).get_cost () };
                centroids.insert (z, parent.get_g () + node.get_backpointer (0).get_cost ());

                // finally, skip the expansion of the goal state. This is very
                // important! on one hand, we are never interested in solutions
                // that contain the goal state more than once; on the other
                // hand, skipping the expansion of the goal state ensures that
                // no centroid starting from it will ever be created
                continue;
            }

            // in case it has never been expanded
            if (ptr == std::string::npos) {

                // then add it to CLOSED for the first time. Note that the new
                // node in CLOSED contains only one labeled backpointer, the one
                // stored in OPEN
                ptr = closed.insert (node);
            } else {

                // Otherwise, if the node already exists in CLOSED then add a
                // new labeled back pointer
                closed[ptr] += node.get_backpointer (0);

                // and check whether a centroid has been discovered, i.e.,
                // whether this node already has some gb-values
                auto parent = closed[node.get_backpointer (0).get_pointer ()];
                for (auto ibg : closed[ptr].get_gb ()) {

                    // and add a centroid from its parent to it with this
                    // gb-value
                    auto overall_cost = parent.get_g () + node.get_backpointer (0).get_cost () + ibg;
                    centroid_t z { node.get_backpointer (0).get_pointer (), ptr, overall_cost };
                    centroids.insert (z, overall_cost);
                }

                // and continue, skipping the expansion of this node. Here you
                // are the beauty of sBELA*
                continue;
            }

            // expand this node
            bsolver<T>::_expansions++;
            node.get_state ().children (
                0,
                _goal.get_state (),
                [&] (int g, int h, T&& successor) {
                    // create a new labeled node with this successor
                    idnode_t<T> onode{std::move(successor), 0, node.get_g ()+g};

                    // set the labeled backpointer to the location of its parent
                    onode += labeledbackpointer_t{ptr, g};

                    // and add it to OPEN using the f-value as its index
                    open.insert (std::move (onode), onode.get_f ());                    
                });
        }

        // At this point, the OPEN list has been exhausted so that the only way
        // to find new shortest-paths consists of using the centroids
        while (centroids.size () > 0) {

            // get the next centroid and compute all its paths. Note that, in
            // the process, other centroids are expected to be found, though
            // this might not happen in the case of directed graphs, where the
            // number of paths between two vertices can be bounded
            auto z = centroids.pop_front ();
            ksolution_t<T, std::vector> solutions = get_paths (z, closed, centroids, bsolver<T>::_k - ksolution.size ());

            // and add them to the solutions already found. In case the
            // requested number of solutions has been already found, then
            // stop the chrono and exit immediately. Job done!
            ksolution += solutions;
            if (ksolution.size () >= bsolver<T>::_k) {
                bsolver<T>::_tend = std::chrono::system_clock::now ();
                return ksolution;
            }
        }

        // As noted above, when solving the k-shortest non-simple path problem,
        // the number of solutions returned might not get to k. In this case,
        // there is not much to do other than just returning the number of paths
        // encountered between both vertices (which is paramount to count the
        // number of paths between two vertices)
        return ksolution;
    }

    // heuristic implementation of sBELA* with consistent heuristics
    template <typename T>
    ksolution_t<T, std::vector> sbela<T>::solveStar () {

        // Start the chrono!
        bsolver<T>::_tstart = std::chrono::system_clock::now ();

        // First things first, create a container to store all solutions found
        ksolution_t<T, std::vector> ksolution{bsolver<T>::_k, _start.get_state (), _goal.get_state ()};

        // In case the start and the goal nodes are the same, return immediately
        // with a single empty solution, and only one in spite of the number of
        // solutions requested. Even if the main loop of sBELA* can recognize
        // this case, it would try to find (k-1) additional solution paths, but
        // this is impossible as no solution path should contain the goal state
        // more than once!
        if (_start == _goal) {

            // create then a single solution with no path (and no expansions!)
            std::vector<T> path;
            bsolver<T>::_tend = std::chrono::system_clock::now ();
            ksolution += generate_solution (path, 0);

            // and return
            return ksolution;
        }

        // if the start and goal nodes are different, then create an open list
        // and add the start state to it with its f-value and no labeled
        // backpointer
        _start += labeledbackpointer_t{std::string::npos, 0};
        bucket_t<idnode_t<T>> open;
        _start.set_h (_start.get_state ().h (_goal.get_state ()));
        open.insert (_start, _start.get_state ().h (_goal.get_state ()));

        // also, create a closed list for storing expanded nodes
        closed_t<idnode_t<T>> closed;

        // finally, create a bucket of centroids for storing those that are
        // discovered during the search process
        bucket_t<centroid_t> centroids;

        // iterate utnil the k-shortest path problem has been fully solved, or
        // OPEN is exhausted
        while (open.size ()) {

            // take the first node from OPEN
            auto node = open.pop_front ();

            // before expanding any node from OPEN, check whether there are any
            // centroids with a cost less or equal than the current f-value
            auto minz = centroids.get_mini ();
            while (centroids.size () > 0 && minz <= node.get_f () && centroids.size (minz) > 0 ) {

                // add all paths represented by this centroid and add them to
                // the solution of the k shortest-path problem
                auto z = centroids.pop_front ();

                // get all paths represented by this centroid. Use here an upper
                // bound equal to the number of requested paths minus the number
                // of solution paths already found, so that achieving that
                // number it is possible to abort execution and exit with k
                // shortest-paths
                ksolution_t<T, std::vector> solutions = get_paths (z, closed, centroids, bsolver<T>::_k - ksolution.size ());

                // and add them to the solutions already found. In case the
                // requested number of solutions has been already found, then
                // stop the chrono and exit immediately. Job done!
                ksolution += solutions;
                if (ksolution.size () >= bsolver<T>::_k) {
                    return ksolution;
                }

                // and get the overall cost of the next available centroid
                minz = centroids.get_mini ();
            }

            // check whether this node has been expanded before or not
            auto ptr = closed.find (node);

            // in case this is the goal state
            if (node.get_state () == _goal.get_state ()) {

                // in case it has never been expanded
                if (ptr == std::string::npos) {

                    // then add it to the CLOSED list
                    ptr = closed.insert (node);
                } else {

                    // otherwise, update its labeled backpointers
                    closed[ptr] += node.get_backpointer (0);
                }

                // and make sure the goal state has a backward g-value equal to
                // zero
                closed[ptr] += 0;

                // next, add the edge from the parent to the goal as a centroid
                // ---in representation of direct paths from the start state to
                // the goal. Note that the backwards g-value of the goal state
                // must be zero and thus, only one centroid can be generated
                auto parent = closed[node.get_backpointer (0).get_pointer ()];
                centroid_t z { node.get_backpointer (0).get_pointer (),
                    ptr,
                    parent.get_g () + node.get_backpointer (0).get_cost () };
                centroids.insert (z, parent.get_g () + node.get_backpointer (0).get_cost ());

                // finally, skip the expansion of the goal state. This is very
                // important! on one hand, we are never interested in solutions
                // that contain the goal state more than once; on the other
                // hand, skipping the expansion of the goal state ensures that
                // no centroid starting from it will ever be created
                continue;
            }

            // in case it has never been expanded
            if (ptr == std::string::npos) {

                // then add it to CLOSED for the first time. Note that the new
                // node in CLOSED contains only one labeled backpointer, the one
                // stored in OPEN
                ptr = closed.insert (node);
            } else {

                // Otherwise, if the node already exists in CLOSED then add a
                // new labeled back pointer
                closed[ptr] += node.get_backpointer (0);

                // and check whether a centroid has been discovered, i.e.,
                // whether this node already has some gb-values
                auto parent = closed[node.get_backpointer (0).get_pointer ()];
                for (auto ibg : closed[ptr].get_gb ()) {

                    // and add a centroid from its parent to it with this
                    // gb-value
                    auto overall_cost = parent.get_g () + node.get_backpointer (0).get_cost () + ibg;
                    centroid_t z { node.get_backpointer (0).get_pointer (), ptr, overall_cost };
                    centroids.insert (z, overall_cost);
                }

                // and continue, skipping the expansion of this node. Here you
                // are the beauty of sBELA*
                continue;
            }

            // expand this node
            bsolver<T>::_expansions++;
            node.get_state ().children (
                node.get_h (),
                _goal.get_state (),
                [&] (int g, int h, T&& successor) {
                    // create a new labeled node with this successor
                    idnode_t<T> onode{std::move(successor), h, node.get_g ()+g};
                    
                    // set the labeled backpointer to the location of its parent
                    onode += labeledbackpointer_t{ptr, g};
                    
                    // and add it to OPEN using the f-value as its index
                    open.insert (std::move (onode), onode.get_f ());
                });
        }

        // At this point, the OPEN list has been exhausted so that the only way
        // to find new shortest-paths consists of using the centroids
        while (centroids.size () > 0) {

            // get the next centroid and compute all its paths. Note that, in
            // the process, other centroids are expected to be found, though
            // this might not happen in the case of directed graphs, where the
            // number of paths between two vertices can be bounded
            auto z = centroids.pop_front ();
            ksolution_t<T, std::vector> solutions = get_paths (z, closed, centroids, bsolver<T>::_k - ksolution.size ());

            // and add them to the solutions already found. In case the
            // requested number of solutions has been already found, then
            // stop the chrono and exit immediately. Job done!
            ksolution += solutions;
            if (ksolution.size () >= bsolver<T>::_k) {
                bsolver<T>::_tend = std::chrono::system_clock::now ();
                return ksolution;
            }
        }

        // As noted above, when solving the k-shortest non-simple path problem,
        // the number of solutions returned might not get to k. In this case,
        // there is not much to do other than just returning the number of paths
        // encountered between both vertices (which is paramount to count the
        // number of paths between two vertices)
        return ksolution;
    }

    // the main service of this class computes a solution of the k simple
    // shortest-path problem from the start to the goal using one variant of
    // sBELA*:
    //
    //    * If brute_force is true, then sBELA0*, i.e., with no heuristics
    //      is used
    //
    //    * If brute_force is false then the heuristic variant of sBELA*
    //      with consistent heuristics is employed
    //
    // Importantly, the solutions shall be returned in the same order they
    // are generated!
    template<typename T>
    ksolution_t<T, std::vector> sbela<T>::solve () {

        // in case _brute_force is given, use the brute-force variant of BELA*,
        // otherwise use the heuristic variant
        if (_brute_force) {
            return solve0 ();
        }

        // Otherwise, return the heuristic variant of sBELA* which uses a
        // consistent heuristic
        return solveStar ();
    }

} // namespace khs


#endif // _KHSSBELA_H_


// Local Variables:
// mode:cpp
// fill-column:80
// End:
