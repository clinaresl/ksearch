// -*- coding: utf-8 -*-
// solver.h
// -----------------------------------------------------------------------------
//
// Started on <lun 28-08-2023 00:46:32.176420194 (1693176392)>
// Carlos Linares López <carlos.linares@uc3m.es>
//

//
// Services of a domain-dependent solver for the k shortest-path problem
//

#ifndef _SOLVER_H_
#define _SOLVER_H_

#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <tuple>
#include <vector>

#include "helpers.h"
#include "../src/ksearch.h"

template <typename D>
class solver {

private:

    // INVARIANT: A domain-dependent solver is applied over a specific domain
    // and variant ---where the typename D is the domain type of application,
    // e.g., npancake. A test file with the start states to solve must be given,
    // and also a specification of the k values to try which are given as a
    // string that has to be decoded
    std::string _domain;                                              // Domain
    std::string _variant;                                            // Variant
    std::string _testfile;                                         // Test file
    std::string _kspec;                         // specification of the k values

    // the goal state is assumed to be the same for all instances
    std::unique_ptr<D> _goal;

    // the k values to try are internally stored as a vector of tuples, each
    // specifying the first value to try, the last one and the step between
    // successive values of k
    std::vector<std::tuple<int, int, int>> _ks;

    // The result of reading the test files are stored internally as s couple of
    // vectors. The first one contains the name of every instance, and the
    // second one contains the instance itself
    std::vector<std::string> _names;
    std::vector<D> _instances;

    // the results of the execution are internally stored in a container of
    // solutions of instances of the k shortest-path problem
    khs::ksolutions_t<D> _results;

    public:

    // Default constructors are forbidden
    solver () = delete;

    // Explicit constructor. Both the domain and variant should be given, along
    // with the test file, the specification of the k values to try, and the
    // goal state to consider in *all* instances
    solver (std::string domain, std::string variant,
            std::string testfile, std::string kspec,
            std::unique_ptr<D> goal) :
        _domain { domain },
        _variant { variant },
        _testfile { testfile },
        _kspec { kspec },
        _goal { std::move (goal) }
        {

            // process the specification of values of k and store the specific
            // limits for every application
            _ks = split_ks (_kspec);

            // next, read the contents of the test file and store the names and
            // instances internally
            get_problems<D>(_testfile, _names, _instances);
        }

    // getters
    std::string get_domain () const { return _domain; }
    std::string get_variant () const { return _variant; }
    std::string get_testfile () const { return _testfile; }
    std::string get_kspec () const { return _kspec; }

    // methods

    // the following service automates the execution of the given solver over
    // all instances found in the test file with all values obtained from the
    // specification of the k values, and stores the results internally. In case
    // want_verbose takes the value true additional information is shown on the
    // standard output
    void run (std::string solver_name, bool want_verbose) {

        // set the domain and variant in the container of ksolutions
        _results.set_domain (_domain);
        _results.set_variant (_variant);

        // solve all the instances with the given solver

        // initialize a container for storing all solutions generated in this
        // execution
        khs::ksolutions_t<D> bag;
        bag.set_domain (_domain);
        bag.set_variant (_variant);

        // for all values of k selected by the user
        for (auto ispec: _ks) {

            // consider this single specification of k values
            for (auto k = std::get<0>(ispec) ; k <= std::get<1>(ispec) ; k+= std::get<2>(ispec)) {

                // for all instances
                std::cout << std::endl;
                std::cout << " ⏺ " << solver_name << " ( k=" << k <<  " ): " << std::endl;

                for (auto i = 0 ; i < _instances.size () ; i++) {

                    // create a manager to solve this specific instance
                    khs::bsolver<D>* m = get_solver (solver_name, _instances[i], *_goal, k);

                    std::cout << " ⏵ "; std::cout.flush ();
                    auto ksolution = m->solve ();

                    // give a name to every individual solution
                    for (auto j = 0 ; j < ksolution.size () ; j++) {
                        ksolution[j].set_name (_names[i] + "/" + std::to_string (1+j));
                    }

                    // and show the result on the standard output. Prior to
                    // that, give it a name so that it can be easily recognized
                    // and record also the name of the solver used
                    ksolution.set_name (_names[i]);
                    ksolution.set_solver (m->signature ());
                    ksolution.doctor ();
                    std::cout << ksolution << std::endl;

                    // add this solution to all solutions generated by this
                    // solver
                    bag += ksolution;

                    // in case verbose output was requested, show every solution
                    // path to this instance
                    if (want_verbose) {
                        for (auto i = 0 ; i < ksolution.size (); i++) {
                            std::cout << "   → " << ksolution[i] << std::endl;
                        }
                    }

                    // free the manager
                    delete m;
                }
            }

            // add all solutions generated to the results to report
            _results += bag;
        }
    }

    // show an error summary of the results obtained by the different
    // invocations to run
    void show_error_summary () const {

        // show the error summary of the results obtained by the different
        // invocations to run
        std::cout << std::endl;
        std::cout << " 📊 Error summary: " << std::endl;
        std::cout << _results.get_error_summary () << std::endl;
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
