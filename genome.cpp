#include <cstdlib>
#include <iostream>
#include "genome.h"

#define SHIFT_FACTOR 0.3
#define RANDOM_FACTOR 1
#define MUTATE_PROBABILITY 0.2


genome::genome(std::vector<gene_node> nodes, std::vector<gene_connection> _connections) : connections(_connections) {
    for(size_t i = 0; i < nodes.size(); i++) {
        if(nodes.at(i).get_type() == node_type::input) {
            input_nodes.push_back(nodes.at(i));
        } else if(nodes.at(i).get_type() == node_type::hidden) {
            hidden_nodes.push_back(nodes.at(i));
        } else {
            output_nodes.push_back(nodes.at(i));
        }
    }
}

double genome::get_fitness() {
    return fitness;
}

void genome::set_fitness(double _fitness) {
    fitness = _fitness;
}

double genome::difference(genome& g2) {
    genome g1 = *this;
    size_t g1_index, g2_index;
    int disjoint = 0, excess = 0;
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

    return ((c1 * excess) / N + (c2 * excess) / N + (c3 * weight_difference));
}

std::vector<double> genome::feed_forward(std::vector<double> inputs) {
    if(inputs.size() != input_nodes.size()) {
        std::cout << "invalid input size" << std::endl;
        throw("error");
    }

    for(size_t i = 0; i < input_nodes.size(); i++) {
        input_nodes.at(i).set_value(inputs.at(i));
    }

    for(size_t i = 0; i < hidden_nodes.size(); i++) {
        int new_value = 0;
        gene_node *current_node = &hidden_nodes.at(i);
        for(size_t j = 0; j < connections.size(); j++) {
            if(connections.at(j).get_enabled() && connections.at(j).get_to() == current_node) { //todo make sure this works, otherwise use ids
                new_value += connections.at(j).get_weight() * connections.at(j).get_to()->get_value();
            }
        }
        current_node->set_value(new_value);
    }
    
    for(size_t i = 0; i < output_nodes.size(); i++) {
        int new_value = 0;
        gene_node *current_node = &output_nodes.at(i);
        for(size_t j = 0; j < connections.size(); j++) {
            if(connections.at(j).get_enabled() && connections.at(j).get_to() == current_node) { //todo make sure this works, otherwise use ids
                new_value += connections.at(j).get_weight() * connections.at(j).get_to()->get_value();
            }
        }
        current_node->set_value(new_value);
    }

    std::vector<double> output;
    for(size_t i = 0; i < output_nodes.size(); i++) {
        output.push_back(output_nodes.at(i).get_value());
    }

    return output;
}

genome genome::crossover(genome &g2) { //assuming g1.fitness > g2.fitness
    genome g1 = *this;
    size_t g1_index, g2_index;
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

    for(size_t i = g1_index; i < g1.connections.size(); i++) {
        new_connections.push_back(g1.connections.at(i));
    }

    genome new_genome(*this);
    new_genome.connections = new_connections;
    return new_genome; //todo
}

void genome::mutate(std::vector<gene_node> &all_nodes, std::vector<std::pair<int, int>> &connection_pairs) {
    // std::cout << "mutating" << std::endl;
    if(((double) rand() / (RAND_MAX)) < MUTATE_PROBABILITY) {
        // std::cout << "1" << std::endl;
        mutate_link(connection_pairs);
    }
    if(((double) rand() / (RAND_MAX)) < MUTATE_PROBABILITY) {
        // std::cout << "2" << std::endl;
        mutate_node(all_nodes, connection_pairs);
    }
    if(((double) rand() / (RAND_MAX)) < MUTATE_PROBABILITY) {
        // std::cout << "3" << std::endl;
        mutate_weight_shift();
    }
    if(((double) rand() / (RAND_MAX)) < MUTATE_PROBABILITY) {
        // std::cout << "4" << std::endl;
        mutate_weight_random();
    }
    if(((double) rand() / (RAND_MAX)) < MUTATE_PROBABILITY) {
        // std::cout << "5" << std::endl;
        mutate_link_toggle();
    }
}

void genome::mutate_link(std::vector<std::pair<int, int>> &connection_pairs) {
    std::vector<gene_node> all_nodes;
    all_nodes.insert(all_nodes.end(), input_nodes.begin(), input_nodes.end());
    all_nodes.insert(all_nodes.end(), hidden_nodes.begin(), hidden_nodes.end());
    all_nodes.insert(all_nodes.end(), output_nodes.begin(), output_nodes.end());
    for(int i = 0; i < 50; i++) { //we allow up to 50 failed attempt before continuing
        gene_node *a = &all_nodes.at(rand() % all_nodes.size());
        gene_node *b = &all_nodes.at(rand() % all_nodes.size());

        if(a->get_position() == b->get_position()) {
            continue;
        }

        if(a->get_value() > b->get_value()) { //smaller value must come first to avoid recursion
            gene_node *temp = a;
            a = b;
            b = temp;
        }

        std::pair<int, int> temp(a->get_id(), b->get_id());
        size_t connection_id = connection_pairs.size();
        int insert_index = connections.size();

        for(size_t j = 0; j < connection_pairs.size(); j++) {
            if(connection_pairs.at(j) == temp) {
                connection_id = j;
                break;
            }
        }

        if(connection_id == connection_pairs.size()) {
            connection_pairs.push_back(temp);
        } else {
            for(size_t j = 0; j < connections.size(); j++) { //we must insert it in order of id, otherwise crossover and difference methods will break
                if(connections.at(j).get_id() > (int) connection_id) {
                    insert_index = j;
                    break;
                }
            }
        }

        // gene_connection new_connection(connection_id, a, b, (((double) rand() / (RAND_MAX)) * 2 - 1) * RANDOM_FACTOR);
        connections.insert(connections.begin() + insert_index, gene_connection(connection_id, a, b, (((double) rand() / (RAND_MAX)) * 2 - 1) * RANDOM_FACTOR));
        break;
    }
}

void genome::mutate_node(std::vector<gene_node> &all_nodes, std::vector<std::pair<int, int>> &connection_pairs) {
    if(connections.empty()) {return;}
    gene_connection connection = connections.at(rand() % connections.size());
    connection.set_enabled(false);
    gene_node *from = connection.get_from();
    gene_node *to = connection.get_to();
    gene_node *middle = new gene_node((from->get_value() + to->get_value()) / 2, node_type::hidden);
    all_nodes.push_back(*middle);

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