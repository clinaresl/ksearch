/* 
  KHSmhsolver.h
  Description: Implementation of m-A* (Flerova, Marinescu, Dechter; JAIR 2016)
  ----------------------------------------------------------------------------- 

  Started on  <Thu May 12 16:26:25 2016 Carlos Linares Lopez>
  Last update <martes, 17 mayo 2016 21:31:39 Carlos Linares Lopez (clinares)>
  -----------------------------------------------------------------------------

  $Id::                                                                      $
  $Date::                                                                    $
  $Revision::                                                                $
  -----------------------------------------------------------------------------

  Made by Carlos Linares Lopez
  Login   <clinares@atlas>
*/

#ifndef   	KHSMHSOLVER_H_
# define   	KHSMHSOLVER_H_

#include "../structs/KHSmnode_t.h"
#include "../structs/KHSbucketvd_t.h"
#include "../structs/KHSset_t.h"

#include "KHSsolver.h"

namespace khs {

  using namespace std;

  template<class T>
  class mhsolver : public msolver<T> {

  public:
    
    // Explicit constructor
    mhsolver (const mnode_t<T>& start, const mnode_t<T>& goal, unsigned int k)
      : msolver<T> (start, goal, k)
    { }

    // methods

    // the main service of this class initializes some private attributes and
    // invokes the private methods that actually implement m-A* (Flerova,
    // Marinescu, Dechter; JAIR 2016)
    void solve ();

  private:

    // this function actually applies m-A* (Flerova, Marinescu, Dechter;
    // JAIR 2016) to the contents of the specified open list until the goal node
    // is found
    void _solve (bucketvd_t<mnode_t<T>>& open);

  }; // class mhsolver<T>

  template<class T> void mhsolver<T>::_solve (bucketvd_t<mnode_t<T>>& open)
  {

    // clear the closed list
    msolver<T>::_closed.clear ();
    
    // while the open list is not empty
    while (open.get_size ()) {

      // pop the first node from open
      mnode_t<T> node = open.pop_front ();

      // if this node is a goal node
      if (node.get_state () == solver<T>::_goal.get_state ()) {

	// copy this solution, its cost, step length, the number of expansions
	// and the time required to find this solution since the beginning
	solver<T>::_solution.push_back (node.get_path ());
	solver<T>::_cost.push_back (node.get_g ());
	solver<T>::_length.push_back (node.get_path ().size () - 1);
	solver<T>::_nodes.push_back (solver<T>::_totalnodes);
	solver<T>::_cpu_time.push_back (((double) (clock () - solver<T>::_tstart)) / 
					CLOCKS_PER_SEC);

	// increment the number of found solutions
	solver<T>::_n++;

	// in case the requested number has been found
	if (solver<T>::_n >= solver<T>::_k)
	  break;

	// otherwise
	continue;
      }

      // otherwise, consider expanding this node. First, check if this node has
      // ever been expanded before. If it has not been expanded or, it has been
      // expanded a number of times less than the number of requested paths,
      // then proceed
      mnode_t<T>* prev = msolver<T>::_closed.search (node);
      if (!prev ||
	  (prev && prev->get_m () < solver<T>::_k)) {

	// if this node has not been expanded before, insert it now with m=0
	// (which is the default value) and locate it in the closed list
	if (!prev) {
	  msolver<T>::_closed.insert (node);
	  prev = msolver<T>::_closed.search (node);
	}

	// anyway increment m so that the m-value of a node equals precisely the
	// number of times it has been expanded
	prev->increment_m ();

	// expand also the total number of expansions
	solver<T>::_totalnodes++;
	
	// expand this node using the descendants service which does not use the
	// heuristic information
	deque<mnode_t<T>> children;
	solver<T>::_h_descendants (node, children);

	// and insert all descendants into the open list
	for (typename deque<mnode_t<T>>::iterator it=children.begin ();
	     it != children.end ();
	     ++it) {

	  // add this node to open only in case this descendant has not been
	  // visited in the path to its parent. Use f=g to sort nodes in OPEN as
	  // this is a variant of A*
	  if (!node.find (it->get_state ()))
	    open.insert_front (*it, it->get_f ());
	}
      }
    }
  }

  // the main service of this class initializes some private attributes and
  // invokes the private methods that actually implement m-A*.
  template<class T> void mhsolver<T>::solve ()
  {

    // initialize all the statistics
    solver<T>::_n          = 0;
    solver<T>::_solution   = vector<vector<T>> (0, vector<T>());
    solver<T>::_cost       = vector<int>();
    solver<T>::_length     = vector<unsigned int>();
    solver<T>::_totalnodes = 0;
    solver<T>::_nodes      = vector<unsigned long long int>();
    
    // start the clock
    solver<T>::_tstart = clock ();

    // create an initial open list that only contains the start state
    bucketvd_t<mnode_t<T>> open;

    // create the start state
    mnode_t<T> start (solver<T>::_start.get_state (),
		      solver<T>::_start.get_state ().h (solver<T>::_goal.get_state ()));

    // and finally insert it into the open list in the f-th bucket where f=g+h
    open.insert_front (start, solver<T>::_start.get_f ());

    // compute the solution 
    _solve (open);

    // stop the clock
    solver<T>::_tend = clock ();

    // and compute the CPU time spent
    solver<T>::_total_cpu_time = ((double) (solver<T>::_tend - solver<T>::_tstart)) / 
      CLOCKS_PER_SEC;
  }
  
} // namespace khs

#endif 	    /* !KHSMHSOLVER_H_ */




/* Local Variables: */
/* mode:c++ */
/* fill-column:80 */
/* End: */
