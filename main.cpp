#include <iostream>
#include "neat.h"

double calc_fitness(std::vector<double> (*func)(std::vector<double>)) {
    std::vector<double> input;
    input.push_back(1.0);
    input.push_back(0.0);
    std::vector<double> output;
    output = func(input);
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
    neat n(2, 1, 100, 100, calc_fitness);
    return 0;
}