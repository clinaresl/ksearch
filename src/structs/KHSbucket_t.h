// -*- coding: utf-8 -*-
// KHSbucket_t.h
// -----------------------------------------------------------------------------
//
// Started on <lun 07-08-2023 18:25:00.445545850 (1691425500)>
// Carlos Linares López <carlos.linares@uc3m.es>
// Ian Herman <iankherman@gmail.com>   Ian Herman <iankherman@gmail.com>

//
// Definition of an open list as a bucket of nodes
//

#ifndef _KHSBUCKET_T_H_
#define _KHSBUCKET_T_H_

#include<algorithm>
#include<iostream>
#include<stddef.h>
#include<stdexcept>
#include<vector>

#include "../KHSdefs.h"

namespace khs {

  // Class definition
  template<typename T>
  class bucket_t {

    private:

      // INVARIANT: a bucketed vector-vector consists of a vector of vectors with
      // a non-negative size. All operations preserve the range [mini,maxi] of
      // values containing items unless size=0 in which case mini=maxi=1
      std::vector<std::vector<T>> _queue;            // f-indexes to the buckets
      size_t _size;                          // number of elements in the queue
      int _mini;                                               // minimum index
      int _maxi;                                               // maximum index

    public:

      // Default constructor
      bucket_t ():
        _queue  { std::vector<std::vector<T>> (1, std::vector<T>() ) },
        _size   { 0 },                              // there is only one bucket!
        _mini   { 1 },        // INVARIANT: _mini and _maxi have to be above the
        _maxi   { 1 }           // current number of items if the queue is empty
      { }

      // Explicit constructor - it is also possible to create a bucket giving
      // the initial number of buckets
      bucket_t (size_t capacity):
        _queue  { std::vector<std::vector<T>> (capacity, std::vector<T>() ) },
        _size   { 0 },
        _mini   { 1 },       // INVARIANT: _mini and _maxi have to be above the
        _maxi   { 1 }          // current number of items if the queue is empty
      { }

      // Copy and assignment constructors are explicitly forbidden
      bucket_t (const bucket_t&) = delete;
      bucket_t (bucket_t&&) = delete;

      // Assignment and move operators are disallowed as well
      bucket_t& operator=(const bucket_t&) = delete;
      bucket_t& operator=(bucket_t&&) = delete;

      // get accessors
      const size_t get_nbbuckets () const
      { return _queue.size (); }
      const size_t size () const
      { return _size; }
      const size_t size (const int idx) const
      { return _queue[idx].size (); }
      const int get_mini () const
      { return _mini; }
      const int get_maxi () const
      { return _maxi; }

      // set accessors

      // set the number of buckets to the value specified. In case the number of
      // buckets requested is too large an exception is thrown
      const size_t set_nbbuckets (const size_t nbbuckets);

      // methods

      // add the given item to the bucket with the specified index. It returns
      // true if the operation was successful and false otherwise. It also takes
      // care of preserving the consistency of other internal data. It raises an
      // exception in case the index exceeds the maximum capacity of the buckets
      //
      // It specifically verifies that the number of buckets is enough for
      // inserting the new item. If not, additional space is created
      bool insert (const T& item, const int idx);

      // remove returns and erases the first item with the specified index. If
      // the bucket is empty an exception is thrown. It also takes care of
      // preserving the consistency of other internal data
      T remove (const int idx);

      // pop_front extracts the first item with the minimum index
      T pop_front ()
      { return remove (_mini); }

      // returns the first element without popping it
      T front () {
          return std::move(_queue[_mini].back ());
      }

      bool empty() {
          return _size == 0;
      }

  }; // class bucket_t<T>

  // set the number of buckets to the value specified. In case the number of
  // buckets requested is too large an exception is thrown
  template<typename T>
  const size_t bucket_t<T>::set_nbbuckets (const size_t nbbuckets) {

    // if too many buckets were requested an exception is thrown
    if (nbbuckets >= _queue.max_size ()) {
      throw std::range_error ("[bucketvd::set_nbbuckets] too many buckets!");
    }

    // if the current number of buckets is below the number requested then
    // resize the queue as needed
    auto sz = _queue.size ();
    while (sz < nbbuckets) {
      sz *= 2;
    }
    _queue.resize (sz);

    // and return the current number of buckets
    return _queue.size ();
  }

  // add the given item to the bucket with the specified index. It returns true
  // if the operation was successful and false otherwise. It also takes care of
  // preserving the consistency of other internal data. It raises an exception
  // in case the index exceeds the maximum capacity of the buckets
  //
  // It specifically verifies that the number of buckets is enough for inserting
  // the new item. If not, additional space is created
  template<typename T>
  bool bucket_t<T>::insert (const T& item, const int idx)
  {

    // ensure this bucket can accomodate values in the i-th slot. 1 is added
    // because the first index is 0.
    set_nbbuckets (1+idx);

    // insertion --- bucketed vector-vectors are used to insert by the back
    _queue [idx].push_back (item);

    // update internal data
    _size++;
    _mini = (_size == 1) ? idx : std::min (idx, _mini);
    _maxi = (_size == 1) ? idx : std::max (idx, _maxi);

    // exit with success
    return true;
  }

  // remove returns and erases the first item with the specified index. If
  // the bucket is empty an exception is thrown. It also takes care of
  // preserving the consistency of other internal data
  template<typename T>
  T bucket_t<T>::remove (const int idx)
  {

    // in case the corresponding bucket is empty, throw an exception
    if (!_queue [idx].size ()) {
      throw std::domain_error ("[bucket_t::remove] The idx-th bucket is empty!");
    }

    // extract the first item from the idx bucket and erase it
    T item = std::move(_queue[idx].back ());
    _queue [idx].pop_back ();

    // internal data
    _size--;                           // decrement the overall number of items

    // update the current range
    if (!_size) {            // if there are no more items in all the structure
      _mini = _maxi = 1;                      // then update the current bounds
    }
    else {
      if (!_queue [idx].size ()) {              // if this bucket becomes empty

        // locate the first non-empty bucket and update the corresponding bound
        if (idx <= _mini) {           // in case we are below the maximum index
          for (_mini=idx;_mini<=_maxi && !_queue[_mini].size ();_mini++);// fwd
        }
        if (idx >= _maxi) {
          for (_maxi=idx;_maxi>=_mini && !_queue[_maxi].size ();_maxi--);// bwd
        }
      }
    }

    // and exit
    return item;
  }

} // namespace khs

#endif // _KHSBUCKET_T_H_

// Local Variables:
// mode:cpp
// fill-column:80
// End:
