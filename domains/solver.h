// -*- coding: utf-8 -*-
// solver.h
// -----------------------------------------------------------------------------
//
// Started on <lun 28-08-2023 00:46:32.176420194 (1693176392)>
// Carlos Linares López <carlos.linares@uc3m.es>
// Ian Herman <iankherman@gmail.com>   Ian Herman <iankherman@gmail.com>

//
// Services of a domain-dependent solver for the k shortest-path problem
//

#ifndef _SOLVER_H_
#define _SOLVER_H_

#include <chrono>
#include <fstream>
#include <iostream>
#include <string>
#include <tuple>
#include <unistd.h>
#include <vector>
#include <regex>

#include "helpers.h"
#include "instance.h"
#include "../src/ksearch.h"
#include "../src/version.h"

// the following function provides a report on the memory usage in Kbytes
// Literally taken from the gist thirdwing/memory_check.cpp
#ifdef __unix__
void process_mem_usage(double& vm_usage, double& resident_set)
{
    vm_usage     = 0.0;
    resident_set = 0.0;

    // the two fields we want
    unsigned long vsize;
    long rss;
    {
        std::string ignore;
        std::ifstream ifs("/proc/self/stat", std::ios_base::in);
        ifs >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore
                >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore
                >> ignore >> ignore >> vsize >> rss;
    }

    long page_size_kb = sysconf(_SC_PAGE_SIZE) / 1024; // in case x86-64 is configured to use 2MB pages
    vm_usage = vsize / 1024.0;
    resident_set = rss * page_size_kb;
}
#endif

// Definition of a "generic" domain-dependent solver for the k shortest-path
// problem. This solver registers a domain and a variant, a long with a number
// of k values to try and run specific solvers over a selection of instances.
// Results are automatically stored in a container
// ----------------------------------------------------------------------------
template <typename D>
class solver {

private:

    // INVARIANT: A domain-dependent solver is applied over a specific domain
    // and variant (where the typename D is the domain type of application,
    // e.g., npancake). A vector of instances must be given, and also a
    // specification of the k values to try which are given as a string that has
    // to be decoded
    std::string _domain;                                              // Domain
    std::string _variant;                                            // Variant
    std::vector<instance_t<D>> _instances;                         // instances
    std::string _kspec;                        // specification of the k values

    // the k values to try are internally stored as a vector of tuples, each
    // specifying the first value to try, the last one and the step between
    // successive values of k
    std::vector<std::tuple<int, int, int>> _ks;

    // the results of the execution are internally stored in a container of
    // solutions of instances of the k shortest-path problem
    khs::ksolutions_t<D> _results;

    // methods

