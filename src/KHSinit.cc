/* 
  KHSinit.cc
  Description: Initialization procs
  ----------------------------------------------------------------------------- 

  Started on  <Tue Feb 26 11:13:47 2013 Carlos Linares Lopez>
  Last update <martes, 10 mayo 2016 21:09:03 Carlos Linares Lopez (clinares)>
  -----------------------------------------------------------------------------

  $Id::                                                                      $
  $Date::                                                                    $
  $Revision::                                                                $
  -----------------------------------------------------------------------------

  Made by Carlos Linares Lopez
  Login   <clinares@psyche>
*/

#include "KHSinit.h"

// most of the search algorithms implemented in this library are
// recursive. Thus, for large instances, a SIGSEV is generated if the current
// stack limit is reached. The following service makes sure that the stacksize
// is unlimited
void khs::setrlimit_stack ()
{
  // Check the current limits on the stack size
  struct rlimit rlp;

  if ( getrlimit ( RLIMIT_STACK, &rlp ) < 0 ) {
    
    cout << endl;
    cout << " ----------------------------------------------------------------" << endl;
    cout << " Warning: it was not possible to get the current limit on the" << endl;
    cout << "          stack size. If the program ends with SIGSEGV, make" << endl;
    cout << "          sure to \"limit stacksize unlimited\"" << endl;
    cout << " ----------------------------------------------------------------" << endl;
  }
  else {
    
    if ( rlp.rlim_cur != RLIM_INFINITY ) {

      cout << endl;
      cout << " ----------------------------------------------------------------" << endl;
      cout << " Warning: the current stack size is not unlimited. Setting it" << endl;
      cout << "          to infinity ..." << endl;
      cout << " ----------------------------------------------------------------" << endl;
      
      // Unlimiting the current stack size
      rlp.rlim_cur = RLIM_INFINITY;
      if ( setrlimit ( RLIMIT_STACK, &rlp ) < 0 ) {

	cout << endl;
	cout << " ----------------------------------------------------------------" << endl;
	cout << " Error: it was not possible to set the limit on the stack size" << endl;
	cout << "        to infinity. Try it by hand and run this program again" << endl;
	cout << " ----------------------------------------------------------------" << endl;
      }
    }
  }
}

/* Local Variables: */
/* mode:c++ */
/* fill-column:80 */
/* End: */
