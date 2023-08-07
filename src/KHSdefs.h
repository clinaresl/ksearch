/* 
  KHSdefs.h
  Description: Generic definitions
  ----------------------------------------------------------------------------- 

  Started on  <Mon Feb 25 17:03:20 2013 Carlos Linares Lopez>
  Last update <jueves, 21 julio 2016 10:20:00 Carlos Linares Lopez (clinares)>
  -----------------------------------------------------------------------------

  Made by Carlos Linares Lopez
  Login   <carlos.linares@uc3m.es>
*/

#include <vector>

#ifndef KHSDEFS_H_
# define KHSDEFS_H_

#define EXIT_FAILURE 1
#define NUMBER_BUCKETS 501                         // initial number of buckets

namespace khs {

  using namespace std;

  // template functions
  
  // the following service returns true if two vectors are the same and false
  // otherwise
  template<class T> 
  extern bool duplicate (const vector<T>& path1, const vector<T>& path2)
  {

    // first, if they have different lengths, then they are different
    if (path1.size () != path2.size ())
      return false;

    // second, traverse all the elements of each path
    for (unsigned int idx=0 ; idx < path1.size () ; idx++)

      // if they differ in one item, then they are different
      if ( !(path1[idx] == path2[idx]) )
        return false;

    // if this point was reached, then both paths are the same
    return true;
  }
  
} // namespace khs

#endif /* !KHSDEFS_H_ */



/* Local Variables: */
/* mode:c++ */
/* fill-column:80 */
/* End: */
