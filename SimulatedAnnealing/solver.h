/*
 *  solver.h
 *  optimization
 *
 *  Created by Corey M. Hoffstein on 1/15/10.
 *  Copyright 2010 Newfound Research, LLC. All rights reserved.
 *
 */

#ifndef __SOLVER_H__
#define __SOLVER_H__

#include "state.h"

namespace Optimization {
	namespace SimulatedAnnealing {
		std::pair<double, state_ptr> solve(state_ptr initial_state, 
										   double temperature, 
										   double cost,
										   int max_iterations,
										   int step_size,
										   bool verbose);
	}
}

#endif