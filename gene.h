#ifndef NEAT_GENE_H
#define NEAT_GENE_H

enum node_type {
    input, hidden, output
};

class gene {
protected:
    int id;
    gene(int _id);

public:
    int get_id();
};

class gene_node : public gene {
private:
    int counter;
    double value, position; //position represents location to prevent recursion, from 0-1
    node_type type;

public:
    gene_node(double _position, node_type _type);

    double get_value();
    double get_position();
    node_type get_type();
    void set_value(double _value);
};

class gene_connection : public gene {
private:
    gene_node *from, *to;
    double weight;
    bool enabled;

public:
    gene_connection(int _input, gene_node *_from, gene_node *_to, double _weight);

    double get_weight();
    bool get_enabled();
    gene_node* get_from();
    gene_node* get_to();
    void set_weight(double _weight);
    void set_enabled(bool _enabled);
};


#endif