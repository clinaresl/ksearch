// -*- coding: utf-8 -*-
// npancake.cc
// -----------------------------------------------------------------------------
//
// Started on <lun 29-11-2021 18:48:45.490311369 (1638208125)>
// Carlos Linares López <carlos.linares@uc3m.es>
// Ian Herman <iankherman@gmail.com>   Ian Herman <iankherman@gmail.com>

//
// Implementation of a k-shortest path solver for the n-pancake
//

#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <regex>
#include <sstream>
#include <stdlib.h>
#include <string>
#include <tuple>
#include <vector>

#include <getopt.h>

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
    {"solver", required_argument, 0, 's'},
    {"file", required_argument, 0, 'f'},
    {"variant", required_argument, 0, 'r'},
    {"k", required_argument, 0, 'k'},
    {"csv", required_argument, 0, 'C'},
    {"no-color", no_argument, 0, 'X'},
    {"no-doctor", no_argument, 0, 'D'},
    {"summary", no_argument, 0, 'S'},
    {"show-solution", no_argument, 0, 'P'},
    {"verbose", no_argument, 0, 'v'},
    {"help", no_argument, 0, 'h'},
    {"version", no_argument, 0, 'V'},
    {NULL, 0, NULL, 0}
};

const std::string get_domain ();
static int decode_switches (int argc, char **argv,
                            std::string& solver_name, std::string& filename, std::string& variant,
                            std::string& k_params, std::string& csvname,
                            bool& no_color, bool& no_doctor, bool& want_summary,
                            bool& show_solution, bool& want_verbose);
static void usage (int status);

