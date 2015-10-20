#include "MyUnionFind.h"


MyUnionFind::MyUnionFind (int sz)
{
    trees.resize(sz);
    depths.resize(sz);

    // In the beginning all nodes point to themselves
    this->reset();
}


MyUnionFind::~MyUnionFind ()
{

}


int MyUnionFind::myFind (int node)
{
    if (this->trees[node] == node)
        return node;

    return this->myFind(this->trees[node]);
}


bool MyUnionFind::myUnion (int node1, int node2)
{
    int root1 = this->myFind(node1);
    int root2 = this->myFind(node2);

    if (root1 != root2)
    {
        // The nodes are not in the same tree -> make a union
        if (this->depths[root1] > this->depths[root2])
        {
            // Append 2 to 1
            this->trees[root2] = root1;
            this->depths[root1] = std::max(this->depths[root1], this->depths[root2]+1);
        }
        else
        {
            // Append 1 to 2
            this->trees[root1] = root2;
            this->depths[root2] = std::max(this->depths[root2], this->depths[root1]+1);
        }

        // We made a union -> now the nodes are in the same tree
        this->num_trees--;
        return true;
    }

    // No union was made -> the nodes already were in one tree
    return false;
}


void MyUnionFind::reset ()
{
    for (int i = 0; i < this->trees.size(); ++i) {
        this->trees[i] = i;
        this->depths[i] = 0;
    }

    // Each node is a tree in the begining
    this->num_trees = this->trees.size();
}


int MyUnionFind::getNumTrees ()
{
    return this->num_trees;
}


