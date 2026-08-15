// -*- coding: utf-8 -*-
// graph_t.h
// -----------------------------------------------------------------------------
//
// Started on <jue 12-10-2023 18:03:55.079429321 (1697126635)>
// Carlos Linares López <carlos.linares@uc3m.es>
// Ian Herman <iankherman@gmail.com>   Ian Herman <iankherman@gmail.com>

//
// Definition of weighted directed graphs using adjacency lists.
//

#ifndef _GRAPH_T_H_
#define _GRAPH_T_H_

#include <algorithm>
#include <array>
#include <cmath>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <numbers>
#include <regex>
#include <string>
#include <vector>

// Definitions
// ----------------------------------------------------------------------------

// maximum length for various fields given in the records that
// serialize/deserialize data
constexpr int MAX_PATHNAME_LENGTH = 1024;

// earth radius used to compute the haversine function
constexpr int EARTH_RADIUS = 6'350'000;

// regular expression used to process the names of the files of the 9th DIMACS
// Shortest Path Challenge
constexpr auto dimacs_regex = R"(^USA-road-([td])\.([A-Z]+)\.gr$)";

// Free functions
// ----------------------------------------------------------------------------

// Return the radians of a given number of decimal degrees
inline double toRadians(double deg) {
    return deg * std::numbers::pi_v<double> / 180.0;
}

// Haversine distance between two points given by their (latitude, longitude) as
// decimal degrees
inline double haversine(double lat1, double lon1, double lat2, double lon2) {
    double dLat = toRadians(lat2 - lat1);
    double dLon = toRadians(lon2 - lon1);
    double a = std::pow(std::sin(dLat / 2), 2) +
               std::cos(toRadians(lat1)) * std::cos(toRadians(lat2)) *
               std::pow(std::sin(dLon / 2), 2);
    double c = 2 * std::asin(std::sqrt(a));
    return EARTH_RADIUS * c;
}

// write the given value to an output binary stream
template <typename T>
void write_binary(std::ostream& os, const T& value) {
    os.write(reinterpret_cast<const char*>(&value), sizeof(T));
    if (!os) {
        throw std::runtime_error("[graph_t/write_binary] Failed to write binary data");
    }
}

// Retrieve the following value from the input binary stream
template <typename T>
void read_binary(std::istream& is, T& value) {
    is.read(reinterpret_cast<char*>(&value), sizeof(T));
    if (!is) {
        throw std::runtime_error("[graph_t/read_binary] Failed to read binary data");
    }
}

// return the name of the coordinates filename. In case the file does not exist
// an exception is raised.
inline std::string get_coordinates_filename (const std::string& filename) {

    // the filename of the coordinates file follows the name of the distance
    // graph, i.e., the coordinates file is always given using "-d", so that if
    // the filename choosen was travel time, it is necessary to substitute it
    // properly. Thus, substitute "-t" by "-d" in the filename in case it is
    // found
    std::string result {filename};
    auto pos = result.find("-t.");
    if (pos != std::string::npos)
        result[pos + 1] = 'd';    

    // replace the extension of the graph name by "co"
    auto path = std::filesystem::path (result);
    auto cofile = path.replace_extension("co");

    // in case the file does not exist, raise an exception
    if (not std::filesystem::exists (cofile)) {
        throw std::runtime_error ("[graph_t/get_coordinates_filename] The coordinates filename does not exist");
    }

    // otherwise, return the name of the coordinates file
    return cofile.string ();
}

// compute the name of the binary file encoding the information generated after
// processing the data in the given input filename. The name chosen adheres to
// the conventions of the 9th DIMACS Shortest-Path Challenge. Also, the binary
// file appeaers located at the directory where the primary file resides
inline const std::string get_bin_filename (std::string const& filename) {

    // get the filename
    std::string pfilename = std::filesystem::path (filename).filename ().string ();
    
    // get the metric and city/region name of both files
    std::regex filename_regex (dimacs_regex);
    std::smatch pmatches;
    if (not std::regex_search (pfilename, pmatches, filename_regex)) {
        throw std::runtime_error ("[graph_t/get_bin_filename] The file given does not adhere to the naming conventions of the 9th DIMACS Shortest Path Challenge");
    }

    // The base filename will be constructed according to the format of the 9th
    // DIMACS Shortest-Path Challenge but placing the t/d in the order specified
    // by the primary and resource filenames
    std::string basename = "USA-road-" + std::string{pmatches[1]} + "." + std::string{pmatches[2]} + ".bin";

    // and add to this file the path of the given filename
    auto pathname = std::filesystem::path {filename}.parent_path().lexically_normal();

    // concatenate both and return a string
    return (pathname / basename).string();
}

// Classes
// ----------------------------------------------------------------------------

// Definition of the record of a vertex used for serializing/deserializing
struct vertex_record_t {

    // Each vertex is located at a specific longitude and latitude which is
    // originally given as an int. The resulting double value is obtained
    // dividing by 10^6. Since the procedure followed to generate it will be
    // exactly the same to store it and retrieve it we should get a bit-by-bit
    // perfect copy of the original double
    double _longitude;
    double _latitude;
}; // vertex_record_t

// Definition of a vertex as a pair of longitude (x-value) and latitude
// (y-value)
struct vertex_t {

    // every vertex contains information about its position in the plane
    // measured in standard decimal degrees
    double _longitude;
    double _latitude;

    // default constructor
    vertex_t () :
        _longitude { 0.0 },
        _latitude  { 0.0 }
        {}

    // explicit constructor
    vertex_t (double longitude, double latitude) :
        _longitude { longitude },
        _latitude  { latitude  }
        {}

    // accessors
    [[nodiscard]] double get_longitude () const { return _longitude; }
    [[nodiscard]] double get_latitude () const { return _latitude; }

    // operators
    friend std::ostream& operator<<(std::ostream& stream, const vertex_t& value) {
        stream << "(" << std::fixed << std::setprecision (6) << value.get_latitude () << ", " << value.get_longitude () << ")";
        return stream;
    }

    // Methods

    // return the haversine distance from this vertex to another one
    double haversine (vertex_t const& to) {
        return ::haversine (_latitude, _longitude, to.get_latitude (), to.get_longitude ());
    }

}; // struct vertex_t

// Definition of the record of an edge used for serializing/deserializing
struct edge_record_t {

    // for every edge both the from and to vertices of the edge are recorded
    std::size_t _from;
    std::size_t _to;

    // and also the cost of every edge
    int _cost;
}; // edge_record_t

// Definition of an edge as the ending vertex of the edge and the cost weight
struct edge_t {

    // every edge contains information about the destination vertex and the
    // weight cost. Vertices are identified with their index into the private
    // vector used for storing the entire graph. Weights are assumed to be
    // always integer numbers
    size_t _to;
    int _weight;

    // explicit constructor
    edge_t (std::size_t to, int weight) :
        _to     {     to },
        _weight { weight }
        {}

    // accessors
    [[nodiscard]] size_t get_to () const { return _to; }
    [[nodiscard]] int get_weight () const { return _weight; }

    // operators

    // Two edges are the same if they go from the same initial vertex to the
    // same final vertex in spite of the cost. The costs can not be added to the
    // eq comparison because it is used when adding new edges and, at the time,
    // the costs are unknown. Also, because multigraphs are not considered, the
    // costs are removed when doing lt/gt because there should be only one edge
    // with the same destination.
    std::strong_ordering operator<=>(edge_t const& other) const {
        return (_to <=> other.get_to ());
    }
    bool operator==(edge_t const& other) const {
        return _to == other.get_to ();
    }

    friend std::ostream& operator<<(std::ostream& stream, const edge_t& value) {

        stream << "<" << value.get_to () << ", " << value.get_weight ();
        return stream;
    }

}; // struct edge_t

// Definition of the record of a graph used for serializing/deserializing
struct graph_record_t {

    // first of all, prepend a magic list of numbers used to identify this file
    // and also a version number (to keep track of changes along time)
    std::array<char, 6> _prefix{'D','I','M','A','C','S'};

    // current version is 2 because information about the parents is also
    // included now
    int _revision = 2;

    // Store also the name of the .gr file used to populate a specific graph
    char _filename[MAX_PATHNAME_LENGTH];
    
    // The following two attributes are used to access the binary file for
    // reading both the vertices and edges
    int _number_vertices = 0;
    int _number_edges = 0;
}; // graph_record_t

// To ensure that all data has been correctly processed add a magic suffix at
// the end of the binary file
struct graph_suffix_t {
    std::array<char, 6> _suffix{'S','C','A','M','I','D'};    
};

// Definition of a graph as a vector of adjacency lists whose elements store
// edges as defined above
class graph_t {

private:

    // INVARIANT: A graph consists of a container of vertices each with its own
    // adjacency list. The definition also stores the total number of edges
    // separately

    // store the number of edges in the graph
    size_t _nbedges;

    // the adjacency list is a vector of vectors of edges
    std::vector<std::vector<edge_t>> _edges;

    // also another adjacency list is required to store the parents of each node
    std::vector<std::vector<edge_t>> _parents;

    // vertices are indexed in a vector by their id
    std::vector<vertex_t> _vertices;

    // finally every graph is loaded from a specific file
    std::string _filename {""};
    
public:

    // default constructors create an empty collection of vertices and edges
    graph_t () :
        _nbedges {0},
        _edges {std::vector<std::vector<edge_t>>()}
        {}

    // move and copy constructors are forbidden
    graph_t (const graph_t& other) = delete;
    graph_t (graph_t&& other) = delete;

    // as much as the copy and assignment operator
    graph_t& operator= (graph_t const&) = delete; 
    graph_t& operator= (graph_t const&&) = delete; 

    // getters
    [[nodiscard]] size_t get_nbedges () const {
        return _nbedges;
    }
    [[nodiscard]] const std::vector<edge_t>& get_edges (size_t vertex) const {
        return _edges[vertex];
    }
    [[nodiscard]] const std::vector<edge_t>& get_parents (std::size_t vertex) const {
        return _parents[vertex];
    }
    [[nodiscard]] const vertex_t get_vertex (size_t vertex) const {
        return _vertices[vertex];
    }
    [[nodiscard]] size_t get_nbvertices () const {
        return _vertices.size ();
    }
    [[nodiscard]] const std::vector<vertex_t>& get_vertices () const {
        return _vertices;
    }
    [[nodiscard]] std::string get_filename () const {
        return _filename;
    }    

    // operators
    friend std::ostream& operator<<(std::ostream& stream, const graph_t& value) {

        for (size_t ivertex = 0 ; ivertex < value.get_nbvertices () ; ++ivertex) {
            for (auto const& jvertex : value.get_edges (ivertex)) {
                stream << " [" << ivertex << "]: " << value.get_vertex (ivertex) << " -> " << jvertex << std::endl;
            }
        }
        return stream;
    }

    // methods

    // add a new vertex to the graph. It returns true if the vertex was
    // effectively added and false otherwise (e.g., because it exceeds the
    // overall capacity of the graph)
    bool add_vertex (std::size_t const& vertex);

    // add a new edge to the graph. Return true if the edge has been effectively
    // added and false otherwise ---mostly because it already exists!!
    //
    // In case the operation is not feasible, it immediately raises an
    // exception.
    bool add_edge (std::size_t const& from, std::size_t const& to, int weight);
    
    // load a graph from a file with the format of the 9th DIMACS competition,
    // and stores the location of each vertex according to the information given
    // in the coordinates file whose name is derived from the given filename. It
    // returns the number of edges processed.
    int load (std::string const& filename);

    // retrieve the contents from a binary file and recreates the whole contents
    // of a graph from them. It returns true if the file exists and was used to
    // deserialize data, and false if the file does not exist. In case the file
    // exists but an error was detected, an exception is raised.
    //
    // The name of the binary file to read is computed automatically from the
    // location and name of the input file given
    bool read_binary (std::string const& filename, bool want_verbose);

    // write the contents of this graph into a binary file. If an error happened
    // then an exception is raised. The location and name of the binary file is
    // computed automatically from the location and name of the given filename
    // used to populate this graph
    void write_binary ();
    
    // erase the graph
    void clear () {
        _nbedges = 0;
        _edges.clear ();
        _parents.clear ();
        _vertices.clear ();
    }
    
}; // class graph_t

#endif // _GRAPH_T_H_

// Local Variables:
// mode:cpp
// fill-column:80
// End:
