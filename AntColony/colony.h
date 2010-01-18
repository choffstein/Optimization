/*
 *  colony.h
 *  Fastest Path
 *
 *  Created by Corey M. Hoffstein on 1/18/10.
 *  Copyright 2010 Newfound Research, LLC. All rights reserved.
 *
 */

#ifndef __COLONY_H__
#define __COLONY_H__

#include "node.h"
#include "path.h"

namespace Optimization {
	namespace AntColony {
		void simulate(int num_ants, node_ptr origin, const std::vector<path_ptr>& paths, int steps, 
							 double evaporation_rate, double pheromone_influence, double path_cost_influence,
						     double desirability_influence);
	}
}

#endif
