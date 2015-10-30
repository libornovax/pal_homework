//
// Libor Novak
// 29.10.2015
//
// PAL Homework 2 - Reverse an Edge
//

#include <iostream>
#include <cstdio>
#include <algorithm>
#include <vector>
#include <stack>

#include "SuccessorMatrix.h"


void loadData (std::vector<NodeAdjList> &nodes)
{
    int num_nodes, num_edges;
    std::scanf("%d %d\n", &num_nodes, &num_edges);

    // Read the list of node weights
    nodes.resize(num_nodes);
    for (int i = 0; i < num_nodes; ++i)
        std::scanf("%d\n", &nodes[i].weight);

    // Read the list of edges
    int node1, node2;
    for (int i = 0; i < num_edges; ++i)
    {
        std::scanf("%d %d\n", &node1, &node2);
        nodes[node1].successors.push_back(node2);
        nodes[node2].num_incoming_edges += 1;
    }
}


void findStronglyConnectedComponent (std::vector<NodeAdjList> &nodes, Bitset2D &sm_raw, int &node1_max, int &node2_max, int &weight_max)
{
    // For all edges determine how big a strongly connected component will be created and remember
    // the max value
    weight_max = 0;

    for (int node1 = 0; node1 < nodes.size(); ++node1)
    {
        // We can skip the nodes with only one outcoming edge - if we flip it, we will have no way how to get
        // back -> this cannot create a strongly connected component
        if (nodes[node1].successors.size() < 2)
            continue;

        for (int j = 0; j < nodes[node1].successors.size(); ++j)
        {
            // For each edge (node1, node2) determine the weight of the created strongly connected component if
            // it gets reversed
            int node2 = nodes[node1].successors[j];

            // We can skip the nodes with only one incoming edge - if we flip it, we will have no way how to get
            // back here
            if (nodes[node2].num_incoming_edges < 2)
                continue;

            int weight_total = 0;
            for (int k = 0; k < sm_raw.n; ++k)
            {
                if (sm_raw(node1, k) && sm_raw(k, node2))
                    // The node will be in the strongly connected component
                    weight_total += nodes[k].weight;
            }

            // We know that the strongly connected component will contain at least 3 nodes and the weight of
            // all nodes are greater than 0, therefore we can check if there are some other nodes apart from
            // the start and end nodes by requiring the weight sum to be greater than 0
            if (weight_total > 0)
            {
                // Ok, there are some other nodes
                weight_total += nodes[node1].weight + nodes[node2].weight;

                // Check for a new max
                if (weight_total > weight_max)
                {
                    // We found a new max
                    node1_max = node1;
                    node2_max = node2;
                    weight_max = weight_total;
                }

                if (weight_total == weight_max)
                {
                    // We found a strongly connected component with the same weight as max
                    // We need to check if it is not lexicographically before
                    if (node1 < node1_max || (node1 == node1_max && node2 < node2_max))
                    {
                        // The new edge is lexicographically before the current max
                        node1_max = node1;
                        node2_max = node2;
                        weight_max = weight_total;
                    }
                }
            }
        }
    }
}


int main (int argc, char* argv[])
{
    // -- LOAD DATA -- //
    // We will be doing DFS traversal, therefore load the nodes into an adjacency list
    std::vector<NodeAdjList> nodes;
    loadData(nodes);


    // -- TOPOLOGICAL ORDERING -- //
    // Instead of topoligical ordering we create a matrix of successors, where each row has 'true's at the node
    // ids, which are reachable from the node corresponding to the row. The beauty is that now each column marks
    //predecessors of the node, which corresponds to the column id
    SuccessorMatrix sm(nodes);


    // -- REVERSE AN EDGE -- //
    // Find the biggest (weight-wise) strongly connected component that will be created by reversing an edge of
    // the original DAG
    int node1, node2, weight;
    findStronglyConnectedComponent(nodes,sm.getSuccessorMatrix(), node1, node2, weight);

    std::cout << node1 << " " << node2 << " " << weight << std::endl;


	return 0;
}


