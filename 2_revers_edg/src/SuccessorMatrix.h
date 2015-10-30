#ifndef SUCCESSORMATRIX_H
#define SUCCESSORMATRIX_H

#include <vector>

#include "Bitset2D.h"
#include "NodeAdjList.h"


class SuccessorMatrix
{
    // Matrix containing true or false for each pair of nodes. The value on position [i][j] determines,
    // whether the node j is reachable from the node i by a directed path
    Bitset2D successor_matrix;


public:

    SuccessorMatrix (std::vector<NodeAdjList> &nodes);
    ~SuccessorMatrix ();

    Bitset2D& getSuccessorMatrix ();

    void print ();


private:

    // Fills the seccessor_matrix with ones for those directed paths that exist in the graph given by the
    // adjacency lists of nodes
    void _fillSuccessorMatrix (std::vector<NodeAdjList> &nodes);

    // Depth first search procedure of filling the successor matrix. Coming from the leaves, we copy all
    // successors of the successors of a node to its successors and so on
    void _dfsFill (std::vector<NodeAdjList> &nodes, std::vector<bool> &visited, int root_id);

};

#endif // SUCCESSORMATRIX_H
