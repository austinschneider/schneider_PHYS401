// Written by Austin Schneider (721001767) for CSCE 420
#include <algorithm>
#include <iostream>
#include <iterator>
#include <stdlib.h>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <math.h>
#include <vector>
#include <queue>
#include <deque>
#include <cmath>
#include <stack>
#include <map>
#include <set>

const int N_STEPS = 200;

// 2D point
struct Point {
	double x;
	double y;
	bool operator<(Point const& rhs ) const {
		return (x < rhs.x) || ((x == rhs.x) && (y < rhs.y));
	}
	Point(): x(0), y(0) {};
	Point(double x_, double y_): x(x_), y(y_) {};
	Point(Point const & p): x(p.x), y(p.y) {};
	~Point() {};
};

double distance(Point const & a_, Point const & b_) {
    return std::pow(std::pow(a_.x - b_.x, 2.0) + std::pow(a_.y - b_.y, 2.0), 0.5);
};

// Node in the travelling salesman problem
struct TSPNode {
	std::vector<Point> * path;

	TSPNode():path(new std::vector<Point>()) {};
	TSPNode(TSPNode const & x): path(new std::vector<Point>()) {
		path->resize(x.path->size());
		for(int i=0; i<x.path->size(); ++i) {
			path->at(i) = x.path->at(i);
		}
	};
	
	TSPNode & operator=(const TSPNode & x) {
		if(path != NULL) delete path;
		path = new std::vector<Point>();
		path->resize(x.path->size());
		for(int i=0; i<x.path->size(); ++i) {
			path->at(i) = x.path->at(i);
		}
	};
	
	~TSPNode() {
		if(path != NULL)
			delete path;
	};
	
	std::ostream & operator<<(std::ostream & os) {
		for(int i=0; i<path->size(); ++i) {
			os << "(" << path->at(i).x << "," << path->at(i).y << ")" << std::endl;
		}
		return os;
	};
	
	bool operator<(TSPNode const & b) const {
	for(int i=0; i<path->size(); ++i) {
		if(i >= b.path->size())
			return false;
		if(path->at(i).x <b.path->at(i).x) {
			return true;
		}
		else if(path->at(i).x == b.path->at(i).x) {
			if(path->at(i).y <b.path->at(i).y)
				return true;
			else if(path->at(i).y == b.path->at(i).y)
				continue;
			else
				return false;
		}
		else
			return false;
	}
	if(path->size() < b.path->size())
		return true;

	return false;
	};
	
	bool operator==(TSPNode const & b) {
		if(path->size() == b.path->size()) {
			for(int i=0; i<path->size(); ++i) {
				if(path->at(i).x == b.path->at(i).y) {
					if(path->at(i).y != b.path->at(i).y)
						return false;
				}
				else
					return false;
			}
		}
		else return false;
		return true;
	};
};

std::basic_string<int> shift(int n, int start, int end, int new_start) {
	int s = n;
	std::basic_string<int> order;
	for(int i=0; i<s; ++i) {
		order.push_back(i);
	}
	
	std::basic_string<int> sub;
	std::basic_string<int> rest;
	
	if(end > start) {
		sub = order.substr(start, end-start);
		std::basic_string<int> l = order.substr(0, start);
		std::basic_string<int> r = order.substr(end, (s-end));
		rest = r+l;
	}
	else {
		rest = order.substr(end, (start-end));
		std::basic_string<int> l = order.substr(0, end);
		std::basic_string<int> r = order.substr(start, (s-start));
		sub = r+l;
	}
	
	if(rest.size() != 0) {		
		if(new_start > start) {
			int diff = (new_start-start) % rest.size();
			std::basic_string<int> l = rest.substr(0, diff);
			std::basic_string<int> r = rest.substr(diff);
			order = sub + r + l;
		}
		else {
			int diff = (start-new_start) % rest.size();
			std::basic_string<int> l = rest.substr(0, rest.size() - (diff));
			std::basic_string<int> r = rest.substr(rest.size() - (diff));
			order = sub + r + l;
		}
	}
	return order;
};

// Expand the children of a tsp node
TSPNode * tsp_next(TSPNode * node) {
	if(node == NULL) return NULL;
	int s = node->path->size();
	int start = rand() % s;
	int end = rand() % s + 1;
	TSPNode * d = new TSPNode();
	d->path->resize(s);
	
	int pick = rand() % 2;
	
	if(pick) {
		int new_start = rand() % 2;
		std::basic_string<int> order = shift(s, start, end, new_start);
		for(int i=0; i<s; ++i) {
			d->path->at(i) = node->path->at(order[i]);
		}
	}
	else {
		std::basic_string<int> order;
		for(int i=0; i<s; ++i) {
			order.push_back(i);
		}
		
		std::basic_string<int> sub;
		std::basic_string<int> rest;
	
		if(end > start) {
			sub = order.substr(start, end-start);
			std::basic_string<int> l = order.substr(0, start);
			std::basic_string<int> r = order.substr(end, (s-end));
			rest = r+l;
		}
		else {
			rest = order.substr(end, (start-end));
			std::basic_string<int> l = order.substr(0, end);
			std::basic_string<int> r = order.substr(start, (s-start));
			sub = r+l;
		}
		order.clear();
		for(int i=0; i<sub.size(); ++i) {
				order.push_back(sub[sub.size() - i - 1]);
		}
		order += rest;
		
		for(int i=0; i<s; ++i) {
			d->path->at(i) = node->path->at(order[i]);
		}
	}
	
	return d;
};

float tsp_heuristic(TSPNode * n) {
	float ret = 0;
	for(int a=0; a<n->path->size(); ++a) {
		int b = (a+1) % n->path->size();
		ret += distance(n->path->at(a), n->path->at(b));
	}
	return ret;
};

float tsp_temp(int step, TSPNode *) {
	int max_temp = 20;
	
	return max_temp*(((double)(N_STEPS-step))/((double)N_STEPS));
};

int tsp_sweeps(int step) {
    return 10000*(step+1);
};

template <class T>
T * sa_search(T & init_state, T * (*expand)(T *), float (*heuristic)(T *), float (*temp)(int, T *), std::ostream & os) {
	// Initialize
	T * current_state = new T(init_state);
	T * best_state = current_state;
	float current_heur = heuristic(current_state);
	float best_heur = current_heur;
	float next_heur = current_heur;
	int n_steps = 0;
	bool done = false;
	
	// Loop until done
	while(!done) {
		done = n_steps >= N_STEPS;
		for(int i=0; i<tsp_sweeps(n_steps); ++i) {
			//std::cout << "n_steps " << n_steps << std::endl;
			T * next_state = expand(current_state);
			if(next_state == NULL) {
				std::cout << "NULL expansion!" << std::endl;
				return NULL;
			}
			next_heur = heuristic(next_state);
			
			if(next_heur < current_heur) {
				if(best_state != current_state) delete current_state;
				current_state = next_state;
				next_state = NULL;
				current_heur = next_heur;
				if(current_heur < best_heur) {
					delete best_state;
					best_state = current_state;
					best_heur = current_heur;
				}
			}
			else {
				double r = ((double) rand() / (RAND_MAX));
				float t = temp(n_steps, current_state);
				double p = std::exp(-((next_heur-current_heur)/t));
				if(r < p) {
					if(best_state != current_state) delete current_state;
					current_state = next_state;
					current_heur = next_heur;
				}
				else
					delete next_state;
			}
		}
		os << temp(n_steps, best_state) << " " << best_heur << std::endl;
		++n_steps;
	}

	return best_state;
}

TSPNode build_tsp(std::istream & is, std::map<Point, std::string> & store) {
	TSPNode init;
	double x;
	double y;
	while(is) {
		is >> x >> y;
        std::stringstream ss;
        ss << x << " " << y;
		if(is) {
			Point p(x, y);
			store[p] = ss.str();
			init.path->push_back(p);
		}
	}
	return init;
};

int main(int argc, char* argv[]) {
	srand(time(NULL));
	std::ofstream out("out.txt");
    std::ofstream out_path("path.txt");
	std::ifstream in("tour.txt");
	
	std::map<Point, std::string> store;
	TSPNode init_state = build_tsp(in, store);
	
	TSPNode * final_state = sa_search(init_state, tsp_next, tsp_heuristic, tsp_temp, out);
	for(int i=0; i<final_state->path->size(); ++i) {
		out_path << store[final_state->path->at(i)] << std::endl;
	}
    out_path << store[final_state->path->at(0)] << std::endl;
	std::cout << "heuristic: " << tsp_heuristic(final_state) << std::endl;
	
	out << std::endl;
	
	return 0;
}
