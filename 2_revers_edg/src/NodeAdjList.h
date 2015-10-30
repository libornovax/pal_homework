#ifndef NODEADJLIST_H
#define NODEADJLIST_H

#include <vector>


struct NodeAdjList {
    int weight;
    // The nodes that have a directed edge from this node
    std::vector<int> successors;
    // Number of edges, which end in this node
    int num_incoming_edges;


    NodeAdjList ()
    {
        this->num_incoming_edges = 0;
//        this->weight = 0;
    }
};


#endif // NODEADJLIST_H

