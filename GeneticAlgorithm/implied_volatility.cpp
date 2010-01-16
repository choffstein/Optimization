/*
 *  price_option.cpp
 *  
 *
 *  Created by Corey M. Hoffstein on 1/16/10.
 *  Copyright 2010 Newfound Research, LLC. All rights reserved.
 *
 */


//GENETIC ALGORITHM TO FIND IMPLIED VOLATILITY OF OPTION

#include <iostream>
#include <cmath>

#include <cstdlib> 
#include <ctime>

#include "population.h"
#include "evolvable.h"

class Double : public Optimization::GeneticAlgorithm::Evolvable {
public:
	Double() { _d = static_cast<double>(100.0 * static_cast<float>(rand()) / static_cast<float>(RAND_MAX)); }
	void mutate() { _d = static_cast<double>(100.0 * static_cast<float>(rand()) / static_cast<float>(RAND_MAX)); }
	double value() const { return _d; }
	double distance(const Double& other) const {
		return value() - other.value();
	}
	
private: 
	double _d;
};


double S = 100;
double K = 85;
double pv = 0.986344099;
double T = 0.25;
double P = 17.0;

double normdist(double xi, double mean=0, double standard_dev=1)
{
    double res;
    double x=(xi - mean) / standard_dev;
    if (x == 0)
    {
        res=0.5;
    }
    else
    {
        double oor2pi = 1/(sqrt(double(2) * 3.14159265358979323846));
        double t = 1 / (double(1) + 0.2316419 * fabs(x));
        t *= oor2pi * exp(-0.5 * x * x) 
		* (0.31938153   + t 
		   * (-0.356563782 + t
			  * (1.781477937  + t 
				 * (-1.821255978 + t * 1.330274429))));
        if (x >= 0)
        {
            res = double(1) - t;
        }
        else
        {
            res = t;
        }
    }
    return res;
}

// some random fitness function
float fitness_function(const Optimization::GeneticAlgorithm::Chromosome<Double>::dna_vector& input) {
	double h = (log(S/(K*pv))/(input[0].value()*sqrt(T)))+(0.5)*input[0].value()*sqrt(T);
	return pow(S*normdist(h)-K*pv*normdist(h-input[0].value()*sqrt(T)) - P, 2);
}

int main (int argc, char * const argv[]) {
	srand ( time(NULL) );
	
	Optimization::GeneticAlgorithm::Population<Double> population(10000, 1);
	std::pair<float, Optimization::GeneticAlgorithm::Chromosome<Double>::dna_vector> result = population.evolve(&fitness_function, 0.000001);
	
	std::cout << result.first << ": ";
	for(int i = 0; i < result.second.size(); i++) {
		std::cout << result.second[i].value();
	}
	std::cout << std::endl;
    return 0;
}
