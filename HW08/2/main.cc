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

void map_2d_to_1d(int & x, int x0, int x1, int N0, int N1) {
    while(x0 < 0)
        x0 += N0;
    while(x1 < 0)
        x1 += N1;
    while(x0 >= N0)
        x0 -= N0;
    while(x1 >= N1)
        x1 -= N1;
    x = (x0 % N0) + (x1 % N1)*N0;
}

void map_1d_to_2d(int x, int & x0, int & x1, int N0, int N1) {
    x0 = x % N0;
    x1 = x / N0;
}

int * * generate_2d_lookup_table(int N0, int N1) {
    int * * nb = new int *[4];
    for(int i=0; i<4; ++i) {
        nb[i] = new int[N0*N1];
    }
    int t, t0, t1, t2, t3;
    for(int i=0; i<N0; ++i) {
        for(int j=0; j<N1; ++j) {
            map_2d_to_1d(t, i, j, N0, N1);
            map_2d_to_1d(t0, i+1, j, N0, N1);
            map_2d_to_1d(t1, i-1, j, N0, N1);
            map_2d_to_1d(t2, i, j+1, N0, N1);
            map_2d_to_1d(t3, i, j-1, N0, N1);
            nb[0][t] = t0;
            nb[1][t] = t1;
            nb[2][t] = t2;
            nb[3][t] = t3;
        }
    }
    return nb;
}

