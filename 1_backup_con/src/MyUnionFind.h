#ifndef MYUNIONFIND_H
#define MYUNIONFIND_H

#include <vector>


class MyUnionFind
{
    std::vector<int> trees;
    std::vector<int> depths;
    int num_trees;


public:
    MyUnionFind (int sz);
    ~MyUnionFind ();

    int myFind (int node);
    bool myUnion (int node1, int node2);

    void reset ();

    int getNumTrees();

};

#endif // MYUNIONFIND_H
