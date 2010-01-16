/*
 *  evolvable.h
 *  optimization
 *
 *  Created by Corey M. Hoffstein on 1/14/10.
 *  Copyright 2010 Newfound Research, LLC. All rights reserved.
 *
 */

#ifndef __EVOLVABLE_H__
#define __EVOLVABLE_H__

namespace Optimization {
	namespace GeneticAlgorithm {
		class Evolvable {
		public:
			Evolvable() {}
			virtual ~Evolvable() {}
			virtual void mutate() = 0;
		};
	}
}

#endif