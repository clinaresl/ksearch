// -*- coding: utf-8 -*-
// generator.cc
// -----------------------------------------------------------------------------
//
// Started on <vie 24-11-2023 16:30:46.788864626 (1700839846)>
// Carlos Linares López <carlos.linares@uc3m.es>
//

//
// Generator of random instances in the npancake domain
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

#include "../helpers.h"
#include "../solver.h"
#include "../../src/ksearch.h"
#include "../../src/version.h"

#include "npancake_t.h"

extern "C" {
    char *xstrdup (char *p);
}

/* Globals */
char *program_name;                       // The name the program was run with,

static struct option const long_options[] =
{
    {"size", required_argument, 0, 's'},
    {"number", required_argument, 0, 'n'},
    {"file", required_argument, 0, 'f'},
    {"distance", required_argument, 0, 'D'},
    {"variant", required_argument, 0, 'X'},
    {"verbose", no_argument, 0, 'v'},
    {"help", no_argument, 0, 'h'},
    {"version", no_argument, 0, 'V'},
    {NULL, 0, NULL, 0}
};

int get_instances (int size, int num_instances, int distance,
                   std::vector<instance_t<npancake_t>>& instances);
void write_instances (const std::vector<instance_t<npancake_t>>& instances, std::string filename);
static int decode_switches (int argc, char **argv,
                            int& size, int& number, std::string& filename, int& distance, std::string& variant,
                            bool& want_verbose);
static void usage (int status);

// main entry point
int main (int argc, char** argv) {

    int size;                         // length of the permutations to generate
    int number;                              // number of instances to generate
    std::string filename;                        // file with all cases to solve
    int distance;              // minimum distance between start and goal state
    std::string variant;  // variant of the n-pancake, either unit or heavy-cost
    bool want_verbose;                  // whether verbose output was requested
    std::chrono::time_point<std::chrono::system_clock> tstart, tend;// CPU time

    // variables
    program_name = argv[0];
    std::vector<std::string> variant_choices = {"unit", "heavy-cost"};

    // arg parse
    decode_switches (argc, argv, size, number, filename, distance, variant, want_verbose);

    // parameter checking

    // --size
    if (size == 0) {
        std::cerr << "\n Please, provide the length of the permutations to generate" << std::endl;
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
        std::cerr << "\n Please, provide the filename where instances must be stored" << std::endl;
        std::cerr << " See " << program_name << " --help for more details" << std::endl << std::endl;
        exit(EXIT_FAILURE);
    }

    // --distance
    if (distance < 0) {
        std::cerr << "\n Please, provide a valid distance between the start and goal states" << std::endl;
        std::cerr << " See " << program_name << " --help for more details" << std::endl << std::endl;
        exit(EXIT_FAILURE);
    }

    // --variant
    if (!get_choice (variant, variant_choices)) {
        std::cerr << "\n Please, provide a correct name for the variant with --variant" << std::endl;
        std::cerr << " See " << program_name << " --help for more details" << std::endl << std::endl;
        exit(EXIT_FAILURE);
    }

    /* do the work */

    // /* !------------------------- INITIALIZATION --------------------------! */

    // first, things first, initialize the heuristic table of the n-pancake
    npancake_t::init (variant);

    /* !-------------------------------------------------------------------! */

    std::cout << std::endl;
    std::cout << " size           : " << size << std::endl;
    std::cout << " file           : " << filename << " (" << number << " instances)" << std::endl;
    std::cout << " distance       : " << distance << std::endl;
    std::cout << " variant        : " << variant << std::endl << std::endl;

    /* !--------------------------- GENERATION ----------------------------! */

    // start the clock
    tstart = std::chrono::system_clock::now ();

    // generate the random instances
    std::vector<instance_t<npancake_t>> tasks;
    get_instances (size, number, distance, tasks);

    // and write them in the specified file
    write_instances (tasks, filename);

    // end the clock
    tend = std::chrono::system_clock::now ();

    /* !-------------------------------------------------------------------! */

    // Well done! Keep up the good job!
    return (EXIT_SUCCESS);
}

