#ifndef NODEWITHNEIGHBORS_H
#define NODEWITHNEIGHBORS_H

#include <iostream>
#include <vector>
#include <stdexcept>


typedef std::vector<bool> Certificate;

//
// A structure for storing all data for a node in a tree that is needed for the task
//
struct NodeWithNeighbors {
    // The neighboring nodes of this node
    std::vector<int> neighbors;
    std::vector<bool> active;

    // Since we do not want to duplicate nodes in the memory, we will just save the certificate for
    // each node in the node itself
    Certificate certificate;
    int num_active_neighbors;
    // This actually is not real depth but distance to the farthest leaf node successor
    int depth;


    NodeWithNeighbors ()
    {
        this->num_active_neighbors = 0;
        this->depth = 0;
    }


    void addNeighbor (int n)
    {
        this->neighbors.push_back(n);
        this->active.push_back(true);
        this->num_active_neighbors++;
    }


    // While building the certificates we need to mark whether a certain neighbor of the node is already
    // being processed and this is a way how to do it
    void deactivateNeighbor (int n)
    {
        for (int i = 0; i < this->neighbors.size(); ++i)
        {
            if (this->neighbors[i] == n)
            {
                this->active[i] = false;
                // Subtract one from the active neighbors
                this->num_active_neighbors--;
                return;
            }
        }

        throw std::logic_error("NodeWithNeighbors::deactivateNeighbor(): Neighbor not found!");
    }


    // Helper function for printing the certificate of the node
    void printCertificate ()
    {
        for (int i = 0; i < this->certificate.size(); ++i)
        {
            std::cout << this->certificate[i];
        }
    }


    // Returns all nodes, which we deactivatedd during the certificate computation - only the successors
    // get deactivated (nodes from the subtree)
    std::vector<int> getSuccessors ()
    {
        // Successors are the inactive nodes
        std::vector<int> succ;

        for (int i = 0; i < this->active.size(); ++i)
        {
            if (!this->active[i])
                succ.push_back(this->neighbors[i]);
        }

        return succ;
    }


     // Find this node's "predecessor" in the tree -> it is the only active neighbor
    int getPredecessor ()
    {
        for (int i = 0; i < this->active.size(); ++i)
        {
            if (this->active[i])
            {
                return this->neighbors[i];
            }
        }

        throw std::logic_error("NodeWithNeighbors::getPredecessor(): No predecessor found!");
    }
};


#endif // NODEWITHNEIGHBORS_H

