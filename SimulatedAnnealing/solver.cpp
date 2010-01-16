/*
 *  solver.cpp
 *  optimization
 *
 *  Created by Corey M. Hoffstein on 1/15/10.
 *  Copyright 2010 Newfound Research, LLC. All rights reserved.
 *
 */

#include "solver.h"

#include <cmath>
#include <iostream>

namespace Optimization {
	namespace SimulatedAnnealing {
		std::pair<double, state_ptr> solve(state_ptr initial_state, 
				   double temperature = 10.0, 
				   double cost = 0.999,
				   int max_iterations = 10000,
				   int step_size = 10,
				   bool verbose = true) {
			
			//initialize our random number generator
			srand(time(NULL));
			
			state_ptr current_state = initial_state;
			double current_energy = initial_state->get_energy();
			
			state_ptr best_state = initial_state;
			double best_energy = initial_state->get_energy();
			
			int current_iteration = 0;
			while(current_iteration < max_iterations) {
				state_ptr new_state = current_state->get_random_neighbor();
				double new_energy = new_state->get_energy();
				
				if (new_energy < best_energy) {
					
					 if(verbose) {
						 std::cout << "Iteration " << current_iteration << ": ";
						 std::cout << new_energy << std::endl;
						 new_state->pretty_print();
					 }
					
					best_state = new_state;
					best_energy = new_energy;
				}
				
				float r = rand() / static_cast<float>(RAND_MAX);
				if(exp(-(new_energy-current_energy)/temperature) > r) {
					current_state = new_state;
					current_energy = new_energy;
				}
				   
				if(current_iteration % step_size == 0)
					temperature = temperature * cost;
				
				current_iteration++;
			}
			
			return std::make_pair(best_energy, best_state);
		}
	}
}