// main entry point
int main (int argc, char** argv) {

    std::string solver_name;                       // user selection of solvers
    std::string filename;                       // file with all cases to solve
    std::string variant;                               // variant of the domain
    std::string k_params;                  // user selection of the values of k
    std::string csvname;                     // name of the output csv filename
    bool no_color;         // whether colour output has to be suppressed or not
    bool no_doctor;                    // whether the doctor is disabled or not
    bool want_summary;      // whether a summary of results is requested or not
    bool want_solution;        // whether solution paths should be shown or not
    bool want_verbose;                  // whether verbose output was requested
    std::chrono::time_point<std::chrono::system_clock> tstart, tend;// CPU time

    // variables
    program_name = argv[0];
    std::vector<std::string> variant_choices = {"unit", "heavy-cost"};

    // arg parse
    decode_switches (argc, argv, solver_name, filename, variant, k_params, csvname, no_color, no_doctor, want_summary, want_solution, want_verbose);

    // parameter checking

    // --file
    if (filename == "") {
        std::cerr << "\n Please, provide a file with the information of all start states to solve" << std::endl;
        std::cerr << " wrt the identity permutation" << std::endl;
        std::cerr << " See " << program_name << " --help for more details" << std::endl << std::endl;
        exit(EXIT_FAILURE);
    }

    // --variant

    // ensure that either the unit or heavy-cost variant is chosen
    if (!get_choice (variant, variant_choices)) {
        std::cerr << "\n Please, provide a correct name for the variant with --variant" << std::endl;
        std::cerr << " See " << program_name << " --help for more details" << std::endl << std::endl;
        exit(EXIT_FAILURE);
    }

    /* do the work */

    /* !------------------------- INITIALIZATION --------------------------! */

    // process the solver names and heuristics selection to get a vector with
    // the signatures of the combinations of solvers and heuristics to execute
    std::vector<std::string> solvers = split_option (solver_name, ack_solvers);

    // and also process the user selection of the K values
    auto kspec = split_ks (k_params);

    // open the given file and retrieve all cases from it
    std::vector<std::string> names;
    std::vector<std::vector<std::string>> instances;
    get_problems (filename, names, instances);
    if (!instances.size ()) {
        std::cerr << std::endl;
        std::cerr << " Error: The file '" << filename << "' contains no instances to solve!" << std::endl;
        std::cerr << std::endl;
        exit (EXIT_FAILURE);
    }

    // create the goal state which is represented as the identity permutation
    // using the symbols in the range [1, N]. All instances are assumed to have
    // the same length, so use the length of any as the length of the N-Pancake
    // to solve next
    std::vector<int> permutation;
    for (auto i = 0 ; i < instances[0].size () ; permutation.push_back (++i));
    npancake_t goal (permutation);

    // and now create a vector of tasks to solve
    std::vector<instance_t<npancake_t>> tasks;
    for (auto i = 0 ; i < instances.size () ; i++) {
        std::vector<int> permutation;
        for (auto& disc: instances[i]) {
            permutation.push_back (stoi (disc));
        }
        tasks.push_back (instance_t<npancake_t> {names[i], permutation, goal});
    }

    /* !-------------------------------------------------------------------! */

    std::cout << std::endl;
    std::cout << " ✔ solver       : " << solver_name << " " << KSEARCH_GIT_VERSION << std::endl;
    std::cout << " ✔ file         : " << filename << " (" << instances.size () << " instances)" << std::endl;
    std::cout << " ✔ variant      : " << variant << std::endl;

    // show the rest of the info
    std::cout << " ✔ size         : " << npancake_t::get_n () << std::endl;
    std::cout << " ✔ K            : ";
    for (auto& ispec: kspec) {
        std::cout << "[" << get<0>(ispec) << ", " << get<1>(ispec) << ", " << get<2> (ispec) << "] ";
    }
    std::cout << std::endl;

    /* !----------------------------- SEARCH ------------------------------! */

    // start the clock
    tstart = std::chrono::system_clock::now ();

    // create an instance of the "generic" domain-dependent solver
    solver<npancake_t> manager (get_domain (), variant,
                                tasks, k_params);

    // solve all the instances with every combination of solver and heuristic
    // selected by the user and in the same order given
    for (auto isolver : solvers) {

        // initialize the incremental table with the updates of the gap
        // heuristic
        npancake_t::init (variant);

        // and solve all instances
        manager.run (isolver, no_doctor, want_summary, want_verbose, not no_color, want_solution);
    }

    // and stop the clock
    tend = std::chrono::system_clock::now ();

    // to conclude, show an error summary and store all the results in a csv
    // file in case any was given
    manager.show_error_summary (no_doctor);
    manager.write_csv (csvname);
    std::cout << " 🕒 CPU time: " << 1e-9*std::chrono::duration_cast<std::chrono::nanoseconds>(tend - tstart).count() << " seconds" << std::endl;
    std::cout << std::endl;

    /* !-------------------------------------------------------------------! */

    // Well done! Keep up the good job!
    return (EXIT_SUCCESS);
}

// return the domain of this solver
const std::string get_domain () {
    return "n-pancake";
}