    // return a (plain) pointer to a specific solver for solving instances in
    // the specified domain D according to the given name. The solver is
    // initialized with the following data:
    //
    // * start: instance to solve
    // * goal: goal to reach
    // * k: number of paths to find
    khs::bsolver<D>* _get_solver (const std::string name,
                                  const D& start, const D& goal,
                                  const int k) {

        // create a pointer to a solver
        khs::bsolver<D>* m;

        // Extract parameters from name if they exist
        std::regex solver_regex(solver_regex_spec);
        std::smatch matches;
        if (not std::regex_search(name, matches, solver_regex)) {
            throw std::invalid_argument ("[solver<D>::_get_solver] Syntax error in the specification of solver: " + name);
        }
        std::string nameProcessed = matches[1];

        // Optional parameters
        std::vector<std::string> params;
        try {
            if (matches[2] != "") {
                params.push_back (matches[2].str ());
            } else {
                params.push_back ("");
            }
            if (matches[3] != "") {
                params.push_back(matches[3].str ());
            } else {
                params.push_back ("");
            }
            if (matches[4] != "") {
                params.push_back(matches[4].str ());
            } else {
                params.push_back ("");
            }
            if (matches[5] != "") {
                params.push_back(matches[5].str ());
            } else {
                params.push_back ("");
            }
        } catch (...) {
            throw std::invalid_argument ("[solver<D>::_get_solver] Syntax error in the parameters of solver: " + name);
        }

        // and now choose according to the given name
        if (nameProcessed == "mDijkstra") {
            m = new khs::mA<D> (k, start, goal, true);
        } else if (nameProcessed == "BELA0") {
            m = new khs::bela<D> (k, start, goal, true);
        } else if (nameProcessed == "K0") {
            // use the default values in case that some parameters have not been given
            params = union_string (std::vector<std::string>{"20", "20", "1", "0"}, params);
            m = new khs::kStar<D>(k, start, goal, std::stoul(params[0]), std::stoul(params[1]), std::stoi(params[2]), std::stoi(params[3]), true, false);
        } else if (nameProcessed == "bBELA0") {
            // use the default values in case that some parameters have not been given
            params = union_string (std::vector<std::string>{"0"}, params);
            auto b = new khs::bbela<D> (k, start, goal, true);
            b->set_n (std::stoi (params[0]));
            m = (khs::bsolver<D>*)b;
        } else if (nameProcessed == "sBELA0") {
            m = new khs::sbela<D> (k, start, goal, true);
        }

        else if (nameProcessed == "K*") {
            // use the default values in case that some parameters have not been given
            params = union_string (std::vector<std::string>{"20", "20", "1", "0"}, params);
            m = new khs::kStar<D>(k, start, goal, std::stoul(params[0]), std::stoul(params[1]), std::stoi(params[2]), std::stoi(params[3]), false, false);
        } else if (nameProcessed == "mA*") {
            m = new khs::mA<D> (k, start, goal, false);
        } else if (nameProcessed == "BELA*") {
            m = new khs::bela<D> (k, start, goal, false);
        } else if (nameProcessed == "bBELA*") {
            // use the default values in case that some parameters have not been given
            params = union_string (std::vector<std::string>{"0"}, params);
            auto b = new khs::bbela<D>(k, start, goal, false);
            b->set_n (std::stoi (params[0]));
            m = (khs::bsolver<D>*)b;
        } else if (nameProcessed == "sBELA*") {
            m = new khs::sbela<D>(k, start, goal, false);
        }

        else {
            throw std::invalid_argument{"Unknown solver!"};
        }

        // and return a pointer to the selected solver
        return m;
    }

    public:

    // Explicit constructor. Both the domain and variant should be given, along
    // with a vector of instances to solve, and the specification of the k
    // values to try
    solver (std::string const domain, std::string const variant,
            std::vector<instance_t<D>> const instances,
            std::string const kspec) :
        _domain { domain },
        _variant { variant },
        _instances { instances },
        _kspec { kspec }
        {

            // process the specification of values of k and store the specific
            // limits for every application
            _ks = split_ks (_kspec);
        }

    // getters
    [[nodiscard]] const std::string get_domain () const { return _domain; }
    [[nodiscard]] const std::string get_variant () const { return _variant; }
    [[nodiscard]] const std::vector<instance_t<D>> get_instances () const { return _instances; }
    [[nodiscard]] const std::string get_kspec () const { return _kspec; }

    // methods

