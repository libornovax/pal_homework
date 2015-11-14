#include "CertificateMatcher.h"

CertificateMatcher::CertificateMatcher(std::shared_ptr<std::vector<NodeWithNeighbors> > nodesA, std::shared_ptr<std::vector<NodeWithNeighbors> > nodesB)
    : nodesA_(nodesA),
      nodesB_(nodesB)
{
    // Leaf prototype
    this->prot1_ = {0,1};
    // Prototype of a node with one leaf
    this->prot2_ = {0,0,1,1};
}


CertificateMatcher::~CertificateMatcher()
{

}


void CertificateMatcher::matchCertificateGroups (std::vector<int> idxA, std::vector<int> idxB)
{
    // Finds matches of the certificates of the given nodes in the tree A and B and recursively continues to the
    // subtree if the certificates differ

    // Variables for keeping track of the matched certificates
    std::vector<bool> matchedA, matchedB;
    matchedA.resize(idxA.size(), false);
    matchedB.resize(idxB.size(), false);


    int num_matched = 0;
    // For each node in A find a node from B to be paired with
    for (int i = 0; i < idxA.size(); ++i)
        for (int j = 0; j < idxB.size(); ++j)
        {
            if (!matchedB[j] && CertificateMatcher::certEqual_(this->nodesA_->operator [](idxA[i]).certificate, this->nodesB_->operator [](idxB[j]).certificate))
            {
                // We found a pair for which the certificates match and they have not been paired yet
                matchedA[i] = true;
                matchedB[j] = true;
                num_matched++;
                break;
            }
        }


    // Check if we found enough pairs
    // If there are 2 or more unpaired nodes something is wrong - this problem can be caused by a wrong assumption
    // of the main centroid for the case where the number of centroids change after the leaf removal
    if (idxA.size()-num_matched >= 2 || idxB.size()-num_matched >= 2)
        // We did not find enough pairs - probably wrong combination of centroids
        return;


    // Find the unpaired node in the tree B
    int unpairedB = CertificateMatcher::findUnpaired_(matchedB);

    // There might or might not be an unpaired node in the tree A
    int unpairedA = CertificateMatcher::findUnpaired_(matchedA);


    // Unfortunately there might be more subtrees (certificates), which shape is the same and therefore we need to
    // consider all of them as candidates for the changged leaf
    for (int i = 0; i < idxB.size(); ++i)
    {
        // Check if there are the same certificates as the unpaired one in B -> they would all be candidates
        if (i == unpairedB || CertificateMatcher::certEqual_(this->nodesB_->operator [](idxB[i]).certificate, this->nodesB_->operator [](idxB[unpairedB]).certificate))
        {
            // We found a node (certificate), which is the same as the unpaired one

            // It can already be the candidate leaf
            if (CertificateMatcher::certEqual_(this->nodesB_->operator [](idxB[i]).certificate, this->prot1_))
            {
                // This is a candidate leaf
                this->candidates_.push_back(idxB[i]);
                continue;
            }

            // Or it can be that there is a leaf in A (01), but a node with one leaf in B (0011), then the
            // successor node of this one is a candidate
            if (CertificateMatcher::certEqual_(this->nodesB_->operator [](idxB[i]).certificate, this->prot2_) && unpairedA != -1 && CertificateMatcher::certEqual_(this->nodesA_->operator [](idxA[unpairedA]).certificate, this->prot1_))
            {
                // The child of this one is a candidate leaf
                this->candidates_.push_back(this->nodesA_->operator [](idxA[unpairedA]).getSuccessors()[0]);
                return;
            }

            // Dig deeper
            this->matchCertificateGroups(this->nodesA_->operator [](idxA[unpairedA]).getSuccessors(), this->nodesB_->operator [](idxB[i]).getSuccessors());
        }
    }
}


void CertificateMatcher::printSortedCandidates()
{
    // Sort the candidate leafs
    std::sort(this->candidates_.begin(), this->candidates_.end());

    // Print them
    for (int i = 0; i < this->candidates_.size(); ++i)
        std::cout << this->candidates_[i] << " ";
    std::cout << std::endl;
}



// ---------------------------------------- PRIVATE METHODS ---------------------------------------- //

int CertificateMatcher::findUnpaired_ (const std::vector<bool> &matched)
{
    // Returns the index of the first unpaired certificate
    int unpaired = -1;

    for (int i = 0; i < matched.size(); ++i)
    {
        if (!matched[i])
        {
            unpaired = i;
            break;
        }
    }

    return unpaired;
}


bool CertificateMatcher::certEqual_ (const Certificate &cA, const Certificate &cB)
{
    // First quickly check the lengths
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
