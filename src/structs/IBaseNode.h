//
// Created by iherman on 12/11/23.
//

#ifndef KSEARCH_IBASENODE_H
#define KSEARCH_IBASENODE_H

#include <unordered_map>

typedef const int solutions_t;
typedef int cost_t;

namespace khs {
    /// Class which represents a generic search node in A*-like algorithms
/// \tparam T state type
    template<class T, class Hash = std::hash<T>>
    class BaseNode {
    public:
/// Allows hashing for nodes, needed for some stl containers
        struct hash {
            std::size_t operator()(BaseNode n) const {
                return std::hash<T>()(n.state);
            }
        };

        BaseNode(cost_t gcost, cost_t fcost, T state) : state {state} {
            this->gcost = gcost;
            this->fcost = fcost;
            this->expanded = false;
        };

        /// Sets the adjacency list of the node
        /// \param adjacent adjacency list, presumably given from graph
        virtual void expand(std::unordered_map<T, cost_t, Hash> adjacent);

        /// Checks if the node has been expanded (in closed)\
    /// \return expanded or not
        virtual bool isExpanded() const;

        /// adjacency list getter
        /// \return adjacency list of this node
        virtual const std::unordered_map<T, cost_t, Hash> &getAdjacent() const;

        /// gcost getter
        /// \return gcost of this nodeur
        cost_t getGCost() const;

        /// gcost setter
        /// \param c gcost of this node
        void setGCost(cost_t c);

        /// fcost getter
        /// \return fcost of this node
        cost_t getFCost() const;

        /// fcost setter
        /// \param c fcost of this node
        void setFCost(cost_t c);

        /// state getter
        /// \return state of this node
        virtual T &getState();

        virtual T const &getState() const;

        /// Comparison class to be passed to stl structures
        class Compare {
        public:
            bool operator()(BaseNode *a, BaseNode *b) {
                return a->fcost > b->fcost;
            }

            bool operator()(const BaseNode &a, const BaseNode &b) {
                return a.fcost > b.fcost;
            }
        };

    protected:
        T state;
        bool expanded;
        cost_t gcost;
        cost_t fcost;
        std::unordered_map<T, cost_t, Hash> adjacent;
    };

    template<class T, class Hash>
    bool BaseNode<T, Hash>::isExpanded() const {
        return expanded;
    }

    template<class T, class Hash>
    void BaseNode<T, Hash>::expand(std::unordered_map<T, cost_t, Hash> adjacent) {
        this->adjacent = adjacent;
        expanded = true;
    }

    template<class T, class Hash>
    const std::unordered_map<T, cost_t, Hash> &BaseNode<T, Hash>::getAdjacent() const {
        return adjacent;
    }

    template<class T, class Hash>
    cost_t BaseNode<T, Hash>::getGCost() const {
        return gcost;
    }

    template<class T, class Hash>
    void BaseNode<T, Hash>::setGCost(cost_t c) {
        this->gcost = c;
    }

    template<class T, class Hash>
    cost_t BaseNode<T, Hash>::getFCost() const {
        return fcost;
    }

    template<class T, class Hash>
    void BaseNode<T, Hash>::setFCost(cost_t c) {
        this->fcost = c;
    }

    template<class T, class Hash>
    const T &BaseNode<T, Hash>::getState() const {
        return state;
    }

    template<class T, class Hash>
    T &BaseNode<T, Hash>::getState() {
        return state;
    }
}



#endif //KSEARCH_IBASENODE_H
