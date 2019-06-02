#ifndef NEAT_GENE_H
#define NEAT_GENE_H


class gene {
protected:
    const int id;
    gene();
    gene(int i);

public:
    int get_id() const;
};

class gene_node : public gene {
private:
    int counter;
    double value; // represents location to prevent recursion, from 0-1

public:
    gene_node(double v);
    double get_value();
    // gene_node(int i, double v) : gene(i), value(v) {};
};

class gene_connection : public gene {
private:
    int counter;
    const gene_node *from, *to;
    double weight;
    bool enabled;

public:
    gene_connection(gene_node *f, gene_node *t);
    gene_connection(gene_node *f, gene_node *t, double w);
    gene_connection(int i, gene_node *f, gene_node *t, double w);
    bool operator==(gene_connection &other);
    double get_weight();
    bool get_enabled();
    void set_weight(double w);
    void set_enabled(bool e);
};


#endif