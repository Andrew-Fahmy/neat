#include "genome.h"

class neat {
private:
    double max_fitness;
    std::vector<gene_node> nodes;
    std::vector<std::pair<int, int>> connections;
    std::vector<genome> pool;
    double (*fitness_func)(genome g);//std::vector<double> (*func)(std::vector<double>));
    

    void reset(int inputs, int outputs, int population_size);
    void generation(int population_size);

public:
    neat(int inputs, int outputs, int generations, int population_size, double (*fitness)(genome g));//(std::vector<double> (*func)(std::vector<double>)));
};