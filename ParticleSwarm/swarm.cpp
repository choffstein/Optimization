/*
 *  swarm.cpp
 *  optimization
 *
 *  Created by Corey M. Hoffstein on 1/15/10.
 *  Copyright 2010 Newfound Research, LLC. All rights reserved.
 *
 */

#include "swarm.h"
#include <boost/bind.hpp>
#include <boost/foreach.hpp>

#include <iostream>

namespace Optimization {
	namespace ParticleSwarm {
		std::pair<float, feature_vector> optimize(boost::function<float (const feature_vector&)> fitness_function,
												  int feature_dimensions,
												  const feature_limits_type& feature_limits,
												  int swarm_size, 
												  int max_iterations,
												  double inertia,
												  double cognitive,
												  double social,
												  bool verbose=true) {
			
			
			std::vector<fly_ptr> flies;
			for(int i = 0; i < swarm_size; i++) {
				flies.push_back(fly_ptr(new Fly(feature_dimensions, feature_limits, inertia, cognitive, social)));
			}
			
			// initialize absolutely random global best features
			feature_vector global_best_features(feature_dimensions);
			feature_limits_type::const_iterator it = feature_limits.begin();
			for(int i = 0; it != feature_limits.end(); it++, i++) {
				float r = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
				global_best_features[i] = (*it).first + r * ((*it).second - (*it).first);
			}
			
			double global_best_fitness = std::numeric_limits<float>::infinity();
			
			int current_iteration = 0;
			while(current_iteration < max_iterations) {
				
				BOOST_FOREACH( fly_ptr f, flies ) {
					f->update_position(global_best_features);
				}
		
				
				std::vector<std::pair<float, fly_ptr> > population_fitness(swarm_size);
				
				// transform our population and create an array with fitnesses
				std::transform(flies.begin(), flies.end(), 
							   population_fitness.begin(), boost::bind(&apply_fitness, fitness_function, _1));
				
				// sort our population by their fitnesses
				std::sort(population_fitness.begin(), population_fitness.end(), fitness_comparator());
				
				if(population_fitness[0].first < global_best_fitness) {
					global_best_fitness = population_fitness[0].first;
					feature_vector t = population_fitness[0].second->get_features();
					std::copy(t.begin(), t.end(), global_best_features.begin());
				
					if(verbose) {
						std::cout << current_iteration << ": ";
						std::cout << global_best_fitness << ": ";
						for(int i = 0; i < global_best_features.size(); i++) {
							std::cout << global_best_features[i];
						}
						std::cout << std::endl;
					}
				}
				
				current_iteration++;
			}
			
			return std::make_pair(global_best_fitness, global_best_features);
		}
		
		std::pair<float, fly_ptr > apply_fitness(boost::function<float (const feature_vector&)> fitness_function, 
												 fly_ptr fly) {
			fly->update_fitness(fitness_function);
			float fitness = fly->get_fitness();
			
			return std::make_pair(fitness, fly);
		}
	}
}