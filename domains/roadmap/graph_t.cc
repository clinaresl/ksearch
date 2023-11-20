// -*- coding: utf-8 -*-
// graph_t.cc
// -----------------------------------------------------------------------------
//
// Started on <jue 12-10-2023 18:34:16.501618311 (1697128456)>
// Carlos Linares López <carlos.linares@uc3m.es>
// Ian Herman <iankherman@gmail.com>   Ian Herman <iankherman@gmail.com>

//
// Definition of weighted directed graphs using adjacency lists.
//

#include "graph_t.h"

using namespace std;

// add a new vertex to the graph. It returns true if the vertex was effectively
// added and false otherwise (e.g., because it exceeds the overall capacity of
// the graph)
bool graph_t::add_vertex (size_t vertex) {

    // first, ensure the vertex can be allocated. If not, return false right
    // away
    if (vertex > _edges.max_size()) {
        return false;
    }

    // otherwise, just ensure the current vector has space enough to store the
    // new vertex. The reason why we proceed this way is because instead of
    // populating the vector pushing back, we need random access to positions
    // which are available in memory
    if (vertex >= _edges.size ()) {
        _edges.resize (1+vertex, vector<edge_t>());
    }

    // and return true
    return true;
}

// add a new edge to the graph. In case the operation is not feasible, it
// immediately raises an exception. Otherwise, it returns nothing
void graph_t::add_edge (size_t from, size_t to, int weight) {

    // Make sure the from vertex exists
    if (!add_vertex (from)) {
        throw std::range_error ("[graph_t::add_edge] It was not possible to add the starting vertex");
    }

    // even if it is not going to be used by this edge, ensure the to vertex
    // exists as well
    if (!add_vertex (to)) {
        throw std::range_error ("[graph_t::add_edge] It was not possible to add the ending vertex");
    }

    // check whether this edge already exists or not
    vector<edge_t> edges = _edges[from];
    if (find (edges.begin (), edges.end (), edge_t{to, weight}) != edges.end ()) {
        return;
    }

    // next, just simply add a new edge to it
    _edges[from].push_back (edge_t {to, weight});

    // and increment the number of edges
    _nbedges++;
}

// load a graph from a file with the format of the 9th DIMACS competition. It
// returns the number of edges processed
int graph_t::load (const std::string& filename) {

    // First of all, ensure the graph is void
    clear ();

    ifstream stream (filename);

    // create regex to process each line separately. Lines in the DIMACS
    // competition format start with a character: 'c' is used for comments; 'p'
    // is used for providing properties; 'a' adds a new edge to the graph. In
    // the following, both 'c' and 'p' are ignored
    regex comment ("^[cp].*");

    // Lines starting with 'a' add a new edge, and other than the prefix, the
    // come with three numbers: the vertex from, to and the edge weight. The
    // following regexp is used for processing those lines and, in particular,
    // for extracting its components
    regex newedge (R"(^a\s+(\d+)\s+(\d+)\s+(\d+)\s*$)");

    // parse the contents of the file
    string line;
    int lineno = 0;
    while (getline (stream, line)) {

        // skip this line in case it should be ignored
        if (regex_match (line, comment)) {
            continue;
        }

        // at this point, lines must match the add command 'a'
        smatch m;
        if (regex_match (line, m, newedge)) {

            // add a new edge from the first match to the second match with the
            // cost weight given third
            size_t from = stoi (m[1].str ());
            size_t to = stoi (m[2].str ());
            int cost = stoi (m[3].str ());

            // and add this edge to the graph
            add_edge (from, to, cost);

            // and add the number of edges processed
            lineno++;

        } else {

            // otherwise, a syntax error should be raised immediately
            throw runtime_error (" Syntax error found in the graph file");
        }
    }

    // return the number of edges processed, i.e., the number of lines parsed
    return lineno;
}


// Local Variables:
// mode:cpp
// fill-column:80
// End:
