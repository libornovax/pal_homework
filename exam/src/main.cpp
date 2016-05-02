//
// Libor Novak
// 18.1.2016
//
// PAL EXAM - Connected Networks
//

#include <iostream>
#include <cstdio>
#include <vector>
#include <memory>

#include "NodeWithNeighbors.h"
#include "TreeCertificate.h"



void loadData (std::vector<NodeWithNeighbors> &nodes)
{
    int N; // Total number of nodes in the network
    int M; // Total number of connections (edges)
    std::scanf("%d %d\n", &N, &M);

    nodes.resize(N);

    // Read the list of edges in the network
    int node1, node2;
    for (int i = 0; i < M; ++i)
    {
        std::scanf("%d %d\n", &node1, &node2);
        nodes[node1].addNeighbor(node2);
        nodes[node2].addNeighbor(node1);
    }
}


int main (int argc, char* argv[])
{
#ifdef MEASURE_TIME
    std::clock_t cl = std::clock();
#endif
    // -- LOAD DATA -- //
    // Load data into a bidirectional adjacency list
    auto nodes = std::vector<NodeWithNeighbors>();
    loadData(nodes);


    // -- COMPUTE CERTIFICATES -- //
    // Considering each local network is a tree, we can compute a certificate for each one. The computation will
    // stop when all roots - servers are found
    std::vector<int> servers = TreeCertificate::computeTreeCertificate(nodes);


    // -- MATCH -- //
    // There are at most two local network types
    int numt1 = 0;
    int numt2 = 0;

    // Check if the depths differ - then we do not have to match the certificates
    int depth1 = nodes[servers[0]].depth;
    int num_with_depth1 = 1;
    for (int i = 1; i < servers.size(); ++i)
    {
        if (nodes[servers[i]].depth == depth1)
            num_with_depth1++;
    }

    if (num_with_depth1 != servers.size())
    {
        // The depths differ - we have 2 sets with different depths
        numt1 = num_with_depth1;
        numt2 = servers.size() - numt1;
    }
    else
    {
        // The depths are the same - we have to match the certificates
        // numt1 will be the number of trees that are the same as the tree 1
        numt1 = 1;

        for (int i = 1; i < servers.size(); ++i)
        {
            if (TreeCertificate::certificatesEqual(nodes[servers[0]].certificate, nodes[servers[i]].certificate))
            {
                // Same as the first tree
                numt1++;
            }
            else
            {
                // Then it must be the other tree
                numt2++;
            }
        }
    }


    // -- PRINT THE NUMBERS OF NETWORKS -- //
    if (numt1 < numt2)
        std::cout << numt1 << " " << numt2 << std::endl;
    else
        std::cout << numt2 << " " << numt1 << std::endl;


#ifdef MEASURE_TIME
    std::cout << "TOTAL: " << ((double)std::clock() - cl)/CLOCKS_PER_SEC << "s" << std::endl;
#endif

	return 0;
}


