#include <iostream>
#include "neat.h"

double calc_fitness(genome g) {//std::vector<double> (genome::*func)(std::vector<double>)) {
    std::vector<double> input;
    input.push_back(1.0);
    input.push_back(0.1);
    std::vector<double> output;
    output = g.feed_forward(input);
    if(output.size() != 1) {
        std::cout << "output size error" << std::endl;
    }
    if(output.at(0) >= 1) {
        return 2;
    } else {
        return 1.5 * output.at(0);
    }
}

int main() {
    neat n(2, 1, 10, 10, calc_fitness);

    return 0;
}