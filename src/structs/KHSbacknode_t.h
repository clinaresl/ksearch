// -*- coding: utf-8 -*-
// KHSbacknode_t.h
// -----------------------------------------------------------------------------
//
// Started on <lun 07-08-2023 16:45:08.285586486 (1691419508)>
// Carlos Linares López <carlos.linares@uc3m.es>
//

//
// Definition of nodes as used by m-Dijkstra/m-A*
//

#ifndef _KHSBACKNODE_T_H_
#define _KHSBACKNODE_T_H_

#include<vector>

#include "KHSnode_t.h"

namespace khs {

    // Backpointers are used to reconstruct the solution path. They consist of
    // an index within a container and the location of the object pointed to
    struct backpointer_t {

        // INVARIANT: every backpointer contains an index within a container and
        // a position within the vector of the object pointed to
        size_t _pointer;
        size_t _index;

        // Constructor
        backpointer_t (size_t pointer, size_t index) :
            _pointer { pointer },
            _index { index } {}

        // operators
        bool operator==(const backpointer_t& other) const {
            return _pointer == other._pointer && _index == other._index;
        }

        // getters
        const size_t& get_pointer () const { return _pointer; }
        const size_t& get_index () const { return _index; }
    }; // backpointer_t

    template <typename T>
    class backnode_t : public node_t<T> {

    private:

        // INVARIANT: backnodes use a vector of backpointers to reconstruct the
        // solution path. Backpointers consist not only of an index within a
        // container but also its location within the vector of the object
        // pointed to.
        std::vector<backpointer_t> _backpointers;

    public:

        // Default constructors are forbidden
        backnode_t () = delete;

        // Explicit constructors
        backnode_t (const T& state, int h=0, int g=0) :
            node_t<T> { state, h, g }
        {}

        // Copy constructor and assignment constructor by default
        backnode_t (const backnode_t&) = default;
        backnode_t (backnode_t&&) = default;

        // getters
        const std::vector<backpointer_t>& get_backpointers () const
            { return _backpointers; }
        const backpointer_t& get_backpointer (size_t index) const
            { return _backpointers[index]; }

        // setters
        void set_backpointer (const backpointer_t& backpointer)
            { _backpointers = std::vector<backpointer_t>{backpointer}; }

        // operators

        // Adding inserts a new backpointer to the list of backpointers of this
        // node
        backnode_t<T>& operator+= (const backpointer_t& backpointer) {
            _backpointers.push_back (backpointer);
            return *this;
        }
    }; // class backnode_t<T

} // namespace khs

namespace std {

    // Definition of a hash function for backnodes. The definition is included
    // in the std namespace so that it can be used directly by the functions
    // implemented in that namespace
    template<typename T>
    struct hash<khs::backnode_t<T>> {

        // hash function
        size_t operator() (const khs::backnode_t<T>& node) const {

            // just return the hash value implemented in the hash function of
            // the underlying type which has to be defined in the std namespace
            return std::hash<T>{}(node.get_state ());
        }

    }; // struct hash<node_t<T>>
} // namespace std


#endif // _KHSBACKNODE_T_H_

// Local Variables:
// mode:cpp
// fill-column:80
// End:
