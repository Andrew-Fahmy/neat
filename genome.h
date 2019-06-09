#ifndef NEAT_GENOME_H
#define NEAT_GENOME_H

#include <vector>
#include <map>
#include "gene.h"

class genome {
private:
    double fitness;
    std::vector<gene_node> input_nodes;
    std::vector<gene_node> hidden_nodes;
    std::vector<gene_node> output_nodes;
    std::vector<gene_connection> connections;

    void mutate_link(std::vector<std::pair<int, int>> &connection_pairs);
    void mutate_node(std::vector<gene_node> &all_nodes, std::vector<std::pair<int, int>> &connection_pairs);
    void mutate_weight_shift();
    void mutate_weight_random();
    void mutate_link_toggle();

public:
    genome(std::vector<gene_node> nodes, std::vector<gene_connection> _connections);

    double get_fitness();
    void set_fitness(double _fitness);
    std::vector<double> feed_forward(std::vector<double> inputs);
    double difference(genome &g2);
    genome crossover(genome &g2);
    void mutate(std::vector<gene_node> &all_nodes, std::vector<std::pair<int, int>> &connection_pairs);
};


#endif