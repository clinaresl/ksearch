/* 
  KHSmnode_t.h
  Description: information of a tree node with information on the
  number of expansions
  ----------------------------------------------------------------------------- 

  Started on  <Tue May 10 13:01:55 2016 Carlos Linares Lopez>
  Last update <martes, 10 mayo 2016 23:18:55 Carlos Linares Lopez (clinares)>
  -----------------------------------------------------------------------------

  Made by Carlos Linares Lopez
  Login   <clinares@atlas>
*/

#ifndef   	KHSMNODE_T_H_
# define   	KHSMNODE_T_H_

#include <vector>

#include "KHSnode_t.h"

namespace khs {

  using namespace std;

  template<class T>
  class mnode_t : public node_t<T> {

  public:

    // Explicit constructor
    mnode_t (const T& state, unsigned int h=0, unsigned int g=0, unsigned m=0)
      : node_t<T>(state, h, g),
	_m {m}
    {  }

    mnode_t (const T& state, unsigned int h, unsigned int g, unsigned int m, const vector<T>& path)
      : node_t<T> (state, h, g, path),
	_m {m}
    {  }

    // Copy constructor
    mnode_t (const mnode_t& node) = default;

    // get accessors
    unsigned int get_m () const
    { return _m; }

    // set accessors
    void set_m (unsigned int m)
    { _m = m; }

    // methods
    void increment_m (const unsigned int value = 1)
    { _m += value; }

    // printing services
    friend ostream& operator<< (ostream& stream, mnode_t node)
    {
      stream << " <" << node.get_state ();
      stream << ", " << node.get_g ();
      stream << " + " << node.get_h ();
      stream << " = " << node.get_f ();
      stream << ", m =" << node.get_m ();
      stream << ">: [";
      
      vector<T> path = node.get_path ();
      copy (path.begin (), path.end (), ostream_iterator<T> (cout, " "));
      cout << "]";
      
      return stream;
    }
    
  protected:

    // Invariant: every node keeps information on the number of times it has
    // been expanded. This is zero upon creation and this value is then updated
    // every time a node is expanded. No node shall be expanded more than a
    // predefined number of times (the upper bound dictated by m-A*)

    // Attributes
    unsigned int _m;			// number of expansions of this node

  }; // class mnode_t<T>
} // namespace khs

#endif 	    /* !KHSMNODE_T_H_ */




/* Local Variables: */
/* mode:c++ */
/* fill-column:80 */
/* End: */
