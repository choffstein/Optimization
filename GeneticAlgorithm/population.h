/*
 *  population.h
 *  optimization
 *
 *  Created by Corey M. Hoffstein on 1/14/10.
 *  Copyright 2010 Newfound Research, LLC. All rights reserved.
 *
 */

#ifndef __POPULATION_H__
#define __POPULATION_H__

#include <algorithm>
#include <vector>
#include <utility>

#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>

#include "chromosome.h"

namespace Optimization {
	namespace GeneticAlgorithm {
		
		template<class chromosome_type>
		class Population {
		public:
			Population(int population_size, int chromosome_length) : _chromosome_length(chromosome_length) {
				for(int i = 0; i < population_size; i++) {
					_population.push_back(chromosome_type_ptr(new Chromosome<chromosome_type>(_chromosome_length)));
				}
			}
			
			~Population() {}
			
			inline int size() const {
				return _population.size();
			}
			
			std::pair<float, typename Chromosome<chromosome_type>::dna_vector>
			evolve(boost::function<float (const typename Chromosome<chromosome_type>::dna_vector&)> fitness_function, 
						float convergence_target,
						int max_iterations = 10000,
						float mutation_rate = 0.05,
						bool verbose = true) {
				
				int current_iteration = 0;
				float current_convergence = std::numeric_limits<float>::infinity();
				
				chromosome_type_ptr global_best;
				float fitness_best = std::numeric_limits<float>::infinity();
				
				//continue working while we have neither reached our convergence target, or exceeded
				//our maximum iterations
				while(current_iteration < max_iterations && current_convergence > convergence_target) {
					std::vector<std::pair<float, chromosome_type_ptr> > population_fitness(size());
					
					// transform our population and create an array with fitnesses
					std::transform(_population.begin(), _population.end(), 
								   population_fitness.begin(), boost::bind(&apply_fitness, fitness_function, _1));
					
					// sort our population by their fitnesses
					std::sort(population_fitness.begin(), population_fitness.end(), fitness_comparator());

					// check for our global best solution
					if(population_fitness[0].first < fitness_best) {
						global_best = population_fitness[0].second;
						fitness_best = population_fitness[0].first;
					}
					
					if(verbose) {
						std::cout << current_iteration;
						std::cout << "\t" << population_fitness[0].first << ": ";
						for(int i = 0; i < population_fitness[0].second->dna().size(); i++) {
							std::cout << population_fitness[0].second->dna()[i].value();
						}
						std::cout << std::endl;
					}

					
					// We need to measure our 'convergence'
					// This can be done in several ways -- you can keep your last population best and check the 
					// current population best, or we can check a certain percentage of our population for convergence
					// Here, we define 'convergence' as to how 'close' two chromosomes are.
					// We will define this as the Euclidean distance of the chromosomes, and use average of
					// the top 10% of the population
					int ten_percent_limit = static_cast<int>(static_cast<float>(size()) * 0.10);
					float total = 0;
					int count = 0;
					
					for(int i = 0; i < ten_percent_limit; i++) {
						for(int j = i+1; j < ten_percent_limit; j++) {
							
							/*
							 std::cout << "Comparing: ";
							for(int k = 0; k < population_fitness[i].second->dna().size(); k++) {
								std::cout << population_fitness[i].second->dna()[k].value();
							}
							std::cout << " VERSUS ";
							for(int k = 0; k < population_fitness[j].second->dna().size(); k++) {
								std::cout << population_fitness[j].second->dna()[k].value();
							}
							std::cout << " : ";
							*/
							
							total = total + population_fitness[i].second->distance(population_fitness[j].second);
							
							//std::cout << total << std::endl;
							
							count = count + 1;
						}
					}
					current_convergence = total / count;
					
					if(verbose) {
						std::cout << "Convergence: " << current_convergence << std::endl;
					}
					
					// Here we take a little leeway as to how we can implement our population optimization
					// We will do the following
					//		(i)	Mate the top 10% to create the next 30%
					//		(ii)	Mate the top 50% to create the next 50%
					//		(iii)	Randomly create the last 20%
					
					population_vector new_population;

					// (i) 
					int first_limit = static_cast<int>(static_cast<float>(size()) * 0.10);
					for(int i = 0; i < first_limit; i++) {
						new_population.push_back(population_fitness[i].second);
					}
					
					
					// (ii) 
					// Note that even though we want to add 20%, we have a limit of 
					// 10% since our reproduction gives 2 children
					int second_limit = static_cast<float>(size()) * 0.1;
					for(int i = 0; i < second_limit; i++) {
						int father_location = static_cast<int>(static_cast<float>(size()) * 0.1 * rand() / static_cast<float>(RAND_MAX));
						int mother_location = static_cast<int>(static_cast<float>(size()) * 0.1 * rand() / static_cast<float>(RAND_MAX));
						
						chromosome_type_ptr father = population_fitness[father_location].second;
						chromosome_type_ptr mother = population_fitness[mother_location].second;
						
						std::pair<chromosome_type_ptr, chromosome_type_ptr> child_pair;
						
						child_pair = father->reproduce(mother, mutation_rate);
						
						new_population.push_back(child_pair.first);
						new_population.push_back(child_pair.second);
					}

					// (iii)
					// Again, we only do 1/2 of our target size, since mother/father creates double
					int third_limit =  static_cast<float>(size()) * 0.25;
					for(int i = 0; i < third_limit; i++) {
						int father_location = static_cast<int>(static_cast<float>(size()) * 0.5 * rand() / static_cast<float>(RAND_MAX));
						int mother_location = static_cast<int>(static_cast<float>(size()) * 0.5 * rand() / static_cast<float>(RAND_MAX));
						
						chromosome_type_ptr father = population_fitness[father_location].second;
						chromosome_type_ptr mother = population_fitness[mother_location].second;
						
						std::pair<chromosome_type_ptr, chromosome_type_ptr> child_pair;
						
						child_pair = father->reproduce(mother, mutation_rate);
						
						new_population.push_back(child_pair.first);
						new_population.push_back(child_pair.second);
					}
					
					// (iv)
					int fourth_limit =  static_cast<float>(size()) * 0.2;
					for(int i = 0; i < fourth_limit; i++) {
						new_population.push_back(chromosome_type_ptr(new Chromosome<chromosome_type>(_chromosome_length)));
					}
					
					// Set our new population
					std::copy(new_population.begin(), new_population.end(), _population.begin());
					
					// Increase the current iteration
					current_iteration++;
				}
				
				return std::make_pair(fitness_best, global_best->dna());
			}
			
		private:
			typedef boost::shared_ptr<Chromosome<chromosome_type> > chromosome_type_ptr;
			typedef std::vector<chromosome_type_ptr> population_vector;
			population_vector _population;
			int _chromosome_length;
			
			// apply our fitness function to a chromosome, and return the fitness/chromosome pair
			static std::pair<float, chromosome_type_ptr > apply_fitness(
						boost::function<float (const typename Chromosome<chromosome_type>::dna_vector&)> fitness_function, 
						chromosome_type_ptr chromosome) {
				
				float fitness = fitness_function(chromosome->dna());
				return std::make_pair(fitness, chromosome);
			}
																		   
			// our comparator for fitness / chromosome pairs
			struct fitness_comparator {
				bool operator()(std::pair<float, chromosome_type_ptr > first,
								std::pair<float, chromosome_type_ptr > second) {
					return (first.first < second.first);
				}
			};
		};
	}
}

#endif