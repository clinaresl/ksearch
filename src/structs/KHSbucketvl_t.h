/* 
  KHSbucketvl_t.h
  Description: Definition of 1-buckets represented with vectors, each
  hosting a list. It also adds a pointer to separate the 1-bucket into
  two segments 
  ----------------------------------------------------------------------------- 

  Started on  <Sun May 29 14:48:38 2016 Carlos Linares Lopez>
  Last update <miércoles, 01 junio 2016 12:44:18 Carlos Linares Lopez (clinares)>
  -----------------------------------------------------------------------------

  $Id::                                                                      $
  $Date::                                                                    $
  $Revision::                                                                $
  -----------------------------------------------------------------------------

  Made by Carlos Linares Lopez
  Login   <clinares@atlas>
*/

#ifndef   	KHSBUCKETVL_T_H_
# define   	KHSBUCKETVL_T_H_

#include <algorithm>
#include <exception>
#include <iostream>
#include <iterator>
#include <limits>
#include <list>
#include <stdexcept>
#include <vector>

#include "../KHSdefs.h"

namespace khs {

  using namespace std;

  // Class definition
  template<class T>
  class bucketvl_t {

    // 1-buckets of vectors each hosting a list are designed to access items
    // sequentially ---i.e., random access is not allowed. It provides a private
    // iterator that points to the next item, separating the whole sequence in
    // two segments: a shaded one which is ignored unless the pointer moves to
    // the beginning of it; and an active segment which is assumed to be the
    // segment to use by default. The allowed operations are:

    // inserting a new item at the front of a specific bucket. If all items in
    // this bucket are either shaded or active, the insertion is performed
    // directly. Otherwise, the item is inserted at the location pointed to by
    // the private iterator ---i.e., first in the active segment.

    // removing the current item. It removes the item pointed to by the private
    // iterator if any.

    // front returns the current item, i.e., the item pointed to by the private
    // iterator. It does not remove it from the sequence

    // next moves the private pointer forward, thus enlarging the shaded segment
    // by one.

    // rewind takes the private pointer to the beginning of the sequence. Hence,
    // the shaded segment dissapears and the whole sequence becomes active.
    
  public:

    // Constructor
    bucketvl_t ():
      _queue  { vector<list<T>> (NUMBER_BUCKETS, list<T>() ) },
      _pf     { numeric_limits<unsigned int>::max ()},   // non-existing bucket
      _size   { 0 },                                      // no items initially
      _minf   { 1 }, // invariant: _minf and _maxf have to be above the current
      _maxf   { 1 }                    // number of items if the queue is empty
    { }

    // get accessors
    unsigned long long int get_size () const
    { return _size; }
    unsigned int get_size (const unsigned int f) const
    { return _queue [f].size (); }
    unsigned int get_pf () const
    { return _pf; } 
    unsigned int get_minf () const
    { return _minf; }
    unsigned int get_maxf () const
    { return _maxf; }

    // methods

    // return true if the public region is empty and false otherwise
    bool end ()
    { return _pointer == _queue[_maxf].end (); }
    
    // front returns the first item from the sequence, if any, or nullptr in
    // case the sequence is exhausted. It does not remove it.
    T* front ();

    // insert adds the given item to the front of the bucket with the specified
    // f-value. If the front of the f-bucket is shaded, then the new item is
    // added to the first location after the pointer
    void insert (const T& item, unsigned int f);

    // erase removes the item pointed to by the private iterator. It returns the
    // item following it in the sequence, or end if the last item was removed
    void remove ();

    // next makes the private pointer to point to the next item in the sequence
    // or end, if it was already pointing to the last one. It returns a pointer
    // to the next item or nullptr if it the sequence is exhausted
    T* next ();

    // rewind takes the private iterator back to the first location of the
    // shaded region. It returns nothing
    void rewind ();

    // printing services
    friend ostream& operator<< (ostream& stream, bucketvl_t bucket)
    {

      register unsigned int idx=bucket.get_minf ();              // idx-th bucket
      
      stream << " \t# items: " << bucket.get_size () << endl;
      stream << " \trange  : [" << bucket.get_minf () << ", " << bucket.get_maxf () << "]" << endl;

      while (idx <= bucket.get_maxf ()) {

	stream << " \t\t# items [" << idx << "]: " << bucket.get_size (idx) << endl;
	for (idx+=1; idx<=bucket.get_maxf () && !bucket.get_size (idx); idx++);
      }
      
      return stream;
    }

  private:

    // invariants: a 1-bucket vector-list consists of a vector of lists with a
    // non-negative size. All operations preserve the range [minf,maxf] of
    // f-values containing items unless size=0 in which case minf=maxf=1.

    // It does also provide a private pointer that splits the sequence in two
    // regions: shaded and active. The iterator necessarily points to an item
    // when size!=0 which might be the end of the last bucket in case the active
    // region is exhausted. The attribute _pf is used to know the bucket the
    // pointer is pointing to and, it points to an unexisting bucket in case the
    // sequence is empty. 

    vector<list<T>> _queue;                         // f-indexes to the buckets
    typename list<T>::iterator _pointer;     // beginning of the active segment
    unsigned int _pf;                // f-value (bucket) of the private pointer
    unsigned long long int _size;            // number of elements in the queue
    unsigned int _minf;                                 // minimum f-value used
    unsigned int _maxf;                                 // maximum f-value used

  }; // class bucketvl_t<T>

