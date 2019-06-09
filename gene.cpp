#include "gene.h"

gene::gene(int _id) : id(_id) {};
gene_node::gene_node(double _position, node_type _type) : gene(counter++), value(0), position(_position), type(_type) {}
gene_connection::gene_connection(int _id, gene_node *_from, gene_node *_to, double _weight) : gene(_id), from(_from), to(_to), weight(_weight), enabled(true) {}


int gene::get_id() {
    return id;
}

double gene_node::get_value() {
    return value;
}

double gene_node::get_position() {
    return position;
}

node_type gene_node::get_type() {
    return type;
}

void gene_node::set_value(double _value) {
    value = _value;
}


double gene_connection::get_weight() {
    return weight;
}

bool gene_connection::get_enabled() {
    return enabled;
}

gene_node* gene_connection::get_from() {
    return from;
}
gene_node* gene_connection::get_to() {
    return to;
}

void gene_connection::set_weight(double _weight) {
    weight = _weight;
}

void gene_connection::set_enabled(bool _enabled) {
    enabled = _enabled;
}