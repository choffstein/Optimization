#include <iostream>
#include <cstdlib>
#include <ctime>

#include <utility>
#include <vector>

#include <boost/shared_ptr.hpp>

#include "ant.h"
#include "path.h"
#include "node.h"
#include "colony.h"

std::vector<std::pair<int, int> > classifications;

struct scorer {
	virtual bool operator()(int input) = 0;
};

typedef boost::shared_ptr<scorer> scorer_ptr;

struct less_than_scorer : public scorer {
	int _lt;
	
	less_than_scorer(int lt) : _lt(lt) {}
	
	bool operator()(int input) {
		return (input < _lt);
	}
};

struct greater_than_scorer : public scorer {
	int _gt;
	
	greater_than_scorer(int gt) : _gt(gt) {}
	
	bool operator()(int input) {
		return (input > _gt);
	}
};

struct is_even_scorer : public scorer {
	bool operator()(int input) {
		return (input % 2 == 0);
	}
};

struct is_odd_scorer : public scorer {
	bool operator()(int input) {
		return (input % 2 == 1);
	}
};

class ScoreNode : public Optimization::AntColony::Node {
public:
	ScoreNode(std::string id, scorer_ptr scorer) : _id(id), _scorer(scorer), _desirability(0) {
		generate_desirability();
	}
	~ScoreNode() {}
	
	void generate_desirability() {
		std::vector<std::pair<int, int> >::iterator it = classifications.begin();
		
		int true_pos = 0;
		int false_neg = 0;
		int true_neg = 0; 
		int false_pos = 0;
		
		//The quality of a rule is
		// [TruePos / (TruePos + FalseNeg)] * [TrueNeg / (FalsePos + TrueNeg)]
		for(it; it != classifications.end(); it++) {
			bool b = (*_scorer)(it->first);
			
			if(b  && it->second) {
				true_pos = true_pos + 1;
			} else if (b && !it->second) {
				false_pos = false_pos + 1;
			} else if (!b && it->second) {
				false_neg = false_neg + 1;
			} else {
				true_neg = true_neg + 1;
			}
		}
		
		double pos_ratio = static_cast<double>(true_pos) / (static_cast<double>(true_pos) + static_cast<double>(false_neg));
		double neg_ratio = static_cast<double>(true_neg) / (static_cast<double>(false_pos) + static_cast<double>(true_neg));
		
		_desirability = pos_ratio * neg_ratio;
		
		std::cout << _id << " " << _desirability << std::endl;
	}
	
	double get_desirability() const {
		return _desirability;
	}
	
	std::string get_id() const {
		return _id;
	}
	
private:
	scorer_ptr _scorer;
	std::string _id;
	double _desirability;
};

class StartNode : public Optimization::AntColony::Node {
public:
	StartNode(std::string id) : _id(id) {}
	~StartNode() {}
	
	double get_desirability() const {
		return 0.0;
	}
	
	std::string get_id() const {
		return _id;
	}
	
private:
	std::string _id;
};

