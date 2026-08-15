// -*- coding: utf-8 -*-
// TSTsbelafixture.h
// -----------------------------------------------------------------------------
//
// Started on <mié 04-06-2025 14:02:46.396123853 (1749038566)>
// Carlos Linares López <carlos.linares@uc3m.es>
//

//
// Fixture  used for testing the sBELA* search algorithm
//

#ifndef _TSTSBELAFIXTURE_H_
#define _TSTSBELAFIXTURE_H_

#include<algorithm>
#include<ctime>
#include<unordered_set>

#include "gtest/gtest.h"

#include "../TSTdefs.h"
#include "../TSThelpers.h"
#include "../../src/algorithm/KHSbbela.h"
#include "../../src/algorithm/KHSsbela.h"


// Class definition
//
// Defines a Google test fixture for testing the sBELA* search algorithm
class sBELAFixture : public ::testing::Test {

protected:

    void SetUp () override {

        // just initialize the random seed to make sure that every iteration is
        // performed over different random data
        srand (time (nullptr));
    }

    // check that no suffix contains the given starting vertex represented with
    // an int value (which is used like a pointer in CLOSED)
    bool hasStart (std::vector<std::vector<size_t>>& paths, const size_t start) {

        // for all paths
        for (auto i = 0 ; i < paths.size () ; i++) {
            std::vector<size_t> path = paths[i];
            if (auto it = std::find (path.begin (), path.end (), start); it != path.end ()) {
                return true;
            }
        }

        // At this point, no path contains the given start vertex
        return false;
    }

    // check whether all paths given in the vector of vectors with size_t items
    // (which are known to be pointers to a closed list) are all different or
    // not. It is assumed that if two values are different they refer then to
    // different elements in closed and thus, there is no need to pass closed
    bool equalPaths (std::vector<std::vector<size_t>>& paths) {

        // take quadratic time to verify that all paths are different
        for (auto i = 0 ; i < paths.size () ; i++) {
            for (auto j = i+1 ; j < paths.size () ; j++) {
                if (paths[i] == paths[j]) {
                    return true;
                }
            }
        }

        // At this point, all paths have been proven to be different
        return false;
    }

    // verify that every path is correctly identified starting with the value
    // pid0, i.e., that the first path contains nodes with a pid equal to pid0;
    // the second path contains nodes with a pid equal to pid0+1, etc.
    template<typename D>
    bool isIdentified (std::vector<std::vector<size_t>>& paths,
                       khs::closed_t<khs::idnode_t<D>> closed,
                       const size_t pid0) {

        // for every path
        for (auto i = 0 ; i < int (paths.size ()) ; i++) {

            // and all nodes of this path
            for (auto j = 0 ; j < int (paths[i].size ()) ; j++) {

                // check that this node contains a path identifier equal to
                // pid0+i
                auto pids = closed[paths[i][j]].get_pids ();
                if (std::find (pids.begin (), pids.end (), pid0+i) == pids.end ()) {

                    // if not, return false. This path is not correctly
                    // identified
                    return false;
                }
            }
        }

        // At this point all nodes contain correct pids
        return true;
    }

    // determine whether the given path is simple, i.e., it contains no
    // duplicates
    bool isSimple (std::vector<size_t>& path) {

        // create an unordered set to remember those nodes already visited
        std::unordered_set<size_t> visited;
        for (auto i = 0 ; i < int (path.size ()) ; i++) {

            // check if this item has been already visited
            if (auto ptr = visited.find (path[i]); ptr != visited.end ()) {
                return false;
            }

            // and insert this element into the map
            visited.insert (path[i]);
        }

        // At this point all nodes in the path have been found
        return true;
    }

    // check that there are as many pids as paths. Also, check the information
    // in closed to ensure that all pids have been correctly computed, i.e.,
    // that pid[i] consists of the union of all pids found in closed for all
    // nodes in the i-th path. For this, only pids which are strictly greater or
    // equal than the given pid are considered
    template<typename D>
    bool correctPids (const std::vector<std::unordered_set<size_t>>& pids,
                      const std::vector<std::vector<size_t>>& paths,
                      khs::closed_t<khs::idnode_t<D>>& closed,
                      size_t pid0 = 0) {

        // first, ensure there are as many pids as paths
        if (pids.size () != paths.size ()) {
            return false;
        }

        // next, for every path
        for (auto i = 0 ; i < int (paths.size ()) ; ++i) {

            // get the i-th path
            std::vector<size_t> ipath = paths[i];

            // compute all its pids as they are found in closed
            std::unordered_set<size_t> ipids;
            for (size_t ivertex : ipath) {

                // get all pids of this vertex in closed and add them to the set
                // of pids of this path if and only if they are strictly greater
                // or equal than the first pid given
                std::vector<size_t> vpids = closed[ivertex].get_pids ();
                for (auto cpid : vpids) {
                    if (cpid >= pid0) {
                        ipids.insert (cpid);
                    }
                }
            }

            // now, verify that this set and the one corresponding to the i-th
            // path are the same
            if (ipids != pids[i]) {
                return false;
            }
        }

        // at this point, everything is correct
        return true;
    }

    // update the backward g-value of a node in closed
    template <template <typename> class T, typename D>
    void update_gbvalue (const T<D>& node, const int gb, khs::closed_t<T<D>>& closed) {

        // First, look for this node in closed
        size_t ptr = closed.find (node);

        // and add the given backward g-value unless it is already stored
        if (not closed[ptr].find_gb (gb)) {
            closed[ptr] += gb;
        }
    }
};

#endif // _TSTSBELAFIXTURE_H_

// Local Variables:
// mode:cpp
// fill-column:80
// End:
