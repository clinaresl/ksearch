// -*- coding: utf-8 -*-
// graph_t.h
// -----------------------------------------------------------------------------
//
// Started on <jue 12-10-2023 18:03:55.079429321 (1697126635)>
// Carlos Linares López <carlos.linares@uc3m.es>
//

//
// Definition of weighted directed graphs using adjacency lists.
//

#ifndef _GRAPH_T_H_
#define _GRAPH_T_H_

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <vector>

// Definition of an edge as the ending vertex of the edge and the cost weight
struct edge_t {

    // every edge contains information about the destination vertex and the
    // weight cost. Vertices are identified with their index into the private
    // vector used for storing the entire graph. Weights are assumed to be
    // always integer numbers
    size_t _to;
    int _weight;

    // default constructors are forbidden
    edge_t () = delete;

    // explicit constructor
    edge_t (size_t to, int weight) :
        _to     {     to },
        _weight { weight }
        {}

    // accessors
    const size_t get_to () const { return _to; }
    const int get_weight () const { return _weight; }
}; // struct edge_t

// Definition of a graph as a vector of adjacency lists whose elements store
// edges as defined above
class graph_t {

private:

    // store the number of edges in the graph
    size_t _nbedges;

    // the adjacency list is a vector of vectors of edges
    std::vector<std::vector<edge_t>> _edges;

public:

    // default constructors create an empty collection of vertices and edges
    graph_t () :
        _nbedges {0},
        _edges {std::vector<std::vector<edge_t>>()}
        {}

    // move and copy constructors are forbidden
    graph_t (const graph_t& other) = delete;
    graph_t (graph_t&& other) = delete;

    // getters
    const size_t get_nbedges () const { return _nbedges; }
    const std::vector<edge_t>& get_edges (size_t vertex) const { return _edges[vertex]; }

    // methods

    // add a new vertex to the graph. It returns true if the vertex was
    // effectively added and false otherwise (e.g., because it exceeds the
    // overall capacity of the graph)
    bool add_vertex (size_t vertex);

    // add a new edge to the graph. In case the operation is not feasible, it
    // immediately raises an exception. Otherwise, it returns nothing
    void add_edge (size_t from, size_t to, int weight);

    // clear entire erases the graph
    void clear () {
        _nbedges = 0;
        _edges.clear ();
    }

    // load a graph from a file with the format of the 9th DIMACS competition.
    // It returns the number of edges processed
    int load (const std::string& filename);

}; // class graph_t

#endif // _GRAPH_T_H_

// Local Variables:
// mode:cpp
// fill-column:80

// End:
