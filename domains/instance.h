// -*- coding: utf-8 -*-
// instance.h
// -----------------------------------------------------------------------------
//
// Started on <sáb 09-05-2026 23:50:54.732581975 (1778363454)>
// Carlos Linares López <carlos.linares@uc3m.es>
//

//
// Definition of a specific instance to solve
//

#ifndef _INSTANCE_H_
#define _INSTANCE_H_

#include <string>

// Definition of an instance
template <typename T>
class instance_t {

private:

    // INVARIANT: every instance consists of a name and a pair of states, the
    // start and goal state
    std::string _name;
    T _start;
    T _goal;

public:

    // Explicit constructor
    instance_t (std::string name, T start, T goal) :
        _name { name },
        _start { start },
        _goal { goal }
        {}

    // getters
    [[nodiscard]] std::string get_name () const { return _name; }
    [[nodiscard]] T get_start () const { return _start; }
    [[nodiscard]] T get_goal () const { return _goal; }

    friend std::ostream& operator<< (std::ostream& stream, instance_t<T> right) {
        stream << right.get_name () << " " << right.get_start () << " " << right.get_goal ();
        return stream;
    }

}; // class instance_t<T>

#endif // _INSTANCE_H_

// Local Variables:
// mode:cpp
// fill-column:80
// End:
