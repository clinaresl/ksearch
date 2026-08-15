// Ported from GitHub I-Herman/bela by the author of that repo
// Ian Herman <iankherman@gmail.com>
// Carlos Linares López <carlos.linares@uc3m.es>
// created on 28/10/23
// Implementation of the K* algorithm for the k-shortest path problem
// Based on the version of K* specified in On K* Search for Top-K Planning (Lee, 2023)
// ASSUMES POSITIVE COST

#ifndef LIBKSEARCH_KHSIKStar_H
#define LIBKSEARCH_KHSIKStar_H

#include <unordered_set>
#include <list>
#include "KHSbsolver.h"
#include "../structs/KHSnode_t.h"
#include "../structs/KHSIBaseNode.h"
#include "../../domains/roadmap/roadmap_t.h"

typedef const int solutions_t;
typedef int cost_t;

/// K* algorithm
/// \tparam T state type
namespace khs {
    template<typename T>
    class kStar : public bsolver<T> {
    public:
        /// Constructor
        /// \param k k solutions to find
        /// \param start start state
        /// \param goals goal states (may have undefined behavior for multi-goal problems)
        /// \param sog flag which decides whether we should switch to EA on goal expansion
        /// \param alb lower bound on number of A* iterations to run before switching
        /// \param aub upper bound on number of A* iterations to run before switching
        /// \param rsEA whether or not to restart EA on switch from A* (Always true, alternative not currently implemented)
        /// \param bruteForce whether we use the brute force variant or not
        kStar(solutions_t k, const T &start, const T &goal, unsigned long alb = 20, unsigned long aub = 20, bool sog = false, bool rsEA = true,
              bool bruteForce = false, bool inconsistentHeuristic = true);

        /// Solve k-shortest-path problem with K*
        /// \param solutions Vector to output solutions to
        /// \return Number of solutions found
        ksolution_t<T, std::vector> solve();

        const std::string signature() const;

        /// Destructor
        ~kStar();

        // Getters for unit test
        const T& get_start () const { return start; }
        const T& get_goal () const { return goal; }

    private:

        kStar(const kStar &); // This class should never be copied, it makes no sense.
        kStar &operator=(const kStar &);

        T start;
        T goal;

        // Flag parameters for EA and A* switching
        bool switchOnGoal, rsEA;
        unsigned long aStarLowerB, aStarUpperB;

        // If this is true, we don't use the heuristic
        bool _brute_force;

        // True if the heuristic is inconsistent which modifies certain swapping criteria.
        bool _inconsistent_heuristic;

        /// Represents a Side Tracked Edge (STE) which the key component of EA and K*
        class SideTrackedEdge {
        public:
            SideTrackedEdge(T e1, T e2, cost_t detourCost);

            T getE1() const {
                return e1;
            }

            T getE2() const {
                return e2;
            }

            cost_t getDetourCost() const {
                return detourCost;
            }

            void increaseDetourCost(cost_t c) {
                detourCost += c;
            }

            void setDetourCost(cost_t c) { // Should only be used in the inconsistent case when a reopen occurs
                detourCost = c;
            }
        private:
            T e1;
            T e2;
            cost_t detourCost;
        };

        std::unordered_set<SideTrackedEdge *> STEList;
        std::unordered_map<T, bool> treeIsBuilt;
        std::unordered_map<T, std::list<SideTrackedEdge *>> incomingHeap;
        std::unordered_map<T, std::list<SideTrackedEdge *>> treeHeap;

        /// Class for search nodes in the Path Graph
        class PathGraphNode {
        public:
            PathGraphNode(cost_t pathCost, SideTrackedEdge *ste,
                          typename std::list<SideTrackedEdge *>::iterator heapLoc,
                          bool onTree, PathGraphNode *back, bool crossing, T treeLoc);

            /// Getter for back pointer
            /// \return back pointer in path graph
            PathGraphNode *getBack() const;

            /// Getter for crossing variable
            /// \return boolean indicating whether this node is connected to its parent by a cross edge
            bool isCrossing() const;

            /// Get Side Track Edge associated with this node
            /// \return pointer to ste
            SideTrackedEdge *getSte() const;

            /// Get iterator to heap location associated with this node
            /// \return iterator on tree or incoming heap
            typename std::list<SideTrackedEdge *>::iterator getHeapLoc() const;

            /// Get bool indicating whether heapLoc points to a node on the tree heap or incoming heap
            /// \return true if on tree heap, false if on incoming heap
            bool isOnTree();

            /// Gets cost corresponding to the path represented by this node
            /// \return cost
            cost_t getCost() const;

            /// Gets tree heap that this node is on
            /// Has undefined behaviour if not on tree heap
            /// \return state corresponding to tree heap
            T getTree() const;

            class Compare {
            public:
                bool operator()(PathGraphNode *a, PathGraphNode *b) {
                    return a->cost > b->cost;
                }

                bool operator()(const PathGraphNode &a, const PathGraphNode &b) {
                    return a.cost > b.cost;
                }
            };

        private:
            SideTrackedEdge *ste;
            typename std::list<SideTrackedEdge *>::iterator heapLoc;
            bool onTree;
            T treeLoc;
            cost_t cost;
            PathGraphNode *back;
            bool crossing;
        };

        /// Internal class for A* search nodes
        class Node : public BaseNode<T> {
        public:
            /// Constructor
            /// \param gcost cost of path leading to this node from start state
            /// \param fcost \p gcost + heuristic value at state
            /// \param state underlying state of the node
            /// \param back pointer to parent of this node in the search tree
            Node(cost_t gcost, cost_t fcost, T state, Node *back);

            /// Getter for back pointer
            /// \return back pointer in A* search tree
            Node *getBack() const;

            /// Setter for back pointer
            /// \param back new back pointer in A* search tree
            void setBack(Node *back);

        private:
            Node *back;
        };


        std::unordered_map<T, Node *> state_to_node;
        bucket_t<Node *> open;
        std::unordered_set<Node *> closed;
        cost_t EAThr;
        unsigned long aStarIter;

        bucket_t<PathGraphNode *> openEA;
        std::unordered_set<PathGraphNode *> pathGraphNodeSet; // Used for deconstruction

        /// Decide whether to switch from A* to EA
        /// \param goalGen whether the goal has been generator or not
        /// \return
        bool switchToEA(bool goalGen, bool reopen);

        /// Decide whether to switch from EA to A*
        /// \param EAThr may be modified
        /// \param fmin minimum fvalue in A* open list
        /// \return
        bool switchToAStar();

        /// Prepares EA for djikstra's search after A*
        /// \param reopen whether A* has reopened a node
        void prepareEA(bool reopen, bool goalGen, cost_t goalCost, ksolution_t<T, std::list> &sol);

        /// Gets a reference to the tree heap for state \p s, and if unbuilt, builds it
        /// \param s
        /// \return built tree heap at \p s
        std::list<SideTrackedEdge *> &getLazyTreeHeap(T s);

        /// Reconstruct path associated with path graph node \p n and push it to \p sol
        /// \param n
        /// \param sol
        void reconstructPath(kStar::PathGraphNode *n, ksolution_t<T, std::list> &sol);

        /// Successor function for the path graph
        /// \param i parent path graph node
        /// \return successors with cost and bool indicating whether or not an edge is a crossing
        std::unordered_set<PathGraphNode *> getPathGraphSuccessors(PathGraphNode *i);

        /// Returns the max cost of successors without creating new nodes
        /// \param i parent path graph node
        /// \return max cost of all successors
        cost_t maxSuccessorCost(PathGraphNode * i);

        /// Creates new STE and updates heaps
        /// \param e1 first node in ste
        /// \param e2 second node in ste
        /// \param d detour cost
        void newSte(T e1, T e2, cost_t d);

        /// Updates tree heaps with new STE recursively
        /// \param ste new STE
        /// \param curState track current state
        void newTreeNode(SideTrackedEdge *ste, T curState, std::unordered_map<T, bool> &visited);

        /// Updates tree heaps with new root STE on an incoming heap recursively
        /// \param ste new root STE
        /// \param curState track current state
        void newTreeFromRootChange(SideTrackedEdge *ste, SideTrackedEdge *oldSte, T curState,
                                   std::unordered_map<T, bool> &visited);

        /// Ensures that the detour costs of STEs incumbent on \p state are correct and modifies heaps accordingly
        /// \param state state to update
        /// \param increase cost by which the node associated with \p state has decreased
        void updateDetourCostOfStes(T state, cost_t increase);

        /// Only used in the inconsistent case without rs-EA.
        /// Iterates to the search frontier in the path graph and regenerates the open list.
        void regenerateEAOpen();

        const solution_t<T, std::list> generate_solution (const std::list<T>& path,
                                                     const int g,
                                                     const std::string& signature) {
            bsolver<T>::_tend = std::chrono::system_clock::now ();
            // return a solution with this information
            return solution_t<T, std::list> (bsolver<T>::_k,
                                        path,
                                        start,
                                        goal,
                                        0,
                                        bsolver<T>::_h0,
                                        g,
                                        bsolver<T>::_expansions,
                                        bsolver<T>::get_cpu_time (),
                                        0,
                                        signature,
                                        false);
        }

        /// Wrapper for heuristic function that handles the brute force case
        /// \param state state we want to compute the heuristic for
        /// \return 0 if brute force, heuristic value otherwise
        cost_t heuristic(T state) const {
            return _brute_force ? 0 : state.h(goal);
        }
    };

    template<typename T>
    const std::string kStar<T>::signature() const {
        if (_brute_force) {
            return "K0";
        }
        if (_inconsistent_heuristic) {
            return "iK*";
        }
        return "K*";
    }

    template<typename T>
    void kStar<T>::regenerateEAOpen() {
        std::unordered_set<PathGraphNode *> searchFrontier;

        while (!openEA.empty())
            searchFrontier.insert(openEA.pop_front()->getBack());

        for (auto & parent: searchFrontier) {
            // Parent can only be null when the root node was in open.
            // In this case, there will be no other elements in the frontier as the root is always the first node to be expanded.
            // Then, the root will be generated in prepareEA()
            if (parent != nullptr)
                for (auto &child: getPathGraphSuccessors(parent))
                    openEA.insert(child, child->getCost());
        }
    }

    template<class T>
    void kStar<T>::updateDetourCostOfStes(T state, cost_t increase) {
        if (incomingHeap[state].empty())
            return;
        for (auto ste: incomingHeap[state])
            ste->increaseDetourCost(increase);

        std::unordered_map<T, bool> visited;
        newTreeFromRootChange(incomingHeap[state].front(), incomingHeap[state].front(), state, visited);
    }

    template<class T>
    void kStar<T>::newTreeFromRootChange(kStar::SideTrackedEdge *ste, kStar::SideTrackedEdge *oldSte, T curState,
                                         std::unordered_map<T, bool> &visited) {
        // If rsEA, the tree will be deleted anyway when switching to EA so we want to skip all this work
        if (visited[curState] || !treeIsBuilt[curState] || rsEA)
            return;
        visited[curState] = true;
        bool insertionDone = false;
        bool removalDone = false;
        auto j = treeHeap[curState].begin();
        while (j != treeHeap[curState].end()) {
            if ((*j) == oldSte) {
                j = treeHeap[curState].erase(j);
                removalDone = true;
                continue;
            }
            if ((*j)->getDetourCost() > ste->getDetourCost()) {
                treeHeap.at(curState).insert(j, ste);
                insertionDone = true;
                break;
            };
            j++;
        }
        if (!insertionDone)
            treeHeap[curState].push_back(ste);

        if (!removalDone)
            for (; j != treeHeap[curState].end(); ++j)
                if ((*j) == oldSte) {
                    treeHeap[curState].erase(j);
                    break;
                }

        for (auto i: state_to_node[curState]->getAdjacent()) {
            if (state_to_node.find(i.first) == state_to_node.end() || state_to_node[i.first]->getBack() == nullptr ||
                state_to_node[i.first]->getBack()->getState() != curState)
                continue;
            newTreeFromRootChange(ste, oldSte, i.first, visited);
        }
    }


    template<class T>
    void kStar<T>::newTreeNode(SideTrackedEdge *ste, T curState, std::unordered_map<T, bool> &visited) {
        // If rsEA, the tree will be deleted anyway when switching to EA so we want to skip all this work
        if (visited[curState] || !treeIsBuilt[curState] || rsEA)
            return;
        visited[curState] = true;
        bool insertionDone = false;
        for (auto j = treeHeap[curState].begin(); j != treeHeap[curState].end(); ++j)
            if ((*j)->getDetourCost() > ste->getDetourCost()) {
                treeHeap.at(curState).insert(j, ste);
                insertionDone = true;
                break;
            };
        if (!insertionDone)
            treeHeap[curState].push_back(ste);

        for (auto i: state_to_node[curState]->getAdjacent()) {
            if (visited[i.first] || !treeIsBuilt[i.first])
                continue;
            if (state_to_node[i.first]->getBack() == nullptr ||
                state_to_node[i.first]->getBack()->getState() != curState)
                continue;
            newTreeNode(ste, i.first, visited);
        }
    }


    template<class T>
    void kStar<T>::newSte(T e1, T e2, cost_t d) {
        auto *newSte = new SideTrackedEdge(e1, e2, d);
        STEList.insert(newSte);
        bool insertionDone = false;
        if (incomingHeap[e2].empty()) {
            std::unordered_map<T, bool> visited;
            newTreeNode(newSte, e2, visited);
        }

        for (auto j = incomingHeap[e2].begin(); j != incomingHeap[e2].end(); ++j)
            if ((*j)->getDetourCost() > d) {
                if (j == incomingHeap[e2].begin()) {
                    std::unordered_map<T, bool> visited;
                    newTreeFromRootChange(newSte, *j, e2, visited);
                }
                incomingHeap.at(e2).insert(j, newSte);
                insertionDone = true;
                break;
            };
        if (!insertionDone)
            incomingHeap[e2].push_back(newSte);
    }

    template<class T>
    T kStar<T>::PathGraphNode::getTree() const {
        return treeLoc;
    }

    template<class T>
    bool kStar<T>::PathGraphNode::isOnTree() {
        return onTree;
    }

    template<class T>
    typename std::list<typename kStar<T>::SideTrackedEdge *>::iterator kStar<T>::PathGraphNode::getHeapLoc() const {
        return heapLoc;
    }

    template<class T>
    kStar<T>::~kStar() {
        for (auto &i: state_to_node)
            delete i.second;
        for (auto &i: pathGraphNodeSet)
            delete i;
        for (auto &i: STEList)
            delete i;
    }

    template<class T>
    cost_t kStar<T>::PathGraphNode::getCost() const {
        return cost;
    }

    template<class T>
    typename kStar<T>::SideTrackedEdge *kStar<T>::PathGraphNode::getSte() const {
        return ste;
    }

    template<class T>
    void kStar<T>::reconstructPath(kStar::PathGraphNode *n, ksolution_t<T, std::list> &sol) {
        std::vector<SideTrackedEdge *> steList;
        bool followedCross = true;
        PathGraphNode *curNode = n;

        while (curNode->getBack() != nullptr) {
            if (followedCross)
                steList.push_back(curNode->getSte());
            followedCross = curNode->isCrossing();
            curNode = curNode->getBack();
        }

        T lastState = goal;
        std::list<T> path;

        for (int i = steList.size() - 1; i >= 0; i--) {
            while (lastState != steList[i]->getE2()) {
                path.push_front(lastState);
                lastState = state_to_node[lastState]->getBack()->getState();
            }

            path.push_front(lastState);
            lastState = steList[i]->getE1();
        }

        while (lastState != start) {
            path.push_front(lastState);
            lastState = state_to_node[lastState]->getBack()->getState();
        }

        path.push_front(lastState);
        sol += generate_solution(path, n->getCost(), signature());
    }

    template<class T>
    std::list<typename kStar<T>::SideTrackedEdge *> &kStar<T>::getLazyTreeHeap(T s) {
        if (!treeIsBuilt[s]) {
            if (state_to_node[s]->getBack() != nullptr) {
                std::list<SideTrackedEdge *> parentList = getLazyTreeHeap(state_to_node[s]->getBack()->getState());
                treeHeap[s].splice(treeHeap[s].end(), parentList);
            }

            if (!incomingHeap[s].empty()) {
                SideTrackedEdge *toInsert = incomingHeap[s].front();
                bool insertionDone = false;
                for (auto i = treeHeap[s].begin(); i != treeHeap[s].end(); ++i)
                    if ((*i)->getDetourCost() > toInsert->getDetourCost()) {
                        treeHeap[s].insert(i, toInsert);
                        insertionDone = true;
                        break;
                    };
                if (!insertionDone)
                    treeHeap[s].insert(treeHeap[s].end(), toInsert);
            }

            treeIsBuilt[s] = true;
        }
        return treeHeap[s];
    }


    template<class T>
    void kStar<T>::prepareEA(bool reopen, bool goalGen, cost_t goalCost, ksolution_t<T, std::list>& sol) {
        if (rsEA) { // Reset heaps
            for (auto s: treeIsBuilt)
                treeHeap[s.first].clear();
            treeIsBuilt.clear();
            while(!openEA.empty())
                openEA.pop_front();
            for (int i = sol.size() - 1; i >= 0; i--)
                sol.remove(i);
            for (auto & p : pathGraphNodeSet) // Conserves memory
                delete p;
            pathGraphNodeSet.clear();
        }

        if (!openEA.empty())
            return;
        if (open.empty() || goalCost <= open.front()->getFCost()) {
            auto *newNode = new PathGraphNode(goalCost, nullptr, treeHeap[goal].end(), true, nullptr, false, goal);
            openEA.insert(newNode, newNode->getCost());
            pathGraphNodeSet.insert(newNode);
        }
    }

    template<class T>
    bool kStar<T>::switchToAStar() {
        if (open.empty())
            return false;
        cost_t fmin = open.front()->getFCost();
        if (rsEA) {
            EAThr = openEA.front()->getCost();
        }
        else {
            EAThr = maxSuccessorCost(openEA.front());
        }
        return EAThr > fmin;
    }

    template<class T>
    bool kStar<T>::switchToEA(bool goalGen, bool reopen) {
        cost_t fmin = open.front()->getFCost();
        if (switchOnGoal && goalGen)
            return true;
        if (aStarIter >= aStarUpperB)
            return true;
        if (EAThr == 0 && aStarIter > aStarLowerB)
            return true;
        if (!reopen || (aStarIter > aStarLowerB && !openEA.empty())) {
            return EAThr <= fmin;
        }
        return false;
    }

    template<class T>
    ksolution_t<T, std::vector> kStar<T>::solve() {
        // Start the system clock
        bsolver<T>::_tstart = std::chrono::system_clock::now ();

        // Our list of solutions
        ksolution_t<T, std::list> ksolution{bsolver<T>::_k, start, goal};

        // We have no work to do if the start and goal are equal
        if (start == goal) {

            // create then a single solution with no path (and no expansions!)
            std::list<T> path;
            ksolution += generate_solution (path, 0, signature ());

            // and return
            return static_cast<ksolution_t<T, std::vector>>(ksolution);
        }

        // Create the first node and insert it into open
        Node *startNode = new Node(0, heuristic(start), start, nullptr);
        open.insert(startNode, startNode->getFCost());
        state_to_node[start] = startNode;

        // Set all flags to initial values
        EAThr = 0;
        aStarIter = 0;
        bool goalExp = false;
        cost_t goalCost;
        bool reopen = false;
        bool firstTimeGoal = true;

        // Solver loop continues until there are no more nodes in either the path graph or the original graph
        while (!openEA.empty() || !open.empty()) {
            // aStarIter counts the number of iterations of A* done before switching to EA
            aStarIter = 0;
            do {
                // Track if incoming heaps of each node have been invalidated by decreasing all detour costs
                std::unordered_map<T, bool> invalidatedIncomingHeaps;

                // Detect duplicate nodes in open/reopens
                cost_t costWhenInserted = open.get_mini();
                Node *nextParent = open.pop_front();
                bool curNodeReopened = false;
                if (nextParent->isExpanded()) {
                    if (nextParent->getFCost() != costWhenInserted)
                        continue; // If this condition isn't also true, a reopen occurred
                    else
                        curNodeReopened = true;
                }

                // Increment number of expansions performed and iterations of A*
                bsolver<T>::_expansions++;
                aStarIter++;

                // Get successor states and update node accordingly
                std::unordered_map<T, cost_t> adj;
                nextParent->getState ().children (
                    0,
                    goal,
                    [&] (int g, int h, T&& successor) {
                        adj[std::move (successor)] = g;
                    });
                nextParent->expand(adj);

                // Goal found
                if (!goalExp && goal == nextParent->getState()) {
                    goalCost = nextParent->getGCost();
                    goalExp = true;
                    break;
                }

                // Process each child of current node
                for (auto &i: nextParent->getAdjacent()) {
                    T state = i.first;
                    cost_t edgeCost = i.second;
                    Node *child;

                    // Check if node has already been discovered
                    if (state_to_node.find(state) != state_to_node.end()) {
                        child = state_to_node[state];

                        // Check if new path to child is better than the already known best path
                        if (nextParent->getGCost() + edgeCost < child->getGCost()) {
                            // If a better path is found to an already expanded node a reopen has occurred
                            if (child->isExpanded()) {
                                reopen = true;
                                aStarIter = 0; // We reset the counter of A* iterations
                            }

                            // We must update the detour costs of the STEs
                            updateDetourCostOfStes(state, child->getGCost() - nextParent->getGCost() - edgeCost);
                            child->setGCost(nextParent->getGCost() + edgeCost);
                            child->setFCost(child->getGCost() + heuristic(state));

                            // If the tree edge of the child changes, the old tree edge will become a new STE
                            if (child->getBack() != nextParent) {
                                cost_t sideEdgeCost = child->getBack()->getAdjacent().at(state);
                                cost_t detourCost = child->getBack()->getGCost() + sideEdgeCost - child->getGCost();
                                newSte(child->getBack()->getState(), state, detourCost);
                                child->setBack(nextParent);

                                // We must also delete the STE newParent -> child if it exists
                                auto j = incomingHeap[state].begin();
                                while (j != incomingHeap[state].end()) {
                                    if ((*j)->getE1() == nextParent->getState()) {
                                        incomingHeap[state].erase(j);
                                        break;
                                    }
                                    j++;
                                }
                            }

                            open.insert(child, child->getFCost());
                        } else {
                            cost_t detourCost = nextParent->getGCost() + edgeCost - child->getGCost();

                            // Detour costs can decrease when there is a reopen so we update them,
                            // moreover, the STE represented by this edge must already
                            // be in closed as the parent node must have already been expanded
                            // so, we delete the STE with the old cost and reinsert it
                            if (curNodeReopened) {
                                //delete the STE
                                auto j = incomingHeap[state].begin();
                                while (j != incomingHeap[state].end()) {
                                    if ((*j)->getE1() == nextParent->getState()) {
                                        incomingHeap[state].erase(j);
                                        break;
                                    }
                                    j++;
                                }
                            }

                            newSte(nextParent->getState(), state, detourCost);
                        }
                    } else { // Child node has not been discovered, so we create it
                        child = new Node(nextParent->getGCost() + edgeCost,
                                         nextParent->getGCost() + edgeCost + heuristic(state),
                                         state, nextParent);
                        open.insert(child, child->getFCost());
                        state_to_node[state] = child;
                    }
                }
            } while (!(open.empty() || (goalExp && switchToEA(firstTimeGoal, reopen))));
            prepareEA(reopen, firstTimeGoal, goalCost, ksolution);
            firstTimeGoal = false;

            while (!openEA.empty() && !switchToAStar()) {
                reopen = false;
                PathGraphNode *parent = openEA.pop_front();
                reconstructPath(parent, ksolution);
                if (ksolution.size () == bsolver<T>::_k)
                    return static_cast<ksolution_t<T, std::vector>>(ksolution);
                for (auto &i: getPathGraphSuccessors(parent))
                    openEA.insert(i, i->getCost());

            }
        }

        return static_cast<ksolution_t<T, std::vector>>(ksolution);
    }

    template<class T>
    kStar<T>::kStar(solutions_t k, const T &start, const T &goal, unsigned long alb, unsigned long aub, bool sog,
                    bool rsEA,
                    bool bruteForce, bool inconsistentHeuristic) :
            bsolver<T>(k),
            start {start},
            goal {goal}
    {
        bsolver<T>::_h0 = heuristic(start);
        this->switchOnGoal = sog;
        this->aStarLowerB = alb;
        this->aStarUpperB = aub;
        this->_brute_force = bruteForce;
        this->_inconsistent_heuristic = !bruteForce && inconsistentHeuristic; // If brute force, heuristic will not be used
        this->rsEA = true;
    }

    template<class T>
    kStar<T>::SideTrackedEdge::SideTrackedEdge(T e1, T e2, cost_t detourCost) : e1 {e1}, e2 {e2} {
        this->detourCost = detourCost;
    }

    template<class T>
    bool kStar<T>::PathGraphNode::isCrossing() const {
        return crossing;
    }

    template<class T>
    typename kStar<T>::PathGraphNode *kStar<T>::PathGraphNode::getBack() const {
        return back;
    }

    template<class T>
    kStar<T>::PathGraphNode::PathGraphNode(cost_t pathCost, SideTrackedEdge *ste,
                                           typename std::list<SideTrackedEdge *>::iterator heapLoc,
                                           bool onTree, PathGraphNode *back, bool crossing, T treeLoc)
            : treeLoc {treeLoc} {
        this->cost = pathCost;
        this->ste = ste;
        this->heapLoc = heapLoc;
        this->onTree = onTree;
        this->back = back;
        this->crossing = crossing;
    }

    template<class T>
    std::unordered_set<typename kStar<T>::PathGraphNode *> kStar<T>::getPathGraphSuccessors(PathGraphNode *i) {
        std::unordered_set<typename kStar<T>::PathGraphNode *> out;
        PathGraphNode *newNode;
        typename std::list<SideTrackedEdge *>::iterator newHeapIter;
        SideTrackedEdge *nextSte;

        // If i is the root node, do the following
        if (i->getSte() == nullptr) {
            newHeapIter = getLazyTreeHeap(goal).begin();
            nextSte = *newHeapIter;
            if (nextSte != nullptr) {
                newNode = new PathGraphNode(i->getCost() + nextSte->getDetourCost(), nextSte,
                                            getLazyTreeHeap(goal).begin(), true,
                                            i, true, goal);
                out.insert(newNode);
                pathGraphNodeSet.insert(newNode);
            }
            return out;
        }

        // When following heap edge on tree heap
        if (i->isOnTree()) {
            newHeapIter = std::next(i->getHeapLoc());
            nextSte = *newHeapIter;
            if (newHeapIter != treeHeap[i->getTree()].end()) {
                newNode = new PathGraphNode(i->getCost() + nextSte->getDetourCost() - i->getSte()->getDetourCost(),
                                            nextSte,
                                            newHeapIter,
                                            true, i, false, i->getTree());
                out.insert(newNode);
                pathGraphNodeSet.insert(newNode);
            }

            // Prepare for incoming heap edge
            newHeapIter = std::next(incomingHeap[i->getSte()->getE2()].begin());
        } else
            // Prepare for incoming heap edge
            newHeapIter = std::next(i->getHeapLoc());
        // When following heap edge on incoming heap
        if (newHeapIter != incomingHeap[i->getSte()->getE2()].end()) {
            nextSte = *newHeapIter;
            newNode = new PathGraphNode(i->getCost() + nextSte->getDetourCost() - i->getSte()->getDetourCost(), nextSte,
                                        newHeapIter,
                                        false, i, false, goal);
            out.insert(newNode);
            pathGraphNodeSet.insert(newNode);
        }

        // When following cross edge
        newHeapIter = getLazyTreeHeap(i->getSte()->getE1()).begin();
        nextSte = *newHeapIter;
        if (nextSte != nullptr) {
            newNode = new PathGraphNode(i->getCost() + nextSte->getDetourCost(), nextSte, newHeapIter, true, i, true, i->getSte()->getE1());
            out.insert(newNode);
            pathGraphNodeSet.insert(newNode);
        }
        return out;
    }

    template<typename T>
    cost_t kStar<T>::maxSuccessorCost(kStar::PathGraphNode *i) {
        typename std::list<SideTrackedEdge *>::iterator newHeapIter;
        SideTrackedEdge *nextSte;
        cost_t maxCost = 0;

        // If i is the root node, do the following
        if (i->getSte() == nullptr) {
            newHeapIter = getLazyTreeHeap(goal).begin();
            if (!getLazyTreeHeap(goal).empty()) {
                nextSte = *newHeapIter;
                return i->getCost() + nextSte->getDetourCost();
            } else {
                return 0;
            }
        }

        // Perform the following if node's STE is on the tree heap
        if (i->isOnTree()) {
            // Tree edge calculation
            newHeapIter = std::next(i->getHeapLoc());
            if (newHeapIter != treeHeap[i->getTree()].end()) {
                nextSte = *newHeapIter;
                cost_t cost = i->getCost() + nextSte->getDetourCost() - i->getSte()->getDetourCost() > maxCost;
                if (cost > maxCost)
                    maxCost = cost;
            }

            // Prepare for incoming heap edge
            newHeapIter = std::next(incomingHeap[i->getSte()->getE2()].begin());
        } else
            // Prepare for incoming heap edge
            newHeapIter = std::next(i->getHeapLoc());

        // Incoming edge calculation
        if (newHeapIter != incomingHeap[i->getSte()->getE2()].end()) {
            nextSte = *newHeapIter;
            cost_t cost = i->getCost() + nextSte->getDetourCost() - i->getSte()->getDetourCost() > maxCost;
            if (cost > maxCost)
                maxCost = cost;
        }

        // Cross edge calculation
        newHeapIter = getLazyTreeHeap(i->getSte()->getE1()).begin();
        if (!getLazyTreeHeap(i->getSte()->getE1()).empty()) {
            nextSte = *newHeapIter;
            cost_t cost = i->getCost() + nextSte->getDetourCost();
            if (cost > maxCost)
                maxCost = cost;
        }

        return maxCost;
    }


    /*
     * BEGIN NODE IMPLEMENTATION
     */

    template<class T>
    kStar<T>::Node::Node(cost_t gcost, cost_t fcost, T state, Node *back) : BaseNode<T>(gcost, fcost, state) {
        this->back = back;
    }

    template<class T>
    typename kStar<T>::Node *kStar<T>::Node::getBack() const {
        return back;
    }

    template<class T>
    void kStar<T>::Node::setBack(kStar::Node *back) {
        this->back = back;
    }

/*
 * END NODE IMPLEMENTATION
 */
}

#endif //LIBKSEARCH_KHSIKStar_H
