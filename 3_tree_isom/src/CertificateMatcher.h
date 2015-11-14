#ifndef CERTIFICATEMATCHER_H
#define CERTIFICATEMATCHER_H

#include <memory>
#include <algorithm>

#include "NodeWithNeighbors.h"


//
// From the already built subtreee certificates finds the leaf candidates, which might
// have been removed from the tree B to create the tree A
//
class CertificateMatcher
{
    // Nodes of the tree with already computed certificates
    std::shared_ptr<std::vector<NodeWithNeighbors> > nodesA_;
    std::shared_ptr<std::vector<NodeWithNeighbors> > nodesB_;

    // Certificate prototypes for the exitting conditions
    std::vector<bool> prot1_;
    std::vector<bool> prot2_;

    // Vector of the candidate leaf nodes from tree B (gets filled when the function
    // matchCertificateGroups is called)
    std::vector<int> candidates_;


public:

    CertificateMatcher(std::shared_ptr<std::vector<NodeWithNeighbors> > nodesA_, std::shared_ptr<std::vector<NodeWithNeighbors> > nodesB_);
    ~CertificateMatcher();

    // The core function of the algorithm. Given two sets of nodes (certificates) it
    // finds the nodes with matching subtrees and recursively it continues until it
    // finds all candidate leafs
    void matchCertificateGroups (std::vector<int> idxA, std::vector<int> idxB);

    // Sorts the candidate leaves and prints them to std::cout
    void printSortedCandidates();


private:

    static int findUnpaired_ (const std::vector<bool> &matched);
    static bool certEqual_ (const Certificate &cA, const Certificate &cB);

};

#endif // CERTIFICATEMATCHER_H
