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
#include <vector>

#include <getopt.h>

#include "../solver.h"
#include "../../src/ksearch.h"
#include "../../src/version.h"

#include "roadmap_t.h"

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

int get_instances (size_t num_instances, int distance,
                   std::vector<instance_t<roadmap_t>>& instances);
void write_instances (const std::vector<instance_t<roadmap_t>>& instances, std::string filename);
static int decode_switches (int argc, char **argv,
                            std::string& graph_name, int& number, std::string& filename, int& distance,
                            bool& want_verbose);
static void usage (int status);

// main entry point
int main (int argc, char** argv) {

    std::string graph_name;                        // file with the graph definition
    std::string coordinates_name;   // filename with the coordinates of all vertices
    std::map<size_t, std::pair<double, double>> coordinates;    // coords of all vertices
    int number;                              // number of instances to generate
    std::string filename;                            // file with all cases to solve
    int distance;              // minimum distance between start and goal state
    bool want_verbose;                  // whether verbose output was requested
    std::chrono::time_point<std::chrono::system_clock> tstart, tend;          // CPU time

    // variables
    program_name = argv[0];

    // arg parse
    decode_switches (argc, argv, graph_name, number, filename, distance, want_verbose);

    // parameter checking

    // --graph_name
    if (graph_name == "") {
        std::cerr << "\n Please, provide a file with the contents of the roadmap to load" << std::endl;
        std::cerr << " See " << program_name << " --help for more details" << std::endl << std::endl;
        exit(EXIT_FAILURE);
    }

    // --number
    if (number < 0) {
        std::cerr << "\n Please, provide a valid number of instances to generate" << std::endl;
        std::cerr << " See " << program_name << " --help for more details" << std::endl << std::endl;
        exit(EXIT_FAILURE);
    }

    // --file
    if (filename == "") {
        std::cerr << "\n Please, provide a file with the information of all start states to solve" << std::endl;
        std::cerr << " wrt the identity permutation" << std::endl;
        std::cerr << " See " << program_name << " --help for more details" << std::endl << std::endl;
        exit(EXIT_FAILURE);
    }

    // --distance
    if (distance < 0) {
        std::cerr << "\n Please, provide a valid minimum distance between the start and goal states" << std::endl;
        std::cerr << " See " << program_name << " --help for more details" << std::endl << std::endl;
        exit(EXIT_FAILURE);
    }

    /* do the work */

    // /* !------------------------- INITIALIZATION --------------------------! */

    // initialize the static data members of the definition of a roadmap under
    // the unit variant
    roadmap_t::init (graph_name, "unit");
    auto nbedges = roadmap_t::get_graph ().get_nbedges ();

    /* !-------------------------------------------------------------------! */

    std::cout << std::endl;
    std::cout << " graph        : " << graph_name << " (" << nbedges << " edges processed)" << std::endl;
    std::cout << " file         : " << filename << " (" << number << " instances)" << std::endl;
    std::cout << " distance     : " << distance << " meters" << std::endl << std::endl;

    /* !--------------------------- GENERATION ----------------------------! */

    // start the clock
    tstart = std::chrono::system_clock::now ();

    // generate the random instances
    std::vector<instance_t<roadmap_t>> tasks;
    get_instances (number, distance, tasks);

    // and write them in the specified file
    write_instances (tasks, filename);

    // end the clock
    tend = std::chrono::system_clock::now ();

    /* !-------------------------------------------------------------------! */

    // Well done! Keep up the good job!
    return (EXIT_SUCCESS);
}

// Randomly generate num_instances in the specified graph and return them in a
// vector. The heuristic distance between the start and goal states of every
// task has to be at least equal to the given distance.
//
// Return the number of instances generated
int get_instances (size_t num_instances, int distance,
                   std::vector<instance_t<roadmap_t>>& instances) {

    // get the number of locations in the roadmap
    auto nbvertices = roadmap_t::get_graph ().get_nbvertices ();

    // random generator
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist(1, nbvertices-1);

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
            instances.push_back (instance_t<roadmap_t>{std::to_string (instances.size ()),
                    start, goal});
        }
    }

    return instances.size ();
}

// write all the given instances in the specified filename
void write_instances (const std::vector<instance_t<roadmap_t>>& instances, std::string filename) {

    std::ofstream istream (filename, std::ios::out);

    // verify I/O operations are available
    if (!istream.good ()) {
        throw std::runtime_error ("[write_instances] Error opening file!");
    }

    // write all instances in the given file following the format used in the
    // ksearch library, i.e., each line consists of an id followed by two
    // integers which are the indexes to the start and goal

    for (size_t idx = 0 ; idx < instances.size () ; ++idx) {
        istream << std::setw (2) << std::setfill ('0') << idx << " ";
        istream << std::setw (8) << std::setfill (' ') << instances[idx].get_start ().get_index () << " ";
        istream << std::setw (8) << instances[idx].get_goal ().get_index () << std::endl;
    }
}

// Set all the option flags according to the switches specified. Return the
// index of the first non-option argument
static int
decode_switches (int argc, char **argv,
                 std::string& graph_name, int& number, std::string& filename, int& distance,
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
            number = std::stoi (optarg);
            break;
        case 'f':  /* --file */
            filename = optarg;
            break;
        case 'D': /* --distance */
            distance = std::stoi (optarg);
            break;
        case 'v':  /* --verbose */
            want_verbose = true;
            break;
        case 'V':
            std::cout << " khs (roadmap generator) " << KSEARCH_VERSION << " (" << KSEARCH_GIT_VERSION << ")" << std::endl;
            std::cout << " " << CMAKE_BUILD_TYPE << " Build Type" << std::endl;
            exit (EXIT_SUCCESS);
        case 'h':
            usage (EXIT_SUCCESS);
        default:
            std::cout << std::endl << " Unknown argument!" << std::endl;
            usage (EXIT_FAILURE);
        }
    }
    return optind;
}


static void
usage (int status)
{
    std::cout << std::endl << " " << program_name << " Random generator of instances for the Roadmap domain" << std::endl << std::endl;
    std::cout << " Usage: " << program_name << " [OPTIONS]" << std::endl << std::endl;
    std::cout << "\
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
