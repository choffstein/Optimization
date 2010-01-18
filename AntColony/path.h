/*
 *  path.h
 *  optimization
 *
 *  Created by Corey M. Hoffstein on 1/14/10.
 *  Copyright 2010 Newfound Research, LLC. All rights reserved.
 *
 */

#ifndef __PATH_H__
#define __PATH_H__

#include "node.h"

#include <boost/shared_ptr.hpp>

namespace Optimization {
	namespace AntColony {
		class Node;
		typedef boost::shared_ptr<Node> node_ptr;
		
		class Path {
			
		public:
			Path(node_ptr target, double cost, double initial_pheromones) :
				_target(target),
				_cost(cost),
				_pheromones(initial_pheromones),
				_new_pheromones(0.0) {
			}
			
			~Path() {}
			
			inline node_ptr get_target() const {
				return _target;
			}
			
			inline double get_pheromones() const {
				return _pheromones;
			}
			
			inline void add_pheromones() {
				_new_pheromones = _new_pheromones + 1;
			}
			
			inline double get_cost() const {
				return _cost;
			}
			
			void update(double evaporation_rate) {
				_pheromones = _pheromones * (1.0 - evaporation_rate);
				_pheromones = _pheromones + _new_pheromones;
				_new_pheromones = 0;
			}
			
		private:
			double _pheromones;
			double _new_pheromones;
			double _cost;
			node_ptr _target;
		};
		
		typedef boost::shared_ptr<Path> path_ptr;
	}
}

#endif