    // the following service automates the execution of the given solver over
    // all instances found in the test file with all values obtained from the
    // specification of the k values, and stores the results internally.
    //
    // Unless no_doctor takes the value true, all single solution paths and also
    // the solutions to every instance of the k shortest-path problem are
    // checked for correctness.
    //
    // summary determines whether only the results of the last solution path
    // found for every instance, or the results of every single solution path,
    // are shown.
    //
    // In case want_verbose takes the true, different stats are given per
    // solution
    //
    // If want_color is true, then the output is coloured.
    // 
    // In case want_solution is given, then every single solution path is given.
    // It has no effect if verbose is not provided as well
    void run (std::string solver_name, bool no_doctor, bool want_summary, bool want_verbose, bool want_color, bool want_solution=false) {

        // bBELA0/bBELA* are the only algorithms that might miss some solutions.
        // In case this possibility is detected, execution immediately halts so
        // that the user has to be reported. All warnings are stored in the same
        // string and they are issued together
        std::ostringstream wstream;
        if (solver_name.starts_with("bBELA0") or solver_name.starts_with("bBELA*")) {
            wstream << " ⚠ Warning: " << solver_name << " does not guarantee to produce correct solutions!" << std::endl;
            wstream << "   In case a possibility of generating incorrect solutions is detected, execution will be halted immediately!" << std::endl;
        }

        if (want_solution and not want_verbose) {
            wstream << " ⚠ Warning: --show-solution has no effect without --verbose" << std::endl;
            want_solution = false;
        }
        
        if (want_summary and not no_doctor) {
            wstream << " ⚠ Warning: --summary disables the doctor. No solution paths will be verified!" << std::endl;
            no_doctor = true;
        }
        
        if (want_summary and want_verbose) {
            wstream << " ⚠ Warning: --summary disables --verbose output. No information about the solution paths will be shown!" << std::endl;
            want_verbose = false;
        }
        
        if (want_summary and want_solution) {
            wstream << " ⚠ Warning: --summary disables --show-solution. No solution paths will be shown!" << std::endl;
            want_verbose = false;
        }

        // in case any warnings were issued show them together
        if (wstream.str ().size () > 0 ) {
            std::cerr << std::endl;
            std::cerr << std::format ("{}{}{}", khs::ansi::Gold, wstream.str (), khs::ansi::reset);
        }
        
        // set the domain and variant in the container of ksolutions, and also
        // the summary flag
        _results.set_domain (_domain);
        _results.set_variant (_variant);
        _results.set_version (KSEARCH_GIT_VERSION);
        _results.set_summary (want_summary);

        // for all values of k selected by the user
        for (auto ispec: _ks) {

            // initialize a container for storing all solutions generated in this
            // execution
            khs::ksolutions_t<D> bag;
            bag.set_domain (_domain);
            bag.set_variant (_variant);

            // Measure the time taken by this solver for solving all its instances
            auto tstart = std::chrono::system_clock::now ();

            // consider this single specification of k values
            for (auto k = std::get<0>(ispec) ; k <= std::get<1>(ispec) ; k+= std::get<2>(ispec)) {

                std::cout << std::endl;
                
                // for all instances
                std::cout << " ⏺ " << solver_name << " ( k=" << k <<  " ): " << std::endl;

                // Get the current time to measure the wall-clock time required
                // to solve all instances with this value of k
                auto kstart = std::chrono::high_resolution_clock::now();

                int lineno = 0;
                for (auto i = 0 ; i < _instances.size () ; i++) {

                    // create a manager to solve this specific instance
                    auto m = _get_solver (solver_name,
                                          _instances[i].get_start (),
                                          _instances[i].get_goal (),
                                          k);

                    auto ksolution = m->solve ();

                    // Get ready to show the stats of this solution. Show the
                    // information in table mode with blocks of ten lines,
                    // unless verbose output has been requested
                    if (lineno%10==0 or want_verbose) {

                        // in case this is not the first line leave a blank line
                        // to make the next block more visible
                        if (lineno>0) {
                            std::cout << std::endl;
                        }
                        std::cout << ksolution.headers (khs::set_mode ((want_color ? "color" : "console"))) << std::endl;
                    }
                    std::cout << " ⏵ "; std::cout.flush ();

                    // in case that no solution has been generated, then skip it
                    if (ksolution.size () == 0) {
                        std::cout << _instances[i].get_name () << " ⚠ No solution found!" << std::endl;
                        continue;
                    }

                    // give a name to every individual solution
                    for (auto j = 0 ; j < ksolution.size () ; j++) {
                        ksolution[j].set_name (_instances[i].get_name () + "/" + std::to_string (1+j));
                    }

                    // give this instance a name so that it can be easily
                    // recognized and record also the name of the solver used
                    // and the version of this code
                    ksolution.set_name (_instances[i].get_name ());
                    ksolution.set_solver (m->signature ());
                    ksolution.set_version (KSEARCH_GIT_VERSION);

                    // in case a summary was requested then remove all solutions
                    // but the information of the last one, which is used to
                    // show the statistics for solving the whole k shortest-path
                    // problem
                    if (want_summary) {
                        while (ksolution.size () > 1) {
                            ksolution.remove(0);
                        }
                    }

#ifdef __unix__
                    // and while the solver is still alive (and all its data in
                    // main memory) measure memory usage again in Mbytes. Note
                    // that if want_summary was given, then the memory needed
                    // for storing the solutions is not taken into account
                    double vm, rss;
                    process_mem_usage(vm, rss);
                    ksolution.set_mem_usage((vm + rss)/1024.0);
#endif

                    // Verify the solution, if the user requested so and show
                    // the result on the terminal
                    if (!no_doctor) {
                        ksolution.doctor ();
                    }
                    std::cout << khs::set_mode ((want_color ? "color" : "console")) << ksolution << std::endl;

                    // add this solution to all solutions generated by this
                    // solver
                    bag += ksolution;

                    // in case verbose output was requested, show every solution
                    // path to this instance
                    if (want_verbose) {
                        for (auto i = 0 ; i < ksolution.size (); i++) {

                            // show information on every solution in blocks of 10
                            if (i%10 == 0) {
                                std::cout << std::endl;
                                std::cout << "      " << ksolution[0].headers (khs::set_mode ((want_color ? "color" : "console"))) << std::endl;
                            }
                            std::cout << "          ◦ " << khs::set_mode ((want_color ? "color" : "console")) << ksolution[i] << std::endl;
                            if (want_solution) {
                                for (std::size_t idx=0; auto const& istate: ksolution[i].get_solution ()) {
                                    std::ostringstream sstate;
                                    sstate << istate;
                                    
                                    if (idx == 0) {
                                        std::cout << "               🙪 " << std::format ("{}{}{}", (want_color ? khs::ansi::Gainsboro : ""), sstate.str (), (want_color ? khs::ansi::reset : "")) << std::endl;
                                    } else if (idx == ksolution[i].get_solution ().size () - 1) {
                                        std::cout << "               🙫 " << std::format ("{}{}{}", (want_color ? khs::ansi::Gainsboro : ""), sstate.str (), (want_color ? khs::ansi::reset : "")) << std::endl;
                                    } else {
                                        std::cout << "                 " << std::format ("{}{}{}", (want_color ? khs::ansi::LightSlateGrey : ""), sstate.str (), (want_color ? khs::ansi::reset : "")) << std::endl;
                                    }
                                    idx++;
                                }
                            }
                        }
                    }

                    // and increment the line counter
                    lineno++;

                    // free the manager
                    delete m;
                }

                // Stop the chronometer and show the time taken to solve all instances with this value of k
                auto kend = std::chrono::high_resolution_clock::now();
                std::cout << std::endl << "   🕒 (k=" << k << ") CPU time: " << 1e-9*std::chrono::duration_cast<std::chrono::nanoseconds>(kend - kstart).count() << " seconds" << std::endl;
            }

            // Stop the chronometer and show the time taken by this solver
            auto tend = std::chrono::system_clock::now ();
            std::cout << std::endl << "   🕒 (" << solver_name << ") CPU time: " << 1e-9*std::chrono::duration_cast<std::chrono::nanoseconds>(tend - tstart).count() << " seconds" << std::endl;

            // add all solutions generated to the results to report
            _results += bag;
        }
    }

