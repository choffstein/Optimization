/*
 *  node.h
 *  optimization
 *
 *  Created by Corey M. Hoffstein on 1/14/10.
 *  Copyright 2010 Newfound Research, LLC. All rights reserved.
 *
 */

#ifndef __NODE_H__
#define __NODE_H__

#include "path.h"

#include <boost/shared_ptr.hpp>
#include <vector>

namespace Optimization {
	namespace AntColony {
		class Path;
		typedef boost::shared_ptr<Path> path_ptr;
		
		class Node {
		
		public: 
			Node() {}
			
			virtual ~Node() {}
			
			virtual double get_desirability() const = 0;
			
			inline void add_connection(path_ptr connection) {
				_connections.push_back(connection);
			}
			
			inline const std::vector<path_ptr>& get_connections() const {
				return _connections;
			}
			
		private:
			std::vector<path_ptr> _connections;
		};
		
		typedef boost::shared_ptr<Node> node_ptr;
	}
}

#endif
