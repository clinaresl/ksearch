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
        // the and CPU running time are reported for every single solution found
        labelednode_t<T> _start;                                  // start node
        labelednode_t<T> _goal;                                    // goal node

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

            // base case - node is the start state
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

                // A very important property is that ptr is never the goal
                // state. Otherwise, a centroid arising from the goal state
                // might be created at this point. This *can not* happen anyway,
                // because the goal state should never be expanded
                closed[ptr] += cost;
                new_gb = true;
            }

            // and consider all labeled backpointers
            for (auto& ibp: bps) {

                // if this backpointer leads to a parent on the optimal path
                labelednode_t<T> parent = closed[ibp.get_pointer ()];
                if (parent.get_g () + ibp.get_cost () == closed[ptr].get_g ()) {

                    // and recursively look for the optimal paths to the parent
                    // node incrementing the backward g-value accordingly
                    path.push_back (closed[ptr].get_state ());
                    auto subprefixes = _get_prefixes (ibp.get_pointer (),
                                                      closed, cost + ibp.get_cost (),
                                                      centroids, path);
                    path.pop_back ();

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

        // return all paths from the designated vertex to the goal. For doing
        // this, use the information stored in the given closed list.
        std::vector<std::vector<T>> _get_suffixes (const size_t ptr,
                                                   closed_t<labelednode_t<T>>& closed,
                                                   int cost,
                                                   std::vector<T>& path) {

            // The enumeration of all paths from the given vertex to the goal is
            // performed in depth-first order, i.e., recursively

            // base case - node is the goal state
            if (closed[ptr] == _goal) {

                // add this node to the path
                path.push_back (closed[ptr].get_state ());

                // and return this solution
                return std::vector<std::vector<T>> { path };
            }

            std::vector<std::vector<T>> suffixes;

            // general case - this node is not the goal state, then expand this
            // node and select only those children with backward g-values that
            // decrease accordingly. When expanding this node, both the
            // heuristic value and the true goal are dismished. We are
            // interested only in the true descendants.
            T state = closed[ptr].get_state ();
            vector<tuple<int, int, T>> successors;
            state.children (0, state, successors);

            // and consider all successors from this node
            path.push_back (closed[ptr].get_state ());
            for (auto& successor: successors) {

                // look for this node in CLOSED
                auto it = closed.find (std::get<2>(successor));
                if (it != std::string::npos) {

                    // then verify whether this node has a backward g-value
                    // which decreases accordingly, i.e, that it has a
                    // g-backward value equal to cost minus the cost of the
                    // operator that gets to it
                    if (closed[it].find_gb (cost - std::get<0>(successor))) {

                        // then continue recursively
                        auto subpaths = _get_suffixes (it, closed, cost - std::get<0>(successor), path);

                        // and add them to the suffixes to return
                        suffixes.insert (suffixes.end (), subpaths.begin (), subpaths.end ());
                    }
                }
            }

            return suffixes;
        }

    public:

        // Default constructors are forbidden
        bela () = delete;

        // Explicit constructor
        bela (const int k, const T& start, const T& goal) :
            bsolver<T>(k),
            _start    { start },
            _goal     { goal }
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
        const solution_t<T> generate_solution (const vector<T>& path,
                                               const int g,
                                               const string& signature) {

            // return a solution with this information
            return solution_t<T> (bsolver<T>::_k,
                                  path,
                                  _start.get_state (),
                                  _goal.get_state (),
                                  bsolver<T>::_h0,
                                  g,
                                  bsolver<T>::_expansions,
                                  bsolver<T>::get_cpu_time (),
                                  signature);
        }

        // every solver must be uniquely identified by a signature
        const string signature () const {
            return "BELA*";
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

        // The following service computes all suffixes of the given centroid.
        // Because all the necessary information is in CLOSED, it has to be
        // passed as an argument also
        std::vector<std::vector<T>> get_suffixes (closed_t<labelednode_t<T>>& closed,
                                                  const centroid_t& centroid);

        // Given a centroid, return all paths it represeents as a solution of
        // the k-shortest path problem. In case a bound is given, no more than
        // bound paths are returned
        //
        // The process: first, needs to access the closed list; second, new
        // centroids might be discovered. This is why the closed list and the sorted
        // bucket of centroids must be specified as well
        ksolution_t<T> get_paths (const centroid_t& centroid,
                                  closed_t<labelednode_t<T>>& closed,
                                  bucket_t<centroid_t>& centroids,
                                  size_t bound = std::numeric_limits<size_t>::max ());

        // the main service of this class computes a solution of the k-shortest
        // path problem from the start to the goal. Importantly, the solutions
        // shall be returned in the same order they are generated!
        ksolution_t<T> solve ();

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
        return _get_prefixes (centroid.get_start(),
                              closed,
                              centroid.get_cost () - gstart,
                              centroids, {});
    }

    // The following service computes all suffixes of the given centroid.
    // Because all the necessary information is in CLOSED, it has to be
    // passed as an argument also
    template <typename T>
    std::vector<std::vector<T>> bela<T>::get_suffixes (closed_t<labelednode_t<T>>& closed,
                                                       const centroid_t& centroid) {

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
        int cost = 0;
        for (auto& bp : closed[ptr].get_backpointers ()) {

            // both, the backpointers and the centroid use pointers to the
            // closed list. Thus, two nodes are equal if they have the same
            // location
            if (bp.get_pointer () == centroid.get_start ()) {
                cost = bp.get_cost ();
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
        int bg = centroid.get_cost () - gstart - cost;
        if (!closed[ptr].find_gb (bg)) {
            closed[ptr] += bg;
        }

        // return all suffixes.
        vector<T> path;
        return _get_suffixes (ptr, closed, bg, path);
    }

    // Given a centroid, return all paths it represeents as a solution of the
    // k-shortest path problem. In case a bound is given, no more than bound
    // paths are returned
    //
    // The process: first, needs to access the closed list; second, new
    // centroids might be discovered. This is why the closed list and the sorted
    // bucket of centroids must be specified as well
    template <typename T>
    ksolution_t<T> bela<T>::get_paths (const centroid_t& centroid,
                                       closed_t<labelednode_t<T>>& closed,
                                       bucket_t<centroid_t>& centroids,
                                       size_t bound) {

        // Every centroid is the representative of a class of paths that get
        // from s to t through it. Their computation is just the cross product
        // of all its prefixes with all its suffixes
        ksolution_t<T> solutions { bsolver<T>::_k, _start.get_state (), _goal.get_state () };
        for (auto& prefix : get_prefixes (closed, centroid, centroids)) {
            for (auto& suffix: get_suffixes (closed, centroid)) {

                // compute the concatenation of this prefix and suffix. Note
                // that the starting vertex of the centroid is included in the
                // prefix, and that all suffixes start with its ending vertex,
                // and so there is no need to add it explicitly here
                std::vector<T> path;
                path.insert (path.end (), prefix.begin (), prefix.end ());
                path.insert (path.end (), suffix.begin (), suffix.end ());

                // create a single solution with this path and add it to the
                // collection of solutions to reeturn. Note the cost of every
                // path is equal to the overall cost of the centroid indeed!
                bsolver<T>::_tend = chrono::system_clock::now ();
                solutions += generate_solution (path, centroid.get_cost (), signature ());

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

    // the main service of this class computes a solution of the k-shortest path
    // problem from the start to the goal. Importantly, the solutions shall be
    // returned in the same order they are generated!
    template <typename T>
    ksolution_t<T> bela<T>::solve () {

        // Start the chrono!
        bsolver<T>::_tstart = chrono::system_clock::now ();

        // First things first, create a container to store all solutions found
        ksolution_t<T> ksolution{bsolver<T>::_k, _start.get_state (), _goal.get_state ()};

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
            ksolution += generate_solution (path, 0, signature ());

            // and return
            return ksolution;
        }

        // if the start and goal nodes are different, then create an open list
        // and add the start state to it with a f-value equal to its g-value, 0
        // and no labeled backpointer
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
            // centroids with a cost less or equal than the f-value value of the
            // current layer
            auto minz = centroids.get_mini ();
            while (centroids.size () > 0 && minz <= node.get_g () && centroids.size (minz) > 0 ) {

                // add all paths represented by this centroid and add them to
                // the solution of the k shortest-path problem
                auto z = centroids.pop_front ();

                // get all paths represented by this centroid. Use here an upper
                // bound equal to the number of requested paths minus the number
                // of solution paths already found, so that if that number is
                // achieved now it is possible to abort execution and exit with
                // k shortest-paths
                ksolution_t<T> solutions = get_paths (z, closed, centroids, bsolver<T>::_k - ksolution.size ());

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

            // in case this is the goal state
            if (node.get_state () == _goal.get_state ()) {

                // first, ensure that the goal state gets also stored in CLOSED
                auto ptr = closed.find (node);

                // in case has never been generated
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
                // must be zero and thus, it is not used in the computation of
                // the overall cost of the new centroid
                auto parent = closed[node.get_backpointer (0).get_pointer ()];
                centroid_t z { node.get_backpointer (0).get_pointer (),
                    ptr,
                    parent.get_g () + node.get_backpointer (0).get_cost () };
                centroids.insert (z, parent.get_g () + node.get_backpointer (0).get_cost ());

                // finally, skip the expansion of the goal state. This is very
                // important! on one hand, we are never interested in solutions
                // that contain the goal state more than once, on the other
                // hand, skipping the expansion of the goal state ensures that
                // no centroid starting from it will ever be created
                continue;
            }

            // check whether this node has been expanded before or not
            auto ptr = closed.find (node);

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
                // whether getting to this node through the edge that created it
                // in OPEN is suboptimal
                auto parent = closed[node.get_backpointer (0).get_pointer ()];
                if (parent.get_g () + node.get_backpointer (0).get_cost () > closed[ptr].get_g ()) {

                    // if, and only if, this node has been used in the
                    // construction of prefixes before, i.e., if and only if it
                    // has at least one backward g-value. Then, create a new
                    // centroid for each backward g-value found
                    for (auto ibg : closed[ptr].get_gb ()) {

                        // in case a centroid has been discovered, then add it
                        // to the collection of centroids to process
                        auto overall_cost = parent.get_g () + node.get_backpointer (0).get_cost () + ibg;
                        centroid_t z { node.get_backpointer (0).get_pointer (), ptr, overall_cost };
                        centroids.insert (z, overall_cost);
                    }
                }

                // and continue, skipping the expansion of this node. Here you
                // are the beauty of BELA*
                continue;
            }

            // expand this node. Note that the heuristic value is dismissed
            bsolver<T>::_expansions++;
            vector<tuple<int, int, T>> successors;
            const_cast<T&>(node.get_state ()).children (0, _goal.get_state (), successors);

            // and insert them all in OPEN adding the right labeled backpointers
            for (auto& successor : successors) {

                // create a new labeled node with this successor. Note that the
                // h value is dismissed
                labelednode_t<T> onode{get<2>(successor), 0, node.get_g ()+get<0>(successor)};

                // set the labeled backpointer to the location of its parent
                onode += labeledbackpointer_t{ptr, get<0>(successor)};

                // and add it to OPEN using f=g
                open.insert (onode, onode.get_g ());
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
            ksolution_t<T> solutions = get_paths (z, closed, centroids, bsolver<T>::_k - ksolution.size ());

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

} // namespace khs

#endif // _KHSBELA_H_

// Local Variables:
// mode:cpp
// fill-column:80
// End:
