/*
 *  implied_volatility.cpp
 *  
 *
 *  Created by Corey M. Hoffstein on 1/16/10.
 *  Copyright 2010 Newfound Research, LLC. All rights reserved.
 *
 */

//PARTICLE SWARM OPTIMIZATION TO FIND IMPLIED VOLATILITY OF OPTION

#include <iostream>
#include <cmath>

#include <cstdlib> 
#include <ctime>

#include "swarm.h"

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

double S = 100;
double K = 85;
double pv = 0.986344099;
double T = 0.25;
double P = 17.0;

// some random fitness function
float fitness_function(const Optimization::ParticleSwarm::feature_vector& input) {
	double h = (log(S/(K*pv))/(input[0]*sqrt(T)))+(0.5)*input[0]*sqrt(T);
	double price = S*normdist(h)-K*pv*normdist(h-input[0]*sqrt(T));
	
	return pow(price - P, 2);
}

int main (int argc, char * const argv[]) {
	srand ( time(NULL) );
	
	Optimization::ParticleSwarm::feature_limits_type feature_limits;
	feature_limits.push_back(std::pair<double, double>(0.0, 100.0));
	
	std::pair<float, Optimization::ParticleSwarm::feature_vector> result = \
	Optimization::ParticleSwarm::optimize(&fitness_function,
										  1,
										  feature_limits,
										  10000,
										  1000,
										  1,
										  2,
										  2);
	
	std::cout << result.first << ": ";
	for(int i = 0; i < result.second.size(); i++) {
		std::cout << result.second[i];
	}
	std::cout << std::endl;
    return 0;
}