int main (int argc, char * const argv[]) {
	
	srand(time(NULL));
	
	for(int i = 0; i < 100; i++) {
		if(i >= 50 && i <= 70 && (i % 2 == 1)) {
		   classifications.push_back(std::make_pair(i, 1));
		} else {
		   classifications.push_back(std::make_pair(i, 0));
		}
	}
		   
	std::vector<Optimization::AntColony::node_ptr> nodes;
	std::vector<Optimization::AntColony::path_ptr> paths;
	
	int num_ants = 5000;
		   
	nodes.push_back(Optimization::AntColony::node_ptr(new StartNode("Start")));	   
	
	nodes.push_back(Optimization::AntColony::node_ptr(new ScoreNode("< 10", scorer_ptr(new less_than_scorer(10)))));
	nodes.push_back(Optimization::AntColony::node_ptr(new ScoreNode("< 20", scorer_ptr(new less_than_scorer(20)))));
	nodes.push_back(Optimization::AntColony::node_ptr(new ScoreNode("< 30", scorer_ptr(new less_than_scorer(30)))));
	nodes.push_back(Optimization::AntColony::node_ptr(new ScoreNode("< 40", scorer_ptr(new less_than_scorer(40)))));
	nodes.push_back(Optimization::AntColony::node_ptr(new ScoreNode("< 50", scorer_ptr(new less_than_scorer(50)))));
	nodes.push_back(Optimization::AntColony::node_ptr(new ScoreNode("< 60", scorer_ptr(new less_than_scorer(60)))));
	nodes.push_back(Optimization::AntColony::node_ptr(new ScoreNode("< 70", scorer_ptr(new less_than_scorer(70)))));
	nodes.push_back(Optimization::AntColony::node_ptr(new ScoreNode("< 80", scorer_ptr(new less_than_scorer(80)))));
	nodes.push_back(Optimization::AntColony::node_ptr(new ScoreNode("< 90", scorer_ptr(new less_than_scorer(90)))));
	nodes.push_back(Optimization::AntColony::node_ptr(new ScoreNode("< 100", scorer_ptr(new less_than_scorer(100)))));
		   
	nodes.push_back(Optimization::AntColony::node_ptr(new ScoreNode("> 0", scorer_ptr(new greater_than_scorer(0)))));
	nodes.push_back(Optimization::AntColony::node_ptr(new ScoreNode("> 10", scorer_ptr(new greater_than_scorer(10)))));
	nodes.push_back(Optimization::AntColony::node_ptr(new ScoreNode("> 20", scorer_ptr(new greater_than_scorer(20)))));
	nodes.push_back(Optimization::AntColony::node_ptr(new ScoreNode("> 30", scorer_ptr(new greater_than_scorer(30)))));
	nodes.push_back(Optimization::AntColony::node_ptr(new ScoreNode("> 40", scorer_ptr(new greater_than_scorer(40)))));
	nodes.push_back(Optimization::AntColony::node_ptr(new ScoreNode("> 50", scorer_ptr(new greater_than_scorer(50)))));
	nodes.push_back(Optimization::AntColony::node_ptr(new ScoreNode("> 60", scorer_ptr(new greater_than_scorer(60)))));
	nodes.push_back(Optimization::AntColony::node_ptr(new ScoreNode("> 70", scorer_ptr(new greater_than_scorer(70)))));
	nodes.push_back(Optimization::AntColony::node_ptr(new ScoreNode("> 80", scorer_ptr(new greater_than_scorer(80)))));
	nodes.push_back(Optimization::AntColony::node_ptr(new ScoreNode("> 90", scorer_ptr(new greater_than_scorer(90)))));
		   
	nodes.push_back(Optimization::AntColony::node_ptr(new ScoreNode("is even", scorer_ptr(new is_even_scorer()))));
	nodes.push_back(Optimization::AntColony::node_ptr(new ScoreNode("is odd", scorer_ptr(new is_odd_scorer()))));
		   
	for(int i = 0; i < nodes.size(); i++) {
		for(int j = 1; j < nodes.size(); j++) {
			if(i != j) {
				Optimization::AntColony::path_ptr p(new Optimization::AntColony::Path(nodes[j], 1, num_ants));
				nodes[i]->add_connection(p);
				paths.push_back(p);				
			}
		}
	}
		   	
	// run the simulation
	int num_steps = 1000;
	Optimization::AntColony::simulate(num_ants, nodes[0], paths, num_steps, 0.001, 2.0, 1.0, 2.0);
	
	// drop an ant and follow where it goes to find 'optimal' path
	Optimization::AntColony::ant_ptr a(new Optimization::AntColony::Ant(nodes[0]));
	for(int i = 0; i < 10; i++) {
		a->update(2.0, 1.0, 1.0, true);
		ScoreNode *f = dynamic_cast<ScoreNode *>(a->get_location().get());
		std::cout << " " << f->get_id() << " ";
	}
	std::cout << std::endl;
}
