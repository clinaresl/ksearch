// -*- coding: utf-8 -*-
// generator.cc
// -----------------------------------------------------------------------------
//
// Started on <vie 24-11-2023 16:30:46.788864626 (1700839846)>
// Carlos Linares López <carlos.linares@uc3m.es>
//

//
// Generator of random instances in the roadmap domain
//

#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <random>
#include <regex>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>

#include <getopt.h>

#include "../solver.h"
#include "../../src/ksearch.h"

#include "roadmap_t.h"

#define EXIT_OK 0
#define EXIT_FAILURE 1

constexpr double PI = 3.141592653589793238462643383279502884197;

using namespace std;

extern "C" {
    char *xstrdup (char *p);
}

/* Globals */
char *program_name;                       // The name the program was run with,

static struct option const long_options[] =
{
    {"graph", required_argument, 0, 'g'},
    {"number", required_argument, 0, 'n'},
    {"file", required_argument, 0, 'f'},
    {"distance", required_argument, 0, 'D'},
    {"verbose", no_argument, 0, 'v'},
    {"help", no_argument, 0, 'h'},
    {"version", no_argument, 0, 'V'},
    {NULL, 0, NULL, 0}
};

int get_instances (int num_instances, int distance,
                   vector<instance_t<roadmap_t>>& instances);
void write_instances (const vector<instance_t<roadmap_t>>& instances, string filename);
bool get_coordinates_filename (const string& filename, string& coordinates_filename);
bool get_coordinates (const string& coordinates_filename,
                      map<int, pair<double, double>>& coordinates);
static int decode_switches (int argc, char **argv,
                            string& graph_name, int& number, string& filename, int& distance,
                            bool& want_verbose);
static void usage (int status);

// main entry point
int main (int argc, char** argv) {

    string graph_name;                        // file with the graph definition
    string coordinates_name;   // filename with the coordinates of all vertices
    map<int, pair<double, double>> coordinates;  // coordinates of all vertices
    int number;                              // number of instances to generate
    string filename;                            // file with all cases to solve
    int distance;              // minimum distance between start and goal state
    bool want_verbose;                  // whether verbose output was requested
    chrono::time_point<chrono::system_clock> tstart, tend;          // CPU time

    // variables
    program_name = argv[0];

    // arg parse
    decode_switches (argc, argv, graph_name, number, filename, distance, want_verbose);

    // parameter checking

    // --graph_name
    if (graph_name == "") {
        cerr << "\n Please, provide a file with the contents of the roadmap to load" << endl;
        cerr << " See " << program_name << " --help for more details" << endl << endl;
        exit(EXIT_FAILURE);
    }

    // get the coordinates filename from the graph name
    if (!get_coordinates_filename (graph_name, coordinates_name)) {
        cerr << " Warning: no coordinates file found!" << endl;
        cerr << "          it will not be possible to apply any heuristics" << endl << endl;
    }

    if (!get_coordinates (coordinates_name, coordinates)) {
        cerr << " Error: the coordinates file '" << coordinates_name << "' could not be processed" << endl << endl;
        exit (EXIT_FAILURE);
    }

    // --number
    if (number < 0) {
        cerr << "\n Please, provide a valid number of instances to generate" << endl;
        cerr << " See " << program_name << " --help for more details" << endl << endl;
        exit(EXIT_FAILURE);
    }

    // --file
    if (filename == "") {
        cerr << "\n Please, provide a file with the information of all start states to solve" << endl;
        cerr << " wrt the identity permutation" << endl;
        cerr << " See " << program_name << " --help for more details" << endl << endl;
        exit(EXIT_FAILURE);
    }

    // --distance
    if (distance < 0) {
        cerr << "\n Please, provide a valid minimum distance between the start and goal states" << endl;
        cerr << " See " << program_name << " --help for more details" << endl << endl;
        exit(EXIT_FAILURE);
    }

    /* do the work */

    // /* !------------------------- INITIALIZATION --------------------------! */

    // initialize the static data members of the definition of a roadmap under
    // the unit variant
    roadmap_t::init (graph_name, coordinates, false, "unit");
    auto nbedges = roadmap_t::get_graph ().get_nbedges ();

    /* !-------------------------------------------------------------------! */

    cout << endl;
    cout << " graph        : " << graph_name << " (" << nbedges << " edges processed)" << endl;
    cout << " file         : " << filename << " (" << number << " instances)" << endl;
    cout << " distance     : " << distance << " meters" << endl << endl;

    /* !--------------------------- GENERATION ----------------------------! */

    // start the clock
    tstart = chrono::system_clock::now ();

    // generate the random instances
    vector<instance_t<roadmap_t>> tasks;
    get_instances (number, distance, tasks);

    // and write them in the specified file
    write_instances (tasks, filename);

    // end the clock
    tend = chrono::system_clock::now ();

    /* !-------------------------------------------------------------------! */

    // Well done! Keep up the good job!
    return (EXIT_OK);
}

