#ifndef NODEWITHNEIGHBORS_H
#define NODEWITHNEIGHBORS_H

#include <vector>
#include <stdexcept>


typedef std::vector<bool> Certificate;

struct NodeWithNeighbors {
    // The neighboring nodes of this node
    std::vector<int> neighbors;
    std::vector<bool> active;

    // Since we do not want to duplicate nodes in the memory, we will just save the certificate for
    // each node in the node itself
    Certificate certificate;
    int num_active_neighbors;
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


    void deactivateNeighbor (int n)
    {
        for (int i = 0; i < this->neighbors.size(); ++i)
        {
            if (this->neighbors[i] == n)
            {
                this->active[i] = false;
                return;
            }
        }

        throw std::logic_error("NodeWithNeighbors::deactivateNeighbor(): Neighbor not found!");
    }


    void printCertificate ()
    {
        for (int i = 0; i < this->certificate.size(); ++i)
        {
            std::cout << this->certificate[i];
        }
    }


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
};


#endif // NODEWITHNEIGHBORS_H

