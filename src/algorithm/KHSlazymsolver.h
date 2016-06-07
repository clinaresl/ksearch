/* 
  KHSlazymsolver.h
  Description: Implementation of Lazy m-Dijkstra
  ----------------------------------------------------------------------------- 

  Started on  <Mon May 16 16:09:01 2016 Carlos Linares Lopez>
  Last update <martes, 07 junio 2016 12:51:33 Carlos Linares Lopez (clinares)>
  -----------------------------------------------------------------------------

  $Id::                                                                      $
  $Date::                                                                    $
  $Revision::                                                                $
  -----------------------------------------------------------------------------

  Made by Carlos Linares Lopez
  Login   <clinares@atlas>
*/

#ifndef   	KHSLAZYMSOLVER_H_
# define   	KHSLAZYMSOLVER_H_

#include <algorithm>

#include "../structs/KHSmnode_t.h"
#include "../structs/KHSbucketvl_t.h"
#include "../structs/KHSset_t.h"

#include "KHSsolver.h"

namespace khs {

  using namespace std;

  template<class T>
  class lazymsolver : public solver<T> {

  public:
    
    // Explicit constructor
    lazymsolver (const mnode_t<T>& start, const mnode_t<T>& goal, unsigned int k)
      : solver<T> (start, goal, k),
      _m {1}            // originally, nodes cannot be expanded more than once!
    { }

    // get accessors
    unsigned int get_m () const
    { return _m; }
    const set_t<mnode_t<T>>& get_closed () const
    { return _closed; }
    
    // methods

    // the following service returns a couple of vectors: the first contains the
    // average number of expansions per node with a specific f-value. The second
    // contains the number of nodes found in closed with a specific f-value.
    void statistics (vector<double>& stats,
		     vector<unsigned long long int>& samples);
    
    // the main service of this class initializes some private attributes and
    // invokes the private methods that actually implement Lazy m-Dijkstra
    void solve ();

  private:

    // this function actually applies Lazy m-Dijkstra to the contents of the
    // specified open list until the goal node is found
    void _solve (bucketvl_t<mnode_t<T>>& open);

  protected:

    // Lazy m-Dijkstra defers the expansion of nodes instead of expanding them
    // eagerly. To do this, the maximum number of expansions per node is managed
    // in a separate variable, called "m" for historical reasons
    unsigned int _m;                   // maximum number of expansions per node
    
    // every search algorithm uses a closed list to store information about all
    // expanded nodes. Since this information is shared among various services,
    // it is stored as a data member of this class
    set_t<mnode_t<T>> _closed;                                   // closed list
    
  }; // class lazymsolver<T>

