#include <stdio.h>
#include <string.h>

#include <cmath>
#include <iomanip>
#include <sstream>
#include <iostream>
#include <iterator>

template<class T, int d>
struct Cell {
	bool is_const;
	T potential;
	T charge;

	Cell<T, d> * up[d];
	Cell<T, d> * down[d];
	
	Cell(): is_const(false), potential(0), charge(0) {
		for(int i=0; i<d; ++i) {
			up[i] = NULL;
			down[i] = NULL;
		}
	}
	
	Cell(Cell<T, d> & cell):is_const(cell.is_const), charge(cell.charge), potential(cell.potential) {
		std::copy(cell.up, cell.up + d, up);
		std::copy(cell.down, cell.down + d, down);
	}
	
	Cell<T,d>& operator=(const Cell<T,d>& other) {
		charge = other.charge;
		potential = other.potential;
		is_const = other.is_const;
		std::copy(other.up, other.up + d, up);
		std::copy(other.down, other.down + d, down);
		return *this;
	}
	
	Cell(Cell<T, d> & cell, T delta[d]):is_const(cell.is_const), charge(cell.charge) {
		std::copy(cell.up, cell.up + d, up);
		std::copy(cell.down, cell.down + d, down);
		if(cell.is_const) {
			potential = cell.potential;
		}
		else {
			potential = 4*M_PI*charge;
			for(int i=0; i<d; ++i)
				potential *= delta[i];
			Cell<T, d> * c;
			for(int i=0, p=0; i<d; ++i, ++p) {
				if(p%2 == 0) {
					c = up[i];
					--i;
				}
				else
					c = down[i];
				if(c != NULL)
					potential += (1/((T)(2.0*d)))*(c->potential);
			}
		}
	}
};

template<class T>
Cell<T,2> * * build_2d_lattice(int x, int y) {
	Cell<T,2> * * lattice = new Cell<T,2>*[x];
	int i_up, i_down, j_up, j_down;
	for(int i=0; i<x; ++i) {
		lattice[i] = new Cell<T,2>[y]();
	}
	for(int i=0; i<x; ++i) {
		i_up = i+1;
		i_down = i-1;
		if(i_up < x)
			for(int j=0; j<y; ++j)
				lattice[i][j].up[1] = & lattice[i_up][j];
		if(i_down >= 0)
			for(int j=0; j<y; ++j)
				lattice[i][j].down[1] = & lattice[i_down][j];
	}
	for(int j=0; j<y; ++j) {
		j_up = j+1;
		j_down = j-1;
		if(j_up < y)
			for(int i=0; i<x; ++i)
				lattice[i][j].up[0] = & lattice[i][j_up];
		if(j_down >= 0)
			for(int i=0; i<x; ++i)
				lattice[i][j].down[0] = & lattice[i][j_down];
	}
	
	return lattice;
}

template<class T>
void delete_2d_lattice(Cell<T,2> * * lattice, int x) {
	if(lattice != NULL) {
		for(int i=0; i<x; ++i)
			if(lattice[i] != NULL)
				delete[] lattice[i];
		delete[] lattice;
	}
}

template<class T>
void update_2d_lattice(Cell<T,2> ** init, Cell<T,2> ** next, int x, int y, T delta[2]) {
	if(init != NULL && next != NULL) {
		for(int i=0; i<x; ++i) {
			if(init[i] != NULL && next[i] != NULL) {
				for(int j=0; j<y; ++j) {
					next[i][j] = Cell<T,2>(init[i][j], delta);
				}
			}
		}
	}
}

template<class T>
void print_2d_lattice(std::ostream & os, Cell<T,2> ** init, int x, int y) {
	if(init != NULL) {
		for(int i=0; i<x; ++i) {
			if(init[i] != NULL) {
				for(int j=0; j<y; ++j) {
					os << i << " " << j << " " << init[i][j].potential << std::endl;
				}
			}
		}
	}
}

template<class T>
void fix_border_2d_lattice(Cell<T,2> * * lattice, int x, int y) {
	if(lattice != NULL) {
		if(lattice[0] != NULL) {
			for(int j=0; j<y; ++j) {
				lattice[0][j].is_const = true;
			}
		}
		if(lattice[x-1] != NULL) {
			for(int j=0; j<y; ++j) {
				lattice[x-1][j].is_const = true;
			}
		}
		for(int i=0; i<x; ++i) {
			if(lattice[i] != NULL) {
				lattice[i][0].is_const = true;
				lattice[i][y-1].is_const = true;
			}
		}
	}
}

int main(int argc, char * argv[]) {
	typedef long double TT;
	int N_Iter = 1000;
	
	if(argc > 1) {
		std::stringstream ss;
		ss << argv[1];
		ss >> N_Iter;
	}
	int x = 101;
	int y = 101;
	TT delta[2];
	delta[0] = 1.0 / x;
	delta[1] = 1.0 / y;
	Cell<TT,2> * * lattice_0 = build_2d_lattice<TT>(x, y);
	Cell<TT,2> * * lattice_1 = build_2d_lattice<TT>(x, y);
	fix_border_2d_lattice(lattice_0, x, y);
	
	TT x0_pos = 0.25;
	int x0_block = x0_pos/delta[0];
	TT x1_pos = 0.75;
	int x1_block = x1_pos/delta[0];
	
	TT y0_pos = 0.2;
	int y0_block = y0_pos/delta[1];
	TT y1_pos = 0.8;
	int y1_block = y1_pos/delta[1];
	
	for(int j=0; j<y; ++j) {
		if(j >= y0_block && j <= y1_block) {
			lattice_0[x0_block][j].is_const = true;
			lattice_0[x0_block][j].potential = 0.2;
			lattice_0[x1_block][j].is_const = true;
			lattice_0[x1_block][j].potential = -0.2;
		}
	}
	
	//print_2d_lattice(std::cout, lattice_0, x, y);
	for(int i=0; i<N_Iter; ++i) {
		if(i%2 == 0) {
			update_2d_lattice(lattice_0, lattice_1, x, y, delta);
			//print_2d_lattice(std::cout, lattice_1, x, y);
		}
		else {
			update_2d_lattice(lattice_1, lattice_0, x, y, delta);
			//print_2d_lattice(std::cout, lattice_0, x, y);
		}
	}
	print_2d_lattice(std::cout, lattice_1, x, y);
	
	return 0;
}