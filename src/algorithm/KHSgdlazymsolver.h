/* 
  KHSgdlazymsolver.h
  Description: Implementation of goal-driven Lazy m-Dijkstra/m-A*
  ----------------------------------------------------------------------------- 

  Started on  <Mon May 16 16:09:01 2016 Carlos Linares Lopez>
  Last update <martes, 07 junio 2016 20:25:33 Carlos Linares Lopez (clinares)>
  -----------------------------------------------------------------------------

  $Id::                                                                      $
  $Date::                                                                    $
  $Revision::                                                                $
  -----------------------------------------------------------------------------

  Made by Carlos Linares Lopez
  Login   <clinares@atlas>
*/

#ifndef   	KHSGDLAZYMSOLVER_H_
# define   	KHSGDLAZYMSOLVER_H_

#include <algorithm>
#include <exception>
#include <stdexcept>

#include "../structs/KHSfmnode_t.h"
#include "../structs/KHSbucketvl_t.h"
#include "../structs/KHSset_t.h"

#include "KHSsolver.h"

namespace khs {

  using namespace std;

  template<class T>
  class gdlazymsolver : public solver<T> {

  public:
    
    // Explicit constructor
    gdlazymsolver (const fmnode_t<T>& start, const fmnode_t<T>& goal, unsigned int k)
      : solver<T> (start, goal, k),
      _m {1}            // originally, nodes cannot be expanded more than once!
    { }

    // get accessors
    unsigned int get_m () const
    { return _m; }
    const set_t<fmnode_t<T>>& get_closed () const
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

    // return a vector with all the descendants of the given node. It does not
    // compute the heuristic value of any node
    void _descendants (const fmnode_t<T>& node, deque<fmnode_t<T>>& children) const;
        
    // return a vector with all the descendants of the given node. It does
    // compute the heuristic value of each descendant generated
    void _h_descendants (const fmnode_t<T>& node, deque<fmnode_t<T>>& children) const;
    
    // this function actually applies Lazy m-Dijkstra to the contents of the
    // specified open list until the goal node is found
    void _solve (bucketvl_t<fmnode_t<T>>& open);

  protected:

    // Goal-driven Lazy m-Dijkstra defers the expansion of nodes instead of
    // expanding them eagerly. To do this, the maximum number of expansions per
    // node is managed in a separate variable, called "m" for historical
    // reasons. Additionally, it only re-expands those nodes that have been
    // discovered to be part of an optimal solution path.
    unsigned int _m;                   // maximum number of expansions per node
    
    // every search algorithm uses a closed list to store information about all
    // expanded nodes. Since this information is shared among various services,
    // it is stored as a data member of this class
    set_t<fmnode_t<T>> _closed;                                  // closed list
    
  }; // class gdlazymsolver<T>

  template<class T> void gdlazymsolver<T>::_descendants (const fmnode_t<T>& node, 
							 deque<fmnode_t<T>>& children) const
  {
    unsigned int g = node.get_g ();              // retrieve the current g-cost
    T state = node.get_state ();               // retrieve the underlying state
    
    // get all the successors of the state specified in the node along with the
    // cost of the edge
    deque<pair<unsigned int, T>> successors = state.pair_descendants ();
    for (typename deque<pair<unsigned int, T>>::iterator isuccessor = successors.begin ();
	 isuccessor != successors.end ();
	 isuccessor++) {

      // create a child with all the neccesssary information including: the
      // state, its m-value, its flag, its g-cost and the full path from the
      // start state. Note that all children are generated. It is the task of
      // the search algorithm using this expansion procedure to take care of not
      // reexpanding nodes (e.g., using closed lists or maybe just checking the
      // path of this node). Also, m (the number of times this node has been
      // expanded) is initialized to zero and it is the responsibility of the
      // function using this expansion procedure to compute it correctly (e.g.,
      // by looking up the closed list). Likewise, the flag is set to false by
      // default.
      vector<T> path = node.get_path ();     // get the full path to the parent
      path.push_back (isuccessor->second);    // add this successor to the path
      fmnode_t<T> child (isuccessor->second,
			 0,
			 g+isuccessor->first,
			 0,
			 false,
			 path);
      
      // and add it to the set of (legal) descendants
      children.push_back (child);
    }
  }

