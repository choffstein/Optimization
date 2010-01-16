/*
 *  n-queens.cpp
 *  
 *
 *  Created by Corey M. Hoffstein on 1/16/10.
 *  Copyright 2010 Newfound Research, LLC. All rights reserved.
 *
 */


//SIMULATED ANNEALING for n-Queens Problem
#include "solver.h"
#include "state.h"

#include <iostream>
#include <cmath>

#include <cstdlib> 
#include <ctime>
#include <vector>
#include <map>

class BoardState : public Optimization::SimulatedAnnealing::State {
	
private:
	static const int dx[];
	static const int dy[];
	
	std::vector<int> _positions;
	double _energy;
	
	std::vector<std::vector<int> > construct_board() {
		std::vector<std::vector<int> > board(_positions.size(), std::vector<int>(_positions.size(), 0));
		for(int i = 0; i < _positions.size(); i++) {
			board[i][_positions[i]] = 1;
		}
		
		return board;
	}
	
	void calculate_energy() {
		_energy = 0.0;
		std::vector<std::vector<int> > board = construct_board();
		
		int num_queens = _positions.size();
		int conflicts = 0;
		for(int i = 0; i < num_queens; i++) {
			int x = i;
			int y = _positions[i];
			
			for(int j = 0; j < 3; j++) {
				int tempx = x;
				int tempy = y;
				
				while(true) {
					tempx = tempx + dx[j];
					tempy = tempy + dy[j];
					
					if(tempx < 0 || tempx >= num_queens || tempy < 0 || tempy >= num_queens)
						break;
					
					if(board[tempx][tempy] == 1) {
						conflicts = conflicts + 1;
					}
				}
			}
		}
		
		_energy = conflicts;
	}
	
public:
	
	BoardState(int num_queens) : _positions(num_queens), _energy(0.0) {
		for(int i = 0; i < num_queens; i++) {
			_positions[i] = i;
		}
		
		calculate_energy();
	}
	
	BoardState(std::vector<int> positions) : 
	_positions(positions),
	_energy(0.0) {
		
		calculate_energy();
	}
	
	virtual ~BoardState() {}
	
	virtual Optimization::SimulatedAnnealing::state_ptr get_random_neighbor() {
		int num_queens = _positions.size();
		std::vector<int> new_positions(num_queens);
		
		std::copy(_positions.begin(), _positions.end(), new_positions.begin());
		
		int r1 = static_cast<int>(static_cast<float>(num_queens) * static_cast<float>(rand()) / static_cast<float>(RAND_MAX));
		int r2 = static_cast<int>(static_cast<float>(num_queens) * static_cast<float>(rand()) / static_cast<float>(RAND_MAX));
		
		int t = new_positions[r1];
		new_positions[r1] = new_positions[r2];
		new_positions[r2] = t;
		
		return Optimization::SimulatedAnnealing::state_ptr(new BoardState(new_positions));
	}
	
	virtual double get_energy() {
		return _energy;
	}
	
	void pretty_print() {
		for(int i = 0; i < _positions.size(); i++) {
			for(int j = 0; j < _positions.size(); j++) {
				if(_positions[i] == j) { 
					std::cout << "|Q";
				} else {
					std::cout << "|_";
				}
			}
			std::cout << "|" << std::endl;
		}
	}
};

const int BoardState::dx[4] = {1, -1, 1, -1};
const int BoardState::dy[4] = {1, -1, -1, 1};

int main (int argc, char * const argv[]) {
	srand ( time(NULL) );
	
	Optimization::SimulatedAnnealing::state_ptr initial_state(new BoardState(64));
	std::pair<double, Optimization::SimulatedAnnealing::state_ptr> result = Optimization::SimulatedAnnealing::solve(initial_state, 30.0, 0.999, 1000000, 50); 
	
	std::cout << result.first << std::endl;
	result.second->pretty_print();
	
	return 0;
}