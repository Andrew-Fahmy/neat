#include <cstdlib>
#include "genome.h"

#define SHIFT_FACTOR 0.3
#define RANDOM_FACTOR 1
#define MUTATE_PROBABILITY 0.2


genome::genome(std::vector<gene_node*> n, std::vector<gene_connection> c) : nodes(n), connections(c) {}

int genome::get_fitness() {
    return fitness;
}

void genome::calculate_fitness() {
    fitness = 1; //todo
}

double genome::difference(genome& g2) {
    genome g1 = *this;
    int g1_index, g2_index,
        disjoint, excess;
    double weight_difference;

    while(g1_index < g1.connections.size() && g2_index < g2.connections.size()) {
        int g1_id = g1.connections.at(g1_index).get_id();
        int g2_id = g2.connections.at(g2_index).get_id();

        if(g1_id == g2_id) {
            weight_difference += std::abs(g1.connections.at(g1_index).get_weight() - g2.connections.at(g2_index).get_weight()) / 2.0;
            g1_index++;
            g2_index++;
        } else if (g1_id > g2_id) {
            disjoint++;
            g2_index++;
        } else {
            disjoint++;
            g1_index++;
        }
    }

    if(g1_index < g1.connections.size()) {
        excess += g1.connections.size() - g1_index;
    } else if(g2_index < g2.connections.size()) {
        excess += g2.connections.size() - g2_index;
    }

    //difference equation: (c1*E)/N + (c2*D)/N + (c3*W)
    //where c1, c2, and c3 let us adjust how much each factor effects the difference,
    //E is excess genes, D is disjoint genes, and W is the average weight,
    //and N is the number of genes (connections) in the larger genome, or 1 if both are less then 20
    //
    //these equations were taken from "Evolving Neural Networks throughAugmenting Topologies"
    //by Kenneth O. Stanley and Risto Miikkulainen
    //http://nn.cs.utexas.edu/downloads/papers/stanley.ec02.pdf

    double c1 = 1.0, c2 = 1.0, c3 = 1.0, N = 1.0;

    if(g1.connections.size() >= 20 && g1.connections.size() > g2.connections.size()) {
        N = g1.connections.size();
    } else if(g2.connections.size() >= 20 && g2.connections.size() > g1.connections.size()) {
        N = g2.connections.size();
    }

    return ((c1 * excess) / N + (c1 * excess) / N + (c3 * weight_difference));
}

std::vector<double> genome::feed_forward(std::vector<double> inputs) {
    
}

genome genome::crossover(genome &g2) { //assuming g1.fitness > g2.fitness
    genome g1 = *this;
    int g1_index, g2_index;
    std::vector<gene_connection> new_connections;

    while(g1_index < g1.connections.size() && g2_index < g2.connections.size()) {
        gene_connection g1_gene = g1.connections.at(g1_index);
        gene_connection g2_gene = g2.connections.at(g2_index);
        int g1_id = g1_gene.get_id();
        int g2_id = g2_gene.get_id();

        if(g1_id == g2_id) {
            if(rand() % 2 == 0) {
                new_connections.push_back(g1_gene);
            } else {
                new_connections.push_back(g2_gene);
            }
            g1_index++;
            g2_index++;
        } else if(g1_id > g2_id) {
            g2_index++;
        } else {
            new_connections.push_back(g1_gene);
            g1_index++;
        }
    }

    for(int i = g1_index; i < g1.connections.size(); i++) {
        new_connections.push_back(g1.connections.at(i));
    }
    return genome(g1.nodes, new_connections); //todo
}

void genome::mutate(std::vector<gene_node*> &all_nodes, std::vector<std::pair<int, int>> &connection_pairs) {
    if(((double) rand() / (RAND_MAX)) < MUTATE_PROBABILITY) {
        mutate_link(connection_pairs);
    }
    if(((double) rand() / (RAND_MAX)) < MUTATE_PROBABILITY) {
        mutate_node(all_nodes, connection_pairs);
    }
    if(((double) rand() / (RAND_MAX)) < MUTATE_PROBABILITY) {
        mutate_weight_shift();
    }
    if(((double) rand() / (RAND_MAX)) < MUTATE_PROBABILITY) {
        mutate_weight_random();
    }
    if(((double) rand() / (RAND_MAX)) < MUTATE_PROBABILITY) {
        mutate_link_toggle();
    }
}

void genome::mutate_link(std::vector<std::pair<int, int>> &connection_pairs) {

    for(int i = 0; i < 50; i++) { //we allow up to 50 failed attempt before continuing
        gene_node *a = nodes.at(rand() % nodes.size());
        gene_node *b = nodes.at(rand() % nodes.size());

        if(a->get_value() == b->get_value()) {
            continue;
        }

        if(a->get_value() > b->get_value()) { //smaller value must come first to avoid recursion
            gene_node *temp = a;
            a = b;
            b = temp;
        }

        std::pair<int, int> temp(a->get_id(), b->get_id());
        int connection_id = connection_pairs.size();

        for(int j = 0; j < connection_pairs.size(); j++) {
            if(connection_pairs.at(j) == temp) {
                connection_id = j;
                break;
            }
        }

        if(connection_id == connection_pairs.size()) {
            connection_pairs.push_back(temp);
        }

        int insert_index = connections.size();
        for(int j = 0; j < connections.size(); j++) { //we must insert it in order of id, otherwise crossover and difference methods will break
            if(connections.at(j).get_id() > connection_id) {
                insert_index = j;
            }
        }
        connections.insert(connections.begin() + insert_index, gene_connection(connection_id, a, b, (((double) rand() / (RAND_MAX)) * 2 - 1) * RANDOM_FACTOR));
        break;
    }
}

void genome::mutate_node(std::vector<gene_node*> &all_nodes, std::vector<std::pair<int, int>> &connection_pairs) {
    if(connections.empty()) {return;}
    gene_connection connection = connections.at(rand() % connections.size());
    connection.set_enabled(false);
    gene_node *from = connection.get_from();
    gene_node *to = connection.get_to();
    gene_node *middle = new gene_node((from->get_value() + to->get_value()) / 2);
    all_nodes.push_back(middle);

    connections.push_back(gene_connection(connection_pairs.size(), from, middle, 1.0)); //add first new connection to current genome
    connection_pairs.push_back(std::pair<int, int>(from->get_id(), middle->get_id())); //add first connection to list of all connections

    connections.push_back(gene_connection(connection_pairs.size(), middle, to, connection.get_weight())); //add second new connection to current genome
    connection_pairs.push_back(std::pair<int, int>(middle->get_id(), to->get_id())); //add second connection to list of all connections
}

void genome::mutate_weight_shift() {
    if(connections.empty()) {return;}
    gene_connection connection = connections.at(rand() % connections.size());
    connection.set_weight(connection.get_weight() + (((double) rand() / (RAND_MAX)) * 2 - 1) * SHIFT_FACTOR);
}

void genome::mutate_weight_random() {
    if(connections.empty()) {return;}
    gene_connection connection = connections.at(rand() % connections.size());
    connection.set_weight((((double) rand() / (RAND_MAX)) * 2 - 1) * RANDOM_FACTOR);
}

void genome::mutate_link_toggle() {
    if(connections.empty()) {return;}
    gene_connection connection = connections.at(rand() % connections.size());
    connection.set_enabled(!connection.get_enabled());
}