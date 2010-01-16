/*
 *  state.h
 *  optimization
 *
 *  Created by Corey M. Hoffstein on 1/15/10.
 *  Copyright 2010 Newfound Research, LLC. All rights reserved.
 *
 */

#ifndef __STATE_H__
#define __STATE_H__

#include <vector>
#include <boost/shared_ptr.hpp>

namespace Optimization {
	namespace SimulatedAnnealing {
		
		class State;
		
		typedef boost::shared_ptr<State> state_ptr;
		
		class State {
			
		public:
			State() {}
			virtual ~State() {}
			
			virtual state_ptr get_random_neighbor() = 0;
			virtual double get_energy() = 0;
			
			virtual void pretty_print() = 0;
		};
	}
}

#endif