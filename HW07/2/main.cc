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

double calc_energy(Lattice * lat) {
	double energy = 0;
	for(int i=0; i<lat->d; ++i) {
        for(int j=0; j<lat->N; ++j) {
		    energy += lat->spins[i]*lat->spins[lat->lookup_table[i][j]];
        }
	}
	return -1.0 * energy / lat->d;
}

const int E_index = 0;

void calc_E_(double & q, Lattice * lat, Observables * obs) {
    q = calc_energy(lat);
}

void up_E_(double & q, Lattice * lat, Observables * obs, double delta_e, int n) {
    q += delta_e;
}

const int EN_index = 1;

void calc_EN_(double & q, Lattice * lat, Observables * obs) {
    q = obs->obs[E_index].peek() / lat->N;
}

void up_EN_(double & q, Lattice * lat, Observables * obs, double delta_e, int n) {
    q = obs->obs[E_index].peek() / lat->N;
}

const int M_index = 2;

void calc_M_(double & q, Lattice * lat, Observables * obs) {
    q = 0;
    for(int i=0; i<lat->N; ++i) {
        q += lat->spins[i];
    }
}

void up_M_(double & q, Lattice * lat, Observables * obs, double delta_e, int n) {
    q += lat->spins[n]*2.0;
}

const int MN_index = 3;

void calc_MN_(double & q, Lattice * lat, Observables * obs) {
    q = obs->obs[M_index].peek() / lat->N;
}

void up_MN_(double & q, Lattice * lat, Observables * obs, double delta_e, int n) {
    q = obs->obs[M_index].peek() / lat->N;
}

const int M2_index = 4;

void calc_M2_(double & q, Lattice * lat, Observables * obs) {
    q = std::pow(obs->obs[M_index].peek(), 2.0);
}

void up_M2_(double & q, Lattice * lat, Observables * obs, double delta_e, int n) {
    q = std::pow(obs->obs[M_index].peek(), 2.0);
}

const int M2N_index = 5;

void calc_M2N_(double & q, Lattice * lat, Observables * obs) {
    q = obs->obs[M2_index].peek() / lat->N;
}

void up_M2N_(double & q, Lattice * lat, Observables * obs, double delta_e, int n) {
    q = obs->obs[M2_index].peek() / lat->N;
}

template <class U>
double test_temperature(Lattice * lat, Observables * obs, double temp, U & functor, int thermal_steps, int data_steps) {
	randomize_lattice(lat, functor);
	double energy = calc_energy(lat);
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
    Observables o_trials[trials];
	double energy_trials[trials];
	double e_total;
	double mean;
	double var;
	for(double t = t_min; t<=t_max; t += dt) {
		e_total = 0;
		for(int i=0; i<trials; ++i) {
            o_trials[i] = *obs;
			energy_trials[i] = test_temperature(lat, &(o_trials[i]), t, functor, thermal_steps, data_steps) / ((double) lat->N);
			e_total += energy_trials[i];
        }
        for(int i=0; i<data_steps; ++i) {
            os << i;
            for(int oo=0; oo<obs->obs.size(); ++oo) {
                double total = 0;
                for(int tr=0; tr<trials; ++tr) {
                    total += o_trials[tr].obs[oo].measured_quantities[i];
                }
                total /= trials;
                os << ", " << total;
            }
            os << std::endl;
        }
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
  o_E.update_ = up_E_;
  o_E.calculate_ = calc_E_;
  obs.add("E", o_E);
  
  Observable o_EN;
  o_EN.update_ = up_EN_;
  o_EN.calculate_ = calc_EN_;
  obs.add("E/N", o_EN);

  Observable o_M;
  o_M.update_ = up_M_;
  o_M.calculate_ = calc_M_;
  obs.add("M", o_M);

  Observable o_MN;
  o_MN.update_ = up_MN_;
  o_MN.calculate_ = calc_MN_;
  obs.add("M/N", o_MN);

  Observable o_M2;
  o_M2.update_ = up_M2_;
  o_M2.calculate_ = calc_M2_;
  obs.add("M2", o_M2);

  Observable o_M2N;
  o_M2N.update_ = up_M2N_;
  o_M2N.calculate_ = calc_M2N_;
  obs.add("M2/N", o_M2N);

  for(int k=1; k<=max_pow; ++k) {
    std::stringstream ss;
    ss << "output_";
    lat.N = std::pow(10.0, (double)k);
    ss << lat.N << ".dat";
    std::fstream out(ss.str().c_str(), std::ios_base::out);
    out << "#t";
    for(int i=0; i<obs.obs.size(); ++i)
        out << ", " << obs.names[i] << " mean";
    out << std::endl;
    lat.lookup_table = generate_1d_lookup_table(lat.N);
    lat.spins = new int[lat.N];
    test_temperature_range(&lat, &obs, mtr, 0, 6000, 100, 0.5, 0.5, 0.5, out);
  }
}

