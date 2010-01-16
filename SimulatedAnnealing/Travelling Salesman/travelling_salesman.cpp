/*
 *  travelling_sales_man.cpp
 *  
 *
 *  Created by Corey M. Hoffstein on 1/16/10.
 *  Copyright 2010 Newfound Research, LLC. All rights reserved.
 *
 */


//SIMULATED ANNEALING WITH TRAVELING SALES MAN PROBLEM
#include "solver.h"
#include "state.h"

#include <iostream>
#include <cmath>

#include <cstdlib> 
#include <ctime>
#include <vector>
#include <map>

//Our Traveling Sales Man State
class TSMState : public Optimization::SimulatedAnnealing::State {
	
private:
	class Map {
	private:
		std::map<int, std::map<int, double> > _cost_map;
	public:
		Map(int num_cities) {
			for(int i = 0; i < num_cities; i++) {
				for(int j = 0; j < num_cities; j++) {
					if(i == j) {
						_cost_map[i][j] = 0.0;
					} else {
						_cost_map[i][j] = 100.0 * static_cast<float>(rand()) / static_cast<float>(RAND_MAX); //random value between 0 and 100 miles
					}
				}
			}
		}
		
		double get_distance(int i, int j) {
			return _cost_map[i][j];
		}
	};
	
	typedef boost::shared_ptr<Map> map_ptr;
	
	map_ptr _map;
	std::vector<int> _path;
	double _energy;
	
public:
	TSMState(int num_cities) : _path(num_cities), _energy(0.0) {
		
		//initialize our path.  Naive -- go from city 0 to 1 to 2 to 3...
		for (int i = 0; i < num_cities; i++) {
			_path[i] = i;
		}
		
		//randomly initialize our travelling costs
		_map.reset(new Map(num_cities));
		
		for(int i = 1; i < _path.size(); i++) {
			_energy = _energy + _map->get_distance(_path[i-1], _path[i]);
		}
		//add the energy for the round trip	
		_energy = _energy + _map->get_distance(_path[_path.size()-1], _path[0]);
	}
	
	TSMState(map_ptr map, std::vector<int> path) : 
	_map(map),
	_path(path),
	_energy(0.0) {
		
		for(int i = 1; i < _path.size(); i++) {
			_energy = _energy + _map->get_distance(_path[i-1], _path[i]);
		}
		//add the energy for the round trip	
		_energy = _energy + _map->get_distance(_path[_path.size()-1], _path[0]);
	}
	
	virtual ~TSMState() {}
	
	virtual Optimization::SimulatedAnnealing::state_ptr get_random_neighbor() {
		
		int num_cities = _path.size();
		int city_a_loc = static_cast<int>(static_cast<float>(num_cities) * static_cast<float>(rand()) / static_cast<float>(RAND_MAX));
		int city_b_loc = static_cast<int>(static_cast<float>(num_cities) * static_cast<float>(rand()) / static_cast<float>(RAND_MAX));
		
		std::vector<int> new_path(num_cities);
		std::copy(_path.begin(), _path.end(), new_path.begin());
		
		int t = new_path[city_b_loc];
		new_path[city_b_loc] = new_path[city_a_loc];
		new_path[city_a_loc] = t;
		
		return Optimization::SimulatedAnnealing::state_ptr(new TSMState(_map, new_path));
	}
	
	virtual double get_energy() {
		return _energy;
	}
	
	void pretty_print() {
		for(int i = 1; i < _path.size(); i++) {
			std::cout << _path[i-1] << " --" << _map->get_distance(_path[i-1], _path[i]) << "--> ";
		}
		std::cout << _path[_path.size()-1] << " --" << _map->get_distance(_path[_path.size()-1], _path[0]) << "--> ";
		std::cout << std::endl;
	}
};

int main (int argc, char * const argv[]) {
	srand ( time(NULL) );
	
	Optimization::SimulatedAnnealing::state_ptr initial_state(new TSMState(100));
	std::pair<double, Optimization::SimulatedAnnealing::state_ptr> result = Optimization::SimulatedAnnealing::solve(initial_state, 30.0, 0.999, 500000, 50); 
	
	std::cout << result.first << std::endl;
	result.second->pretty_print();
	
	return 0;
}
