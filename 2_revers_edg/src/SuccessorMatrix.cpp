#include <stack>

#include "SuccessorMatrix.h"


SuccessorMatrix::SuccessorMatrix(std::vector<NodeAdjList> &nodes)
    : successor_matrix(nodes.size(), nodes.size())
{
    // Fill the matrix of successors
    this->_fillSuccessorMatrix(nodes);
}


SuccessorMatrix::~SuccessorMatrix()
{

}


Bitset2D& SuccessorMatrix::getSuccessorMatrix ()
{
    return this->successor_matrix;
}


void SuccessorMatrix::print ()
{
    this->successor_matrix.print();
}



// ----------------------------------------------  PRIVATE METHODS  ---------------------------------------------- //

void SuccessorMatrix::_fillSuccessorMatrix(std::vector<NodeAdjList> &nodes)
{
    // This is not exacly topological orddering...
    // Use an enhanced Tarjan's algorithm to create lists of successors for each node in the graph. We define a successor
    // as a node, for which exists a directed path from the start node

    // Find nodes, which have no predecessors - 0 incoming edges. If we start the DFS from them, we will go through the
    // whole graph
    std::stack<int, std::vector<int> > start_nodes;
    for (int i = 0; i < nodes.size(); ++i)
    {
        if (nodes[i].num_incoming_edges == 0)
            // This node has no predecessors
            start_nodes.push(i);
    }

    // Run DFS from each of the start nodes and create the successor matrix
    std::vector<bool> visited(nodes.size(), false);
    while (!start_nodes.empty())
    {
        int sn = start_nodes.top();
        start_nodes.pop();

        this->_dfsFill(nodes, visited, sn);
    }
}


void SuccessorMatrix::_dfsFill (std::vector<NodeAdjList> &nodes, std::vector<bool> &visited, int root_id)
{
    if (visited[root_id])
        return;

    NodeAdjList *n = &nodes[root_id];

    // Mark node as visited
    visited[root_id] = true;

    if (n->successors.size() == 0)
        // This is a leaf or the node was already visited
        return;


    // This is not a leaf -> go deeper
    for (int i = 0; i < n->successors.size(); ++i)
    {
        // Visit the child node
        int child_id = n->successors[i];
        this->_dfsFill(nodes, visited, child_id);

        // Copy the nodes that are reachable from this child to the reachable nodes from this root
        for (int j = 0; j <  this->successor_matrix.n; ++j)
        {
            // We want to do OR operator on each successor - this will assure that the successors
            // we already have we will still have and we add the new ones
            successor_matrix(root_id, j) = successor_matrix(root_id, j) + successor_matrix(child_id, j);
        }

        // Set that the child is reachable from this node
        successor_matrix(root_id, child_id) = true;
    }
}


