#ifndef TREECERTIFICATE_H
#define TREECERTIFICATE_H

#include <vector>
#include <queue>

#include "NodeWithNeighbors.h"


namespace TreeCertificate
{
    // Compares the two certificates lexicographically
    bool compareLexicographically (const Certificate* c1, const Certificate* c2)
    {
        int min_length = std::min(c1->size(), c2->size());
        for (int i = 0; i < min_length; ++i)
        {
            // Compare bit after bit until they differ or the end is reached
            if ((*c1)[i] != (*c2)[i])
                return (*c1)[i] < (*c2)[i];
        }

        // If the for finished without finding a difference the certificates are the same
        // NOTE: It cannot happen that the shorter certificate would be equal to the begining of the longer one
        return true;
    }


    void computeNodeCertificate (std::vector<NodeWithNeighbors> &nodes, NodeWithNeighbors *node)
    {
        // Take certificates of all "successors" (inactive neighbors)
        std::vector<Certificate*> certificates;
        int length_total = 2;
        int max_depth = 0;
        for (int i = 0; i < node->active.size(); ++i)
        {
            if (!node->active[i])
            {
                certificates.push_back(&nodes[node->neighbors[i]].certificate);
                length_total += nodes[node->neighbors[i]].certificate.size();
                max_depth = std::max(max_depth, nodes[node->neighbors[i]].depth);
            }
        }

        // Sort certificates lexicographically
        std::sort(certificates.begin(), certificates.end(), compareLexicographically);

        // Connect all certificates to create a certificate for this node
        node->certificate.clear(); node->certificate.reserve(length_total);
        node->certificate.push_back(0);
        for (int i = 0; i < certificates.size(); ++i)
        {
            node->certificate.insert(node->certificate.end(), certificates[i]->begin(), certificates[i]->end());
        }
        node->certificate.push_back(1);

        // Set node depth (max distance from leaf)
        node->depth = max_depth + 1;
    }


    // Computes the certificate of the tree and also for each node of the tree given by the nodes. The
    // computed certificate is saved inside of the nodes
    std::vector<int> computeTreeCertificate (std::vector<NodeWithNeighbors> &nodes)
    {
        // Queue of leaves to be processed
        std::queue<int> q;
        int to_queue = nodes.size();

        // Fill up the queue with the leaves
        for (int i = 0; i < nodes.size(); ++i)
        {
            if (nodes[i].neighbors.size() == 1)
            {
                // Node with one neighbor is a leaf
                q.push(i);
                to_queue--;
                // All nodes in the queue already have a certificate -> they are leaves
                nodes[i].certificate = {0, 1};
            }
        }

        // Pop the leaves one by one and compute their certificate
        while (!q.empty())
        {
            if (to_queue == 0 && q.size() == 2)
            {
                // We have the last 2 nodes in the queue -> we have to check whether they are not both candidates
                // for the root
                if (nodes[q.front()].depth == nodes[q.back()].depth)
                    // The nodes have the same depth - both are candidates for the root
                    break;
            }

            int n_id = q.front();
            q.pop();
            NodeWithNeighbors* n = &nodes[n_id];

            // Find this node's "predecessor" in the tree -> it is the only active neighbor
            int p_id;
            NodeWithNeighbors* p = &nodes[p_id];
            for (int i = 0; i < n->active.size(); ++i)
            {
                if (n->active[i])
                {
                    p_id = n->neighbors[i];
                    p = &nodes[p_id];
                    break;
                }
            }

            // Subtract one from the active neighbors
            p->num_active_neighbors--;
            // Deactivate this "successor"
            p->deactivateNeighbor(n_id);

            if (p->num_active_neighbors <= 1)
            {
                // This node was the last active "successor" in the tree -> compute the node's certificate
                // and add it to the leaf queue for processing
                computeNodeCertificate(nodes, p);

                if (p->num_active_neighbors == 0)
                {
                    // This is the root of the tree
                    // It will stay in the queue
                    break;
                }
                else
                {
                    q.push(p_id);
                    to_queue--;
                }
            }
        }


        // Return the root(s) of the tree
        std::vector<int> roots;
        while (!q.empty())
        {
            roots.push_back(q.front());
            q.pop();
        }

        return roots;
    }
}


#endif // TREECERTIFICATE_H

