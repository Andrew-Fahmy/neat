#include <algorithm>
#include "neat.h"


neat::neat(int inputs, int outputs, int generations, int population_size) {
    reset(inputs, outputs, population_size);
    for(int i = 0; i < generations; ++i) {
        generation(population_size);
    }
}


void neat::reset(int inputs, int outputs, int population_size) {
    nodes.clear();
    connections.clear();
    for(int i = 0; i < inputs; ++i) {
        nodes.push_back(new gene_node(0));
    }
    for(int i = 0; i < outputs; ++i) {
        nodes.push_back(new gene_node(1));
    }
    for(int i = 0; i < genomes.size(); ++i) {
        genomes.push_back(genome(nodes, std::vector<gene_connection>()));
    }
}


void neat::generation(int population_size) {
    selection();
    crossover();
    mutate();
}


bool compare_fitness(genome g1, genome g2) {
    return g1.get_fitness() > g2.get_fitness();
}

void neat::selection() {
    #define DIFFERENCE_FACTOR 1 //todo change to real value
    for(int i = 0; i < genomes.size(); i++) {
        //todo calculate fitness
        genomes.at(i).calculate_fitness();
    }

    //seperate similar species into arrays
    std::vector<std::vector<genome>> species;
    for(int i = 0; i < genomes.size(); i++) {
        bool species_placed = false;
        for(int j = 0; j < species.size(); j++) { //for all of the different species (currently)
            for (int k = 0; k < species.at(j).size(); k++) { //for all of the genomes in a specific species
                if(genomes.at(i).difference(species.at(j).at(k)) < DIFFERENCE_FACTOR) { //if it is similar, add it
                    species.at(j).push_back(genomes.at(i));
                    species_placed = true;
                    break;
                }
            }
            if(species_placed) {
                break;
            }
        }
        if(!species_placed) { //if it is not similar to any other species, create a new one
            species.push_back(std::vector<genome>(1, genomes.at(i)));
        }
    }

    for(int i = 0; i < species.size(); i++) { //sorts speices array by fitness, height fitness at index 0
        std::sort(species.at(i).begin(), species.at(i).end(), compare_fitness);
        for (int j = 0; j < species.at(i).size() / 2; j++) { //delete lowest fitness 50% of the species (roughly)
            species.at(i).pop_back();                        //todo make it so it removes based on percent, not always bottom 50%
        }
    }
}