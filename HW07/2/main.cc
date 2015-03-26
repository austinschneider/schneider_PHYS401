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

const int E_index = 0;

void calc_E_1d_(double & q, Lattice * lat, Observables * obs) {
    q = calc_energy_1d(lat);
}

void up_E_1d_(double & q, Lattice * lat, Observables * obs, double delta_e, int n) {
    q += delta_e;
}

const int EN_index = 1;

void calc_EN_1d_(double & q, Lattice * lat, Observables * obs) {
    q = obs->obs[E_index].peek() / lat->N;
}

void up_EN_1d_(double & q, Lattice * lat, Observables * obs, double delta_e, int n) {
    q = obs->obs[E_index].peek() / lat->N;
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
	return e_total / ((double)data_steps);
}

template <class U>
void test_temperature_range(Lattice * lat, Observables * obs, U & functor, int thermal_steps, int data_steps, int trials, double t_min, double t_max, double dt, std::ostream & os) {
    double o_trials[obs->obs.size()][trials];
    double o_means[obs->obs.size()];
    double o_stdev[obs->obs.size()];
	double energy_trials[trials];
	double e_total;
	double mean;
	double stdev;
	for(double t = t_min; t<=t_max; t += dt) {
		e_total = 0;
		for(int i=0; i<trials; ++i) {
            for(int oo=0; oo<obs->obs.size(); ++oo) obs->obs[oo].measured_quantities.clear();
			energy_trials[i] = test_temperature(lat, obs, t, functor, thermal_steps, data_steps) / ((double) lat->N);
            for(int oo=0; oo<obs->obs.size(); ++oo) o_trials[oo][i] = obs->obs[oo].mean();
            std::cout << "Diff: " << o_trials[1][i] - energy_trials[i] << std::endl;
			e_total += energy_trials[i];
		}
		mean = e_total / trials;
        for(int oo=0; oo<obs->obs.size(); ++oo) o_means[oo] = utils::mean(o_trials[oo], trials);
		stdev = 0;
		for(int i=0; i<trials; ++i) {
			stdev += std::pow(std::abs(mean - energy_trials[i]), 2.0);
		}
		stdev /= trials;
        stdev = sqrt(stdev);
        for(int oo=0; oo<obs->obs.size(); ++oo) o_stdev[oo] = utils::std_dev(o_trials[oo], trials);
		os << t << ", " << mean << ", " << stdev;
        for(int oo=0; oo<obs->obs.size(); ++oo) {
            os << ", " << o_means[oo] << ", " << o_stdev[oo];
        }
        os << std::endl;
        
	}
}

int main() {
  MTRand_closed mtr;
  mtr.seed(utils::seedgen());
  Lattice lat;
  lat.d = 2;
  int max_pow = 3;

  Observables obs;
  obs.lat = &lat;
  Observable o_E;
  o_E.update_ = up_E_1d_;
  o_E.calculate_ = calc_E_1d_;
  obs.add("E", o_E);
  
  Observable o_EN;
  o_EN.update_ = up_EN_1d_;
  o_EN.calculate_ = calc_EN_1d_;
  obs.add("E/N", o_EN);

  for(int k=1; k<=max_pow; ++k) {
    std::stringstream ss;
    ss << "output_";
    lat.N = std::pow(10.0, (double)k);
    ss << lat.N << ".dat";
    std::fstream out(ss.str().c_str(), std::ios_base::out);
    out << "#temp, E/N mean, E/N stdev";
    for(int i=0; i<obs.obs.size(); ++i)
        out << ", " << obs.names[i] << " mean, " << obs.names[i] << " stdev";
    out << std::endl;
    lat.lookup_table = generate_1d_lookup_table(lat.N);
    lat.spins = new int[lat.N];
    test_temperature_range(&lat, &obs, mtr, 3000, 3000, 10, 0.5, 5.0, 0.5, out);
  }
}

