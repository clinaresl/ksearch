// -*- coding: utf-8 -*-
// KHSclosed_t.h
// -----------------------------------------------------------------------------
//
// Started on <lun 07-08-2023 19:47:41.880272912 (1691430461)>
// Carlos Linares López <carlos.linares@uc3m.es>
// Ian Herman <iankherman@gmail.com>

//
// Implementation of closed lists as associative containers indexed by their
// hash value
//

#ifndef _KHSCLOSED_T_H_
#define _KHSCLOSED_T_H_

#include<iterator>
#include<string>
#include<unordered_map>
#include<utility>
#include<vector>

namespace khs {

    using namespace std;

    template<typename T>
    class closed_t {

    private:

        // INVARIANT: closed lists are used mostly for membership operations. As
        // such, they are implemented with an unordered associative container
        // indexed by hash values that can handle collisions
        unordered_multimap<T, size_t> _closed;

        // in addition, every element in the closed list points to another item
        // in closed as well ---though that's the responsibility of items stored
        // in the closed list. The following list is used to update information
        // about entries stored in the closed list
        vector<T> _list;

    public:

        // Default constructor
        closed_t () = default;

        // modifiers

        // insert an item into closed, both in the associative container and the
        // vector, and return a stable index to it in the vector. It also
        // ensures that nodes inserted are properly indexed in the closed list
        size_t insert (T& item) {

            // first, insert this item into the vector used for providing stable
            // indices after setting its location into the internal vector
            size_t index = _list.size ();
            item.set_pointer (index);
            _list.push_back (item);

            // secondly, insert it into the associative container
            _closed.insert ({item, index});

            // and return the location of this item into the vector
            return index;
        }

        // Lookup

        // given a stable index into closed, return the item that is located at
        // that position. In case the index is incorrect, the behaviour is
        // undefined
        T& operator[] (const size_t index) {
            return _list[index];
        }

        // return a stable index to the item given in case it is found in the
        // vector of the closed list. Otherwise, return string::npos
        size_t find (const T& item) const {

            // compute the range of elements with the hash key of the given
            // item. 'equal_range' returns two iterators to occurrences {T,
            // size_t}: the first pointing to the beginning, and the second
            // pointing to the end
            auto range = _closed.equal_range (item);

            // and now traverse the "collision list", so to say the items with
            // the same hash key
            for (auto it = range.first ; it != range.second ; ++it) {

                // if this is the requested item, return its value, i.e., the
                // stable index to the vector in closed. Now, 'it' is a single
                // occurrence of the form {T, size_t}
                if (it->first == item) {
                    return it->second;
                }
            }

            // if the item has not been found in closed, then return a null
            // value
            return string::npos;
        }

        // capacity

        // return the number of elements in this closed list
        size_t size ()
            { return _list.size (); }

    }; // class closed_t<T>

} // namespace khs

#endif // _KHSCLOSED_T_H_

// Local Variables:
// mode:cpp
// fill-column:80
// End:
