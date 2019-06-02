#include "genome.h"

class neat {
private:
    std::vector<gene_node*> nodes;
    std::vector<std::pair<int, int>> connections;
    std::vector<genome> genomes;

public:
    neat(int inputs, int outputs, int generations, int genomes);
    void reset(int inputs, int outputs, int population_size);
    void generation(int population_size);
    void selection();
};