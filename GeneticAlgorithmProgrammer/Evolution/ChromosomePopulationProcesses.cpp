//
//  ChromosomePopulationProcesses.cpp
//  GeneticAlgorithmProgrammer
//
//  Created by Rohan Kapur on 28/10/15.
//  Copyright © 2015 Rohan Kapur. All rights reserved.
//

#include "ChromosomePopulationProcesses.hpp"
#include "Constants.h"

#include <cstdlib>
#include <iostream>

#pragma mark - Evolution Methods

Chromosome ChromosomePopulationProcesses::generateRandomChromosome(std::string charset, int preferredLength) {
    std::string genome;
    int charsetLength = charset.length();
    for (int i = 0; i < preferredLength; i++)
        genome.push_back(charset[rand() % charsetLength]);
    return Chromosome(genome);
}

std::vector<Chromosome> ChromosomePopulationProcesses::mateChromosomes(std::string charset, Chromosome chromosome1, Chromosome chromosome2) {
    std::vector<Chromosome> children;
    std::string parentGenome1 = chromosome1.genome;
    std::string parentGenome2 = chromosome2.genome;
    // Crossover
    std::string childGenome1 = parentGenome1.substr(0, (parentGenome1.size() / 2)).append(parentGenome2.substr(parentGenome2.size() / 2, parentGenome2.size() - 1));
    std::string childGenome2 = parentGenome2.substr(0, (parentGenome2.size() / 2)).append(parentGenome1.substr(parentGenome1.size() / 2, parentGenome1.size() - 1));
    children.push_back(Chromosome(childGenome1));
    children.push_back(Chromosome(childGenome2));
    return children;
}

Chromosome ChromosomePopulationProcesses::randomlyMutateChromosome(std::string charset, Chromosome chromosome) {
    int charsetLength = charset.length();
    double randomState = ((double)rand()/(RAND_MAX));
    if (randomState > constants::POPULATION_POOL_MUTATE_PROBABILITY)
        return chromosome; // Random state says no to mutation
    randomState = ((double)rand()/(RAND_MAX)); // Reassign for next random interactions
    std::string genome = chromosome.genome;
    // TODO: Look over the accuracy of this code, and look over if addition/subtraction is a good idea
    if (randomState <= constants::POPULATION_POOL_MUTATE_ADDITION_PROBABILITY) // Add a character
        genome.push_back(charset[rand() % charsetLength]); // Correct ASCII range
    else if (randomState <= constants::POPULATION_POOL_MUTATE_REPETITION_PROBABILITY + constants::POPULATION_POOL_MUTATE_ADDITION_PROBABILITY) { // Repetition of a character
        if (genome.size() >= 3)
            genome.insert(genome.begin() + rand() % genome.size(), genome[rand() % genome.size()]); // Mutation-by-repetition replicates a random character in the string and adds it at a random position in the genome
    }
    else if (randomState <= constants::POPULATION_POOL_MUTATE_SUBTRACTION_PROBABILITY + constants::POPULATION_POOL_MUTATE_ADDITION_PROBABILITY + constants::POPULATION_POOL_MUTATE_REPETITION_PROBABILITY && genome.size()) { // Remove a character
        genome.erase(rand() % genome.size(), 1);
    }
    else if (randomState <= constants::POPULATION_POOL_MUTATE_MODIFICATION_PROBABILITY + constants::POPULATION_POOL_MUTATE_SUBTRACTION_PROBABILITY + constants::POPULATION_POOL_MUTATE_ADDITION_PROBABILITY + constants::POPULATION_POOL_MUTATE_REPETITION_PROBABILITY && genome.size()) { // Modify a character
        int randomIndex = rand() % genome.size();
        genome[randomIndex] = charset[rand() % charsetLength];
    }
    else if (randomState <= constants::POPULATION_POOL_MUTATE_MODIFICATION_PROBABILITY + constants::POPULATION_POOL_MUTATE_SUBTRACTION_PROBABILITY + constants::POPULATION_POOL_MUTATE_ADDITION_PROBABILITY + constants::POPULATION_POOL_MUTATE_REPETITION_PROBABILITY + constants::POPULATION_POOL_MUTATE_INSERTION_PROBABILITY && genome.size()) { // Insert a character
        int randomIndex = rand() % genome.size();
        std::string addition = "";
        addition.push_back((char)charset[rand() % charsetLength]);
        genome.insert(randomIndex, addition);
    }
    else if (randomState <= constants::POPULATION_POOL_MUTATE_MODIFICATION_PROBABILITY + constants::POPULATION_POOL_MUTATE_SUBTRACTION_PROBABILITY + constants::POPULATION_POOL_MUTATE_ADDITION_PROBABILITY + constants::POPULATION_POOL_MUTATE_REPETITION_PROBABILITY + constants::POPULATION_POOL_MUTATE_INSERTION_PROBABILITY + constants::POPULATION_POOL_MUTATE_SWAP_PROBABILITY && genome.size()) { // Swap two characters
        int randomIndex = rand() % genome.size();
        int randomIndex2 = rand() % genome.size();
        char tempOne = genome[randomIndex];
        char tempTwo = genome[randomIndex2];
        genome[randomIndex] = tempTwo;
        genome[randomIndex2] = tempOne;
    }
    else if (randomState >= constants::POPULATION_POOL_MUTATE_MODIFICATION_PROBABILITY + constants::POPULATION_POOL_MUTATE_SUBTRACTION_PROBABILITY + constants::POPULATION_POOL_MUTATE_ADDITION_PROBABILITY + constants::POPULATION_POOL_MUTATE_REPETITION_PROBABILITY + constants::POPULATION_POOL_MUTATE_INSERTION_PROBABILITY + constants::POPULATION_POOL_MUTATE_REPLACEMENT_PROBABILITY + constants::POPULATION_POOL_MUTATE_SWAP_PROBABILITY && genome.size()) { // Replace a character
        int randomIndex = rand() % genome.size();
        int randomIndex2 = rand() % genome.size();
        genome[randomIndex] = genome[randomIndex2];
    }
    chromosome.genome = genome;
    return chromosome;
}