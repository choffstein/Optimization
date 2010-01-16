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
		
		template <class type> class Node;
		
		template <class type>
		class Path {
			
		public:
			typedef boost::shared_ptr<Path<type> > path_ptr;
			
			Path(typename Node<type>::node_ptr target, double cost) :
				_cost(cost),
				_target(target) {
			}
			
			~Path() {}
			
			typename Node<type>::node_ptr get_target() const {
				return _target;
			}
			
			double get_cost() const {
				return _cost;
			}
			
		private:
			double _cost;
			double _pheramones;
			typename Node<type>::node_ptr _target;
		};
}

#endif