/* 
  KHSbucketvd_seq_t.h
  Description: Definition of 1-buckets represented with vectors, each
  hosting a deque that provide a sequential iterator
  ----------------------------------------------------------------------------- 

  Started on  <Fri May 13 13:41:30 2016 Carlos Linares Lopez>
  Last update <domingo, 15 mayo 2016 13:55:59 Carlos Linares Lopez (clinares)>
  -----------------------------------------------------------------------------

  $Id::                                                                      $
  $Date::                                                                    $
  $Revision::                                                                $
  -----------------------------------------------------------------------------

  Made by Carlos Linares Lopez
  Login   <clinares@atlas>
*/

#ifndef   	KHSBUCKETVD_SEQ_T_H_
# define   	KHSBUCKETVD_SEQ_T_H_

#include <iostream>

#include "../KHSdefs.h"
#include "KHSbucketvd_t.h"

namespace khs {

  // Bucket of vectors hosting deques - Forward declaration
  template<class T>
  class bucketvd_t;
  
  // Iterator - Forward declaration
  template<class T>
  class bucketvditerator_t;

  // Class definition
  template<class T>
  class bucketvd_seq_t {

  public:

    // Constructor
    bucketvd_seq_t ()
      : _bucket {bucketvd_t<T> ()},
	_iterator {bucketvditerator_t<T> (_bucket, 0)}
    { }

    // get accessors
    unsigned long long int get_size () const
    { return _bucket.get_size (); }
    unsigned int get_size (const unsigned int f) const
    { return _bucket.get_size (f); }
    unsigned int get_minf () const
    { return _bucket.get_minf (); }
    unsigned int get_maxf () const
    { return _bucket.get_maxf (); }
    bucketvd_t<T> get_bucket () const
    { return _bucket; }
    typename bucketvd_t<T>::iterator get_iterator () const
    { return _iterator; }
    
    // methods

    // insert_front adds the given item to the front of the specified
    // f-bucket. It returns true if the operation was successful and false
    // otherwise. It also takes care of preserving the consistency of other
    // internal data
    bool insert_front (const T& item, unsigned int f)
    { return _bucket.insert (item, f); }
    
    // insert_back adds the given item to the back of the specified f-bucket. It
    // returns true if the operation was successful and false otherwise. It also
    // takes care of preserving the consistency of other internal data
    bool insert_back (const T& item, unsigned int f)
    { return _bucket.insert_back (item, f); }

    // rewind moves the iterator back to the beginning of the sequence
    void rewind ()
    { _iterator = _bucket.begin (); }
    
    // front returns the contents of the position currently pointed to by the
    // iterator
    T& front ()
    { return *_iterator; }

    // next moves the iterator one position forward. It returns true if either
    // the iterator is currently located at the end of the sequence or, if after
    // moving forward, the end of the sequence is reached
    bool next ()
    {
      if (_iterator == _bucket.end ())            // if we are stuck at the end
	return true;     // then it is not possible to move forward any further
      ++_iterator;                                   // otherwise, move forward
      return (_iterator == _bucket.end ());         // have we reached the end?
    }

    // remove the location pointed to by the iterator ---which is not
    // necessarily the first one in the sequence. The iterator is still valid
    // after deletion and it points to the next valid location within the
    // bucket. It returns true if the sequence is empty and false otherwise.
    bool pop ()
    {
      _iterator.remove ();    // remove the position pointed to by the iterator
      return (_iterator == _bucket.end ());  // return whether the seq is empty
    }
    
    // printing services
    friend ostream& operator<< (ostream& stream, bucketvd_seq_t bucket)
    {
      return stream << bucket.get_bucket () << endl;
    }
    
  private:

    // A sequential 1-bucket of vectors each hosting a deque consists of a
    // 1-bucket of vectors each hosting a deque and an iterator within the same
    // bucket
    bucketvd_t<T> _bucket;                // 1-bucket of vectors hosting deques
    typename bucketvd_t<T>::iterator _iterator; // iterator to traverse _bucket
    
  }; // class bucketvd_seq_t<T>

} // namespace hks

#endif 	    /* !KHSBUCKETVD_SEQ_T_H_ */




/* Local Variables: */
/* mode:c++ */
/* fill-column:80 */
/* End: */
