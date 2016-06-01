/* 
  KHSbucketvd_t.h
  Description: Definition of 1-buckets represented with vectors, each hosting a
               deque
  ----------------------------------------------------------------------------- 

  Started on  <Thu Feb  6 16:51:44 2014 Carlos Linares Lopez>
  Last update <lunes, 23 mayo 2016 13:12:26 Carlos Linares Lopez (clinares)>
  -----------------------------------------------------------------------------

  Made by Carlos Linares Lopez
  Login   <clinares@atlas>
*/

#ifndef   	KHSBUCKETVD_T_H_
# define   	KHSBUCKETVD_T_H_

#include <algorithm>
#include <deque>
#include <exception>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <vector>

#include "../KHSdefs.h"

namespace khs {

  using namespace std;

  // Iterator - Forward declaration
  template<class T>
  class bucketvditerator_t;

  // Class definition
  template<class T>
  class bucketvd_t {

  protected:

    // return the bucket and index within the bucket of a given position so that
    // bucketvd_t [pos] = _queue [id][item]. In case the position given goes
    // beyond the size of the bucket, the iterator is located at end, ie., to
    // the first position of a bucket beyond maxf
    void _get_idbucket (unsigned long long int pos,
			unsigned int& id, unsigned long long int& item) const;

  public:

    // Constructor
    bucketvd_t ():
      _queue  { vector<deque<T>> (NUMBER_BUCKETS, deque<T>() ) },
      _size   { 0 },
      _minf   { 1 },  // invariant: _minf and _maxf have to be above the current 
      _maxf   { 1 }   // number of items if the queue is empty
    { }

    // get accessors
    unsigned long long int get_size () const
    { return _size; }
    unsigned int get_size (const unsigned int f) const
    { return _queue [f].size (); }
    unsigned int get_minf () const
    { return _minf; }
    unsigned int get_maxf () const
    { return _maxf; }

    const deque<T>& get_bucket (const unsigned int idx) const
    { return _queue[idx]; }
    
    // methods

    // insert_front adds the given item to the front of the bucket with the
    // specified f-value. It returns true if the operation was successful and
    // false otherwise. It also takes care of preserving the consistency of
    // other internal data
    bool insert_front (const T& item, unsigned int f);

    // insert_back adds the given item to the back of the bucket with the
    // specified f-value. It returns true if the operation was successful and
    // false otherwise. It also takes care of preserving the consistency of
    // other internal data
    bool insert_back (const T& item, unsigned int f);

    // remove returns and erases the first item with the specified f-value. If
    // the bucket is empty an exception is thrown. It also takes care of
    // preserving the consistency of other internal data
    T remove (const unsigned int f);

    // remove returns and erases the item in the specified position in the f-th
    // bucket.  If the combination (f, pos) is not valid, an exception might be
    // thrown. It also takes care of preserving the consistency of internal data
    T remove (const unsigned int f, const unsigned long long int pos);

    // pop_front extracts the first item
    T pop_front ()
    { return remove (_minf); }

    // clear removes all elements restoring the factory settings
    void clear ();

    // operator overloading
    const T& operator[] (const unsigned long long int index) const;

    // printing services
    friend ostream& operator<< (ostream& stream, bucketvd_t bucket)
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

    // Iterators

    // Typedef alias
    typedef bucketvditerator_t<T> iterator;

    // begin points to the very first item of the sequence, in spite of its
    // physical location, it is identified with offset=0
    iterator begin ()
    { return bucketvditerator_t<T> (*this, 0); }

    // end points to the element next to the last one, in spite of its physical
    // location so that if items are indexed in the range [0,n) then the offset
    // of end equals n. Iterators represent the end as the first position (0) in
    // the bucket next to maxf
    iterator end ()
    { return bucketvditerator_t<T> (*this, _size); }
    
  private:

    // Iterators: the following attributes enable iterators on buckets
    friend class bucketvditerator_t<T>;

