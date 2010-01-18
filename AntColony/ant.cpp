/*
 *  ant.cpp
 *  Fastest Path
 *
 *  Created by Corey M. Hoffstein on 1/18/10.
 *  Copyright 2010 Newfound Research, LLC. All rights reserved.
 *
 */

#include <iostream>

#include "ant.h"

namespace Optimization {
	namespace AntColony {
		Ant::Ant(node_ptr origin) :
			_current_location(origin) {}
		
		Ant::~Ant() {}
		
		void Ant::update(double pheromone_influence, double path_cost_influence, double desirability_influence, bool verbose=false) {
			std::vector<path_ptr> neighbor_paths = _current_location->get_connections();
			
			double sum = 0.0;
			BOOST_FOREACH(path_ptr p, neighbor_paths) {
				double pheromone_factor = pow(p->get_pheromones(), pheromone_influence);
				double cost_factor = pow(1.0 / p->get_cost(), path_cost_influence);
				double desirability_factor = pow(p->get_target()->get_desirability(), desirability_influence);
				
				sum = sum +  pheromone_factor * cost_factor * desirability_factor;
			}
			
			float r = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
			
			double running = 0.0;
			path_ptr next_path;
			
			BOOST_FOREACH(path_ptr p, neighbor_paths) {
				double pheromone_factor = pow(p->get_pheromones(), pheromone_influence);
				double cost_factor = pow(1.0 / p->get_cost(), path_cost_influence);
				double desirability_factor = pow(p->get_target()->get_desirability(), desirability_influence);
				
				running = running + pheromone_factor * cost_factor * desirability_factor;
				double prob = running / sum;
				
				if (r <= prob) {
					next_path = p;
					break;
				}
			}
			
			if(next_path) {
				if(verbose) {
					std::cout << "--" << next_path->get_cost() << "-->";
				}
				
				next_path->add_pheromones();
				_current_location = next_path->get_target();
			}
		}
		
		node_ptr Ant::get_location() const {
			return _current_location;
		}
	}
}