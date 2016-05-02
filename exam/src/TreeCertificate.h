#ifndef TREECERTIFICATE_H
#define TREECERTIFICATE_H

#include <vector>
#include <queue>
#include <algorithm>

#include "NodeWithNeighbors.h"


//
// Namespace with functions for computing the tree and node certificates
//
namespace TreeCertificate
{
    // Compare two certificates and determine whether they are equal or they differ
    bool certificatesEqual (const Certificate &c1, const Certificate &c2)
    {
        // First check the certificate lengths
        if (c1.size() != c2.size())
            return false;

        // Compare the bits
        for (int i = 0; i < c1.size(); ++i)
            // Compare bit after bit until they differ or the end is reached
            if (c1[i] != c2[i])
                return false;

        // If the for finished without finding a difference the certificates are the same
        return true;
    }


    // Compares the two certificates lexicographically
    bool compareLexicographically (const Certificate *c1, const Certificate *c2)
    {
        int min_length = std::min(c1->size(), c2->size());
        for (int i = 0; i < min_length; ++i)
        {
            // Compare bit after bit until they differ or the end is reached
            if (c1->operator [](i) != c2->operator [](i))
                return c1->operator [](i) < c2->operator [](i);
        }

        // If the for finished without finding a difference the certificates are the same
        // NOTE: It cannot happen that the shorter certificate would be equal to the begining of the longer one
        // However the comparison function must define a strict comparison!!!!
        return c1 < c2;
    }


    // Computes the certificate of the node by combining lexicagraphically ordered certificates of its
    // neighbors
    void computeNodeCertificate (std::vector<NodeWithNeighbors> &nodes, NodeWithNeighbors *node)
    {
        // Take certificates of all "successors" (inactive neighbors)
        std::vector<Certificate*> certificates;
        // Total length of the resulting combined certificate
        int length_total = 2;
        // The depth of the current node will be the max+1
        int max_depth = 0;

        for (int i = 0; i < node->active.size(); ++i)
            if (!node->active[i])
            {
                certificates.push_back(&nodes[node->neighbors[i]].certificate);

                length_total += nodes[node->neighbors[i]].certificate.size();
                max_depth = std::max(max_depth, nodes[node->neighbors[i]].depth);
            }


        // Sort certificates lexicographically
        std::sort(certificates.begin(), certificates.end(), compareLexicographically);

        // -- APPEND THE CERTIFICATES -- //
        // Connect all certificates to create a certificate for this node
        node->certificate.clear();
        node->certificate.reserve(length_total);

        // The leading zero
        node->certificate.push_back(0);
        // Now append in order all the other certificates
        for (int i = 0; i < certificates.size(); ++i)
            node->certificate.insert(node->certificate.end(), certificates[i]->begin(), certificates[i]->end());
        // Trailing one
        node->certificate.push_back(1);


        // Set node depth (max distance from leaf)
        node->depth = max_depth + 1;
    }


    // Computes the certificate of each tree local network
    std::vector<int> computeTreeCertificate (std::vector<NodeWithNeighbors> &nodes)
    {
        // IDEA: The local networks are trees, whereas the servers are interconnected by a loop, thus when we delete
        // all leaves from the graph, we will be left with the servers only... and the servers are the roots of the
        // trees, for which we then want to compare the certificates

        // Queue of leaves to be processed
        // INFO: The queue will be getting filled with the newly created leaves by deactivating the ones
        // we are removing from the queue until there are no nodes left in the tree
        std::queue<int> q;
        int to_queue = nodes.size();

        // Fill up the queue with the leaves
        for (int i = 0; i < nodes.size(); ++i)
            if (nodes[i].neighbors.size() == 1)
            {
                // Node with one neighbor is a leaf
                q.push(i);
                to_queue--;

                // All nodes in the queue already have a certificate -> they are leaves
                nodes[i].certificate = {0, 1};
            }


        // -- PROCESS THE LEAF QUEUE -- //
        // Pop the leaves one by one and compute their certificate
        while (!q.empty())
        {
            // We will always process the whole queue (all nodes in the trees except for the root) - the trees
            // have one root, which will not be queued

            // Store the front and pop it
            int n_id = q.front(); q.pop();
            NodeWithNeighbors* n = &nodes[n_id];

            // Find this node's "predecessor" in the tree -> it is the only active neighbor
            int p_id = n->getPredecessor();
            NodeWithNeighbors* p = &nodes[p_id];

            // Deactivate this n "successor" of the p "predecessor"
            p->deactivateNeighbor(n_id);

            // Check if we created a leaf from p by removing this "successor"
            if (p->num_active_neighbors <= 1)
            {
                // The node n was the last active "successor" in the tree -> compute the node's certificate
                // and add it to the leaf queue for processing
                computeNodeCertificate(nodes, p);

                // Push this leaf to the queue for processing
                q.push(p_id);
                to_queue--;
            }
        }



        // Return the servers - they are the only nodes with 2 or more active neighbors in the graph
        std::vector<int> servers;
        for (int i = 0; i < nodes.size(); ++i)
            if (nodes[i].num_active_neighbors > 1)
            {
                // Node with 2 or more active neighbors is a server
                servers.push_back(i);
                to_queue--;

                // Compute certificate of the server
                computeNodeCertificate(nodes, &nodes[i]);
            }


        // Check correctness - by now all nodes should be processed
        if (to_queue > 0)
            throw std::logic_error("NodeWithNeighbors::deactivateNeighbor(): Neighbor not found!");


        return servers;
    }
}


#endif // TREECERTIFICATE_H