    // invariants: a 1-bucket vector-deque consists of a vector of deques with a
    // non-negative size. All operations preserve the range [minf,maxf] of
    // f-values containing items unless size=0 in which case minf=maxf=1

    vector<deque<T>> _queue;                        // f-indexes to the buckets
    unsigned long long int _size;            // number of elements in the queue
    unsigned int _minf;                                 // minimum f-value used
    unsigned int _maxf;                                 // maximum f-value used

  }; // class bucketvd_t<T>

  // return the bucket and index within the bucket of a given position so that
  // bucketvd_t [pos] = _queue [id][item]. In case the position given goes
  // beyond the size of the bucket, the iterator is located at end, ie., to the
  // first position of a bucket beyond maxf
  template<class T> void bucketvd_t<T>::_get_idbucket (unsigned long long int pos,
						       unsigned int& id, 
						       unsigned long long int& item) const
  {
    // start searching from the minimum bucket
    id = _minf;

    // and now, until the whole sequence has been examined or the right bucket
    // has been found ---note that empty buckets are always skipped
    while (id <= _maxf && pos >= _queue[id].size ())

      // prevent underflows due to the usage of unsigned arithmetic
      if (!pos && _queue[id].size ())
	break;
      else
	pos -=_queue [id++].size ();

    // and the location within the bucket is stored in pos
    item = pos;
  }

  // insert_front adds the given item to the front of the bucket with the
  // specified f-value. It returns true if the operation was successful and
  // false otherwise. It also takes care of preserving the consistency of other
  // internal data
  template<class T> bool bucketvd_t<T>::insert_front (const T& item, unsigned int f)
  {

    // checks
    if (f >= _queue.max_size ())    // in case the corresponding bucket exceeds
      throw domain_error ("khs::bucketvd_t::insert_front (f >= _queue.max_size ())");
      
    if (f >= _queue.size ()) {        // extend the current bucket if necessary

      // take note of the last item
      unsigned int prev = _queue.size (); 
      _queue.resize (max ((unsigned long long int) 1+f, 
			  (unsigned long long int) 2*_queue.size ()));

      if (f >= _queue.size ())             // make sure there is room available
	throw domain_error ("khs::bucketvd_t::insert_front (f >= _queue.size ())");

      // and initialize all the new positions
      for (register unsigned int index = prev; index < _queue.size (); index++) {
	_queue [index] = deque<T>();
      }
    }

    // insertion --- 1-buckets vector-deques are used to insert by the front
    _queue [f].push_front (item);

    // internal data
    _size++;
    _minf = (_size == 1) ? f : min (f, _minf);
    _maxf = (_size == 1) ? f : max (f, _maxf);

    // exit with success
    return true;
  }

  // insert_back adds the given item to the back of the bucket with the
  // specified f-value. It returns true if the operation was successful and
  // false otherwise. It also takes care of preserving the consistency of other
  // internal data
  template<class T> bool bucketvd_t<T>::insert_back (const T& item, unsigned int f)
  {

    // checks
    if (f >= _queue.max_size ())    // in case the corresponding bucket exceeds
      throw domain_error ("khs::bucketvd_t::insert_back (f >= _queue.max_size ())");
      
    if (f >= _queue.size ()) {        // extend the current bucket if necessary

      // take note of the last item
      unsigned int prev = _queue.size (); 
      _queue.resize (max ((unsigned long long int) 1+f, 
			  (unsigned long long int) 2*_queue.size ()));

      if (f >= _queue.size ())             // make sure there is room available
	throw domain_error ("khs::bucketvd_t::insert_back (f >= _queue.size ())");

      // and initialize all the new positions
      for (register unsigned int index = prev; index < _queue.size (); index++) {
	_queue [index] = deque<T>();
      }
    }

    // insertion to the back
    _queue [f].push_back (item);

    // internal data
    _size++;
    _minf = (_size == 1) ? f : min (f, _minf);
    _maxf = (_size == 1) ? f : max (f, _maxf);

    // exit with success
    return true;
  }

