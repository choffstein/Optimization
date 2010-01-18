/*
 *  colony.cpp
 *  Fastest Path
 *
 *  Created by Corey M. Hoffstein on 1/18/10.
 *  Copyright 2010 Newfound Research, LLC. All rights reserved.
 *
 */

#include "colony.h"

#include "ant.h"

#include <boost/foreach.hpp>

namespace Optimization {
	namespace AntColony {
		void simulate(int num_ants, node_ptr origin, const std::vector<path_ptr>& paths, int steps, 
							double evaporation_rate, double pheromone_influence, double path_cost_influence,
							double desirability_influence) {
			std::vector<ant_ptr> ants;
			for(int i = 0; i < num_ants; i++) {
				ants.push_back(ant_ptr(new Ant(origin)));
			}
			
			for(int current_step = 0; current_step < steps; current_step++) {
				BOOST_FOREACH(ant_ptr ant, ants) {
					ant->update(pheromone_influence, path_cost_influence, desirability_influence, false);
				}
				
				BOOST_FOREACH(path_ptr path, paths) {
					path->update(evaporation_rate);
				}
			}
		}
	}
}