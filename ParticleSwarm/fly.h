/*
 *  fly.h
 *  optimization
 *
 *  Created by Corey M. Hoffstein on 1/15/10.
 *  Copyright 2010 Newfound Research, LLC. All rights reserved.
 *
 */

#ifndef __FLY_H__
#define __FLY_H__

#include <boost/smart_ptr.hpp>
#include <boost/function.hpp>

#include <vector>
#include <utility>

namespace Optimization {
	namespace ParticleSwarm {
		
		typedef std::vector<std::pair<double, double> > feature_limits_type;
		typedef std::vector<double> feature_vector;
		
		class Fly {
		
		public:

			Fly(int feature_dimensions, const feature_limits_type& feature_limits, double inertia, double cognitive, double social);
			~Fly();
			
			void update_fitness(boost::function<float (const feature_vector&)> fitness_function);
			void update_position(const feature_vector& global_best_features);
			
			double get_fitness() const;
			double get_best_fitness() const;
			
			const feature_vector& get_features() const;
			const feature_vector& get_best_features() const;
			
		private:
			double _current_fitness;
			double _best_fitness;
			feature_vector _best_features;
			
			feature_vector _current_features;
			feature_vector _current_velocity;
			
			double _inertia;
			double _cognitive;
			double _social;
		};
		
		typedef boost::shared_ptr<Fly> fly_ptr;
	}
}

#endif