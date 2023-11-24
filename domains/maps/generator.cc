// -*- coding: utf-8 -*-
// generator.cc
// -----------------------------------------------------------------------------
//
// Started on <vie 24-11-2023 16:30:46.788864626 (1700839846)>
// Carlos Linares López <carlos.linares@uc3m.es>
//

//
// Generator of random instances in the maps domain
//

#include <chrono>
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

#include "map_t.h"

#define EXIT_OK 0
#define EXIT_FAILURE 1

using namespace std;

extern "C" {
    char *xstrdup (char *p);
}

/* Globals */
char *program_name;                       // The name the program was run with,

static struct option const long_options[] =
{
    {"map", required_argument, 0, 'm'},
    {"number", required_argument, 0, 'n'},
    {"file", required_argument, 0, 'f'},
    {"distance", required_argument, 0, 'D'},
    {"verbose", no_argument, 0, 'v'},
    {"help", no_argument, 0, 'h'},
    {"version", no_argument, 0, 'V'},
    {NULL, 0, NULL, 0}
};

int get_instances (int num_instances, int distance,
                   vector<instance_t<map_t>>& instances);
void write_instances (const vector<instance_t<map_t>>& instances, string filename);
static int decode_switches (int argc, char **argv,
                            string& mapname, int& number, string& filename, int& distance,
                            bool& want_verbose);
static void usage (int status);

// main entry point
int main (int argc, char** argv) {

    string mapname;                    // name of the file with the map to load
    int number;                              // number of instances to generate
    string filename;                            // file with all cases to solve
    int distance;              // minimum distance between start and goal state
    bool want_verbose;                  // whether verbose output was requested
    chrono::time_point<chrono::system_clock> tstart, tend;          // CPU time

    // variables
    program_name = argv[0];

    // arg parse
    decode_switches (argc, argv, mapname, number, filename, distance, want_verbose);

    // parameter checking

    // --mapname
    if (mapname == "") {
        cerr << "\n Please, provide a file with the contents of the map to load" << endl;
        cerr << " See " << program_name << " --help for more details" << endl << endl;
        exit(EXIT_FAILURE);
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
    if (distance < 0 || distance > 100) {
        cerr << "\n Please, provide a valid distance between the start and goal states" << endl;
        cerr << " See " << program_name << " --help for more details" << endl << endl;
        exit(EXIT_FAILURE);
    }

    /* do the work */

    // /* !------------------------- INITIALIZATION --------------------------! */

    // first, things first, initialize the map, using the unit variant arbitrarily
    map_t::init (mapname, "unit");

    /* !-------------------------------------------------------------------! */

    cout << endl;
    cout << " map            : " << mapname << endl;
    cout << "    width       : " << map_t::get_width () << endl;
    cout << "    height      : " << map_t::get_height () << endl;
    cout << " file           : " << filename << " (" << number << " instances)" << endl;
    cout << " distance       : " << distance << "%" << endl << endl;

    /* !--------------------------- GENERATION ----------------------------! */

    // start the clock
    tstart = chrono::system_clock::now ();

    // generate the random instances
    vector<instance_t<map_t>> tasks;
    get_instances (number, distance, tasks);

    // and write them in the specified file
    write_instances (tasks, filename);

    // end the clock
    tend = chrono::system_clock::now ();

    /* !-------------------------------------------------------------------! */

    // Well done! Keep up the good job!
    return (EXIT_OK);
}

// Randomly generate num_instances in the specified map and return them in a
// vector. The heuristic distance between every the start and goal states of
// every task has to be at least equal to the given distance.
//
// Return the number of instances generated
int get_instances (int num_instances, int distance,
                   vector<instance_t<map_t>>& instances) {

    int width = map_t::get_width ();                        // width of the map
    int height = map_t::get_height ();                     // height of the map

    // random generators
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> wdist(0,width-1);
    std::uniform_int_distribution<std::mt19937::result_type> hdist(0,height-1);

    // compute the maximum heuristic distance, i.e., from (0, 0) to (width-1,
    // height-1)
    map_t min_start {0,0};
    map_t max_goal {width-1, height-1};
    int max_distance = min_start.h (max_goal);

    while (instances.size () < num_instances) {

        // randomly pick up a couple of locations from the given map
        int x0 = wdist (rng); int y0 = hdist (rng);
        int x1 = wdist (rng); int y1 = hdist (rng);
        map_t start{x0, y0};
        map_t goal {x1, y1};

        // compute their heuristic distance
        int h = start.h (goal);

        // accept this pair if and only if they are not blocked and their
        // heuristic distance is at least the specified one
        if (!start.blocked (x0, y0) && !goal.blocked (x1, y1) &&
            h >= distance * max_distance / 100 ) {

            // add this instance to the vector to return
            instances.push_back (instance_t<map_t>{to_string (instances.size ()),
                    start, goal});
        }
    }

    return instances.size ();
}

// write all the given instances in the specified filename
void write_instances (const vector<instance_t<map_t>>& instances, string filename) {

    std::ofstream istream (filename, ios::out);

    // verify I/O operations are available
    if (!istream.good ()) {
        throw runtime_error ("[write_instances] Error opening file!");
    }

    // write all instances in the given file
    for (auto& instance : instances) {

        istream << instance << endl;
    }
}

// Set all the option flags according to the switches specified. Return the
// index of the first non-option argument
static int
decode_switches (int argc, char **argv,
                 string& mapname, int& number, string& filename, int& distance,
                 bool& want_verbose) {

    int c;

    // Default values
    mapname = "";
    number = 100;
    filename = "";
    distance = 0;
    want_verbose = false;

    while ((c = getopt_long (argc, argv,
                             "m"  /* map */
                             "n"  /* number */
                             "f"  /* file */
                             "D"  /* distance */
                             "v"  /* verbose */
                             "h"  /* help */
                             "V", /* version */
                             long_options, (int *) 0)) != EOF) {
        switch (c) {
        case 'm':  /* --map */
            mapname = optarg;
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
            cout << " khs (maps generator) " << CMAKE_VERSION << endl;
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
    cout << endl << " " << program_name << " implements various K shortest-path search algorithms in the Map domain" << endl << endl;
    cout << " Usage: " << program_name << " [OPTIONS]" << endl << endl;
    cout << "\
 Mandatory arguments:\n\
      -m, --map [STRING]         filename with the map to load. The file contents should be arranged according to the file format\n\
                                 given in movingai. See the documentation for additional help\n\
      -n, --number [NUMBER       Number of instances to generate. By default, 100]\n\
      -f, --file [STRING]        filename used to store all the random instances. Each line consists of five digits: the first\n\
                                 one is the problem id, which has to be unique; the second and third digits are the x- and\n\
                                 y-coordinates of the start state; the last two digits are the x- andy-coordinates of the goal\n\
                                 state. Note that test cases are relative to the map given in --map\n\
\n\
 Optional arguments:\n\
      -D, --distance [NUMBER]    Minimum heuristic distance between the start and goal states randomly generated measured as a\n\
                                 percentage ratio of the maximum distance. By default, 0\n                                                        \
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



// Local Variables:
// mode:cpp
// fill-column:80
// End:
