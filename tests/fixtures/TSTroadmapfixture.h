// -*- coding: utf-8 -*-
// TSTroadmapfixture.h
// -----------------------------------------------------------------------------
//
// Started on <vie 13-10-2023 13:46:35.758444242 (1697197595)>
// Carlos Linares López <carlos.linares@uc3m.es>
// Ian Herman <iankherman@gmail.com>

//
// Fixture for testing the roadmap_t class
//

#ifndef _TSTROADMAPFIXTURE_H_
#define _TSTROADMAPFIXTURE_H_

#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <map>
#include <random>
#include <string>
#include <utility>
#include <vector>

#include "gtest/gtest.h"

#include "../TSTdefs.h"
#include "../TSThelpers.h"
#include "../../domains/roadmap/roadmap_t.h"

// Class definition
//
// Defines a Google test fixture for testing the graph_t class
class RoadmapFixture : public ::testing::Test {

protected:

    std::mt19937 rng;
    static graph_t dgraph;
    static graph_t tgraph;
     
     static void SetUpTestSuite () {

         // load the dt and td graphs of NY which are stored in testdata/
         dgraph.load ("testdata/USA-road-d.NY.gr");
         tgraph.load ("testdata/USA-road-t.NY.gr");
     }

    void SetUp () override {

         // just initialize the random seed *in a deterministic way* These
         // are tests so that if a test ever fails, I want for it to fail
         // *again*
         std::mt19937 rng(123456u);
    }

    static void TearDownTestSuite () {

        // remove the binary files generated when loading the dt and td graphs
        auto dbin = std::filesystem::path{get_bin_filename ("testdata/USA-road-d.NY.gr")};
        std::filesystem::remove (dbin);

        auto tbin = std::filesystem::path{get_bin_filename ("testdata/USA-road-t.NY.gr")};
        std::filesystem::remove (tbin);        
    }
    
    // return a random integer value in the range [a, b]
    int dis (int const a, int const b) {
        return std::uniform_int_distribution<int>(a, b) (rng);
    }

    // read the coordinates file of the graph of NY stored in testdata/. Return
    // true if all data could be properly processed and false otherwise. It
    // retrieves all coordinates of all vertices given in the
    // coordinates_filename and stores them in a map indexed by the vertex id
    // that stores the longitude (x-value) and latitude (y-value)
    bool get_coordinates (const std::string& coordinates_filename,
                          std::map<size_t, std::pair<double, double>>& coordinates) {

        std::ifstream stream (coordinates_filename);

        // create regex to process each line separately. Lines in the DIMACS
        // competition format start with a character: 'c' is used for comments; 'p'
        // is used for providing properties; 'v' adds a new vertex. In the
        // following, both 'c' and 'p' are ignored
        std::regex comment ("^[cp].*");

        // Lines starting with 'v' add a new vertex, and other than the prefix, they
        // come with three integers: the vertex id, the longitude (x-value) and the
        // latitude (y-value). Note the longitude and latitude might come with a
        // sign, or not
        std::regex newedge (R"(^v\s+(\d+)\s+([+-]?\d+)\s+([+-]?\d+)\s*$)");

        // parse the contents of the file
        std::string line;
        int lineno = 0;
        while (getline (stream, line)) {

            // skip this line in case it should be ignored
            if (std::regex_match (line, comment)) {

                // increment the line counter and skip it
                lineno++;
                continue;
            }

            // at this point, lines must match the vertex command 'v'
            std::smatch m;
            if (std::regex_match (line, m, newedge)) {

                // add a new vertex to the map using the vertex id as the key and
                // storing a pair with the longitude and latitude. Note that
                // longitude and latitude are given as integers with six digits of
                // precision
                size_t id = stoull (m[1].str ());
                double lon = stoi (m[2].str ()) / 1'000'000.0;
                double lat = stoi (m[3].str ()) / 1'000'000.0;

                // and add these coordinates to the vertex with identifier id with
                // standard degrees
                coordinates[id] = std::make_pair (lon, lat);

                // and add the number of edges processed
                lineno++;

            } else {

                // otherwise, a syntax error has been found
                std::cerr << " Syntax error in '" << coordinates_filename << "'::" << lineno << std::endl;
                return false;
            }
        }

        return true;
    }

    // verify whether the two given vectors of edge_t contain exactly the same
    // data or not
    bool equalEdges (std::vector<edge_t>& v1, std::vector<edge_t>& v2) {

        // first things first, if they do not have the same length then
        // immediately return false
        if (v1.size () != v2.size ()) {
            return false;
        }

        // otherwise, sort both vectors (because maybe its edge_t are not in
        // exactly the same order, still they can be the same vector)
        std::ranges::sort (v1);
        std::ranges::sort (v2);

        // and now verify all items
        for (std::size_t i = 0 ; i < v1.size () ; i++) {
            if (v1[i].get_to () != v2[i].get_to () or
                v1[i].get_weight () != v2[i].get_weight ()) {
                return false;
            }
        }

        // at this point both vectors are provenly equal
        return true;
    }    
};

#endif // _TSTROADMAPFIXTURE_H_

// Local Variables:
// mode:cpp
// fill-column:80
// End:
