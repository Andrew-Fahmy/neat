#ifndef NEAT_GENOME_H
#define NEAT_GENOME_H

#include <vector>
#include "gene.h"

class genome {
private:
    std::vector<gene_node*> nodes;
    std::vector<gene_connection> connections;
    std::vector<std::vector<double>> graph;
    int fitness;

    void mutate_link(std::vector<std::pair<int, int>> &connection_pairs);
    void mutate_node(std::vector<gene_node*> &all_nodes, std::vector<std::pair<int, int>> &connection_pairs);
    void mutate_weight_shift();
    void mutate_weight_random();
    void mutate_link_toggle();

public:
    genome(std::vector<gene_node*> n, std::vector<gene_connection> connections);

    int get_fitness();
    void calculate_fitness();
    std::vector<double> feed_forward(std::vector<double> inputs);
    double difference(genome &g2);
    genome crossover(genome &g2);
    void mutate(std::vector<gene_node*> &all_nodes, std::vector<std::pair<int, int>> &connection_pairs);
};


#endif