  template<class T> void lazymsolver<T>::_solve (bucketvl_t<mnode_t<T>>& open)
  {

    bool solution;                    // the last node examined was a solution?

    // create a specific OPEN list to store the goals
    bucketvl_t<mnode_t<T>> goals;
    
    // clear the closed list
    _closed.clear ();

    // -- Main loop
    while (open.get_size ()) {                 // while there are items in OPEN

      mnode_t<T>* goal = goals.front ();                  // get the first goal
      mnode_t<T>* node = open.front ();           // get the first item in OPEN
      solution = false;                     // by default, it is not a solution

      // -- firstly, consume all solutions if any is found
#ifdef __HEURISTIC__
      while (goal && ((goals.get_size () + solver<T>::_n >= solver<T>::_k) ||
		      (goal->get_f () <= node->get_f ()))) {
#else
      while (goal && goal->get_f () <= node->get_f ()) {
#endif	// __HEURISTIC__

	// cout << " \tLength (GOALS): " << goals.get_size () << endl;
	// cout << " \tC*[" << solver<T>::_n << "]: " << goal->get_g () << endl;
	// cout << " \tf (node): " << node->get_f () << endl << endl; cout.flush ();
	
	// copy this solution, its cost, step length, the number of expansions
	// and the time required to find this solution since the beginning
	solver<T>::_solution.push_back (goal->get_path ());
	solver<T>::_cost.push_back (goal->get_g ());
	solver<T>::_length.push_back (goal->get_path ().size () - 1);
	solver<T>::_nodes.push_back (solver<T>::_totalnodes);
	solver<T>::_cpu_time.push_back (((double) (clock () - solver<T>::_tstart)) / 
					CLOCKS_PER_SEC);
	solver<T>::_n++;             // increment the number of found solutions
	solution = true;      // remember the last node examined was a solution
	if (solver<T>::_n >= solver<T>::_k)  // if _k solutions have been found
	  return;                                                // exit nicely
	goals.remove ();              // otherwise, remove this node from GOALS
	goal = goals.front ();    // and take a pointer to the next goal if any
      }

      // -- secondly, in case we just examined a solution in the previous
      // iteration, then rewind the OPEN list giving the algorithm another
      // chance to find more solutions
      if (solution) {
	// cout << " # solutions: " << solver<T>::_n << endl; cout.flush ();
	_m++;                  // increment the allowed number of re-expansions
	open.rewind ();                          // go to the beginning of OPEN
	continue;                                  // and start a new iteration
      }
      
      // -- thirdly, consider expanding this node. Check if this node has ever
      // been expanded before. If it has been expanded a number of times less
      // than the allowed number of expansions per node, then proceed
      mnode_t<T>* prev = _closed.search (*node);

      // in case this node has been already expanded as many times as _m, then
      // preserve it in OPEN and move to the next one
      if (prev && prev->get_m ()>=_m)
	open.next ();

      // otherwise
      else {

	// if this node has not been expanded before, insert it now with m=0
	// (which is the default value) and locate it in the closed list
	if (!prev) {
	  _closed.insert (*node);
	  prev = _closed.search (*node);
	}

	// anyway increment m so that the m-value of a node equals precisely the
	// number of times it has been expanded
	prev->increment_m ();

	// expand also the total number of expansions
	solver<T>::_totalnodes++;

	// consume this node from OPEN. It is important to remove the current
	// node before its descendants are inserted into OPEN as they are
	// inserted by the front. Preserve, however, the current node as it will
	// be used later
	mnode_t<T> current (*node);
	open.remove ();

	// expand this node using the descendants service which does not use the
	// heuristic information
	deque<mnode_t<T>> children;

#ifdef __HEURISTIC__
	solver<T>::_h_descendants (current, children);
#else
	solver<T>::_descendants (current, children);
#endif	// __HEURISTIC__

	// and insert all descendants into the open list
	for (typename deque<mnode_t<T>>::iterator descendant=children.begin ();
	     descendant != children.end ();
	     ++descendant) {

	  // add this node to open only in case this descendant has not been
	  // visited in the path to its parent. Use f=g to sort nodes in OPEN as
	  // this is a variant of Dijkstra
	  if (!current.find (descendant->get_state ())) {

#ifdef __HEURISTIC__

	    // in case this is a goal, insert it in the GOALS list
	    if (descendant->get_h ()==0)
	      goals.insert (*descendant, descendant->get_f ());

	    // otherwise, insert it into OPEN
	    else
	      open.insert (*descendant, descendant->get_f ());
	    
#else
	      
	    // in case this is a goal, then insert it in the GOALS list
	    if (descendant->get_state () == solver<T>::_goal.get_state ())
	      goals.insert (*descendant, descendant->get_f ());
	      
	    // otherwise, insert it into OPEN
	    else
	      open.insert (*descendant, descendant->get_f ());
	  
#endif	// __HEURISTIC__
	  }
	}
      }
    }
  }

  // the following service returns a couple of vectors: the first contains the
  // average number of expansions per node with a specific f-value. The second
  // contains the absolute number of nodes found in closed with a specific
  // f-value.
  template<class T> void lazymsolver<T>::statistics (vector<double>& stats,
						     vector<unsigned long long int>& samples)
  {

    // initialization - stats is indexed by the f-value of each node. Thus, the
    // maximum index equals the cost of the worst solution computed. As they are
    // computed in increasing order of cost, this is the cost of the last
    // solution.

    // Also, to compute the average on the fly, it is necessary to have another
    // vector of samples where the i-th position is the number of nodes found in
    // closed with a f-value equal to i. It has the same size than the
    // statistics vector
    stats = vector<double>(1+solver<T>::_cost[solver<T>::_cost.size ()-1], 0);
    samples = vector<unsigned long long int>(1+solver<T>::_cost[solver<T>::_cost.size ()-1], 0);
    
    // the number of expansions of each node is stored in the closed list once
    // solve has been invoked. Hence, the closed list is traversed here
    for (typename set<mnode_t<T>>::iterator it = _closed.begin () ;
	 it != _closed.end () ;
	 ++it ) {

      // first, update the average number of nodes with the f-value of this
      // node. Note that nodes are expanded a number of times equal to their
      // m-value
      unsigned int idx = it->get_f ();
      stats [idx] = (it->get_m () +  (stats[idx]*samples[idx]))/(1+samples[idx]);

      // secondly, update the number of nodes with this specific f-value
      samples[idx]++;
    }
  }
  
  // the main service of this class initializes some private attributes and
  // invokes the private methods that actually implement Lazy m-Dijkstra
  template<class T> void lazymsolver<T>::solve ()
  {

    // initialize all the statistics
    solver<T>::_n          = 0;
    solver<T>::_solution   = vector<vector<T>> (0, vector<T>());
    solver<T>::_cost       = vector<int>();
    solver<T>::_length     = vector<unsigned int>();
    solver<T>::_totalnodes = 0;
    solver<T>::_nodes      = vector<unsigned long long int>();

    // and also initialize the maximum number of expansions per node ---even if
    // the constructor already does that job explicitly
    _m = 1;
    
    // start the clock
    solver<T>::_tstart = clock ();

    // create an initial open list that only contains the start state
    bucketvl_t<mnode_t<T>> open;

#ifdef __HEURISTIC__

    // create the start state with information about the heuristic
    mnode_t<T> start (solver<T>::_start.get_state (), solver<T>::_start.get_h ());
    
#else
    
    // create the start state
    mnode_t<T> start (solver<T>::_start.get_state ());
    
#endif
    
    // and insert it into the open list in the f-th bucket where f=g
    open.insert (start, start.get_f ());

    // compute the solution 
    _solve (open);

    // stop the clock
    solver<T>::_tend = clock ();

    // and compute the CPU time spent
    solver<T>::_total_cpu_time = ((double) (solver<T>::_tend - solver<T>::_tstart)) / 
      CLOCKS_PER_SEC;
  }
  
} // namespace khs

#endif 	    /* !KHSLAZYMSOLVER_H_ */




/* Local Variables: */
/* mode:c++ */
/* fill-column:80 */
/* End: */
