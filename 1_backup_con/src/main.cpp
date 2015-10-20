//
// Libor Novak
// 18.10.2015
//
// PAL Homework 1 - Backup Connection
//

#include <iostream>
#include <cstdio>
#include <algorithm>
#include <vector>

#include "MyUnionFind.h"
#include "Edge.h"



void loadData (std::vector<Edge> &edges, int &num_nodes, int &price_low, int &price_high)
{
    std::cin >> num_nodes;

    int num_edges;
    std::cin >> num_edges;

    std::cin >> price_low >> price_high;

    // Read the list of edges
    edges.resize(num_edges);
    for (int i = 0; i < num_edges; ++i)
    {
//        std::cin >> edges[i].node1 >> edges[i].node2 >> edges[i].cost;
        // This is much faster than the stream
        std::scanf("%d %d %d\n", &edges[i].node1, &edges[i].node2, &edges[i].cost);
    }
}


// Find minimum spanning tree cost in the given graph (Kruskal's algorithm)
int mst (MyUnionFind &uf, std::vector<Edge> &edges)
{
    int mst_cost = 0;
    int i = 0;

    // Keep adding edges until we create one spanning tree
    while (uf.getNumTrees() > 1 && i < edges.size())
    {
        // Try to add each edge to the minimum spanning tree
        if (uf.myUnion(edges[i].node1, edges[i].node2))
        {
            // Edge was added
            edges[i].in_mst = true;
            mst_cost += edges[i].cost;
        }

        i++;
    }

    return mst_cost;
}


bool compareLexicographically (const Edge* e1, const Edge* e2)
{
    if (e1->node1 != e2->node1)
        return e1->node1 < e2->node1;

    return e1->node2 < e2->node2;
}


int main (int argc, char* argv[])
{
    // -- LOAD DATA -- //
    std::vector<Edge> edges;
    int num_nodes, price_low, price_high;

    loadData(edges, num_nodes, price_low, price_high);


    // -- SORT THE EDGES -- //
    // Sort according to cost from cheapest to the most expensive
    std::sort(edges.begin(), edges.end());


    // -- MINIMUM SPANNING TREE -- //
    // Use Kruskal's algorithm with union find structure for checking the edges and nodes
    MyUnionFind uf(num_nodes);

    std::cout << mst(uf, edges) << std::endl;


    // -- CONNECTION BREAKS -- //
    // Build a new union find from the edges in the minimum spanning tree only, but leave out
    // the cheapest edge - the result will be two not connected trees
    uf.reset();

    // Leave out the cheapest edge (index 0)
    int i = 1;
    // Keep adding edges until we create two trees
    while (uf.getNumTrees() > 2 && i < edges.size())
    {
        // Use only the edges that are in the original MST
        if (edges[i].in_mst)
            uf.myUnion(edges[i].node1, edges[i].node2);

        i++;
    }


    // -- FIX THE CONNECTION -- //
    // Find edges that can fix the connection
    std::vector<Edge*> fixing_edges;
    for (i = 1; i < edges.size(); i++)
    {
        // Use only the edges that are not the original MST
        if (!edges[i].in_mst && edges[i].cost >= price_low)
        {
            if (edges[i].cost > price_high)
                break;

            if (uf.myFind(edges[i].node1) != uf.myFind(edges[i].node2))
            {
                // Edge connects the two components
                // We want the endpoints of the edges to be arranged in the lower, higher order
                edges[i].arrangeLowHigh();
                fixing_edges.push_back(&edges[i]);
            }
        }
    }

    // Sort the edges lexicographically
    std::sort(fixing_edges.begin(), fixing_edges.end(), compareLexicographically);

    // Output the edges
    for (int ii = 0; ii < fixing_edges.size(); ++ii)
        std::cout << *(fixing_edges[ii]) << std::endl;


	return 0;
}


