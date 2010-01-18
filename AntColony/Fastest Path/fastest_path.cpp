#include <iostream>
#include <cstdlib>
#include <ctime>

#include "ant.h"
#include "path.h"
#include "node.h"
#include "colony.h"

class FloatNode : public Optimization::AntColony::Node {
	public:
		FloatNode(int id, float f) : _id(id), _value(f) {}
		~FloatNode() {}
	
		double get_desirability() const {
			return static_cast<double>(_value);
		}
	
		int get_id() const {
			return _id;
		}
	
	private:
		float _value;
		int _id;
};

int main (int argc, char * const argv[]) {
	
	srand(time(NULL));
	
	std::vector<Optimization::AntColony::node_ptr> nodes;
	std::vector<Optimization::AntColony::path_ptr> paths;
	
	int num_ants = 1000;
	int num_nodes = 100;
	
	for(int i = 0; i < num_nodes; i++) {
		Optimization::AntColony::node_ptr n(new FloatNode(i, 1.0));
		nodes.push_back(n);
	}
						
	for(int i = 0; i < pow(num_nodes, 2)/2; i++) {
		int i1 = static_cast<int>(static_cast<float>(num_nodes) * static_cast<float>(rand()) / static_cast<float>(RAND_MAX));
		int i2 = static_cast<int>(static_cast<float>(num_nodes) * static_cast<float>(rand()) / static_cast<float>(RAND_MAX));
		
		//our distance in miles
		float r = 100.0 * static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
		
		if(i1 != i2) {
			Optimization::AntColony::path_ptr p(new Optimization::AntColony::Path(nodes[i2], r, pow(num_ants, 2)));
			//std::cout << "Created connection from " << i1 << " to " << i2 << std::endl;
			nodes[i1]->add_connection(p);
			paths.push_back(p);
		}
	}
						
	//make sure we have at least 1 connection
	float r = 100.0 * static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
	Optimization::AntColony::path_ptr p(new Optimization::AntColony::Path(nodes[1], r, pow(num_ants, 2)));
	nodes[0]->add_connection(p);
	paths.push_back(p);			
	
	// run the simulation
	int num_steps = 1000;
	Optimization::AntColony::simulate(num_ants, nodes[0], paths, num_steps, 0.001, 2.0, 1.0, 1.0);
						
	// drop an ant and follow where it goes to find 'optimal' path
	Optimization::AntColony::ant_ptr a(new Optimization::AntColony::Ant(nodes[0]));
	for(int i = 0; i < 100; i++) {
		FloatNode *f = dynamic_cast<FloatNode *>(a->get_location().get());
		std::cout << " " << f->get_id() << " ";
		a->update(2.0, 1.0, 1.0, true);
	}
	FloatNode *f = dynamic_cast<FloatNode *>(a->get_location().get());
	std::cout << " " << f->get_id() << " ";
	std::cout << std::endl;
}
