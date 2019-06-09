#ifndef NEAT_NETWORK_H
#define NEAT_NETWORK_H

#include <vector>
#include "gene.h"



class node {
public:
    gene_node *nodes;
    double output;
    std::vector<gene_connection> inputs;

public:
    node(gene_node *_node);

    bool operator>(const node &other);
};

class connection {
private:
    double weight;
    bool enabled;
    node *from, *to;
};

class network {
private:
    std::vector<node> input_nodes;
    std::vector<node> hidden_nodes;
    std::vector<node> output_nodes;

public:
    network(std::vector<gene_node*> nodes, std::vector<gene_connection> connections);

    std::vector<double> feed_forward(std::vector<double> inputs);
};

#endif