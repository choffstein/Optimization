/*
 *  fly.cpp
 *  optimization
 *
 *  Created by Corey M. Hoffstein on 1/15/10.
 *  Copyright 2010 Newfound Research, LLC. All rights reserved.
 *
 */

#include "fly.h"

namespace Optimization {
	namespace ParticleSwarm {
		Fly::Fly(int feature_dimensions, const feature_limits_type& feature_limits, double inertia, double cognitive, double social) :
			_current_features(feature_dimensions),
			_current_velocity(feature_dimensions, 0.0),
			_current_fitness(std::numeric_limits<float>::infinity()),
			_best_features(feature_dimensions),
			_best_fitness(_current_fitness),
			_inertia(inertia),
			_cognitive(cognitive),
			_social(social) {
			
				feature_limits_type::const_iterator it = feature_limits.begin();
				for(int i = 0; it != feature_limits.end(); it++, i++) {
					float r = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
					_current_features[i] = (*it).first + r * ((*it).second - (*it).first);
					_best_features[i] = _current_features[i];
				}
		}
		
		Fly::~Fly() {}
		
		void Fly::update_fitness(boost::function<float (const feature_vector&)> fitness_function) {
			_current_fitness = fitness_function(_current_features);
			
			if(_current_fitness < _best_fitness) {
				_best_fitness = _current_fitness;
				std::copy(_current_features.begin(), _current_features.end(), _best_features.begin());
			}
		}
		
		void Fly::update_position(const feature_vector& global_best_features) {
			for(int i = 0; i < _current_velocity.size(); i++) {
				float r1 = static_cast<float>(rand())/static_cast<float>(RAND_MAX);
				float r2 = static_cast<float>(rand())/static_cast<float>(RAND_MAX);
				_current_velocity[i] = _inertia * _current_velocity[i] + \
							   _cognitive * r1 * (_best_features[i] - _current_features[i]) + \
							   _social * r2 * (global_best_features[i] - _current_features[i]);
			}
			
			for(int i = 0; i < _current_features.size(); i++) {
				_current_features[i] = _current_features[i] + _current_velocity[i];
			}
		}
		
		double Fly::get_fitness() const {
			return _current_fitness;
		}
		
		double Fly::get_best_fitness() const {
			return _best_fitness;
		}
		
		const feature_vector& Fly::get_features() const {
			return _current_features;
		}
		
		const feature_vector& Fly::get_best_features() const {
			return _best_features;
		}
	}
}