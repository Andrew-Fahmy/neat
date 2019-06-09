#include <iostream>
#include <algorithm>
#include "network.h"


node::node(gene_node *_node) : n(_node) {}

bool node::operator>(const node &other) {
    return n->get_value() > other.n->get_value();
}


network::network(std::vector<gene_node*> nodes, std::vector<gene_connection> connections) {
    for(size_t i = 0; i < nodes.size(); i++) {
        if(nodes.at(i)->get_type() == node_type::input) {
            input_nodes.push_back(node(nodes.at(i)));
        } else if(nodes.at(i)->get_type() == node_type::hidden) {
            hidden_nodes.push_back(node(nodes.at(i)));
        } else {
            output_nodes.push_back(node(nodes.at(i)));
        }
    }
    
    for(size_t i = 0; i < connections.size(); i++) {
        bool found = false;
        for(size_t j = 0; j < nodes.size(); j++) {
            if(connections.at(i).get_to() == nodes.at(j).n) {
                nodes.at(j).inputs.push_back(connections.at(i));
                found = true;
                break;
            }
        }
        if(!found) {
           std::cout << "unexpected error" << std::endl;
        }
    }
    std::sort(hidden_nodes.begin(), hidden_nodes.end(), sort_nodes);
}

std::vector<double> network::feed_forward(std::vector<double> inputs) {
    for(size_t i = 0; i < hidden_nodes.size(); i++) {
       
    }
}

bool sort_nodes(node n1, node n2) {
    return n1.n->get_value() < n2.n->get_value();
}
