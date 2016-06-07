/* 
  KHSsolver.h
  Description: Implementation of the base class of different solvers for solving
  the K optimal paths problem
  ----------------------------------------------------------------------------- 

  Started on  <Tue May 10 18:46:43 2016 Carlos Linares Lopez>
  Last update <lunes, 06 junio 2016 17:31:06 Carlos Linares Lopez (clinares)>
  -----------------------------------------------------------------------------

  $Id::                                                                      $
  $Date::                                                                    $
  $Revision::                                                                $
  -----------------------------------------------------------------------------

  Made by Carlos Linares Lopez
  Login   <clinares@atlas>
*/

#ifndef   	KHSSOLVER_H_
# define   	KHSSOLVER_H_

#include <algorithm>
#include <iterator>
#include <map>
#include <vector>

#include "../KHSdefs.h"
#include "../structs/KHSmnode_t.h"

namespace khs {

  using namespace std;

  template<class T>
  class solver {

  public:

    // Explicit constructor
    solver (const mnode_t<T>& start, const mnode_t<T>& goal, unsigned int k)
      : _start {start},
	_goal {goal},
	_k {k},
	_n {0},
	_solution { vector<vector<T>> (0, vector<T>()) },
	_cost { vector<int>() },
	_length { vector<unsigned int>() },
	_totalnodes {0},
	_nodes { vector<unsigned long long int>() }
    { }
    
    // get accessors
    unsigned int get_n () const
    { return _n; }
    vector<vector<T>> get_solution () const
    { return _solution; }
    vector<T> get_solution (const unsigned int idx) const
    { return _solution[idx]; }
    vector<int> get_cost () const
    { return _cost; }
    int get_cost (const unsigned int idx) const
    { return _cost[idx]; }
    vector<unsigned int> get_length () const
    { return _length; }
    unsigned int get_length (const unsigned int idx) const
    { return _length[idx]; }
    unsigned long long int get_totalnodes () const
    { return _totalnodes; }
    vector<unsigned long long int> get_nodes () const
    { return _nodes; }
    unsigned long long int get_nodes (const unsigned int idx) const
    { return _nodes[idx]; }
    vector<double> get_cpu_time () const
    { return _cpu_time; }
    double get_total_cpu_time () const
    { return _total_cpu_time; }

    // methods

    // return true if the solutions in this solver are correct, ie., they are
    // all different and are sorted in increasing order of cost
    bool validate ();

    // critical_paths returns the collection of states that appear in all
    // solutions found by any solver inheriting from this one. It returns the
    // collection of states grouped by the optimal cost of the solutions (cost)
    // and also the overall collection which is computed for all paths
    // independently of the cost.
    void critical_paths (map<unsigned int, vector<T>>& cost, vector<T>& overall);
    
  protected:

    // two different methods for computing descendants are provided so that the
    // evaluation of any node can be always consistently looked up by its
    // f-value
    
    // return a vector with all the descendants of the given node. It does not
    // compute the heuristic value of any node
    void _descendants (const mnode_t<T>& node, deque<mnode_t<T>>& children) const;
        
    // return a vector with all the descendants of the given node. It does
    // compute the heuristic value of each descendant generated
    void _h_descendants (const mnode_t<T>& node, deque<mnode_t<T>>& children) const;
        
    mnode_t<T> _start;						 // start node
    mnode_t<T> _goal;						  // goal node
    unsigned int _k;                          // number of solutions requested

    unsigned int _n;                     // number of solutions found (_n<=_k)
    vector<vector<T>> _solution;                  // vector with all solutions
    vector<int> _cost;                          // cost of the solutions found
    vector<unsigned int> _length;             // length of the solutions found

    unsigned long long int _totalnodes;          // total number of expansions
    vector<unsigned long long int> _nodes;    // number of expansions per path
    
    clock_t _tstart, _tend;	  // stard and end clock ticks of this process
    vector<double> _cpu_time;                    // cpu-time used per solution
    double _total_cpu_time;      // total cpu time for computing all solutions
    
  }; // class solver<T>

  // return a vector with all the descendants of the given node. It does not
  // compute the heuristic value of any node
  template<class T> void solver<T>::_descendants (const mnode_t<T>& node, 
						  deque<mnode_t<T>>& children) const
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
      // state, its m-value, its g-cost and the full path from the start
      // state. Note that all children are generated. It is the task of the
      // search algorithm using this expansion procedure to take care of not
      // reexpanding nodes (e.g., using closed lists or maybe just checking the
      // path of this node). Also, m (the number of times this node has been
      // expanded) is initialized to zero and it is the responsibility of the
      // function using this expansion procedure to compute it correctly (e.g.,
      // by looking up the closed list)
      vector<T> path = node.get_path ();     // get the full path to the parent
      path.push_back (isuccessor->second);    // add this successor to the path
      mnode_t<T> child (isuccessor->second,
			0,
			g+isuccessor->first,
			0,
			path);
      