// Randomly generate num_instances of the specified length and return them in a
// vector. The heuristic distance between the start and goal states of
// every task has to be at least equal to the given distance.
//
// Return the number of instances generated
int get_instances (int size, int num_instances, int distance,
                   std::vector<instance_t<npancake_t>>& instances) {

    // random generator
    auto rng = std::default_random_engine {};

    // create the identity permutation which is used as the goal state using the
    // symbols in the range [1, size]
    std::vector<int> identity;
    for (auto i = 0 ; i < size ; identity.push_back (++i));
    npancake_t goal {identity};

    while (instances.size () < size_t (num_instances)) {

        // create a random instance just by shuffling a copy of the goal state
        std::vector<int> n (identity);
        shuffle (n.begin (), n.end (), rng);
        npancake_t start {n};

        // accept this pair if and only if the heuristic distance is either
        // larger or equal than the given distance
        if (start.h (goal) >= distance) {

            // add this instance to the vector to return
            instances.push_back (instance_t<npancake_t>{std::to_string (instances.size ()),
                    start, goal});
        }
    }

    // return instances.size ();
    return 0;
}

// write all the given instances in the specified filename
void write_instances (const std::vector<instance_t<npancake_t>>& instances, std::string filename) {

    std::ofstream istream (filename, std::ios::out);

    // verify I/O operations are available
    if (!istream.good ()) {
        throw std::runtime_error ("[write_instances] Error opening file!");
    }

    // write all instances in the given file
    for (auto& instance : instances) {

        // do not show the goal, as it is always assumed to be the identity
        // permutation
        istream << instance.get_name () << " " << instance.get_start () << std::endl;
    }
}

// Set all the option flags according to the switches specified. Return the
// index of the first non-option argument
static int
decode_switches (int argc, char **argv,
                 int& size, int& number, std::string& filename, int& distance, std::string& variant,
                 bool& want_verbose) {

    int c;

    // Default values
    size = 0;
    number = 100;
    filename = "";
    distance = 0;
    variant = "unit";
    want_verbose = false;

    while ((c = getopt_long (argc, argv,
                             "s"  /* size */
                             "n"  /* number */
                             "f"  /* file */
                             "D"  /* distance */
                             "X"  /* variant */
                             "v"  /* verbose */
                             "h"  /* help */
                             "V", /* version */
                             long_options, (int *) 0)) != EOF) {
        switch (c) {
        case 's':  /* --size */
            size = std::stoi (optarg);
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
        case 'X':  /* --variant */
            variant = optarg;
            break;
        case 'v':  /* --verbose */
            want_verbose = true;
            break;
        case 'V':
            std::cout << " khs (n-pancake generator) " << KSEARCH_VERSION << " (" << KSEARCH_GIT_VERSION << ")" << std::endl;
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
    std::cout << std::endl << " " << program_name << " Random generator of instances for the N-Pancake domain" << std::endl << std::endl;
    std::cout << " Usage: " << program_name << " [OPTIONS]" << std::endl << std::endl;
    std::cout << "\
 Mandatory arguments:\n\
      -s, --size [NUMBER]        length of the permutations to generate\n\
      -n, --number [NUMBER]      Number of instances to generate. By default, 100]\n\
      -f, --file [STRING]        filename used to store all the random instances. Each line consists of a problem id and the\n\
                                 permutation.\n\
\n\
 Optional arguments:\n\
      -D, --distance [NUMBER]    Minimum heuristic distance between the start and goal states randomly generated. By default, 0\n\
      -X, --variant [STRING]     variant to use for measuring the heuristic distance between two states, either 'unit' or\n\
                                 'heavy-cost'. By default, 'unit'\n\
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