  // return a vector with all the descendants of the given node. It does
  // compute the heuristic value of each descendant generated
  template<class T> void gdlazymsolver<T>::_h_descendants (const fmnode_t<T>& node, 
							   deque<fmnode_t<T>>& children) const
  {
    unsigned int g = node.get_g ();              // retrieve the current g-cost
    T state = node.get_state ();               // retrieve the underlying state
    
    // get all the successors of the state specified in the node along with the
    // cost of the edge
    deque<pair<unsigned int, T>> successors = state.pair_descendants ();
    for (typename deque<pair<unsigned int, T>>::iterator isuccessor = successors.begin ();
	 isuccessor != successors.end ();
	 isuccessor++) {

      // create a child with all the neccesssary information including: the
      // state, its m-value, its h-value, its g-cost and the full path from the
      // start state. Note that all children are generated. It is the task of
      // the search algorithm using this expansion procedure to take care of not
      // reexpanding nodes (e.g., using closed lists or maybe just checking the
      // path of this node). Also, m (the number of times this node has been
      // expanded) is initialized to zero and it is the responsibility of the
      // function using this expansion procedure to compute it correctly (e.g.,
      // by looking up the closed list). Likewise, the flag is set to false by
      // default.
      vector<T> path = node.get_path ();     // get the full path to the parent
      path.push_back (isuccessor->second);    // add this successor to the path
      fmnode_t<T> child (isuccessor->second, 
			 isuccessor->second.h (solver<T>::_goal.get_state ()),
			 g+isuccessor->first,
			 0,
			 false,
			 path);
      
      // and add it to the set of (legal) descendants
      children.push_back (child);
    }
  }
  
  template<class T> void gdlazymsolver<T>::_solve (bucketvl_t<fmnode_t<T>>& open)
  {

    bool solution;                    // the last node examined was a solution?

    // create a specific OPEN list to store the goals
    bucketvl_t<fmnode_t<T>> goals;
    
    // clear the closed list
    _closed.clear ();

    // -- Main loop
    while (open.get_size ()) {                 // while there are items in OPEN

      fmnode_t<T>* goal = goals.front ();                 // get the first goal
      fmnode_t<T>* node = open.front ();          // get the first item in OPEN
      solution = false;                     // by default, it is not a solution

      // -- firstly, consume all solutions if any is found
#ifdef __HEURISTIC__
      while (goal && ((goals.get_size () + solver<T>::_n >= solver<T>::_k) ||
		      (goal->get_f () <= node->get_f ()))) {
#else
      while (goal && goal->get_f () <= node->get_f ()) {
#endif	// __HEURISTIC__

	// first of all, mark all nodes in this solution as been part of a
	// previous optimal solution path
	for (typename vector<T>::const_iterator inode = goal->get_path ().begin ();
	     inode != goal->get_path ().end ();
	     ++inode) {

	  // skip the goal since it is never expanded
	  if (!(*inode == solver<T>::_goal.get_state ())) {
	  
	    // look for this specific node in closed
	    fmnode_t<T>* isol = _closed.search (fmnode_t<T>(*inode));
	    if (!isol) {
	      cout << " Goal            : " << *goal << endl;
	      cout << " Offending node  : " << *inode << endl;
	      cout << " Offending fmnode: " << fmnode_t<T>(*inode) << endl << endl;
	      cout.flush ();
	      throw domain_error ("khs::gdlazymsolver<T>::_solve Node not found in CLOSED!");
	    }
	    
	    isol->set_flag (true);           // and make its flag equal to true
	  }
	}
	
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
	_m++;                  // increment the allowed number of re-expansions
	open.rewind ();                          // go to the beginning of OPEN
	continue;                                  // and start a new iteration
      }
      
      // -- thirdly, consider expanding this node. Check if this node has ever
      // been expanded before. If it has been expanded a number of times less
      // than the allowed number of expansions per node, then proceed
      fmnode_t<T>* prev = _closed.search (*node);

      // in case this node has been already expanded as many times as _m, or it
      // has been already expanded but it is not known to be part of any optimal
      // solution then preserve it in OPEN and move to the next one
      if (prev && (prev->get_m ()>=_m || !prev->get_flag ()))
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
	fmnode_t<T> current (*node);
	open.remove ();

	// expand this node using the descendants service which does not use the
	// heuristic information
	deque<fmnode_t<T>> children;

#ifdef __HEURISTIC__
	_h_descendants (current, children);
#else
	_descendants (current, children);
#endif	// __HEURISTIC__

	// and insert all descendants into the open list
	for (typename deque<fmnode_t<T>>::iterator descendant=children.begin ();
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
  template<class T> void gdlazymsolver<T>::statistics (vector<double>& stats,
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
    for (typename set<fmnode_t<T>>::iterator it = _closed.begin () ;
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
  template<class T> void gdlazymsolver<T>::solve ()
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
    bucketvl_t<fmnode_t<T>> open;

#ifdef __HEURISTIC__

    // create the start state with information about the heuristic
    fmnode_t<T> start (solver<T>::_start.get_state (), solver<T>::_start.get_h ());
    
#else
    
    // create the start state
    fmnode_t<T> start (solver<T>::_start.get_state ());
    
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

#endif 	    /* !KHSGDLAZYMSOLVER_H_ */




/* Local Variables: */
/* mode:c++ */
/* fill-column:80 */
/* End: */
