/*
 *  path.cpp
 *  optimization
 *
 *  Created by Corey M. Hoffstein on 1/14/10.
 *  Copyright 2010 Newfound Research, LLC. All rights reserved.
 *
 */

#include "path.h"
#include "node.h"

namespace Optimization {
	namespace SimulatedAnnealing {
		template <class type> Path<type>::Path(typename Node<type>::node_ptr target, double cost) 
		
		template <class type> Path<type>::~Path() {}
		
		template <class type> Node<type>::node_ptr get_target() const {
			
		}
		
		template <class type> double Path<type>::get_cost() const {
			return _cost;
		}
	}
}