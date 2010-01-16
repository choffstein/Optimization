/*
 *  chromosome.h
 *  optimization
 *
 *  Created by Corey M. Hoffstein on 1/14/10.
 *  Copyright 2010 Newfound Research, LLC. All rights reserved.
 *
 */

#ifndef __CHROMOSOME_H__
#define __CHROMOSOME_H__

#include <vector>
#include <utility>
#include <cstdlib> 
#include <cmath>

#include <boost/shared_ptr.hpp>

namespace Optimization {
	namespace GeneticAlgorithm {
		template <class dna_type>
		class Chromosome {
			
		public:
			typedef Chromosome<dna_type> chromosome_type;
			typedef boost::shared_ptr<chromosome_type> chromosome_type_ptr;
			typedef std::vector<dna_type> dna_vector;
			
			Chromosome() {}
			
			//Basic constructor
			Chromosome(int size) {
				//create an initial mutation of our dna
				
				for(int i = 0; i < size; i++) {
					_dna.push_back(dna_type());
				}
			}
			
			//Create a chromosome from a vector of DNA types
			Chromosome(const dna_vector& dna) : _dna(dna) {}
			
			//copy-constructor
			Chromosome(const chromosome_type_ptr& other) : _dna(other->dna()) {}
			
			~Chromosome() {}
			
			//This method will mate two Chromosomes, and return a pair of their offspring
			std::pair<chromosome_type_ptr, chromosome_type_ptr> reproduce(const chromosome_type_ptr& other, float rate) const {
				//PRECONDITION: The size of the other chromosome must match our own
				if(size() != other->size()) {
					throw "Chromosome sizes must match for reproduction";
				}
				
				//find our point to mutate at.  Cast to int
				float r = rand() / static_cast<float>(RAND_MAX);
				int crossover_point = static_cast<int>(static_cast<float>(size()) * r);
				
				dna_vector dna_child_one(size());
				dna_vector dna_child_two(size());
				
				//Create our 'dna' by using cross-over point to swap values
				std::copy(dna().begin(), dna().begin() + crossover_point, dna_child_one.begin());
				std::copy(other->dna().begin() + crossover_point, other->dna().end(), dna_child_one.begin() + crossover_point);
				
				std::copy(other->dna().begin(), other->dna().begin() + crossover_point, dna_child_two.begin());
				std::copy(dna().begin() + crossover_point, dna().end(), dna_child_two.begin() + crossover_point);
			
				chromosome_type_ptr chromosome_child_one(new Chromosome<dna_type>(dna_child_one));
				chromosome_type_ptr chromosome_child_two(new Chromosome<dna_type>(dna_child_two));
				
				chromosome_child_one->mutate(rate);
				chromosome_child_two->mutate(rate);
				
				return std::make_pair(chromosome_child_one, chromosome_child_two);
			}
			
			//This method will mutate the dna at a given rate.  The higher the rate, the more likely
			//the DNA is to mutate.
			void mutate(float rate) { 
				typename dna_vector::iterator it = _dna.begin();
				for(it; it != _dna.end(); it++) {
					float r = rand() / static_cast<float>(RAND_MAX);
					if(r < rate) {
						it->mutate();
					}
				}
			}
			
			// distance is measured as Euclidean distance
			float distance(const chromosome_type& other) {
				//PRECONDITION: The size of the other chromosome must match our own
				if(size() != other.size()) {
					throw "Chromosome sizes must match for reproduction";
				}
				
				float sum = 0.0;
				typename dna_vector::const_iterator it1 = _dna.begin();
				typename dna_vector::const_iterator it2 = other.dna().begin();
				
				for(it1; it1 != _dna.end(); it1++, it2++) {
					double distance = it1->distance(*it2);
					sum = sum + pow(distance, 2);
				}
				
				return sqrt(sum);
			}
			
			inline int size() const {
				return _dna.size();
			}
			
			//make this const dna_vector&
			inline const dna_vector& dna() const {
				return _dna;
			}
			
		private:
			dna_vector _dna;
		};
	}
}

#endif