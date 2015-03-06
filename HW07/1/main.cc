
#include <cmath>
#include <vector>
#include <fstream>
#include <iomanip>
#include <iostream>

#include <string.h>
#include <unistd.h>

#include "mtrand.h"

long seedgen() {
    long int    seconds;
    return ((time(&seconds)*181)*((getpid() - 83)*359))%104729;
}

template <class T, class U>
T random_spin(U & functor) {
	if(functor() >= 0.5)
        return 1;
    else
        return -1;
}

int * * generate_1d_lookup_table(int N) {
	int * * nb = new int *[2];
	for(int i=0; i<2; ++i) {
		nb[i] = new int[N];
	}
    nb[0][0] = N-1;
    nb[1][0] = 1;
    nb[0][N-1] = N-2;
    nb[1][N-1] = 0;
    for(int i=1; i<N-1; ++i) {
        nb[0][i] = i-1;
        nb[1][i] = i+1;
    }
	return nb;
}

template <class U>
double flip_rand_spin(int * spins, int * * lookup_table, double temp, U & functor, int N, int d) {
	int spin = ((double)N)*functor();
	double delta = 0;
	for(int j=0; j<d; ++j) {
		delta += 2.0*((double)spins[spin])*((double)spins[lookup_table[j][spin]]);
	}
	
	if(functor() < std::exp(-1.0 * delta / temp)) {
		spins[spin] = -spins[spin];
		return delta;
	}
	else {
		return 0.0;
	}
}

template <class U>
void randomize_lattice(int * spins, U & functor, int N) {
	for(int i=0; i<N; ++i) {
		spins[i] = random_spin<int>(functor);
	}
}

template <class U>
void lattice_sweep(int * spins, int * * lookup_table, double temp, U & functor, int N, int d, double & energy) {
	for(int i=0; i<N; ++i) {
		energy += flip_rand_spin(spins, lookup_table, temp, functor, N, d);
	}
}

double calc_energy_1d(int * spins, int * * lookup_table, int N, int d) {
	double energy = 0;
	for(int i=0; i<N; ++i) {
		energy += spins[i]*spins[lookup_table[0][i]];
		energy += spins[i]*spins[lookup_table[1][i]];
	}
	return -1.0 * energy / 2.0;
}

template <class U>
double test_temperature(int * spins, int * * lookup_table, double temp, U & functor, int N, int d, int thermal_steps, int data_steps) {
	randomize_lattice(spins, functor, N);
	double energy = calc_energy_1d(spins, lookup_table, N, d);
	double e_total = 0;
	for(int i=0; i<thermal_steps; ++i) {
		lattice_sweep(spins, lookup_table, temp, functor, N, d, energy);
	}
	for(int i=0; i<data_steps; ++i) {
		lattice_sweep(spins, lookup_table, temp, functor, N, d, energy);
		e_total += energy;
	}
	return e_total / ((double)data_steps);
}

int main() {
	MTRand_closed mtr;
	mtr.seed(seedgen());
	std::fstream out("output.dat", std::ios_base::out);
	
	int d = 2;
    int N = 10;
	int * * nb = generate_1d_lookup_table(10);
	int spins[N];
	
	for(int i=0; i<10; ++i)
		std::cout << test_temperature(spins, nb, 1000, mtr, N, d, 3000, 3000) << std::endl;
}
