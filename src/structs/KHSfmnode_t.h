/* 
  KHSfmnode_t.h
  Description: information of a tree node with information on the number of
  expansions and whether it is part of an optimal solution path
  ----------------------------------------------------------------------------- 

  Started on  <Tue May 10 13:01:55 2016 Carlos Linares Lopez>
  Last update <martes, 07 junio 2016 15:44:17 Carlos Linares Lopez (clinares)>
  -----------------------------------------------------------------------------

  Made by Carlos Linares Lopez
  Login   <clinares@atlas>
*/

#ifndef   	KHSFMNODE_T_H_
# define   	KHSFMNODE_T_H_

#include <vector>

#include "KHSmnode_t.h"

namespace khs {

  using namespace std;

  template<class T>
  class fmnode_t : public mnode_t<T> {

  public:

    // Explicit constructor
    fmnode_t (const T& state, unsigned int h=0, unsigned int g=0, unsigned m=0,
	      bool flag=false)
      : mnode_t<T>(state, h, g, m),
	_flag {flag}
    {  }

    fmnode_t (const T& state, unsigned int h, unsigned int g, unsigned int m,
	      bool flag, const vector<T>& path)
      : mnode_t<T> (state, h, g, m, path),
	_flag {flag}
    {  }

    // Copy constructor
    fmnode_t (const fmnode_t& node) = default;

    // get accessors
    bool get_flag () const
    { return _flag; }

    // set accessors
    void set_flag (bool flag)
    { _flag = flag; }

    // printing services
    friend ostream& operator<< (ostream& stream, fmnode_t node)
    {
      stream << " <" << node.get_state ();
      stream << ", " << node.get_g ();
      stream << " + " << node.get_h ();
      stream << " = " << node.get_f ();
      stream << ", m =" << node.get_m ();
      stream << ", flag=" << node.get_flag ();
      stream << ">: [";
      
      vector<T> path = node.get_path ();
      copy (path.begin (), path.end (), ostream_iterator<T> (cout, " "));
      cout << "]";
      
      return stream;
    }
    
  protected:

    // Invariant: every node stores information in a flag which can be either
    // true or false. This is used to indicate whether this node has been
    // discovered to be part of an optimal solution path.

    // Attributes
    bool _flag;                                                         // flag

  }; // class fmnode_t<T>
} // namespace khs

#endif 	    /* !KHSFMNODE_T_H_ */




/* Local Variables: */
/* mode:c++ */
/* fill-column:80 */
/* End: */
