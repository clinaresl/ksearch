/* 
  KHSset_t.h
  Description: sets of items
  ----------------------------------------------------------------------------- 

  Started on  <Sun Apr  6 16:53:27 2014 Carlos Linares Lopez>
  Last update <martes, 10 mayo 2016 12:40:24 Carlos Linares Lopez (clinares)>
  -----------------------------------------------------------------------------

  $Id::                                                                      $
  $Date::                                                                    $
  $Revision::                                                                $
  -----------------------------------------------------------------------------

  Made by Carlos Linares Lopez
  Login   <clinares@atlas>
*/

#ifndef   	KHSSET_T_H_
# define   	KHSSET_T_H_

#include <algorithm> 
#include <iterator>
#include <set>
#include <utility>

namespace khs {

  using namespace std;

  template<class T>
  class set_t {

  public:

    // Constructor
    set_t ():
      _set { set<T>() }
    { }

    // get accessors
    unsigned long long int get_size () const
    { return _set.size (); }

    // methods

    // insert adds the given item to the set. It returns true if a new item was
    // effectively inserted and false otherwise
    bool insert (T& item)
    { return _set.insert (item).second; }

    // find returns true if the specified item exists in the set and false
    // otherwise
    bool find (const T& item) const
    { return (_set.find (item) != _set.end ()); }

    // return a pointer to the location of the specified item if it exists and
    // nullptr otherwise. This pointer can be used to access directly the item
    T* search (const T& item)
    {
      // first, check whether this item currently exists or not in the set
      typename set<T>::iterator it = _set.find (item);
      
      // in case it exists, then return a reference to it
      if (it != _set.end ())
	return (T*) &(*it);
      
      // otherwise, return a null pointer
      return nullptr;
    }

    // clear the contents of this set and restores the factory settings
    void clear ()
    { _set.clear (); }

    // printing services
    friend ostream& operator<< (ostream& stream, set_t<T> set)
    {
      stream << " Set: [" << endl;
      stream << "   ";
      for (auto item : set)
	stream << item << endl << endl;
      stream << " ]";
      return stream;
    }

    // iterators - implemented by delegation over std::set<T>
    typedef typename set<T>::iterator iterator;
    typedef typename set<T>::const_iterator const_iterator;

    iterator begin() 
    { return _set.begin(); }
    iterator end() 
    { return _set.end(); }

  private:

    // Invariants: a set_t consists of a set of items of type T which might be
    // empty. No item can be repeated by definition
    set<T> _set;                                      // set of items of type T

  }; // set_t
} // namespace khs

#endif 	    /* !KHSSET_T_H_ */




/* Local Variables: */
/* mode:c++ */
/* fill-column:80 */
/* End: */
