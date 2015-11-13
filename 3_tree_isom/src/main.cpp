//
// Libor Novak
// 11.11.2015
//
// PAL Homework 3 - Tree Isomorphism
//

#include <iostream>
#include <cstdio>
#include <vector>
#include <algorithm>

#include "NodeWithNeighbors.h"
#include "TreeCertificate.h"

//#define DEBUG_OUTPUT


void loadData (std::vector<NodeWithNeighbors> &nodesA, std::vector<NodeWithNeighbors> &nodesB)
{
    int num_nodes_in_A;
    // Number of nodes in the tree A, tree B has one node more
    std::scanf("%d\n", &num_nodes_in_A);

    nodesA.resize(num_nodes_in_A);
    nodesB.resize(num_nodes_in_A+1);


    // Read the list of edges in the tree A
    int node1, node2;
    for (int i = 0; i < num_nodes_in_A-1; ++i)
    {
        std::scanf("%d %d\n", &node1, &node2);
        nodesA[node1].addNeighbor(node2);
        nodesA[node2].addNeighbor(node1);
    }

    // Read the list of edges in the tree B
    for (int i = 0; i < num_nodes_in_A; ++i)
    {
        std::scanf("%d %d\n", &node1, &node2);
        nodesB[node1].addNeighbor(node2);
        nodesB[node2].addNeighbor(node1);
    }
}


bool certEqual (Certificate &cA, Certificate &cB)
{
    if (cA.size() != cB.size())
        return false;

    for (int i = 0; i < cA.size(); ++i)
    {
        // Compare each bit of the certificate
        if (cA[i] != cB[i])
            return false;
    }

    return true;
}


void matchCertificates (std::vector<NodeWithNeighbors> &nodesA, std::vector<NodeWithNeighbors> &nodesB, std::vector<int> idxA, std::vector<int> idxB, std::vector<int> &candidates)
{
    // Variables for keeping track of the matched certificates
    std::vector<bool> matchedA, matchedB;
    matchedA.resize(idxA.size(), false);
    matchedB.resize(idxB.size(), false);

#ifdef DEBUG_OUTPUT
    std::cout << "certificatesB: ";
    for (int i = 0; i < idxB.size(); ++i)
    {
        nodesB[idxB[i]].printCertificate(); std::cout << " ";
    }
    std::cout << std::endl;
    std::cout << "certificatesA: ";
    for (int i = 0; i < idxA.size(); ++i)
    {
        nodesA[idxA[i]].printCertificate(); std::cout << " ";
    }
    std::cout << std::endl;
#endif

    int num_matched = 0;
    // For each node in A find a node from B to be apired with
    for (int i = 0; i < idxA.size(); ++i)
    {
        for (int j = 0; j < idxB.size(); ++j)
        {
            if (!matchedB[j] && certEqual(nodesA[idxA[i]].certificate, nodesB[idxB[j]].certificate))
            {
                // We found a pair to be matched
                matchedA[i] = true;
                matchedB[j] = true;
                num_matched++;
                break;
            }
        }
    }

#ifdef DEBUG_OUTPUT
    std::cout << "matchedB: ";
    for (int i = 0; i < matchedB.size(); ++i)
    {
        std::cout << matchedB[i] << " ";
    }
    std::cout << std::endl;
    std::cout << "matchedA: ";
    for (int i = 0; i < matchedA.size(); ++i)
    {
        std::cout << matchedA[i] << " ";
    }
    std::cout << std::endl;
#endif

    // Check if we found enough pairs
    if (idxA.size()-num_matched >= 2 || idxB.size()-num_matched >= 2)
        // We did not find enough pairs - probably wrong combination of roots
        return;

    // Find the unpaired node in B
    int unpairedB;
    for (int i = 0; i < matchedB.size(); ++i)
    {
        if (!matchedB[i])
        {
            unpairedB = i;
#ifdef DEBUG_OUTPUT
            std::cout << "unpairedB: " << i << std::endl;
#endif
            break;
        }
    }

    int unpairedA = -1;
    for (int i = 0; i < matchedA.size(); ++i)
    {
        if (!matchedA[i])
        {
            unpairedA = i;
#ifdef DEBUG_OUTPUT
            std::cout << "unpairedA: " << i << std::endl;
#endif
            break;
        }
    }

    // Check if there are not the same certificates in B -> they would all be candidates
    for (int i = 0; i < idxB.size(); ++i)
    {
        if (i == unpairedB || certEqual(nodesB[idxB[i]].certificate, nodesB[idxB[unpairedB]].certificate))
        {
            // A candidate -> continue with the search
            std::vector<bool> prot1 = {0,1};
            if (certEqual(nodesB[idxB[i]].certificate, prot1))
            {
                // This is a candidate leaf
                candidates.push_back(idxB[i]);
                continue;
            }

            std::vector<bool> prot2 = {0,0,1,1};
            if (certEqual(nodesB[idxB[i]].certificate, prot2) && unpairedA != -1 && certEqual(nodesA[idxA[unpairedA]].certificate, prot1))
            {
                // The child of this one is a candidate leaf
                candidates.push_back(nodesA[idxA[unpairedA]].getSuccessors()[0]);
                return;
            }

            // Dig deeper
            matchCertificates(nodesA, nodesB, nodesA[idxA[unpairedA]].getSuccessors(), nodesB[idxB[i]].getSuccessors(), candidates);
        }
    }

}