  // front returns the first item from the sequence, if any, or nullptr in
  // case the sequence is exhausted. It does not remove it.
  template<class T> T* bucketvl_t<T>::front ()
  {

    // in case the list is exhausted or, in case we are already at the end of
    // the sequence,
    if (_size==0 || _pointer==_queue[_maxf].end ())
      return nullptr;                                  // return a null pointer

    // The pointer is known to point to the beginning of a non-empty active
    // region. Thus, return a pointer to the element it points to.
    return &(*_pointer);
  }
  
  // insert adds the given item to the front of the bucket with the specified
  // f-value. If the front of the f-bucket is shaded, then the new item is added
  // to the first location after the pointer
  template<class T> void bucketvl_t<T>::insert (const T& item, unsigned int f)
  {

    // -- checks
    if (f >= _queue.max_size ())    // in case the corresponding bucket exceeds
      throw domain_error ("khs::bucketvl_t::insert_front (f >= _queue.max_size ())");
      
    if (f >= _queue.size ()) {        // extend the current bucket if necessary

      // take note of the last item
      unsigned int prev = _queue.size (); 
      _queue.resize (max ((unsigned long long int) 1+f, 
			  (unsigned long long int) 2*_queue.size ()));

      if (f >= _queue.size ())             // make sure there is room available
	throw domain_error ("khs::bucketvl_t::insert_front (f >= _queue.size ())");

      // and initialize all the new positions
      for (register unsigned int index = prev; index < _queue.size (); index++) {
	_queue [index] = list<T>();
      }
    }

    // -- insertion --- if this bucket has a shaded segment
    if (f == _pf)

      // then insert the new item before the current location of the pointer and
      // make the new item to be first in the active region
      _pointer = _queue[_pf].insert (_pointer, item);

    else

      // otherwise, just insert the new item by the front of the specified
      // bucket without modifying the location of the private pointer
      _queue [f].push_front (item);

    // -- internal data
    if (!_size) {       // in case the sequence was empty before this insertion
      _pointer = _queue[f].begin ();
      _pf = f;                  // the active region starts now at the f-bucket
    }
    
    _size++;           // increment the size of the sequence effectively by one
    _minf = (_size == 1) ? f : min (f, _minf);
    _maxf = (_size == 1) ? f : max (f, _maxf);
  }

  // erase removes the item pointed to by the private iterator. It returns the
  // item following it in the sequence, or end if the last item was removed
  template<class T> void bucketvl_t<T>::remove ()
  {

    // -- removal --- remove the location pointed to by _pointer
    _pointer = _queue[_pf].erase (_pointer);

    // -- internal data
    _size--;           // effectively decrement the size of the sequence by one

    // update the current range
    if (!_size) {                   // if there are no more items in the bucket
      _pf = numeric_limits<unsigned int>::max ();          // unexisting bucket
      _minf = _maxf = 1;                      // then update the current bounds
    }
    else {

      unsigned int pf=_pf;                   // backup the current value of _pf
      
      // if the end of this bucket has been reached and this is not the last
      // bucket
      if (_pointer == _queue[_pf].end () && _pf < _maxf) {

	// then move forward to the following bucket and point to the beginning
	for (;_pf<=_maxf && !_queue[_pf].size ();_pf++);
	_pointer = _queue[_pf].begin ();
      }

      // if the bucket was left empty
      if (!_queue [pf].size ()) {

	// now, there are two *different* cases, either we left empty the bottom
	// f-bucket or the top f-bucket (but not both simultaneously as this
	// would imply that the sequence is empty and this case has been
	// considered above)
	if (pf == _minf)                 // if we exhausted the bottom f-bucket
	  _minf = _pf;              // then reuse the computation above for _pf
	else if (pf == _maxf) {         // if we exhausted the topmost f-bucket
	  for (;_maxf>=_minf && !_queue[_maxf].size ();_maxf--);   // move back
	  _pf = _maxf;                    // and make the private pointer point
	  _pointer = _queue[_pf].end ();      // to the new end of the sequence
	}
      }
    }
  }

  // next makes the private pointer to point to the next item in the sequence or
  // end, if it was already pointing to the last one. It returns a pointer to
  // the next item or nullptr if it the sequence is exhausted
  template<class T> T* bucketvl_t<T>::next ()
  {

    // in case the sequence is empty of we are currently located at the last
    // position of the last bucket
    if (_size==0 || _pointer==_queue[_maxf].end ())
      return nullptr;                                  // return a null pointer
    
    // else, just move the beginning of the active region one position forward
    _pointer++;

    // if the end of the current f-bucket (different than the last one) was
    // found, then move forward to the next bucket
    if (_pf < _maxf && _pointer == _queue [_pf].end ()) {
      for (++_pf;_pf<=_maxf && !_queue[_pf].size ();_pf++);      // non-empty bucket
      _pointer = _queue[_pf].begin ();   // and _pointer to the beginning of it
    }

    // now, return a pointer to the next item unless we are at the end of the
    // sequence
    if (_pointer == _queue[_maxf].end ())
      return nullptr;
    return (T*) &(*_pointer);
  }
  
  // rewind takes the private iterator back to the first location of the shaded
  // region. It returns nothing
  template<class T> void bucketvl_t<T>::rewind ()
  {

    // just make the private pointer move to the beginning of the first bucket
    // in the sequence unless the sequence is empty
    if (_size) {
      _pf = _minf;
      _pointer = _queue[_pf].begin ();
    }
  }
} // namespace khs
  

#endif 	    /* !KHSBUCKETVL_T_H_ */



/* Local Variables: */
/* mode:c++ */
/* fill-column:80 */
/* End: */