// Randomly generate num_instances in the specified graph and return them in a
// vector. The heuristic distance between every the start and goal states of
// every task has to be at least equal to the given distance.
//
// Return the number of instances generated
int get_instances (int num_instances, int distance,
                   vector<instance_t<roadmap_t>>& instances) {

    // get the number of locations in the roadmap
    auto nbvertices = roadmap_t::get_graph ().get_nbvertices ();

    // random generator
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist(0, nbvertices-1);

    // generate the required number of instances
    while (instances.size () < num_instances) {

        // randomly pick up a couple of locations from the given roadmap
        int loc0 = dist (rng); int loc1 = dist (rng);
        roadmap_t start{size_t(loc0)};
        roadmap_t goal {size_t(loc1)};

        // compute their heuristic distance
        int h = start.h (goal);

        // accept this pair if and only if the heuristic distance between them
        // is at least the given distance
        if (h >= distance) {

            // add this instance to the vector to return
            instances.push_back (instance_t<roadmap_t>{to_string (instances.size ()),
                    start, goal});
        }
    }

    return instances.size ();
}

// write all the given instances in the specified filename
void write_instances (const vector<instance_t<roadmap_t>>& instances, string filename) {

    std::ofstream istream (filename, ios::out);

    // verify I/O operations are available
    if (!istream.good ()) {
        throw runtime_error ("[write_instances] Error opening file!");
    }

    // write all instances in the given file following the format used in the
    // roadmap solver, i.e., the first line contains all the start goals, and
    // the second line contains all the goal vertices

    // first, show all the start states
    for (auto& instance : instances) {
        istream << instance.get_start() << " ";
    }
    istream << endl;

    // next, show all the goal states
    for (auto& instance : instances) {
        istream << instance.get_goal() << " ";
    }
    istream << endl;

}

// return true if the coordinates file of the given graph exists and is
// readable. If true, the name of the coordinates file is returned in the second
// argument; otherwise, its contents are undefined
bool get_coordinates_filename (const string& filename, string& coordinates_filename) {

    // replace the extension of the graph name by "co"
    auto path = filesystem::path (filename);
    auto cofile = path.replace_extension("co");

    // verify the file exists and it is an ordinary file
    if (filesystem::exists (cofile) &&
        filesystem::is_regular_file (cofile)) {

        // check the permissions of the coordinates file
        std::error_code ec;
        auto perms = filesystem::status(cofile, ec).permissions();
        if ((perms & filesystem::perms::owner_read) != filesystem::perms::none &&
            (perms & filesystem::perms::group_read) != filesystem::perms::none &&
            (perms & filesystem::perms::others_read) != filesystem::perms::none) {

            coordinates_filename = cofile.c_str ();
            return true;
        }
    }

    // at this point, the file either does not exist or can not be read, so
    // return false
    return false;
}

// Return true if all data could be properly processed and false otherwise. It
// retrieves all coordinates of all vertices given in the coordinates_filename
// and stores them in a map indexed by the vertex id that stores the longitude
// (x-value) and latitude (y-value). The coordinates are given in radians.
bool get_coordinates (const string& coordinates_filename,
                      map<int, pair<double, double>>& coordinates) {

    ifstream stream (coordinates_filename);

    // create regex to process each line separately. Lines in the DIMACS
    // competition format start with a character: 'c' is used for comments; 'p'
    // is used for providing properties; 'v' adds a new vertex. In the
    // following, both 'c' and 'p' are ignored
    regex comment ("^[cp].*");

    // Lines starting with 'v' add a new vertex, and other than the prefix, they
    // come with three integers: the vertex id, the longitude (x-value) and the
    // latitude (y-value). Note the longitude and latitude might come with a
    // sign, or not
    regex newedge (R"(^v\s+(\d+)\s+([+-]?\d+)\s+([+-]?\d+)\s*$)");

    // parse the contents of the file
    string line;
    int lineno = 0;
    while (getline (stream, line)) {

        // skip this line in case it should be ignored
        if (regex_match (line, comment)) {

            // increment the line counter and skip it
            lineno++;
            continue;
        }

        // at this point, lines must match the vertex command 'v'
        smatch m;
        if (regex_match (line, m, newedge)) {

            // add a new vertex to the map using the vertex id as the key and
            // storing a pair with the longitude and latitude. Note that
            // longitude and latitude are given as integers with six digits of
            // precision
            size_t id = stoi (m[1].str ());
            double lon = stoi (m[2].str ()) / 1'000'000.0;
            double lat = stoi (m[3].str ()) / 1'000'000.0;

            // and add these coordinates to the vertex with identifier id as
            // radians
            coordinates[id] = make_pair (lon*PI/180.0, lat*PI/180.0);

            // and add the number of edges processed
            lineno++;

        } else {

            // otherwise, a syntax error has been found
            cerr << " Syntax error in '" << coordinates_filename << "'::" << lineno << endl;
            return false;
        }
    }

    return true;
}

// Set all the option flags according to the switches specified. Return the
// index of the first non-option argument
static int
decode_switches (int argc, char **argv,
                 string& graph_name, int& number, string& filename, int& distance,
                 bool& want_verbose) {

    int c;

    // Default values
    graph_name = "";
    number = 100;
    filename = "";
    distance = 0;
    want_verbose = false;

    while ((c = getopt_long (argc, argv,
                             "g"  /* graph */
                             "n"  /* number */
                             "f"  /* file */
                             "D"  /* distance */
                             "v"  /* verbose */
                             "h"  /* help */
                             "V", /* version */
                             long_options, (int *) 0)) != EOF) {
        switch (c) {
        case 'g':  /* --graph */
            graph_name = optarg;
            break;
        case 'n':  /* --solver */
            number = stoi (optarg);
            break;
        case 'f':  /* --file */
            filename = optarg;
            break;
        case 'D': /* --distance */
            distance = stoi (optarg);
            break;
        case 'v':  /* --verbose */
            want_verbose = true;
            break;
        case 'V':
            cout << " khs (roadmap generator) " << CMAKE_VERSION << endl;
            cout << " " << CMAKE_BUILD_TYPE << " Build Type" << endl;
            exit (EXIT_OK);
        case 'h':
            usage (EXIT_OK);
        default:
            cout << endl << " Unknown argument!" << endl;
            usage (EXIT_FAILURE);
        }
    }
    return optind;
}


static void
usage (int status)
{
    cout << endl << " " << program_name << " Random generator of instances for the Roadmap domain" << endl << endl;
    cout << " Usage: " << program_name << " [OPTIONS]" << endl << endl;
    cout << "\
 Mandatory arguments:\n\
      -g, --graph [STRING]       filename with the graph to load. The file contents should be arranged according to the 9th DIMACS\n\
                                 Implementation Challenge: Shortest Paths. See the documentation for additional help\n\
      -n, --number [NUMBER       Number of instances to generate. By default, 100]\n\
      -f, --file [STRING]        filename used to store all the random instances. Each line consists of five digits: the first\n\
                                 one is the problem id, which has to be unique; the second and third digits are the x- and\n\
                                 y-coordinates of the start state; the last two digits are the x- andy-coordinates of the goal\n\
                                 state. Note that test cases are relative to the map given in --map\n\
\n\
 Optional arguments:\n\
      -D, --distance [NUMBER]    Minimum heuristic distance between the start and goal states randomly generated measured.\n\
                                 By default, 0\n\
\n\
 Misc arguments:\n\
      --verbose                  print more information\n\
      -h, --help                 display this help and exit\n\
      -V, --version              output version information and exit\n\
\n";
    exit (status);
}

// Local Variables:
// mode:cpp
// fill-column:80
// End:
