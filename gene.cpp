#include "gene.h"

gene::gene() : id(-1) {};
gene::gene(int i) : id(i) {};
gene_node::gene_node(double v) : gene(counter++), value(v) {};
gene_connection::gene_connection(gene_node *f, gene_node *t) : gene(), from(f), to(t), weight(0), enabled(false) {}
gene_connection::gene_connection(gene_node *f, gene_node *t, double w) : gene(counter++), from(f), to(t), weight(w), enabled(true) {}
gene_connection::gene_connection(int i, gene_node *f, gene_node *t, double w) : gene(i), from(f), to(t), weight(w), enabled(true) {}


int gene::get_id() const {
    return id;
}

double gene_node::get_value() {
    return value;
}

bool gene_connection::operator==(gene_connection &other) {
    return from == other.from && to == other.to;
}

double gene_connection::get_weight() {
    return weight;
}

bool gene_connection::get_enabled() {
    return enabled;
}

void gene_connection::set_weight(double w) {
    weight = w;
}

void gene_connection::set_enabled(bool e) {
    enabled = e;
}