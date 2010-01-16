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
		
		template <class type> class Path;
		
		template <class type>
		class Node {
			
			
		public:
			typedef boost::shared_ptr<Node<type> > node_ptr;
			
			Node(type value) : _value(value) {}
			
			~Node() {}
			
			type get_value() const {
				return _value;
			}
			
			void add_connection(typename Path<type>::path_ptr connection) {
				_connections.push_back(connection);
			}
			
		private:
			type _value;
			std::vector<typename Path<type>::path_ptr> _connections;
		};
}

#endif
