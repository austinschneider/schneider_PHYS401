
#include <cmath>
#include <vector>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>

#include <string.h>
#include <unistd.h>

#include "../../Global/utils.h"
#include "lattice.h"
#include "observables.h"
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
double flip_rand_spin(Lattice * lat, Observables * obs, double temp, U & functor) {
	int spin = ((double)lat->N)*functor();
	double delta = 0;
	for(int j=0; j<lat->d; ++j) {
		delta += 2.0*((double)lat->spins[spin])*((double)lat->spins[lat->lookup_table[j][spin]]);
	}

	if(functor() < std::exp(-1.0 * delta / temp)) {
    obs->update(delta, spin);
		lat->spins[spin] = -lat->spins[spin];
    return delta;
	}
  else {
    return 0.0;
  }
}

template <class U>
void randomize_lattice(Lattice * lat, U & functor) {
	for(int i=0; i<lat->N; ++i) {
		lat->spins[i] = random_spin<int>(functor);
	}
}

template <class U>
void lattice_sweep(Lattice * lat, Observables * obs, double temp, U & functor, double & energy) {
	for(int i=0; i<lat->N; ++i) {
		energy += flip_rand_spin(lat, obs, temp, functor);
	}
}

double calc_energy_1d(Lattice * lat) {
	double energy = 0;
	for(int i=0; i<lat->N; ++i) {
		energy += lat->spins[i]*lat->spins[lat->lookup_table[0][i]];
		energy += lat->spins[i]*lat->spins[lat->lookup_table[1][i]];
	}
	return -1.0 * energy / 2.0;
}

template <class U>
double test_temperature(Lattice * lat, Observables * obs, double temp, U & functor, int thermal_steps, int data_steps) {
	randomize_lattice(lat, functor);
	double energy = calc_energy_1d(lat);
	double e_total = 0;
  obs->calculate();
	for(int i=0; i<thermal_steps; ++i) {
		lattice_sweep(lat, obs, temp, functor, energy);
	}
	for(int i=0; i<data_steps; ++i) {
		lattice_sweep(lat, obs, temp, functor, energy);
    obs->measure();
		e_total += energy;
	}
  obs->average();
	return e_total / ((double)data_steps);
}

template <class U>
void test_temperature_range(Lattice * lat, Observables * obs, U & functor, int thermal_steps, int data_steps, int trials, double t_min, double t_max, double dt, std::ostream & os) {
	std::vector<double> avg_energies;
	std::vector<double> stdev_energies;
  std::vector<Observables> o_total;
  Observables o_empty = *obs;
	double energy_trials[trials];
	double e_total;
	double mean;
	double stdev;
	for(double t = t_min; t<=t_max; t += dt) {
		e_total = 0;
    o_total.push_back(o_empty);
		for(int i=0; i<trials; ++i) {
			energy_trials[i] = test_temperature(lat, obs, t, functor, thermal_steps, data_steps) / ((double) lat->N);
      o_total[o_total.size()] += *obs;
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
  Lattice lat;
	lat.d = 2;
  int max_pow = 3;

  Observables obs;
  Observable<double> o_energy;

  for(int k=1; k<=max_pow; ++k) {
    std::stringstream ss;
    ss << "output_";
    lat.N = std::pow(10.0, (double)k);
    ss << lat.N << ".dat";
    std::fstream out(ss.str().c_str(), std::ios_base::out);
    out << "#temp, E/N mean, E/N stdev" << std::endl;
    lat.lookup_table = generate_1d_lookup_table(lat.N);
    lat.spins = new int[lat.N];
    test_temperature_range(&lat, &obs, mtr, 3000, 3000, 10, 0.5, 5.0, 0.5, out);
  }
}