  // remove returns and erases the first item with the specified f-value. If the
  // bucket is empty an exception is thrown. It also takes care of preserving
  // the consistency of other internal data
  template<class T> T bucketvd_t<T>::remove (unsigned int f)
  {

    if (!_queue [f].size ())
      throw domain_error ("khs::bucketvd_t::remove (unsigned int)");

    // extract the first item from the f-bucket
    T item = _queue [f].front ();
    _queue [f].pop_front ();

    // internal data
    _size--;                           // decrement the overall number of items

    // update the current range
    if (!_size)                     // if there are no more items in the bucket
      _minf = _maxf = 1;                      // then update the current bounds
    else if (!_queue [f].size ()) {             // if this bucket becomes empty

      // locate the first non-empty bucket and update the corresponding bound if
      // any has to be modified
      if (f == _minf)      // if we removed the last item from the first bucket
	for (_minf=f;_minf<=_maxf && !_queue[_minf].size ();_minf++);//move fwd
      if (f == _maxf)       // if we removed the last item from the last bucket
	for (_maxf=f;_maxf>=_minf && !_queue[_maxf].size ();_maxf--);//move bwd
    }
    
    // and exit
    return item;
  }

  // remove the item in the specified position in the f-th bucket.  If the
  // combination (f, pos) is not valid, an exception might be thrown. It also
  // takes care of preserving the consistency of internal data
  template<class T> T bucketvd_t<T>::remove (const unsigned int f,
					     const unsigned long long int pos)
  {
    if (!_queue [f].size ())
      throw domain_error ("khs::bucketvd_t::remove (iterator) - no such f");
    if (pos>=_queue[f].size ())
      throw domain_error ("khs::bucketvd_t::remove (iterator) - pos out of bounds");

    // effectively remove this item
    T item = _queue[f][pos];
    _queue[f].erase (_queue[f].begin () + pos);

    // internal data
    _size--;                           // decrement the overall number of items

    // update the current range
    if (!_size)                     // if there are no more items in the bucket
      _minf = _maxf = 1;                      // then update the current bounds
    else if (!_queue [f].size ()) {             // if this bucket becomes empty

      // locate the first non-empty bucket and update the corresponding bound if
      // any has to be modified
      if (f == _minf)      // if we removed the last item from the first bucket
	for (_minf=f;_minf<=_maxf && !_queue[_minf].size ();_minf++);//move fwd
      if (f == _maxf)       // if we removed the last item from the last bucket
	for (_maxf=f;_maxf>=_minf && !_queue[_maxf].size ();_maxf--);//move bwd
    }

    // and exit
    return item;
  }
  
  // clear removes all elements restoring the factory settings
  template<class T> void bucketvd_t<T>::clear ()
  {

    // traverse all items in the queue and remove the contents of all lists
    for (typename vector<deque<T>>::iterator ibucket = _queue.begin ();
	 ibucket != _queue.end ();
	 ++ibucket )
      
      // remove all the elements in this bucket
      ibucket->clear ();

    // now, remove the queue itself and initialize with empty contents
    _queue.clear ();
    _queue = vector<deque<T>> (NUMBER_BUCKETS, deque<T>() );

    // and initialize all attributes to factory settings
    _size = 0;
    _minf = _maxf = 1;
  }

  // operator overloading
  template<class T> const T& bucketvd_t<T>::operator[] (const unsigned long long int index) const
  {
    // in case an item is requested beyond the number of items currently stored
    // raise an exception
    if (index >= _size)
      throw domain_error ("khs::bucketvd_t::operator[]");

    // compute the bucket and index within the bucket that contains the index-th
    // item
    unsigned int id;
    unsigned long long int item;
    _get_idbucket (index, id, item);

    // and return it
    return _queue [id][item];
  }

  // ----------------------------------------------------------------------- //
  
  // Iterator definition
  template<class T>
  class bucketvditerator_t : public iterator<forward_iterator_tag, 
					 T,
					 ptrdiff_t,
					 T*,
					 T&> {

    // Typedef alias
    typedef bucketvditerator_t<T> iterator;

  public:
    
    // Explicit constructors
    bucketvditerator_t (bucketvd_t<T>& bucket, unsigned long long int offset)
      : _bucket { &bucket },
        _offset { offset }
    {
      if (offset > bucket._size) {             // if offset goes beyond the end
	_id = bucket._maxf + 1;       // then make the iterator equal to end ()
	_pos = 0;
      }

      // otherwise
      else
	bucket._get_idbucket (offset, _id, _pos);   // compute the right values
    }

    // get accessors
    bucketvd_t<T>* get_bucket () const
    { return _bucket; }
    unsigned long long int get_offset () const
    { return _offset; }
    unsigned int get_id () const
    { return _id; }
    unsigned long long int get_pos () const
    { return _pos; }
    
    // operator overloading

    // assignment operator
    bucketvditerator_t& operator=(const bucketvditerator_t& right)
    {
      // copy the attributes of the right operand
      _bucket = right.get_bucket ();
      _offset = right.get_offset ();
      _id     = right.get_id ();
      _pos    = right.get_pos ();

      // and return a reference to this instance
      return *this;
    }
    
    // note that in the first two operators, _offset is used instead of _id and
    // _pos. The reason is that _offset should be always consistent with these
    // attributes (it is an invariant)
    bool operator==(const bucketvditerator_t& bucket) const
    {
      return (_bucket == bucket.get_bucket () &&     // either the container is
	      _offset == bucket.get_offset());//different or the offset differs
    }
    bool operator!=(const bucketvditerator_t& bucket) const
    {
      return (_bucket != bucket.get_bucket () ||
	      _offset != bucket.get_offset ());
    }

    // the following operator uses the incremental counters to speed up access
    T& operator* ()
    { return _bucket->_queue [_id][_pos]; }

    T* operator->()
    { return (T*)&_bucket->_queue [_id][_pos]; }      

    // only prefix auto-increment is defined since it is more efficient (it
    // saves a copy)
    iterator& operator++()                             // prefix auto-increment
    {

      // in case we are currently at the last location of the bucket
      if (_pos==_bucket->get_size ()-1)
	*this = _bucket->end ();

      else {                                                       // otherwise
      // compute incrementally the new bucket and the position within the
      // bucket: first, tentatively increment the position within the current
      // bucket
      _pos++;

      // if we are at the end of the current bucket
      if (_bucket->_queue [_id].size () <= _pos) {

	// then look for the next non-empty bucket and initialize the position
	// within it to 0 or, in case the iterator was originally pointing to
	// the last item, then make it point to end ()
	for (_id++; _id <= _bucket->_maxf && !_bucket->_queue [_id].size (); _id++);
	_pos = 0;
      }

      // and update also the global position over the whole bucket
      _offset++;
      }

      // and finally return a reference to this iterator
      return *this;
    }

    // insert_front adds a new item to the front of the specified bucket. It
    // returns true if the operation was successful and false otherwise. An
    // exception might be raised if the bucket is inconsistent. The iterator is
    // still valid after the insertion. In case the bucket is empty, then the
    // iterator will point to the beginning of the sequence; otherwise, it will
    // still point to the same item than before the insertion
    bool insert_front (const T& item, unsigned int id)
    {
      // now, there are two cases, either the bucket is empty before the
      // insertion or not
      if (!_bucket->_size) {

	// if the bucket was empty, then either begin () or end () would point
	// to the end of the sequence
	if (_id == 1 + _bucket->_maxf) {        // iterator = begin () / end ()

	  // make the iterator effectively point to the beginning of the bucket
	  _id = id;
	  _pos = _offset = 0;
	  return _bucket->insert_front (item, id);
	}

	// in any other case, throw an exception, since it means that the
	// iterator was invalid before the insertion
	throw domain_error ("khs::bucketvditerator_t::insert_front (empty bucket)");
      }

      // in case the bucket already contain items
      else {

	// now, if the item is about to be inserted before the current location
	// of the iterator, update the overall position of this iterator
	if (id <= _id)
	  _offset++;

	// and in case it is about to be inserted in the same bucket of this
	// iterator then update the location of this iterator within the same
	// bucket
	if (id == _id)
	  _pos++;
      }

      // effectively insert the item in the bucket and exit
      return _bucket->insert_front (item, id);
    }

    // insert_back adds a new item to the back of the specified bucket. It
    // returns true if the operation was successful and false otherwise. An
    // exception might be raised if the bucket is inconsistent.  The iterator is
    // still valid after the insertion. In case the bucket is empty, then the
    // iterator will point to the beginning of the sequence; otherwise, it will
    // still point to the same item than before the insertion
    bool insert_back (const T& item, unsigned int id)
    {
      // now, there are two cases, either the bucket is empty before the
      // insertion or not
      if (!_bucket->_size) {

	// if the bucket was empty, then either begin () or end () would point
	// to the end of the sequence
	if (_id == 1 + _bucket->_maxf) {        // iterator = begin () / end ()

	  // make the iterator effectively point to the beginning of the bucket
	  _id = id;
	  _pos = _offset = 0;
	  return _bucket->insert_back (item, id);
	}

	// in any other case, throw an exception, since it means that the
	// iterator was invalid before the insertion
	throw domain_error ("khs::bucketvditerator_t::insert_back (empty bucket)");
      }

      // in case the bucket already contain items
      else

	// now, if the item is about to be inserted before the current location
	// of the iterator, update the overall position of this iterator
	if (id < _id)
	  _offset++;

      // effectively insert the item in the bucket and exit
      return _bucket->insert_back (item, id);
    }
    
    // remove the item pointed to by this iterator. If the iterator is not valid
    // an exception might be thrown. The iterator is still valid after the
    // deletion and it points to the next item, which might be the end of the
    // sequence
    void remove ()
    {

      // first, remove this position from the bucket pointed to by this iterator
      _bucket->remove (_id, _pos);

      // now, in case this was the last element in the bucket
      if (!_bucket->get_size ()) {

	// make all attributes to take their default values
	_id = _pos = _offset = 0;
      }

      // otherwise
      else {

	// when erasing an item, the iterator shall point to the next item in
	// the bucket unless the last item of the last bucket was removed, in
	// which case, the iterator shall point to end.
	if (_pos >= _bucket->get_size (_id)) {     // if at the last pos of _id
	  
	  // but if we removed the last item of the last bucket
	  if (_id >= _bucket->get_maxf ())
	    *this = _bucket->end ();            // then make it equal to end ()

	  // if we removed the last item of a bucket different than the last one
	  else {

	    // then we should look for the next non-empty bucket and make the
	    // iterator point to the beginning of it
	    for (_id++;
		 _id <= _bucket->get_maxf () && !_bucket->get_size (_id);
		 _id++);
	    _pos=0;
	  }
	}
      }
    }

  private:

    // Invariants: every iterator over an instance of bucketvd_t contains a
    // reference to an bucketvd_t and also stores additional information to know
    // what item is been referenced: counters. There are two types of counters:
    // over the whole bucket and incremental ones which store the bucket and the
    // relative position within the bucket. They should be always consistent.
    bucketvd_t<T>* _bucket;                                  // bucket iterated

    // counters
    unsigned long long int _offset;           // 0-based index to the next item 
    unsigned int _id;           // 0-based index to the bucket of the next item
    unsigned long long int _pos;         // 0-based index within the bucket _id
  }; // bucketvditerator_t

} // namespace khs


#endif 	    /* !KHSBUCKETVD_T_H_ */




/* Local Variables: */
/* mode:c++ */
/* fill-column:80 */
/* End: */
