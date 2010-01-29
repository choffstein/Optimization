/*
 *  fit_sentence.cpp
 *  
 *
 *  Created by Corey M. Hoffstein on 1/16/10.
 *  Copyright 2010 Newfound Research, LLC. All rights reserved.
 *
 */

//GENETIC ALGORITHM WITH FITTING A RANDOM SENTENCE

#include <iostream>
#include <cmath>

#include <cstdlib> 
#include <ctime>

#include <boost/bind.hpp>

#include "population.h"
#include "evolvable.h"

class Char : public Optimization::GeneticAlgorithm::Evolvable {
public:
	Char() { _c = static_cast<char>(32.0 + 91.0 * rand() / static_cast<float>(RAND_MAX)); }
	void mutate() { _c = static_cast<double>(32.0 + 91.0 * rand() / static_cast<float>(RAND_MAX)); }
	char value() const { return _c; }
	double distance(const Char& other) const {
		return value() - other.value();
	}
	
private: 
	char _c;
};

// some random fitness function
float fitness_function(const std::string& sentence, const Optimization::GeneticAlgorithm::Chromosome<Char>::dna_vector& input) {
	int sum = sentence.length();
	for(int i = 0; i < input.size(); i++) {
		if(input[i].value() == sentence[i])
			sum = sum - 1;
	}
	
	return sum / static_cast<float>(sentence.length());
}

int main (int argc, char * const argv[]) {
	srand ( time(NULL) );
	
	if(argc != 2) { 
		std::cout << "Usage: ./Fit Sentence <sentence>" << std::endl;
		exit(0);
	}
	
	std::string sentence(argv[1]);
	
	Optimization::GeneticAlgorithm::Population<Char> population(1000, sentence.length());
	std::pair<float, Optimization::GeneticAlgorithm::Chromosome<Char>::dna_vector> result = population.evolve(boost::bind(&fitness_function, sentence, _1), 0.05);
	
	std::cout << result.first << ": ";
	for(int i = 0; i < result.second.size(); i++) {
		std::cout << result.second[i].value();
	}
	std::cout << std::endl;
    return 0;
}