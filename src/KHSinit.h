/* 
  KHSinit.h
  Description: Initialization procs
  ----------------------------------------------------------------------------- 

  Started on  <Tue Feb 26 11:07:03 2013 Carlos Linares Lopez>
  Last update <martes, 10 mayo 2016 21:09:38 Carlos Linares Lopez (clinares)>
  -----------------------------------------------------------------------------

  $Id::                                                                      $
  $Date::                                                                    $
  $Revision::                                                                $
  -----------------------------------------------------------------------------

  Made by Carlos Linares Lopez
  Login   <clinares@psyche>
*/

#ifndef   	KHSINIT_H_
# define   	KHSINIT_H_

#include <iostream>

#include <sys/resource.h>
#include <sys/time.h>

namespace khs {

  using namespace std;
  
  // most of the search algorithms implemented in this library are
  // recursive. Thus, for large instances, a SIGSEV is generated if the current
  // stack limit is reached. The following service makes sure that the stacksize
  // is unlimited
  void setrlimit_stack ();

} // namespace khs

#endif 	    /* !KHSINIT_H_ */




/* Local Variables: */
/* mode:c++ */
/* fill-column:80 */
/* End: */