// Set all the option flags according to the switches specified. Return the
// index of the first non-option argument
static int
decode_switches (int argc, char **argv,
                 std::string& solver_name, std::string& filename, std::string& variant,
                 std::string& k_params, std::string& csvname,
                 bool& no_color, bool& no_doctor, bool& want_summary,
                 bool& want_solution, bool& want_verbose) {

    int c;

    // Default values
    solver_name = "";
    filename = "";
    variant = "unit";
    k_params = "";
    csvname = "";
    no_color = false;
    no_doctor = false;
    want_summary = false;
    want_solution = false;
    want_verbose = false;

    while ((c = getopt_long (argc, argv,
                             "s"  /* solver */
                             "f"  /* file */
                             "r"  /* variant */
                             "k"  /* k */
                             "C"  /* csv */
                             "X"  /* no-color */
                             "D"  /* no-doctor */
                             "S"  /* summary */
                             "P"  /* show-solution */
                             "v"  /* verbose */
                             "h"  /* help */
                             "V", /* version */
                             long_options, (int *) 0)) != EOF) {
        switch (c) {
        case 's':  /* --solver */
            solver_name = optarg;
            break;
        case 'f':  /* --file */
            filename = optarg;
            break;
        case 'r': /* --variant */
            variant = optarg;
            break;
        case 'k':  /* --k */
            k_params = optarg;
            break;
        case 'C':  /* --csv */
            csvname = optarg;
            break;
        case 'X':  /* --no-color */
            no_color = true;
            break;
        case 'D':  /* --no-doctor */
            no_doctor = true;
            break;
        case 'S':  /* --summary */
            want_summary = true;
            break;
        case 'P':  /* --show-solution */
            want_solution = true;
            break;
        case 'v':  /* --verbose */
            want_verbose = true;
            break;
        case 'V':
            std::cout << " khs (n-pancake) " << KSEARCH_VERSION << " (" << KSEARCH_GIT_VERSION << ")" << std::endl;
            std::cout << " " << CMAKE_BUILD_TYPE << " Build Type" << std::endl << std::endl;

            // show cpu and mem info
            std::cout << get_cpu_info() << std::endl;
            std::cout << get_mem_info() << std::endl;

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
    std::cout << std::endl << " " << program_name << " implements various K shortest-path search algorithms in the N-Pancake using the GAP heuristic" << std::endl << std::endl;
    std::cout << " Usage: " << program_name << " [OPTIONS]" << std::endl << std::endl;
    std::cout << "\
 Mandatory arguments:\n\
      -s, --solver [STRING]+     K shortest-path algorithms to use. Choices are:\n\
                                    * (non-simple) shortest-path:\n\
                                       + Brute-force search algorithms:\n\
                                          > 'mDijkstra': brute-force variant of mA*\n\
                                          > 'K0': brute-force variant of K*\n\
                                          > 'BELA0': brute-force variant of BELA*\n\
                                       + Heuristic search algorithms:\n\
                                          > 'mA*': mA*\n\
                                          > 'K*': K*\n\
                                          > 'BELA*': BELA*\n\
                                    * (simple) shortest-path:\n\
                                       + Brute-force search algorithms:\n\
                                          > 'bBELA0': brute-force variant of bBELA*\n\
                                          > 'sBELA0': brute-force variant of sBELA*\n\
                                       + Heuristic search algorithms:\n\
                                          > 'bBELA*': baseline simplistic BELA*\n\
                                          > 'sBELA*': simple BELA*\n\
                                 It is possible to provide as many as desired in a blank separated list between double quotes,\n\
                                 e.g., \"mDijkstra belA0\". If non-simple and simple paths are used in the same invocation the cross\n\
                                 verification will likely fail unless it is disabled with --no-doctor\n\
      -f, --file [STRING]        filename with a line for each instance to solve wrt to the identity permutation.\n\
                                 Each line consists of a problem id and the permutation to solve given as a list of numbers\n\
                                 separated by spaces in the range [1, N]\n\
      -r, --variant [STRING]     Variant of the n-Pancake to consider. Choices are {unit, heavy-cost}. By default, unit is used\n\
      -k, --k [NUMBER]+          Definition of the different values of K to test.\n\
                                 The entire specification consists of a semicolon separated list of single specifications\n\
                                 e.g., '1 5; 10 90 10; 100'\n\
                                 Every single specification consists of a blank separated list with up to three integers indicating\n\
                                 the first value of K, the last one and the increment between successive values of K.\n\
                                 If only one value is given (e.g., '100'), only one value of K is used; if only two are given\n\
                                 (e.g., '1 5'), all values of K between them are used with an increment equal to 1\n\
\n\
 Optional arguments:\n\
      -C, --csv [STRING]         name of the csv output files for storing results. If none is given, no file is generated\n\
      -X, --no-color             If given, the output is not coloured. It is recommended to use it when running the experiments\n\
                                 in batch mode and saving the output to files.\n\
      -D, --no-doctor            If given, the automated error checking is disabled. Otherwise, all solutions are automatically\n\
                                 checked for correctness\n\
      -S, --summary              If given, only the results of the last solution path found for every instance are shown. Otherwise,\n\
                                 the results of every single solution path are shown in the output csv file. It has no effect if\n\
                                 --csv is not given\n\
      -P, --show-solution        If given, every solution path from the start state to the goal state is shown on the standard output\n\
                                 It applies only, if --verbose (see below) is given. Otherwise, it has no effect\n\
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
