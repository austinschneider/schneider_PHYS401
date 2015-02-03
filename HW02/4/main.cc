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

template <class T>
Point<T, 1> r4_pot_force(State<T, 1> s) {
	Point<T, 1> f;
	f.x[0] = ((T) -1.0) * std::pow((T) s.x[0], (T) 3.0);
	return f;
}

int main() {
	const long double time_step = 1e-5;
	State<long double, 1> init;
	init.x[0] = 0;
	init.v[0] = 10;
	init.t = 0;
	int swaps = 0;
	bool is_neg = false;
	
	std::fstream rk4_file("orbit_rk4.dat", std::ios_base::out);
	State<long double, 1> s = init;
	rk4_file << "# RK4: t, x, v" << std::endl;
	rk4_file << s << std::endl;
	for(int i=0; i<1e6; ++i) {
		s = RK4_step<long double, 1>(r4_pot_force, s, time_step);
		rk4_file << s << std::endl;
		if((s.x[0] < 0) ^ is_neg) {
				is_neg = !is_neg;
				++swaps;
				if(swaps >= 2)
					break;
		}
	}
	
	swaps = 0;
	is_neg = false;
	
	std::fstream verlet_file("orbit_verlet.dat", std::ios_base::out);
	verlet_file << "# Verlet: t, x, v" << std::endl;
	s = init;
	verlet_file << s << std::endl;
	s = verlet_v_step<long double, 1>(r4_pot_force, s, time_step / ((long double) 2.0));
	for(int i=0; i<1e6; ++i) {
		s = verlet_full_step<long double, 1>(r4_pot_force, s, time_step);
		s.t += time_step;
		verlet_file << s << std::endl;
		if((s.x[0] < 0) ^ is_neg) {
				is_neg = !is_neg;
				++swaps;
				if(swaps >= 2)
					break;
		}
	}
	s = verlet_x_step<long double, 1>(r4_pot_force, s, time_step);
	s = verlet_v_step<long double, 1>(r4_pot_force, s, time_step / ((long double) 2.0));
	verlet_file << s << std::endl;
	
}