      // and add it to the set of (legal) descendants
      children.push_back (child);
    }
  }

  // return a vector with all the descendants of the given node. It does
  // compute the heuristic value of each descendant generated
  template<class T> void solver<T>::_h_descendants (const mnode_t<T>& node, 
						    deque<mnode_t<T>>& children) const
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
      // by looking up the closed list)
      vector<T> path = node.get_path ();     // get the full path to the parent
      path.push_back (isuccessor->second);    // add this successor to the path
      mnode_t<T> child (isuccessor->second, 
			isuccessor->second.h (solver<T>::_goal.get_state ()),
			g+isuccessor->first,
			0,
			path);
      
      // and add it to the set of (legal) descendants
      children.push_back (child);
    }
  }
  
  // return true if the solutions in this solver are correct, ie., they are
  // all different and are sorted in increasing order of cost
  template<class T> bool solver<T>::validate ()
  {

    // first, verify that no more than _k solutions have been generated
    if (_n > _k) {
      cerr << " Error: " << _n << " solutions have been generated but only " << _k << " were requested" << endl << endl;
      exit (EXIT_FAILURE);
    }

    // issue a warning in case that less than _k solutions have been
    // generated. This is not necessarily an error as there might be not _k
    // solutions
    if (_n < _k) {
      cerr << " Warning: " << _n << " solutions have been generated but " << _k << " were requested" << endl << endl;
    }

    // second, verify that all paths are sorted in increasing order of cost and
    // that all descendants are correct
    unsigned int c=0;       // cost of the last solution examined, 0 by default
    int idx=1;                                    // reference to each solution
    for (unsigned int i=0;i<_solution.size ();i++) {

      // first, traverse the path verifying that all descendants are computed
      // correctly. For this, generate a path of mnode_t<T> with the current
      // solution
      unsigned int g=0;                     //g-value of each node in this path
      vector<mnode_t<T>> path;
      for (unsigned int j=0;j<_solution[i].size ();j++) {

	// create a mnode_t with this node and add it to the current path
	deque<mnode_t<T>> children;
	mnode_t<T> node (_solution[i][j], 0, g);
	path.push_back (node);

	// and compute its descendants
	_descendants (node, children);

	// now, verify that the next node in this path exists in the list of
	// descendants
	if (j<_solution[i].size ()-1) {
	  mnode_t<T> descendant ((T&) _solution[i][j+1]);
	  typename deque<mnode_t<T>>::iterator next = find (children.begin (), children.end (), descendant);
	  if (next == children.end ()) {
	    cerr << " Fatal Error: One descendant was not legally generated" << endl;
	    cerr << " Descendant: " << *next << endl << endl;
	    exit (EXIT_FAILURE);
	  }
	  else {

	    // otherwise, update the g-cost of this path
	    g = next->get_g ();
	  }
	}
      }

      // verify that the cost of this path is greater or equal than the cost of
      // the preceding path (which is initialized by default to 0) and update c
      // otherwise
      if (g < c) {
	cerr << " Error: solution #" << idx << " has cost " << path.back ().get_g () << " which is less than the cost of the preceding solution (" << c << ")" << endl << endl;
	exit (EXIT_FAILURE);
      }
      c = g;                   // update the cost of the last solution examined
      
      // and update the counter of solutions
      idx++;
    }
    
    // the complexity of the following check is quadratic in the number of paths
    // as every path is compared with all the others
    for (unsigned int i=0;i<_solution.size ();i++)
      for (unsigned int j=i+1;j<_solution.size ();j++) {

	// if these two paths are the same, then exit with an error
	if (duplicate (_solution[i], _solution[j])) {
	  cerr << " Fatal Error: Solution paths #" << i << " and #" << j << " are the same!" << endl << endl;
	  exit (EXIT_FAILURE);
	}
      }
    
    // if this point is reached, then the solution is correct
    return true;    
  }

  // critical_paths returns the collection of states that appear in all
  // solutions found by any solver inheriting from this one. It returns the
  // collection of states grouped by the optimal cost of the solutions (cost)
  // and also the overall collection which is computed for all paths
  // independently of the cost.
  template<class T> void solver<T>::critical_paths (map<unsigned int, vector<T>>& cost,
						    vector<T>& overall)
  {

    map<T, unsigned int> overallHist;    // number of occurrences of each state
    
    for (unsigned int i=0;i<_solution.size ();i++) {       // for each solution

      unsigned int g=0;                // g-value of each node in this solution
      for (unsigned int j=0;j<_solution[i].size ();j++) {

	// add this node to the histogram of overall occurrences
	typename map<T,unsigned int>::const_iterator it = overallHist.find(_solution[i][j]);
	if (it == overallHist.end())
	  overallHist[_solution[i][j]] = 1;
	else
	  overallHist[_solution[i][j]]++;
	
	mnode_t<T> node (_solution[i][j], 0, g);        // create a new mnode_t
	deque<mnode_t<T>> children;
	_descendants (node, children);             // and compute its descendants

	// now, compute the g value of the next node in this path
	if (j<_solution[i].size ()-1) {
	  mnode_t<T> descendant ((T&) _solution[i][j+1]);
	  typename deque<mnode_t<T>>::iterator next = find (children.begin (), children.end (), descendant);
	  if (next == children.end ()) {
	    cerr << " Fatal Error: One descendant could not be found! Validate the solution!" << endl;
	    cerr << " Descendant: " << *next << endl << endl;
	    exit (EXIT_FAILURE);
	  }
	  else
	    g = next->get_g ();    // otherwise, update the g-cost of this node
	}
      }

      // now, add all the states in this path, but the start and goal states to
      // a histogram
    }

    // analyze the overall critical states
    for (typename map<T, unsigned int>::const_iterator it=overallHist.begin ();
	 it != overallHist.end () ;
	 ++it)

      // now, if this state appears as many times as solutions
      if (it->second >= _solution.size ())
	overall.push_back (it->first);
  }
  
} // namespace khs

#endif 	    /* !KHSSOLVER_H_ */




/* Local Variables: */
/* mode:c++ */
/* fill-column:80 */
/* End: */
