//
// Libor Novak
// 11.11.2015
//
// PAL Homework 3 - Tree Isomorphism
//

#include <iostream>
#include <cstdio>
#include <vector>
#include <memory>

#include "NodeWithNeighbors.h"
#include "TreeCertificate.h"
#include "CertificateMatcher.h"


void loadData (std::shared_ptr<std::vector<NodeWithNeighbors> > nodesA, std::shared_ptr<std::vector<NodeWithNeighbors> > nodesB)
{
    int num_nodes_in_A;
    // Number of nodes in the tree A, tree B has one node more
    std::scanf("%d\n", &num_nodes_in_A);

    nodesA->resize(num_nodes_in_A);
    nodesB->resize(num_nodes_in_A+1);


    // Read the list of edges in the tree A
    int node1, node2;
    for (int i = 0; i < num_nodes_in_A-1; ++i)
    {
        std::scanf("%d %d\n", &node1, &node2);
        nodesA->operator [](node1).addNeighbor(node2);
        nodesA->operator [](node2).addNeighbor(node1);
    }

    // Read the list of edges in the tree B
    for (int i = 0; i < num_nodes_in_A; ++i)
    {
        std::scanf("%d %d\n", &node1, &node2);
        nodesB->operator [](node1).addNeighbor(node2);
        nodesB->operator [](node2).addNeighbor(node1);
    }
}


int main (int argc, char* argv[])
{
    // -- LOAD DATA -- //
    // We have a tree with not oriented edges - load into a node list, where each node knows its neighbors
    auto nodesA = std::make_shared<std::vector<NodeWithNeighbors> >();
    auto nodesB = std::make_shared<std::vector<NodeWithNeighbors> >();
    loadData(nodesA, nodesB);


    // -- COMPUTE CERTIFICATES -- //
    // We compute the certificate for each of the trees. Returned are the tree centroid candidates (it
    // can be 1 or 2)
    std::vector<int> centroidsA = TreeCertificate::computeTreeCertificate(*nodesA);
    std::vector<int> centroidsB = TreeCertificate::computeTreeCertificate(*nodesB);


    // -- FIND CANDIDATES FOR REMOVAL -- //
    // Find which leafs might have been removed from the tree B in order to get the tree A
    CertificateMatcher cm(nodesA, nodesB);

    // There are several possibilities how the centroids may be matched depending on whether the number
    // of centroids changed after the leaf removal (it could have stayed the same or it could have changed
    // from 1 -> 2 or 2 -> 1)
    if (centroidsA.size() == 1 && centroidsB.size() == 1)
    {
        // The nicest case - one centroid and no centroid change
        cm.matchCertificateGroups(nodesA->operator [](centroidsA[0]).neighbors, nodesB->operator [](centroidsB[0]).neighbors);
    }
    else if (centroidsA.size() == 1 && centroidsB.size() == 2)
    {
        // Two possibilities how to match the centroids
        // Centroid 0 was the main one
        cm.matchCertificateGroups(nodesA->operator [](centroidsA[0]).neighbors, nodesB->operator [](centroidsB[0]).neighbors);
        // Centroid 1 was the main one
        cm.matchCertificateGroups(nodesA->operator [](centroidsA[0]).neighbors, nodesB->operator [](centroidsB[1]).neighbors);
    }
    else if (centroidsA.size() == 2 && centroidsB.size() == 1)
    {
        // Two possibilities how to match the centroids
        // Centroid 0 was the main one
        cm.matchCertificateGroups(nodesA->operator [](centroidsA[0]).neighbors, nodesB->operator [](centroidsB[0]).neighbors);
        // Centroid 1 was the main one
        cm.matchCertificateGroups(nodesA->operator [](centroidsA[1]).neighbors, nodesB->operator [](centroidsB[0]).neighbors);
    }
    else
    {
        // For each tree we imagine that there is one more level and we match the centroids directly
        cm.matchCertificateGroups(centroidsA, centroidsB);
    }


    // -- SORT THE CANDIDATE LEAFS -- //
    // Sort and print them
    cm.printSortedCandidates();


	return 0;
}