    // show an error summary of the results obtained by the different
    // invocations to run only if no_doctor is false
    void show_error_summary (bool no_doctor) {

        std::cout << std::endl;
        if (no_doctor) {
            std::cout << " 📊 Error summary: <doctor disabled>" << std::endl;
        } else {

            // show the error summary of the results obtained by the different
            // invocations to run
            std::cout << " 📊 Error summary: " << std::endl;
            std::cout << _results.get_error_summary () << std::endl;
        }

        // perform the cross-validation among solutions generated by different
        // solvers with the same value of k
        std::cout << "        Cross-validation: ";
        if (_results.doctor ()) {
            std::cout << "Ok" << std::endl;
        } else {
            std::cout << "Failure" << std::endl;

            // And show the offending pair
            for (auto const& ierror : _results.get_error_solutions ()) {

                auto first = ierror.get_first ();
                auto second = ierror.get_second ();

                // The difference can not be in the size of each solution or, in
                // case it is, that should have been already detected when using
                // the option --doctor. Thus, each solution is examined just to
                // find the first pair that did not match
                for (int i = 0 ; i < first.size () ; i++) {

                    // cost
                    if (first[i].get_cost () != second[i].get_cost ()) {
                        std::cout << "\t\t" << first << std::endl;
                        std::cout << "\t\t\t" << first[i] << std::endl;
                        std::cout << "\t\t" << second << std::endl;
                        std::cout << "\t\t\t" << second[i] << std::endl << std::endl;

                        // and stop showing other solutions
                        continue;
                    }
                }
            }
        }
    }

    // store the results in a csv file in case any has been given
    void write_csv (const std::string& filename) {

        // only in case a filename for the output csv file has been given
        if (filename.size ()) {

            // create the output file and truncate its contents in case any exists
            std::ofstream csvstream {filename, std::ios::out | std::ios::trunc};
            csvstream << _results;
            std::cout << " 🛈 CSV file generated!" << std::endl;
        } else {
            std::cout << " ⚠ No CSV output generated!" << std::endl;
        }
    }

}; // class solver<D>


#endif // _SOLVER_H_

// Local Variables:
// mode:cpp
// fill-column:80
// End:
