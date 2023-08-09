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

        // INVARIANT: the solution of a k-shortest path problem consists of k
        // solution paths (that might be less, in the case of loopless paths)
        // with the same start and goal
        int _k;                                                   // value of k
        T _start;                               // start state of all solutions
        T _goal;                                 // goal state of all solutions
        std::vector<solution_t<T>> _solutions;        // container of solutions

    public:

        // Default constructor is forbidden
        ksolution_t () = delete;

        // Explicit constructor
        ksolution_t (int k, const T& start, const T& goal) :
            _k { k },
            _start { start },
            _goal { goal },
            _solutions { std::vector<solution_t<T>>() }
            {}

        // copy and move constructors are forbidden
        ksolution_t (const ksolution_t&) = delete;
        ksolution_t (ksolution_t&&) = delete;

        // getters
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

        // operator overloading

        // assignment and move operators are dissallowed as well
        ksolution_t& operator=(const ksolution_t&) = delete;
        ksolution_t& operator=(ksolution_t&&) = delete;

        // solutions can be augmented by adding a single solution
        ksolution_t& operator+= (const solution_t<T>& right) {

            // add this solution to the container
            _solutions.push_back (right);
            return *this;
        }

        // random access operator
        const solution_t<T> operator[] (const size_t idx) const {
            if (idx >= _solutions.size ()) {
                throw std::out_of_range ("[ksolution_t::get] out of bounds!");
            }
            return _solutions[idx];
        }

        // stream out
        friend ostream& operator<< (std::ostream& stream, const ksolution_t& solutions) {

            // data is written in the csv format using the semicolon as
            // separator. Because each line results of the concatenation of data
            // provided by this instance and also its specific solutions, write
            // first to a string stream and then copy its output to the given
            // stream
            std::stringstream ss;
            ss << "id;start;goal;h0;cost;expansions;runtime;expansions/sec;solver" << endl;
            for (const auto& s : solutions.get_solutions ()) {
                ss << s << std::endl;
            }

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
