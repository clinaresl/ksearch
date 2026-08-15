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

#include <cmath>
#include <numbers>

#include "graph_t.h"

// add a new vertex to the graph. It returns true if the vertex was effectively
// added and false otherwise (e.g., because it exceeds the overall capacity of
// the graph)
bool graph_t::add_vertex (std::size_t const& vertex) {

    // first, ensure the vertex can be allocated both in the container of
    // vertices and the adjacency matrix. If not, return false right away
    if (vertex > _vertices.max_size () ||
        vertex > _edges.max_size()) {
        return false;
    }

    // otherwise, ensure there is space enough to store the new vertex both in
    // the container of vertices and also the adjacency matrix.
    if (vertex >= _vertices.size ()) {
        _vertices.resize (1+vertex);
    }

    // When resizing the adjacency matrix, make sure that the vector stored at
    // this location has space enough to store all neighbours. The reason why we
    // proceed this way is because instead of populating the vector pushing
    // back, we need random access to positions which are available in memory
    if (vertex >= _edges.size ()) {
        _edges.resize (1+vertex, std::vector<edge_t>());
    }
    if (vertex >= _parents.size ()) {
        _parents.resize (1+vertex, std::vector<edge_t>());
    }

    // and return true
    return true;
}

// add a new edge to the graph. Return true if the edge has been effectively
// added and false otherwise ---mostly because it already exists!!
//
// In case the operation is not feasible, it immediately raises an exception.
bool graph_t::add_edge (std::size_t const& from, std::size_t const& to, int weight) {

    // Make sure the from vertex exists
    if (!add_vertex (from)) {
        throw std::range_error ("[graph_t::add_edge] It was not possible to add the starting vertex");
    }

    // even if it is not going to be used by this edge, ensure the to vertex
    // exists as well
    if (!add_vertex (to)) {
        throw std::range_error ("[graph_t::add_edge] It was not possible to add the ending vertex");
    }

    // Now, add both edges, from->to and also to->from in the adjacency lists of
    // successors and parents. If any of these edges already exists, then return
    // false.
    
    // check whether this edge already exists or not and, if not, add it with no resource_usage
    std::vector<edge_t> edges = _edges[from];
    if (find (edges.begin (), edges.end (), edge_t{to, weight}) == edges.end ()) {
        _edges[from].push_back (edge_t {to, weight});

        // also, check whether this parent already exists or not and, if not,
        // add it
        std::vector<edge_t> parents = _parents[to];
        if (find (parents.begin (), parents.end (), edge_t{from, weight}) != parents.end ()) {
            return false;
        }
        _parents[to].push_back (edge_t {from, weight});
    } else {
        return false;
    }

    // and increment the number of edges
    _nbedges++;
    return true;
}

// load a graph from a file with the format of the 9th DIMACS competition,
// and stores the location of each vertex according to the information given
// in the coordinates file whose name is derived from the given filename. It
// returns the number of edges processed.
int graph_t::load (std::string const& filename) {
    
    // Ensure the graph is empty!
    clear ();

    // first, get the coordinates of all vertices. The result is stored in a
    // map, coordinates, which will be used by the successive stages of this
    // process
    // ------------------------------------------------------------------------
    
    // process the coordinates filename
    std::string coordinates_filename = get_coordinates_filename (filename);
    std::ifstream cstream (coordinates_filename);
    std::map<size_t, std::pair<double, double>> coordinates;

    // create regex to process each line separately. Lines in the DIMACS
    // competition format start with a character: 'c' is used for comments; 'p'
    // is used for providing properties; 'v' adds a new vertex. In the
    // following, both 'c' and 'p' are ignored
    std::regex comment ("^[cp].*");

    // Lines starting with 'v' add a new vertex, and other than the prefix, they
    // come with three integers: the vertex id, the longitude (x-value) and the
    // latitude (y-value). Note the longitude and latitude might come with a
    // sign, or not
    std::regex newcoord (R"(^v\s+(\d+)\s+([+-]?\d+)\s+([+-]?\d+)\s*$)");

    // parse the contents of the file
    std::string line;
    int lineno = 0;
    while (getline (cstream, line)) {

        // skip this line in case it should be ignored
        if (std::regex_match (line, comment)) {

            // increment the line counter and skip it
            lineno++;
            continue;
        }

        // at this point, lines must match the vertex command 'v'
        std::smatch m;
        if (std::regex_match (line, m, newcoord)) {

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
            throw std::runtime_error ("[graph_t::load] Syntax error found in the coords file!");
        }
    }
    
    // second, read the contents of the graph
    // ------------------------------------------------------------------------
    std::ifstream stream (filename);

    // Lines starting with 'a' add a new edge, and other than the prefix, they
    // come with three numbers: the vertex from, to and the edge weight. The
    // following regexp is used for processing those lines and, in particular,
    // for extracting its components
    std::regex newedge (R"(^a\s+(\d+)\s+(\d+)\s+(\d+)\s*$)");

    // parse the contents of the file
    lineno = 0;
    while (getline (stream, line)) {

        // skip this line in case it should be ignored
        if (regex_match (line, comment)) {
            continue;
        }

        // at this point, lines must match the add command 'a'
        std::smatch m;
        if (regex_match (line, m, newedge)) {

            // add a new edge from the first match to the second match with the
            // cost weight given third
            size_t from = stoi (m[1].str ());
            size_t to = stoi (m[2].str ());
            int cost = stoi (m[3].str ());

            // add this edge to the graph
            if (add_edge (from, to, cost)) {

                // and store the coordinates of both vertices ---does not matter
                // whether we are overriding or not. In doing so, ensure that the
                // coordinates of all the processed vertices are found
                auto ifrom = coordinates.find (from);
                auto ito = coordinates.find (to);
                if ( ifrom == coordinates.end () ) {
                    throw std::range_error ("[graph_t::load] Missing coordinates in 'from' vertex");
                }
                _vertices[from] = vertex_t{ifrom->second.first, ifrom->second.second};

                if ( ito == coordinates.end () ) {
                    throw std::range_error ("[graph_t::load] Missing coordinates in 'to' vertex");
                }
                _vertices[to] = vertex_t{ito->second.first, ito->second.second};
            }
            
            // and add the number of lines processed which is not equal to the
            // number of edges because there are repeated edges in the input!!
            lineno++;
        } else {

            // otherwise, a syntax error should be raised immediately
            throw std::runtime_error (" Syntax error found in the graph file");
        }
    }

    // Verify that both adjacency lists contain the same number of vertices
    if (_edges.size () != _parents.size ()) {
        throw std::runtime_error{"[graph_t::load] The adjacency lists have a different number of vertices!"};
    }

    // and the same number of edges
    int isuccessors = 0;
    int iparents = 0;
    for (std::size_t ivertex = 0 ; ivertex < _vertices.size () ; ivertex++) {
        isuccessors += _edges[ivertex].size ();
        iparents += _parents[ivertex].size ();
    }
    if (isuccessors != iparents) {
        throw std::runtime_error{"[graph_t::load] The adjacency lists have a different number of edges!"};
    }
    
    // annotate the file processed stripping off the pathname
    _filename = filename;

    // and now create the binary file with the same contents of this graph so
    // that it can be used in the future, which is certainly much faster
    write_binary ();

    // and return the number of edges of the graph, which is not necessarily
    // equal to the number of lines because there are usually duplicated lines
    return get_nbedges ();
}

// retrieve the contents from a binary file and recreates the whole contents
// of a graph from them. It returns true if the file exists and was used to
// deserialize data, and false if the file does not exist. In case the file
// exists but an error was detected, an exception is raised.
//
// The name of the binary file to read is computed automatically from the
// location and name of the input file given
bool graph_t::read_binary (std::string const& filename, bool want_verbose) {
    
    // open the binary file to use
    std::string bfilename = get_bin_filename (filename);
    std::ifstream binstream(bfilename, std::ios::binary);

    // Compute the time it takes to retrieve the entire binary file
    std::chrono::time_point<std::chrono::system_clock> tstart = std::chrono::system_clock::now ();
    
    // if verbose output has been requested, then show the information of this
    // binary file
    if (want_verbose) {
        std::cout << "\n";
        std::cout << " Binary file found  : " << bfilename << "\n";
    }
    
    // Retrieve the information from the header
    graph_record_t header;
    ::read_binary (binstream, header);

    // and store the name of the files used for generating the graph stored in
    // the bin file
    _filename = header._filename;
    
    // next, read as many vertices as were declared in the header
    for (auto i = 0 ; i < header._number_vertices ; i++) {

        // read the next vertex
        vertex_record_t ivertex_record;
        ::read_binary (binstream, ivertex_record);

        // and store it internally
        _vertices.push_back (vertex_t {ivertex_record._longitude, ivertex_record._latitude});
    }

    // in case that verbose output has been requested, show the information of
    // the header
    if (want_verbose) {
        std::cout << "     🖺 Revision     : " << header._revision << "\n";
        std::cout << "     🖪 Filename     : " << header._filename << "\n";
    }

    // next, load the adjacency list of successors, which must contain as many
    // edges as were declared in the header. In passing, recreate also the
    // contents of the adjacency list of parents
    _nbedges = header._number_edges;
    _edges.resize (header._number_vertices);
    _parents.resize (header._number_vertices);
    for (auto i = 0 ; i < header._number_edges ; i++) {

        // read the next edge
        edge_record_t iedge_record;
        ::read_binary (binstream, iedge_record);

        // and store it internally in the adjacency list of successors
        _edges[iedge_record._from].push_back (edge_t{
                iedge_record._to,
                iedge_record._cost
            });

        // and also the adjacency list of parents
        _parents[iedge_record._to].push_back (edge_t{
                iedge_record._from,
                iedge_record._cost
            });
    }

    // to ensure that data has been correctly processed, retrieve the suffix
    graph_suffix_t suffix;
    ::read_binary (binstream, suffix);

    // and check that it is exactly the value expected
    constexpr std::array<char,6> expected_suffix{'S','C','A','M','I','D'};
    if (suffix._suffix != expected_suffix) {
        throw std::runtime_error("[graph_t::read_binary] Invalid graph file suffix!");
    }

    // finally, in case that verbose output was requested, show the number of
    // edges and vertices along with the time it took to read the binary file in
    // seconds
    std::chrono::time_point<std::chrono::system_clock> tend = std::chrono::system_clock::now ();
    if (want_verbose) {
        std::cout << "     # Vertices     : " << header._number_vertices << "\n";
        std::cout << "     # Edges        : " << header._number_edges << "\n";
        std::cout << "    ⏳ Time         : " << 1e-9*std::chrono::duration_cast<std::chrono::nanoseconds>(tend - tstart).count() << " seconds" << "\n"; 
    }
    
    return true;
}

// write the contents of this graph into a binary file. If an error happened
// then an exception is raised. The location and name of the binary file is
// computed automatically from the location and name of the given filename
// used to populate this graph
void graph_t::write_binary () {

    // open/create the binary file to use
    std::string filename = get_bin_filename (_filename);
    std::ofstream binstream(filename, std::ios::binary);
    
    // First of all, write the graph header. The magical prefix is automatically
    // written in the following record.
    graph_record_t header;

    // and also the input .gr files used to populate this graph ---for which the
    // directory component was stripped off
    if (_filename.size () > MAX_PATHNAME_LENGTH) {
        throw std::runtime_error ("[graph_t::write_binary] The path of the primary file is too long");
    }
    std::ranges::copy (_filename.begin (), _filename.end (), header._filename);
    header._filename[_filename.size ()] = '\0';
    
    // Notice that the number of vertices stored is going to be equal to the
    // true number of vertices + 1 since vertices in the format of the 9th
    // DIMACS competition start from 1, but they are stored internally counting
    // from 0, so that the first one is always empty.
    header._number_vertices = _vertices.size ();
    header._number_edges = _nbedges;

    ::write_binary (binstream, header);

    // Next, write down all vertices
    for (vertex_t const& ivertex : _vertices) {

        // fill in the data of this vertex in a new record
        vertex_record_t ivertex_record {
            ._longitude = ivertex._longitude,
            ._latitude = ivertex._latitude,
        };

        // and store it in the binary file
        ::write_binary (binstream, ivertex_record);
    }

    // now the adjacency list of successors. Note that the adjacency list of
    // parents is not saved because it can be retrieved from this one.

    // _edges contains as many entries as vertices, so that for each vertex
    for (std::size_t ifrom = 0 ; ifrom < _edges.size () ; ifrom++) {

        // get all its neighbours
        for (edge_t const& iedge : _edges[ifrom]) {

            // fill in the data of this edge in a new record
            edge_record_t iedge_record {
                ._from = ifrom,
                ._to = iedge._to,
                ._cost = iedge._weight
            };

            // and store it in the binary file
            ::write_binary (binstream, iedge_record);
        }
    }

    // and finally write the suffix
    ::write_binary (binstream, graph_suffix_t{});
}

// Local Variables:
// mode:cpp
// fill-column:80
// End:
