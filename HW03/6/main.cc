#include <iostream>
#include <cmath>

template<class T, int d>
struct Cell {
	Cell<T> * neighbors[2*d];
	bool is_const;
	T potential;
	T charge;
	Cell(): is_const(false), potential(0), charge(0) {
		memset (neighbors, 0, sizeof(T)*2*d);
	}
}

int main(int argc, char * argv[]) {
	
}