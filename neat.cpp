#include <iostream>
#include <algorithm>
#include "neat.h"


neat::neat(int inputs, int outputs, int generations, int population_size, double (*fitness)(std::vector<double> (*func)(std::vector<double>))) {
    fitness_func = fitness;
    reset(inputs, outputs, population_size);
    for(int i = 0; i < generations; i++) {
        std::cout << "Generation " << i << ", max fitness of " << max_fitness << std::endl;
        generation(population_size);
    }
}


void neat::reset(int inputs, int outputs, int population_size) {
    nodes.clear();
    connections.clear();
    max_fitness = 0;
    for(int i = 0; i < inputs; i++) {
        nodes.push_back(gene_node(0, node_type::input));
    }
    for(int i = 0; i < outputs; i++) {
        nodes.push_back(gene_node(1, node_type::output));
    }
    for(int i = 0; i < population_size; i++) {
        pool.push_back(genome(nodes, std::vector<gene_connection>()));
    }
}


bool compare_fitness(genome g1, genome g2) {
    return g1.get_fitness() > g2.get_fitness();
}

void neat::generation(int population_size) {
    #define DIFFERENCE_FACTOR 1 //todo change to real value
    for(size_t i = 0; i < pool.size(); i++) {
        //todo calculate fitness
        pool.at(i).set_fitness(fitness_func(pool.at(i).feed_forward));
        if(pool.at(i).get_fitness() > max_fitness) {
            max_fitness = pool.at(i).get_fitness();
        }
    }

    //seperate similar species into arrays
    std::vector<std::vector<genome>> species;
    for(size_t i = 0; i < pool.size(); i++) {
        bool placed = false;
        for(size_t j = 0; j < species.size(); j++) { //for all of the different species (currently)
            for (size_t k = 0; k < species.at(j).size(); k++) { //for all of the genomes in a specific species
                if(pool.at(i).difference(species.at(j).at(k)) < DIFFERENCE_FACTOR) { //if it is similar, add it
                    species.at(j).push_back(pool.at(i));
                    placed = true;
                    break;
                }
            }
            if(placed) {
                break;
            }
        }
        if(!placed) { //if it is not similar to any other species, create a new one
            species.push_back(std::vector<genome>(1, pool.at(i)));
        }
    }

    for(size_t i = 0; i < species.size(); i++) { //sorts speices array by fitness, highest fitness at index 0
        std::sort(species.at(i).begin(), species.at(i).end(), compare_fitness);
        for (size_t j = 0; j < species.at(i).size() / 2; j++) { //delete lowest fitness 50% of the species (roughly)
            species.at(i).pop_back();                        //todo make it so it removes based on probability, not always bottom 50%
        }
    }

    //crossover
    pool.clear();
    while(pool.size() < 100) {
        std::vector<genome> selected = species.at(rand() % species.size());
        if(selected.size() > 1) {
            int first_index = rand() % selected.size(), second_index = rand() % selected.size();
            if(first_index != second_index) {
                genome new_genome = selected.at(first_index).crossover(selected.at(second_index));
                new_genome.mutate(nodes, connections); //mutation
                pool.push_back(new_genome);
            }
        }
    }
    
}