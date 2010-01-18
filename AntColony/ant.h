/*
 *  ant.h
 *  Fastest Path
 *
 *  Created by Corey M. Hoffstein on 1/18/10.
 *  Copyright 2010 Newfound Research, LLC. All rights reserved.
 *
 */

#ifndef __ANT_H__
#define __ANT_H__

#include "node.h"
#include "path.h"

#include <boost/foreach.hpp>
#include <boost/shared_ptr.hpp>

#include <cstdlib>
#include <cmath>

namespace Optimization {
	namespace AntColony {
		
		class Ant {
		private:
			node_ptr _current_location;

		public:
			Ant(node_ptr origin);
			~Ant();
			void update(double pheromone_influence, double path_cost_influence, double desirability_influence, bool verbose);
			node_ptr get_location() const;
		};
		
		typedef boost::shared_ptr<Ant> ant_ptr;
	}
}

#endif