template <class U>
double flip_rand_spin(Lattice * lat, Observables * obs, double temp, U & functor) {
	int spin = ((double)lat->N)*functor();
	double delta = 0;
	for(int j=0; j<lat->d; ++j) {

		double d = 2.0*((double)lat->spins[spin])*((double)lat->spins[lat->lookup_table[j][spin]]);
        delta += d;
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
	return -1.0 * energy / 2.0;
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

const int E2_index = 2;

void calc_E2_(double & q, Lattice * lat, Observables * obs) {
    q = std::pow(obs->obs[E_index].peek(), 2.0);
}

void up_E2_(double & q, Lattice * lat, Observables * obs, double delta_e, int n) {
    q = std::pow(obs->obs[E_index].peek(), 2.0);
}

const int E2N_index = 3;

void calc_E2N_(double & q, Lattice * lat, Observables * obs) {
    q = obs->obs[E2_index].peek() / lat->N;
}

void up_E2N_(double & q, Lattice * lat, Observables * obs, double delta_e, int n) {
    q = obs->obs[E2_index].peek() / lat->N;
}

const int M_index = 4;

void calc_M_(double & q, Lattice * lat, Observables * obs) {
    q = 0;
    for(int i=0; i<lat->N; ++i) {
        q -= lat->spins[i];
    }
}

void up_M_(double & q, Lattice * lat, Observables * obs, double delta_e, int n) {
    q += lat->spins[n]*2.0;
}

const int MN_index = 5;

void calc_MN_(double & q, Lattice * lat, Observables * obs) {
    q = obs->obs[M_index].peek() / lat->N;
}

void up_MN_(double & q, Lattice * lat, Observables * obs, double delta_e, int n) {
    q = obs->obs[M_index].peek() / lat->N;
}

const int M2_index = 6;

void calc_M2_(double & q, Lattice * lat, Observables * obs) {
    q = std::pow(obs->obs[M_index].peek(), 2.0);
}

void up_M2_(double & q, Lattice * lat, Observables * obs, double delta_e, int n) {
    q = std::pow(obs->obs[M_index].peek(), 2.0);
}

const int M2N_index = 7;

void calc_M2N_(double & q, Lattice * lat, Observables * obs) {
    q = obs->obs[M2_index].peek() / lat->N;
}

void up_M2N_(double & q, Lattice * lat, Observables * obs, double delta_e, int n) {
    q = obs->obs[M2_index].peek() / lat->N;
}

const int M4_index = 8;

void calc_M4_(double & q, Lattice * lat, Observables * obs) {
    q = std::pow(obs->obs[M_index].peek(), 4.0);
}

void up_M4_(double & q, Lattice * lat, Observables * obs, double delta_e, int n) {
    q = std::pow(obs->obs[M_index].peek(), 4.0);
}

template <class U>
double test_temperature(Lattice * lat, Observables * obs, double temp, U & functor, int thermal_steps, int data_steps) {
	randomize_lattice(lat, functor);
	double energy = calc_energy(lat);
	double e_total = 0;
    obs->calculate();
	for(int i=0; i<thermal_steps; ++i) {
        //std::cout << energy << std::endl;
		lattice_sweep(lat, obs, temp, functor, energy);
	}
	for(int i=0; i<data_steps; ++i) {
        //std::cout << energy << std::endl;
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
    double o_var[obs->obs.size()];
	double energy_trials[trials];
	double e_total;
	double mean;
	double var;
	for(double t = t_min; t<=t_max; t += dt) {
		e_total = 0;
		for(int i=0; i<trials; ++i) {
            for(int oo=0; oo<obs->obs.size(); ++oo) obs->obs[oo].measured_quantities.clear();
			energy_trials[i] = test_temperature(lat, obs, t, functor, thermal_steps, data_steps) / ((double) lat->N);
            for(int oo=0; oo<obs->obs.size(); ++oo) o_trials[oo][i] = obs->obs[oo].mean();
			e_total += energy_trials[i];
		}
		mean = e_total / trials;
        for(int oo=0; oo<obs->obs.size(); ++oo) o_means[oo] = utils::mean(o_trials[oo], trials);
		var = 0;
		for(int i=0; i<trials; ++i) {
			var += std::pow(std::abs(mean - energy_trials[i]), 2.0);
		}
		var /= trials;
        for(int oo=0; oo<obs->obs.size(); ++oo) o_var[oo] = std::pow(utils::std_dev(o_trials[oo], trials), 2.0);
		os << t << ", " << mean << ", " << var;
        for(int oo=0; oo<obs->obs.size(); ++oo) {
            os << ", " << o_means[oo] << ", " << o_var[oo];
        }
        os << std::endl;
        
	}
}

int main(int argc, char * argv[]) {
  MTRand_closed mtr;
  mtr.seed(utils::seedgen());
  Lattice lat;
  lat.d = 4;
  int max_pow = 5;

  Observables obs;
  obs.lat = &lat;

  Observable o_E;
  o_E.update_ = up_E_;
  o_E.calculate_ = calc_E_;
  o_E.cpu_save = false;
  obs.add("E", o_E);
  
  Observable o_EN;
  o_EN.update_ = up_EN_;
  o_EN.calculate_ = calc_EN_;
  o_EN.cpu_save = true;
  obs.add("E/N", o_EN);

  Observable o_E2;
  o_E2.update_ = up_E2_;
  o_E2.calculate_ = calc_E2_;
  o_E2.cpu_save = true;
  obs.add("E2", o_E2);
  
  Observable o_E2N;
  o_E2N.update_ = up_E2N_;
  o_E2N.calculate_ = calc_E2N_;
  o_E2N.cpu_save = true;
  obs.add("E2/N", o_E2N);

  Observable o_M;
  o_M.update_ = up_M_;
  o_M.calculate_ = calc_M_;
  o_M.cpu_save = false;
  obs.add("M", o_M);

  Observable o_MN;
  o_MN.update_ = up_MN_;
  o_MN.calculate_ = calc_MN_;
  o_MN.cpu_save = true;
  obs.add("M/N", o_MN);

  Observable o_M2;
  o_M2.update_ = up_M2_;
  o_M2.calculate_ = calc_M2_;
  o_M2.cpu_save = true;
  obs.add("M2", o_M2);

  Observable o_M2N;
  o_M2N.update_ = up_M2N_;
  o_M2N.calculate_ = calc_M2N_;
  o_M2N.cpu_save = true;
  obs.add("M2/N", o_M2N);

  Observable o_M4;
  o_M4.update_ = up_M4_;
  o_M4.calculate_ = calc_M4_;
  o_M4.cpu_save = true;
  obs.add("M4", o_M4);


  int Ls[3] = {16, 24, 32};

  double T_min = 2.2;
  double T_max = 2.3;
  double dT = 0.005;
  double T = T_min;
  if(argc >= 2) {
    std::stringstream ss;
    ss << argv[1];
    ss >> T;
    T = T_min + dT*T;
    std::cout << "Running for T=" << T << std::endl;
  }

  for(int l=0; l<3; ++l) {
    std::stringstream ss;
    ss << "output_";
    int L = Ls[l];
    lat.N = L*L;
    ss << lat.N << "_" << T << ".dat";
    std::fstream out(ss.str().c_str(), std::ios_base::out);
    out << "#temp, E/N mean, E/N var";
    for(int i=0; i<obs.obs.size(); ++i)
        out << ", " << obs.names[i] << " mean, " << obs.names[i] << " var";
    out << std::endl;
    lat.lookup_table = generate_2d_lookup_table(L, L);
    lat.spins = new int[lat.N];
    test_temperature_range(&lat, &obs, mtr, 5000, 600000, 1, T, T, dT, out);
  }
}

