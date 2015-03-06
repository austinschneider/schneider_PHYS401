
#include <cmath>
#include <vector>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>

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

template <class U>
void test_temperature_range(int * spins, int * * lookup_table, U & functor, int N, int d, int thermal_steps, int data_steps, int trials, double t_min, double t_max, double dt, std::ostream & os) {
	std::vector<double> avg_energies;
	std::vector<double> stdev_energies;
	double energy_trials[trials];
	double e_total;
	double mean;
	double stdev;
	for(double t = t_min; t<=t_max; t += dt) {
		e_total = 0;
		for(int i=0; i<trials; ++i) {
			energy_trials[i] = test_temperature(spins, lookup_table, t, functor, N, d, thermal_steps, data_steps) / ((double) N);
			e_total += energy_trials[i];
		}
		mean = e_total / trials;
		stdev = 0;
		for(int i=0; i<trials; ++i) {
			stdev += std::pow(std::abs(mean - energy_trials[i]), 2.0);
		}
		stdev /= trials;
		os << t << ", " << mean << ", " << stdev << std::endl;
	}
}

int main() {
	MTRand_closed mtr;
	mtr.seed(seedgen());
	
	int d = 2;
    int max_pow = 3;
    for(int k=1; k<=max_pow; ++k) {
        std::stringstream ss;
        ss << "output_";
        int N = std::pow(10.0, (double)k);
        ss << N << ".dat";
        std::fstream out(ss.str().c_str(), std::ios_base::out);
        out << "#temp, E/N mean, E/N stdev" << std::endl;
	    int * * nb = generate_1d_lookup_table(N);
	    int spins[N];	
	    test_temperature_range(spins, nb, mtr, N, d, 3000, 3000, 10, 0.5, 5.0, 0.5, out);
    }
}
