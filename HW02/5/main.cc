// Anharmonic Oscillator
// Author: Austin Schneider

#include <cmath>
#include <iostream>
#include <fstream>

template <class T, int d>
struct Point {
	T x[d];
	template <class U, int D> friend std::ostream& operator<<(std::ostream& os, const Point<U, D>& p);
};

template <class T, int d>
std::ostream& operator<<(std::ostream& os, const Point<T, d>& p) {
	os << "(";
	if(d >0)
		os << p.x[0];
	for(int i=1; i<d; ++i)
		os << ", " << p.x[i];
	os << ")";
	return os;
}

template <class T, int d>
struct State {
	T t;
	T x[d];
	T v[d];
	template <class U, int D> friend std::ostream& operator<<(std::ostream& os, const State<U, D>& p);
};

template <class T, int d>
T energy(State<T, d> state, T(*potential)(State<T, d>)) {
	T e = 0;

	for(int i=0; i<d; ++i) {
		e += std::pow(state.v[i], (T) 2.0) / ((T) 2.0);
	}
	e += potential(state);;
	return e;
}

template <class T, int d>
std::ostream& operator<<(std::ostream& os, const State<T, d>& s) {
	os << s.t << " ";
	
	if(d >0)
		os << s.x[0];
	for(int i=1; i<d; ++i)
		os << " " << s.x[i];
	
	os << " ";
	
	if(d >0)
		os << s.v[0];
	for(int i=1; i<d; ++i)
		os << " " << s.v[i];
	return os;
}


template <class T, int d> // Runge-Kutta 4th order,  d-dimensional
State<T, d> RK4_step(Point<T, d>(*func)(State<T, d>), State<T, d> s, T h) {
	const int RK_ORDER = 4;

	const T h2 = h/((T)2.0);
	const T h3 = h/((T)3.0);
	const T h6 = h/((T)6.0);
	
	T kx[RK_ORDER][d];
	T kp[RK_ORDER][d];
	State<T, d> s_tmp;
	Point<T, d> f;
	
	s_tmp = s;
	
	for(int i=0; i<d; ++i)
		kx[0][i] = s.v[i];
	f = func(s_tmp);
	for(int i=0; i<d; ++i)
		kp[0][i] = f.x[i];
	
	s_tmp.t = s.t + h2;
	
	for(int i=0; i<d; ++i) {
		kx[1][i] = s.v[i] + h2*kp[0][i];
		s_tmp.v[i] = kx[1][i];
		s_tmp.x[i] = s.x[i] + h2*kx[0][i];
	}
	f = func(s_tmp);
	for(int i=0; i<d; ++i)
		kp[1][i] = f.x[i];
	
	for(int i=0; i<d; ++i) {
		kx[2][i] = s.v[i] + h2*kp[1][i];
		s_tmp.v[i] = kx[2][i];
		s_tmp.x[i] = s.x[i] + h2*kx[1][i];
	}
	f = func(s_tmp);
	for(int i=0; i<d; ++i)
		kp[2][i] = f.x[i];
	
	s_tmp.t = s.t + h;
	
	for(int i=0; i<d; ++i) {
		kx[3][i] = s.v[i] + h*kp[2][i];
		s_tmp.v[i] = kx[3][i];
		s_tmp.x[i] = s.x[i] + h*kx[2][i];
	}
	f = func(s_tmp);
	for(int i=0; i<d; ++i)
		kp[3][i] = f.x[i];
	
	
	for(int i=0; i<d; ++i) {
		s_tmp.x[i] = s.x[i] + h6*kx[0][i] + h3*kx[1][i] + h3*kx[2][i] + h6*kx[3][i];
		s_tmp.v[i] = s.v[i] + h6*kp[0][i] + h3*kp[1][i] + h3*kp[2][i] + h6*kp[3][i];
	}
	
	return s_tmp;
}

template <class T, int d> // Velocity verlet
State<T, d> verlet_x_step(Point<T, d>(*func)(State<T, d>), State<T, d> s, T h) {
	for(int i=0; i<d; ++i) {
		s.x[i] = s.x[i] + h*s.v[i];
	}
	return s;
}

template <class T, int d> // Velocity verlet
State<T, d> verlet_v_step(Point<T, d>(*func)(State<T, d>), State<T, d> s, T h) {
	Point<T, d> f = func(s);
	for(int i=0; i<d; ++i) {
		s.v[i] = s.v[i] + h*f.x[i];
	}
	return s;
}

template <class T, int d> // Velocity verlet
State<T, d> verlet_full_step(Point<T, d>(*func)(State<T, d>), State<T, d> s, T h) {
	s = verlet_x_step(func, s, h);
	return verlet_v_step(func, s, h);
}

template <class T, int d>
void do_verlet(std::ostream & os, Point<T, d>(*force)(State<T, d>), T(*potential)(State<T, d>), State<T, d> init, T time_step, int n_steps) {
	int swaps = 0;
	bool is_neg = false;
	os << "# Verlet: t, x, v, e" << std::endl;
	State<T, d> s = init;
	os << s << " " << energy(s, potential) << std::endl;
	s = verlet_v_step<T, d>(force, s, time_step / ((T) 2.0));
	for(int i=0; i<n_steps; ++i) {
		s = verlet_full_step<T, d>(force, s, time_step);
		s.t += time_step;
		os << s << " " << energy(s, potential) << std::endl;
	}
	s = verlet_x_step<T, d>(force, s, time_step);
	s = verlet_v_step<T, d>(force, s, time_step / ((T) 2.0));
	os << s << " " << energy(s, potential) << std::endl;
}

template <class T, int d>
void do_rk4(std::ostream & os, Point<T, d>(*force)(State<T, d>), T(*potential)(State<T, d>), State<T, d> init, T time_step, int n_steps) {
	int swaps = 0;
	bool is_neg = false;
	os << "# RK4: t, x, v, e" << std::endl;
	State<T, d> s = init;
	os << s << " " << energy(s, potential) << std::endl;
	for(int i=0; i<n_steps; ++i) {
		s = RK4_step<T, d>(force, s, time_step);
		os << s << " " << energy(s, potential) << std::endl;
	}
}

template <class T, int d>
Point<T, d> kepler_r2_force(State<T, d> s) {
	Point<T, d> f;
	T r =0;
	for(int i=0; i<d; ++i) {
		r += s.x[i] * s.x[i];
	}
	r = std::sqrt(r);
	T r3 = std::pow(r, (T) 3.0);
	for(int i=0; i<d; ++i) {
		f.x[i] = ((T) -1.0) * s.x[i] / r3;
	}
	return f;
}

template <class T, int d>
T kepler_r2_pot_energy(State<T, d> s) {
	T r = 0;
	for(int i=0; i<d; ++i) {
		r += s.x[i] * s.x[i];
	}
	r = std::sqrt(r);
	return ((T) -1.0) / r;
}

int main() {
	const double time_step = 1e-3 * 75.6;
	State<double, 2> init;
	init.x[0] = 10;
	init.x[1] = 0;
	init.v[0] = 0;
	init.v[1] = 0.1;
	init.t = 0;

	const int n_steps = (756) / time_step;
	
	
	std::fstream verlet_file("2d_orbit_verlet.dat", std::ios_base::out);
	do_verlet<double, 2>(verlet_file, kepler_r2_force, kepler_r2_pot_energy, init, time_step, n_steps);
	std::fstream rk4_file("2d_orbit_rk4.dat", std::ios_base::out);
	do_rk4<double, 2>(rk4_file, kepler_r2_force, kepler_r2_pot_energy, init, time_step, n_steps);
	
}