int main (int argc, char* argv[])
{
    // -- LOAD DATA -- //
    // We have a tree with not oriented edges - load into a node list, where each node knows its neighbors
    std::vector<NodeWithNeighbors> nodesA, nodesB;
    loadData(nodesA, nodesB);

//    for (int i = 0; i < nodesA.size(); ++i) {
//        for (int j = 0; j < nodesA[i].neighbors.size(); ++j) {
//            std::cout << nodesA[i].neighbors[j] << " ";
//        }
//        std::cout << std::endl;
//    }

    std::vector<int> rootsA = TreeCertificate::computeTreeCertificate(nodesA);
//    for (int i = 0; i < rootsA.size(); ++i) {
//        nodesA[rootsA[i]].printCertificate(); std::cout << std::endl;
//    }
    std::vector<int> rootsB = TreeCertificate::computeTreeCertificate(nodesB);
//    for (int i = 0; i < rootsB.size(); ++i) {
//        nodesB[rootsB[i]].printCertificate(); std::cout << std::endl;
//    }

//    for (int i = 0; i < nodesA.size(); ++i) {
//        std::cout << nodesA[i].neighbors.size() << std::endl;
//    }

    std::vector<int> candidates;
    if (rootsA.size() == 1 && rootsB.size() == 1)
    {
        // The nicest case - no root change
        matchCertificates(nodesA, nodesB, nodesA[rootsA[0]].neighbors, nodesB[rootsB[0]].neighbors, candidates);
    }
    else if (rootsA.size() == 1 && rootsB.size() == 2)
    {
        // Two possibilities how to match
        matchCertificates(nodesA, nodesB, nodesA[rootsA[0]].neighbors, nodesB[rootsB[0]].neighbors, candidates);
        matchCertificates(nodesA, nodesB, nodesA[rootsA[0]].neighbors, nodesB[rootsB[1]].neighbors, candidates);
    }
    else if (rootsA.size() == 2 && rootsB.size() == 1)
    {
        // Two possibilities how to match
        matchCertificates(nodesA, nodesB, nodesA[rootsA[0]].neighbors, nodesB[rootsB[0]].neighbors, candidates);
        matchCertificates(nodesA, nodesB, nodesA[rootsA[1]].neighbors, nodesB[rootsB[0]].neighbors, candidates);
    }
    else
    {
        // For each tree we imagine that there is one more level and we match the root candidates directly
        matchCertificates(nodesA, nodesB, rootsA, rootsB, candidates);
    }


    std::sort(candidates.begin(), candidates.end());

    for (int i = 0; i < candidates.size(); ++i)
        std::cout << candidates[i] << " ";
    std::cout << std::endl;


	return 0;
}


