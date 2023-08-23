// -*- coding: utf-8 -*-
// KHSksolution_t.h
// -----------------------------------------------------------------------------
//
// Started on <mié 09-08-2023 13:40:09.043459139 (1691581209)>
// Carlos Linares López <carlos.linares@uc3m.es>
//

//
// Representation of the k solution paths to a k-shortest path problem
//

#ifndef _KHSKSOLUTION_T_H_
#define _KHSKSOLUTION_T_H_

#include <algorithm>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include "KHSsolution_t.h"

namespace khs {

    template<typename T>
    class ksolution_t {

        // INVARIANT: the solution of a k-shortest path problem (identified with
        // a name) consists of k solution paths (that might be less, in the case
        // of loopless paths) with the same start and goal
        string _name;                                          // instance name
        int _k;                                                   // value of k
        T _start;                               // start state of all solutions
        T _goal;                                 // goal state of all solutions
        std::vector<solution_t<T>> _solutions;        // container of solutions

        // In addition, a number of statistics are reported. Note that these are
        // equal to the same statistics of the last single solution reported in
        // this container
        int _h0;                       // heuristic distance of the start state
        size_t _expansions;                       // total number of expansions
        double _cpu_time;                                   // elapsed CPU time

        // all the work being carried out by a specific solver whose signature
        // is stored also in this container
        string _solver;

        // Finally, a solution to the k-shortest path problem must be verifiable
        // and, as a result, an error code should be given
        solution_error _error_code;

    public:

        // Default constructor is forbidden
        ksolution_t () = delete;

        // Explicit constructor
        ksolution_t (int k, const T& start, const T& goal) :
            _name { "" },
            _k { k },
            _start { start },
            _goal { goal },
            _solutions { std::vector<solution_t<T>>() },
            _h0 { 0 },
            _expansions { 0 },
            _cpu_time { 0.0 },
            _solver { "" }
            {
                // Initially solutions are not checked unless the doctor service
                // is invoked
                _error_code = solution_error::UNCHECKED;
            }

        // getters
        const string& get_name () const {
            return _name;
        }
        const int get_k () const {
            return _k;
        }
        const T& get_start () const {
            return _start;
        }
        const T& get_goal () const {
            return _goal;
        }
        const std::vector<solution_t<T>>& get_solutions () const {
            return _solutions;
        }
        const int get_h0 () const {
            return _h0;
        }
        const size_t get_expansions () const {
            return _expansions;
        }
        const double get_cpu_time () const {
            return _cpu_time;
        }
        const string& get_solver () const {
            return _solver;
        }
        const solution_error get_error_code () const {
            return _error_code;
        }

        // setters
        void set_name (const string& name) {
            _name = name;
        }
        void set_solver (const string& solver) {
            _solver = solver;
        }

        // operator overloading

        // solutions can be augmented by adding a single solution
        ksolution_t& operator+= (const solution_t<T>& right) {

            // add this solution to the container
            _solutions.push_back (right);

            // and update the statistics of the container of solutions
            _h0 = right.get_h0 ();
            _expansions = right.get_expansions ();
            _cpu_time = right.get_cpu_time ();

            return *this;
        }

        // or adding another container directly
        ksolution_t& operator+= (ksolution_t<T>& right) {

            // add every solution in the given container
            for (auto i = 0 ; i < right.size () ; i++) {
                _solutions.push_back (right[i]);
            }

            // and update the statistics of the container of solutions copying
            // those from the last single solution given in right
            _h0 = right[right.size ()-1].get_h0 ();
            _expansions = right[right.size ()-1].get_expansions ();
            _cpu_time = right[right.size ()-1].get_cpu_time ();

            return *this;
        }

        // random access operator
        solution_t<T>& operator[] (const size_t idx) {
            if (idx >= _solutions.size ()) {
                throw std::out_of_range ("[ksolution_t::get] out of bounds!");
            }
            return _solutions[idx];
        }

        // doctor verifies that this solution to a k shortest path problem is
        // correct: on one one hand, it verifies every solution separately; it
        // also verifies that the cost of every solution is greater or equal
        // than the cost of the previous one
        bool doctor () {

            // By default, no error is detected
            _error_code = solution_error::NO_ERROR;

            // verify every solution separately
            for (auto& solution : _solutions) {
                if (!solution.doctor ()) {
                    _error_code = solution.get_error_code ();
                    return false;
                }
            }

            // in case every solution is correct, verify now that the costs are
            // monotonically increasing
            int prev = 0;
            for (auto& solution : _solutions) {
                if (solution.get_cost () < prev) {
                    _error_code = solution_error::ERR_INCR_COST;
                    return false;
                }
                prev = solution.get_cost ();
            }

            // finally, verify the number of solutions is correct
            if (_solutions.size () != _k) {

                // if the number of solutions computed so far is different then
                // an error has been found unless start=goal
                if (!(_start == _goal)) {
                    _error_code = solution_error::ERR_NUM_SOLUTIONS;
                    return false;
                }
            }

            // at this point, this solution to the k-shortest path problem is
            // proven to be correct
            return true;
        }

        // stream out
        friend ostream& operator<< (std::ostream& stream, const ksolution_t& solutions) {

            // data is written in the csv format using the semicolon as
            // separator. Because each line results of the concatenation of data
            // provided by this instance and also its specific solutions, write
            // first to a string stream and then copy its output to the given
            // stream
            std::stringstream ss;
            // for (const auto& s : solutions.get_solutions ()) {
            //     ss << s << std::endl;
            // }

            ss << solutions.get_name () << ";";
            ss << solutions.get_start () << ";";
            ss << solutions.get_goal () << ";";
            ss << solutions.get_h0 () << ";";
            ss << solutions.get_expansions() << ";";
            ss << solutions.get_cpu_time() << ";";
            ss << solutions.get_solver () << ";";
            ss << solution_t<T>::get_error_msg (solutions.get_error_code ());

            // and now redirect the contents of the string stream to the given
            // stream
            stream << ss.str ();

            return stream;
        }

        // methods
        size_t size () const {
            return _solutions.size ();
        }

    }; // class ksolution_t
} // namespace khs


#endif // _KHSKSOLUTION_T_H_

// Local Variables:
// mode:cpp
// fill-column:80
// End:
