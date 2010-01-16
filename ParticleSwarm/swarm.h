/*
 *  swarm.h
 *  optimization
 *
 *  Created by Corey M. Hoffstein on 1/15/10.
 *  Copyright 2010 Newfound Research, LLC. All rights reserved.
 *
 */

#ifndef __SWARM_H__
#define __SWARM_H__

#include "fly.h"

#include <vector>

#include <boost/smart_ptr.hpp>
#include <boost/function.hpp>

namespace Optimization {
	namespace ParticleSwarm {
		std::pair<float, feature_vector> optimize(boost::function<float (const feature_vector&)> fitness_function,
												  int feature_dimensions,
												  const feature_limits_type& feature_limits,
												  int swarm_size, 
												  int max_iterations,
												  double inertia,
												  double cognitive,
												  double social);
	
		std::pair<float, fly_ptr > apply_fitness(boost::function<float (const feature_vector&)> fitness_function, 
												 fly_ptr fly);
		
		// our comparator for fitness / chromosome pairs
		struct fitness_comparator {
			bool operator()(std::pair<float, fly_ptr > first,
							std::pair<float, fly_ptr > second) {
				return (first.first < second.first);
			}
		};
		
	
	}
}

#endif