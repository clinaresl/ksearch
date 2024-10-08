// -*- coding: utf-8 -*-
// KHSksolution_t.h
// -----------------------------------------------------------------------------
//
// Started on <mié 09-08-2023 13:40:09.043459139 (1691581209)>
// Carlos Linares López <carlos.linares@uc3m.es>
// Ian Herman <iankherman@gmail.com>

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

    template<typename T, template <typename...> class path_t>
    class ksolution_t {

        // INVARIANT: the solution of a k-shortest path problem (identified with
        // a name) consists of k solution paths with the same start and goal
        string _name;                                          // instance name
        int _k;                                                   // value of k
        T _start;                               // start state of all solutions
        T _goal;                                 // goal state of all solutions
        std::vector<solution_t<T, path_t>> _solutions;   // container solutions

        // In addition, a number of statistics are reported. Note that these are
        // equal to the same statistics of the last single solution reported in
        // this container
        int _nbcentroids;       // # centroids used in the last single solution
        int _h0;                       // heuristic distance of the start state
        size_t _expansions;                       // total number of expansions
        double _cpu_time;                                   // elapsed CPU time
        size_t _mem_usage;                                // memory usage in MB

        // all the work being carried out by a specific solver whose signature
        // is stored also in this container
        string _solver;

        // Importantly, a solution to the k-shortest path problem must be verifiable
        // and, as a result, an error code should be given
        solution_error _error_code;

        // Finally, to improve traceability, the version of the code that
        // generated this solution is stored as well
        string _version;

    public:

        // Default constructor is forbidden
        ksolution_t () = delete;

        // Explicit constructor
        ksolution_t (int k, const T& start, const T& goal) :
            _name { "" },
            _k { k },
            _start { start },
            _goal { goal },
            _nbcentroids { 0 },
            _solutions { std::vector<solution_t<T, path_t>>() },
            _h0 { 0 },
            _expansions { 0 },
            _cpu_time { 0.0 },
            _mem_usage { 0 },
            _solver { "" },
            _version { "" }
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
        const int get_nbcentroids () const {
            return _nbcentroids;
        }
        const std::vector<solution_t<T, path_t>>& get_solutions () const {
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
        const double get_mem_usage () const {
            return _mem_usage;
        }
        const string& get_solver () const {
            return _solver;
        }
        const solution_error get_error_code () const {
            return _error_code;
        }
        const string& get_version () const {
            return _version;
        }

        // setters
        void set_name (const string& name) {
            _name = name;
        }
        void set_solver (const string& solver) {
            _solver = solver;
        }

        void set_version (const string& version) {
            _version = version;
        }

        void set_mem_usage (const size_t mem_usage) {
            _mem_usage = mem_usage;

            // And here comes the trick. Memory usage is measured only when the
            // k shortest paths have been computed, i.e., we do not want to slow
            // down the algorithm by measuring memory usage for every single
            // solution. This means that memory usage has to be stored in the
            // last solution in this container
            _solutions[_solutions.size ()-1].set_mem_usage (mem_usage);
        }

        // operator overloading

        // Conversion operator to make all solutions stored as vectors
        //
        // This allows us to convert ksolution_t classes with different
        // underlying containers into ones with underlying vectors so they can
        // all be placed into a ksolutions_t class after the algorithm is done
        // running, and said conversion won't count towards runtime
        operator ksolution_t<T, std::vector>() {
            ksolution_t<T, std::vector> n(this->_k, this->_start, this->_goal);
            for (auto & i : _solutions)
                n += static_cast<solution_t<T, std::vector>>(i);
            return n;
        }


        // solutions can be augmented by adding a single solution
        ksolution_t& operator+= (const solution_t<T, path_t>& right) {

            // add this solution to the container
            _solutions.push_back (right);

            // and update the statistics of the container of solutions
            _nbcentroids = right.get_nbcentroids ();
            _h0 = right.get_h0 ();
            _expansions = right.get_expansions ();
            _cpu_time = right.get_cpu_time ();

            return *this;
        }

        // or adding another container directly
        ksolution_t& operator+= (ksolution_t<T, path_t>& right) {

            // add every solution in the given container
            for (auto i = 0 ; i < right.size () ; i++) {
                _solutions.push_back (right[i]);
            }

            // and update the statistics of the container of solutions copying
            // those from the last single solution given in right
            _nbcentroids = right[right.size ()-1].get_nbcentroids ();
            _h0 = right[right.size ()-1].get_h0 ();
            _expansions = right[right.size ()-1].get_expansions ();
            _cpu_time = right[right.size ()-1].get_cpu_time ();

            return *this;
        }

        // random access operator
        solution_t<T, path_t>& operator[] (const size_t idx) {
            if (idx >= _solutions.size ()) {
                throw std::out_of_range ("[ksolution_t::get] out of bounds!");
            }
            return _solutions[idx];
        }

        // methods

        // doctor verifies that this solution to a k shortest path problem is
        // correct:
        //
        // 1. It verifies every solution separately;
        //
        // 2. it also verifies that the cost of every solution is greater or
        //    equal than the cost of the previous one, i.e., that the algorithm
        //    that computed this solution to the k shortest-path problem is
        //    *stable*
        //
        // 3. It also verifies that there are no duplicate solution paths.
        //
        // 4. Lastly, it verifies that the number of solutions is correct.
        //    Because this is not necessarily speaking an error (one might
        //    require a number of solution paths that exceeds the number of
        //    existing ones), this is verified last, so that if this error is
        //    given, all the other verifications are known to have been correct.
        bool doctor () {

            // By default, no error is detected
            _error_code = solution_error::NO_ERROR;

            // verify every solution separately
            for (auto& solution : _solutions) {
                if (!solution.doctor ()) {
                    _error_code = solution.get_error_code ();
                }
            }

            // and in case any failed, then record the whole set as faulty using
            // the error code of the last error
            if (_error_code != solution_error::NO_ERROR) {
                return false;
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

            // next, verify there are no duplicate solution paths in this
            // solution to the k shortest-path problem
            for (auto i = 0 ; i < _solutions.size () ; i++) {
                for (auto j = i+1 ; j < _solutions.size () ; j++) {
                    if (_solutions[i].same_solution_path (_solutions[j])) {
                        _error_code = solution_error::ERR_DUPLICATE_PATH;
                        return false;
                    }
                }
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

        // the following service removes the i-th solution from this container
        void remove (const size_t idx) {

            // first, verify the index is within bounds
            if (idx >= _solutions.size ()) {
                throw std::out_of_range ("[ksolution_t::remove] out of bounds!");
            }

            // and then remove the i-th solution
            _solutions.erase (_solutions.begin () + idx);
        }

        // stream out
        friend ostream& operator<< (std::ostream& stream, const ksolution_t& solutions) {

            // data is written in the csv format using the semicolon as
            // separator. Because each line results of the concatenation of data
            // provided by this instance and also its specific solutions, write
            // first to a string stream and then copy its output to the given
            // stream
            std::stringstream ss;

            ss << solutions.get_name () << ";";
            ss << solutions.get_k () << ";";
            ss << solutions.get_start () << ";";
            ss << solutions.get_goal () << ";";
            ss << solutions.get_h0 () << ";";
            ss << solutions.get_expansions() << ";";
            ss << solutions.get_nbcentroids () << ";";
            ss << solutions.get_mem_usage () << ";";
            ss << solutions.get_cpu_time() << ";";
            ss << solutions.get_solver () << ";";
            ss << solution_t<T, path_t>::get_error_msg (solutions.get_error_code ()) << ";";
            ss << solutions.get_version ();

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
