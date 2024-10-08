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

#include<cmath>
#include<stack>

#include "KHSbsolver.h"
#include "../structs/KHSlabelednode_t.h"

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

        // getters
        const size_t& get_start () const { return _start; }
        const size_t& get_end () const { return _end; }
        const int& get_cost () const { return _cost; }

        // operators
        bool operator==(const centroid_t& other) const {
            return _start == other._start && _end == other._end &&
                _cost == other._cost;
        }
    }; // centroid_t

    template <typename T>
    class bela : public bsolver<T> {

    private:

        // INVARIANT: a solver of the k-shortest path problem computes k paths
        // from a start to a goal. As a result, a number of stats such as the
        // heuristic estimate of the start state, the number of expanded nodes
        // the CPU and running time are reported for every single solution found
        // along with the number of centroids used
        labelednode_t<T> _start;                                  // start node
        labelednode_t<T> _goal;                                    // goal node

        // In addition, this solver can be executed as a brute-force search
        // algorithm, or using heuristics
        bool _brute_force;      // whether the brute-force variant is requested

    public:

        // Default constructors are forbidden
        bela () = delete;

        // Explicit constructor. Note that the brute-force search variant is
        // used by default. To enable heuristic search use brute_force=false
        bela (const int k, const T& start, const T& goal, const bool brute_force = true) :
            bsolver<T>(k),
            _start       { start },
            _goal        { goal },
            _brute_force { brute_force }
        {

            // compute the heuristic estimate of the start state to the goal
            bsolver<T>::_h0 = start.h (goal);

            // This implements BELA* with no heuristics and thus, the start node
            // is not updated with the h value
        }

        // Destructor
        virtual ~bela () = default;

        // getters
        const T& get_start () const { return _start.get_state (); }
        const T& get_goal () const { return _goal.get_state (); }

        // methods

        // show information of the given centroid on the given output stream.
        // This task can be performed only here because centroids consist of
        // ptrs to locations in closed, and only the solver has access to it
        std::ostream& show_centroid (std::ostream& stream,
                                     centroid_t& z,
                                     closed_t<labelednode_t<T>>& closed) const {

            // show first the information of the centroid itself
            stream << "(" << z.get_start () << " -> " << z.get_end() << " / " << z.get_cost () << ")" << std::endl;

            // next, show the information of the start and end nodes
            stream << "\t[" << z.get_start () << "]: " << closed[z.get_start ()] << std::endl;
            stream << "\t[" << z.get_end () << "]: " << closed[z.get_end ()] << std::endl;

            return stream;
        }

        // The following method provides a convenient wrapper to generate
        // solutions more comfortably
        const solution_t<T, vector> generate_solution (const vector<T>& path,
                                                       const int g) {

            // return a solution with this information
            return solution_t<T, vector> (bsolver<T>::_k,
                                          path,
                                          _start.get_state (),
                                          _goal.get_state (),
                                          bsolver<T>::_nbcentroids,
                                          bsolver<T>::_h0,
                                          g,
                                          bsolver<T>::_expansions,
                                          bsolver<T>::get_cpu_time (),
                                          signature ());
        }

        // every solver must be uniquely identified by a signature
        const string signature () const {
            return (_brute_force ? "BELA0" : "BELA*");
        }

        // The following version of get_prefixes simply provides a wrapper for
        // the unit test cases and is not intended to be used by the search
        // algorithm
        std::vector<std::vector<size_t>> get_prefixes(closed_t<labelednode_t<T>> &closed,
                                                      const centroid_t &centroid,
                                                      bucket_t<centroid_t> &centroids,
                                                      const size_t bound = std::numeric_limits<size_t>::max ());

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
        void get_prefixes(closed_t<labelednode_t<T>> &closed,
                          const centroid_t &centroid,
                          bucket_t<centroid_t> &centroids,
                          std::vector<std::vector<size_t>> &prefixes,
                          std::vector<int> &finishedMask,
                          const size_t bound = std::numeric_limits<size_t>::max());

        // The following version of get_prefixes simply provides a wrapper for
        // the unit test cases and is not intended to be used by the search
        // algorithm
        std::vector<std::vector<size_t>> get_suffixes(closed_t<labelednode_t<T>> &closed,
                                                      const centroid_t &centroid,
                                                      const size_t bound = std::numeric_limits<size_t>::max ());

        // The following service computes all suffixes of the given centroid. In
        // case a bound is given, no more than bound suffixes are returned
        //
        // Because all the necessary information is in CLOSED, it has to be
        // passed as an argument also
        //
        // Each suffix is represented as a vector of pointers to CLOSED
        void get_suffixes(closed_t<labelednode_t<T>> &closed,
                          const centroid_t &centroid,
                          std::vector<std::vector<size_t>> &suffixes,
                          std::vector<int> &finishedMask,
                          const size_t bound = std::numeric_limits<size_t>::max ());

        // Given a centroid, return all paths it represents as a solution of
        // the k-shortest path problem. In case a bound is given, no more than
        // bound paths are returned
        //
        // The process: first, needs to access the closed list; second, new
        // centroids might be discovered. This is why the closed list and the sorted
        // bucket of centroids must be specified as well
        ksolution_t<T, vector> get_paths (const centroid_t& centroid,
                                          closed_t<labelednode_t<T>>& closed,
                                          bucket_t<centroid_t>& centroids,
                                          size_t bound = std::numeric_limits<size_t>::max ());

        // brute-force variant of BELA*, the so-called BELA0
        ksolution_t<T, vector> solve0 ();

        // heuristic implementation of BELA*
        ksolution_t<T, vector> solveStar ();

        // the main service of this class computes a solution of the k-shortest
        // path problem from the start to the goal using one variant of BELA*:
        //
        //    * If brute_force is true, then BELA0*, i.e., with no heuristics is
        //      used
        //    * If brute_force is false then the heuristic variant of BELA* is
        //      employed
        //
        // Importantly, the solutions shall be returned in the same order they
        // are generated!
        ksolution_t<T, vector> solve ();

    }; // class bela<T>

    // The following version of get_prefixes simply provides a wrapper for
    // the unit test cases and is not intended to be used by the search
    // algorithm
    template <typename T>
    std::vector<std::vector<size_t>> bela<T>::get_prefixes(closed_t<labelednode_t<T>> &closed,
                                                           const centroid_t &centroid,
                                                           bucket_t<centroid_t> &centroids,
                                                           const size_t bound) {
        std::vector<std::vector<size_t>> prefixes;
        std::vector<std::vector<size_t>> validPrefixes;
        std::vector<int> mask;
        get_prefixes(closed, centroid, centroids, prefixes, mask, bound);
        for (auto i : mask)
            validPrefixes.push_back(prefixes[i]);

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
    void bela<T>::get_prefixes(closed_t<labelednode_t<T>> &closed,
                               const centroid_t &centroid,
                               bucket_t<centroid_t> &centroids,
                               std::vector<std::vector<size_t>> &prefixes,
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
            int cost;
        }; typedef struct params_t params_t;


        int gstart = closed[centroid.get_start ()].get_g ();
        std::stack<params_t> recursionStack;
        int numFinishedPrefixes = 0;
        recursionStack.push({0, centroid.get_start(), centroid.get_cost() - gstart});
        prefixes.emplace_back();

        while (!recursionStack.empty()) {
            int prefixIndex = recursionStack.top().prefixIndex;
            size_t ptr = recursionStack.top().ptr;
            int cost = recursionStack.top().cost;
            recursionStack.pop();

            prefixes[prefixIndex].push_back (ptr);

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
                finishedMask.push_back(prefixIndex);
                numFinishedPrefixes++;
                if (numFinishedPrefixes >= bound)
                    break;
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
                // if this backpointer leads to a parent on the optimal path
                labelednode_t<T> parent = closed[ibp.get_pointer ()];
                if (parent.get_g () + ibp.get_cost () == closed[ptr].get_g ()) {
                    if (firstChild) { // Continue with same prefix
                        recursionStack.push(params_t{prefixIndex, ibp.get_pointer(), cost + ibp.get_cost()});
                        firstChild = false;
                    } else { // Continue with new prefix
                        int nextPrefix = prefixes.size();
                        prefixes.emplace_back(prefixes[prefixIndex]);
                        recursionStack.push(params_t{nextPrefix, ibp.get_pointer(), cost + ibp.get_cost()});
                    }
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
        }
    }

    // The following version of get_prefixes simply provides a wrapper for the
    // unit test cases and is not intended to be used by the search algorithm
    template <typename T>
    std::vector<std::vector<size_t>> bela<T>::get_suffixes(closed_t<labelednode_t<T>> &closed,
                                                           const centroid_t &centroid,
                                                           const size_t bound) {
        std::vector<std::vector<size_t>> suffixes;
        std::vector<std::vector<size_t>> validSuffixes;
        std::vector<int> mask;
        get_suffixes(closed, centroid, suffixes, mask, bound);
        for (auto i : mask)
            validSuffixes.push_back(suffixes[i]);

        return validSuffixes;
    }

    // The following service computes all suffixes of the given centroid. In
    // case a bound is given, no more than bound suffixes are returned
    //
    // Because all the necessary information is in CLOSED, it has to be passed
    // as an argument also
    //
    // Each suffix is represented as a vector of pointers to CLOSED
    template <typename T>
    void bela<T>::get_suffixes(closed_t<labelednode_t<T>> &closed,
                               const centroid_t &centroid,
                               std::vector<std::vector<size_t>> &suffixes,
                               std::vector<int> &finishedMask,
                               const size_t bound) {

        struct params_t {
            int suffixIndex;
            size_t ptr;
            int cost;
        }; typedef struct params_t params_t;

        // Suffixes are defined as any path (non necessarily optimal) getting to
        // the goal from the end vertex of the centroid. To generate them, the
        // algorithm follows the backward g-values decrementing them by the cost
        // of every operator at each step until the goal is found.

        // first, get the cost of the operator used in the centroid. To do this,
        // a lookup in the closed list is necessary. Note that the end vertex of
        // the centroid necessarily exists in CLOSED and thus no checking is
        // performed!
        auto ptr = centroid.get_end ();

        // check all the labeled backpointers of the end vertex of the centroid
        // to get the one leading to the start vertex of the centroid. It is
        // assumed that multi-graphs are not in use and thus, there is only one
        // such backpointer
        int edgeCost = 0;
        for (auto& bp : closed[ptr].get_backpointers ()) {

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

        // Before proceeding, set the backward g-value of the end vertex of this
        // centroid. The initial cost of all suffixes is equal to the overall
        // cost of the centroid minus the optimal cost of the start vertex of
        // the centroid minus the cost of the operator.
        //
        // Centroids are discovered only once and they are used indeed only
        // once. However, the same vertex might be the end vertex of several
        // centroids with the same overall cost! Thus, it is necessary to check
        // that this backward g-value has not been set previously
        int bg = centroid.get_cost () - gstart - edgeCost;
        if (!closed[ptr].find_gb (bg)) {
            closed[ptr] += bg;
        }

        std::stack<params_t> recursionStack;
        int numFinishedSuffixes = 0;
        recursionStack.push({0, ptr, bg});
        suffixes.emplace_back();

        while (!recursionStack.empty()) {
            int suffixIndex = recursionStack.top().suffixIndex;
            ptr = recursionStack.top().ptr;
            int cost = recursionStack.top().cost;
            recursionStack.pop();

            suffixes[suffixIndex].push_back(ptr);

            // base case - node is the goal state
            if (closed[ptr] == _goal) {
                finishedMask.push_back(suffixIndex);
                numFinishedSuffixes++;
                if (numFinishedSuffixes >= bound)
                    break;
                continue;
            }

            // general case - this node is not the goal state, then expand this
            // node and select only those children with backward g-values that
            // decrease accordingly. When expanding this node, both the
            // heuristic value and the true goal are dismished. We are
            // interested only in the true descendants.
            T state = closed[ptr].get_state ();
            vector<tuple<int, int, T>> successors;
            state.children (0, state, successors);

            // and consider all successors from this node
            bool firstChild = true;
            for (auto& successor: successors) {

                // look for this node in CLOSED
                auto it = closed.find (std::get<2>(successor));
                if (it != std::string::npos) {

                    // then verify whether this node has a backward g-value
                    // which decreases accordingly, i.e, that it has a
                    // g-backward value equal to cost minus the cost of the
                    // operator that gets to it
                    if (closed[it].find_gb (cost - std::get<0>(successor))) {
                        if (firstChild) { // Continue with same suffix
                            recursionStack.push(params_t{suffixIndex, it, cost - std::get<0>(successor)});
                            firstChild = false;
                        } else { // Continue with new suffix
                            int nextPrefix = suffixes.size();
                            suffixes.emplace_back(suffixes[suffixIndex]);
                            recursionStack.push(params_t{nextPrefix, it, cost - std::get<0>(successor)});
                        }
                    }
                }
            }
        }
    }

    // Given a centroid, return all paths it represeents as a solution of the
    // k-shortest path problem. In case a bound is given, no more than bound
    // paths are returned
    //
    // The process: first, needs to access the closed list; second, new
    // centroids might be discovered. This is why the closed list and the sorted
    // bucket of centroids must be specified as well
    template <typename T>
    ksolution_t<T, vector> bela<T>::get_paths (const centroid_t& centroid,
                                               closed_t<labelednode_t<T>>& closed,
                                               bucket_t<centroid_t>& centroids,
                                               size_t bound) {

        // Update the number of centroids that are used to compute solution
        // paths
        bsolver<T>::_nbcentroids++;

        // Every centroid is the representative of a class of paths that get
        // from s to t through it. Their computation is just the cross product
        // of all its prefixes with all its suffixes
        std::vector<std::vector<size_t>> prefixes;
        std::vector<int> prefixMask;
        get_prefixes(closed, centroid, centroids, prefixes, prefixMask, bound);

        std::vector<std::vector<size_t>> suffixes;
        std::vector<int> suffixMask;
        get_suffixes(closed, centroid, suffixes, suffixMask, size_t(ceil(double(bound) / double(prefixes.size()))));
        ksolution_t<T, vector> solutions { bsolver<T>::_k, _start.get_state (), _goal.get_state () };
        for (auto i : prefixMask) {
            for (auto j : suffixMask) {

                // compute the concatenation of this prefix and suffix. Note
                // that the starting vertex of the centroid is included in the
                // prefix, and that all suffixes start with its ending vertex,
                // and so there is no need to add it explicitly here
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
                bsolver<T>::_tend = chrono::system_clock::now ();
                solutions += generate_solution (path, centroid.get_cost ());

                // in case the bound has been reached, return the current
                // solutions
                if (solutions.size () >= bound) {
                    return solutions;
                }
            }
        }

        // return the collection of solutions computed so far
        return solutions;
    }

    // brute-force variant of BELA*, the so-called BELA0
    template <typename T>
    ksolution_t<T, vector> bela<T>::solve0 () {

        // Start the chrono!
        bsolver<T>::_tstart = chrono::system_clock::now ();

        // First things first, create a container to store all solutions found
        ksolution_t<T, vector> ksolution{bsolver<T>::_k, _start.get_state (), _goal.get_state ()};

        // In case the start and the goal nodes are the same, return immediately
        // with a single empty solution, and only one in spite of the number of
        // solutions requested. Even if the main loop of BELA* can recognize
        // this case, it would try to find (k-1) additional solution paths, but
        // this is impossible as no solution path should contain the goal state
        // more than once!
        if (_start == _goal) {

            // create then a single solution with no path (and no expansions!)
            std::vector<T> path;
            bsolver<T>::_tend = chrono::system_clock::now ();
            ksolution += generate_solution (path, 0);

            // and return
            return ksolution;
        }

        // if the start and goal nodes are different, then create an open list
        // and add the start state to it with its f-value and no labeled
        // backpointer
        _start += labeledbackpointer_t{string::npos, 0};
        bucket_t<labelednode_t<T>> open;
        open.insert (_start, 0);

        // also, create a closed list for storing expanded nodes
        closed_t<labelednode_t<T>> closed;

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
                ksolution_t<T, vector> solutions = get_paths (z, closed, centroids, bsolver<T>::_k - ksolution.size ());

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
                if (ptr == string::npos) {

                    // then add it to the CLOSED list
                    ptr = closed.insert (node);
                } else {

                    // otherwise, update its labeled backpointers
                    closed[ptr] += node.get_backpointer (0);
                }

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
            if (ptr == string::npos) {

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
                // are the beauty of BELA*
                continue;
            }

            // expand this node
            bsolver<T>::_expansions++;
            vector<tuple<int, int, T>> successors;
            const_cast<T&>(node.get_state ()).children (0, _goal.get_state (), successors);

            // and insert them all in OPEN adding the right labeled backpointers
            for (auto& successor : successors) {

                // create a new labeled node with this successor
                labelednode_t<T> onode{get<2>(successor), 0, node.get_g ()+get<0>(successor)};

                // set the labeled backpointer to the location of its parent
                onode += labeledbackpointer_t{ptr, get<0>(successor)};

                // and add it to OPEN using the f-value as its index
                open.insert (onode, onode.get_f ());
            }
        }

        // At this point, the OPEN list has been exhausted so that the only way
        // to find new shortest-paths consists of using the centroids
        while (centroids.size () > 0) {

            // get the next centroid and compute all its paths. Note that, in
            // the process, other centroids are expected to be found, though
            // this might not happen in the case of directed graphs, where the
            // number of paths between two vertices can be bounded
            auto z = centroids.pop_front ();
            ksolution_t<T, vector> solutions = get_paths (z, closed, centroids, bsolver<T>::_k - ksolution.size ());

            // and add them to the solutions already found. In case the
            // requested number of solutions has been already found, then
            // stop the chrono and exit immediately. Job done!
            ksolution += solutions;
            if (ksolution.size () >= bsolver<T>::_k) {
                bsolver<T>::_tend = chrono::system_clock::now ();
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

    // heuristic implementation of BELA*
    template <typename T>
    ksolution_t<T, vector> bela<T>::solveStar () {

        // Start the chrono!
        bsolver<T>::_tstart = chrono::system_clock::now ();

        // First things first, create a container to store all solutions found
        ksolution_t<T, vector> ksolution{bsolver<T>::_k, _start.get_state (), _goal.get_state ()};

        // In case the start and the goal nodes are the same, return immediately
        // with a single empty solution, and only one in spite of the number of
        // solutions requested. Even if the main loop of BELA* can recognize
        // this case, it would try to find (k-1) additional solution paths, but
        // this is impossible as no solution path should contain the goal state
        // more than once!
        if (_start == _goal) {

            // create then a single solution with no path (and no expansions!)
            std::vector<T> path;
            bsolver<T>::_tend = chrono::system_clock::now ();
            ksolution += generate_solution (path, 0);

            // and return
            return ksolution;
        }

        // if the start and goal nodes are different, then create an open list
        // and add the start state to it with its f-value and no labeled
        // backpointer
        _start += labeledbackpointer_t{string::npos, 0};
        bucket_t<labelednode_t<T>> open;
        _start.set_h (_start.get_state ().h (_goal.get_state ()));
        open.insert (_start, _start.get_state ().h (_goal.get_state ()));

        // also, create a closed list for storing expanded nodes
        closed_t<labelednode_t<T>> closed;

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
                ksolution_t<T, vector> solutions = get_paths (z, closed, centroids, bsolver<T>::_k - ksolution.size ());

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
                if (ptr == string::npos) {

                    // then add it to the CLOSED list
                    ptr = closed.insert (node);
                } else {

                    // otherwise, update its labeled backpointers
                    closed[ptr] += node.get_backpointer (0);
                }

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
            if (ptr == string::npos) {

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
                // are the beauty of BELA*
                continue;
            }

            // expand this node
            bsolver<T>::_expansions++;
            vector<tuple<int, int, T>> successors;
            const_cast<T&>(node.get_state ()).children (node.get_h (), _goal.get_state (), successors);

            // and insert them all in OPEN adding the right labeled backpointers
            for (auto& successor : successors) {

                // create a new labeled node with this successor
                labelednode_t<T> onode{get<2>(successor), get<1>(successor), node.get_g ()+get<0>(successor)};

                // set the labeled backpointer to the location of its parent
                onode += labeledbackpointer_t{ptr, get<0>(successor)};

                // and add it to OPEN using the f-value as its index
                open.insert (onode, onode.get_f ());
            }
        }

        // At this point, the OPEN list has been exhausted so that the only way
        // to find new shortest-paths consists of using the centroids
        while (centroids.size () > 0) {

            // get the next centroid and compute all its paths. Note that, in
            // the process, other centroids are expected to be found, though
            // this might not happen in the case of directed graphs, where the
            // number of paths between two vertices can be bounded
            auto z = centroids.pop_front ();
            ksolution_t<T, vector> solutions = get_paths (z, closed, centroids, bsolver<T>::_k - ksolution.size ());

            // and add them to the solutions already found. In case the
            // requested number of solutions has been already found, then
            // stop the chrono and exit immediately. Job done!
            ksolution += solutions;
            if (ksolution.size () >= bsolver<T>::_k) {
                bsolver<T>::_tend = chrono::system_clock::now ();
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

    // the main service of this class computes a solution of the k-shortest
    // path problem from the start to the goal using one variant of BELA*:
    //
    //    * If brute_force is true, then BELA0*, i.e., with no heuristics is
    //      used
    //    * If brute_force is false then the heuristic variant of BELA* is
    //      employed
    //
    // Importantly, the solutions shall be returned in the same order they
    // are generated!
    template<typename T>
    ksolution_t<T, vector> bela<T>::solve () {

        // in case _brute_force is given, use the brute-force variant of BELA*,
        // otherwise use the heuristic variant
        if (_brute_force) {
            return solve0 ();
        }
        return solveStar ();
    }


} // namespace khs

#endif // _KHSBELA_H_

// Local Variables:
// mode:cpp
// fill-column:80
// End:
