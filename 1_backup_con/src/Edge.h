#ifndef EDGE_H
#define EDGE_H

#include <iostream>

struct Edge {
    int node1;
    int node2;
    int cost;
    bool in_mst; // Marks whether the edge is in the minimum spanning tree


    Edge ()
    {
//        this->node1 = 0;
//        this->node2 = 0;
//        this->cost = 0;
        this->in_mst = false;
    }

    bool operator< (const Edge& other) const
    {
        return cost < other.cost;
    }


    void arrangeLowHigh ()
    {
        if (this->node1 > this->node2)
        {
            // Higher node is before lower - exchange them
            int basket = this->node1;
            this->node1 = this->node2;
            this->node2 = basket;
        }
    }


    friend std::ostream& operator<< (std::ostream& os, const Edge& obj)
    {
        os << obj.node1 << " " << obj.node2 << " " << obj.cost;
        return os;
    }
};


#endif // EDGE_H

