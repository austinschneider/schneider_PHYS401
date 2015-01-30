// Ballistic Trajectory
// Author: Austin Schneider

#include <cmath>
#include <iostream>
#include <fstream>

const int EULER_LIMIT = 100000;
const double EULER_TIMESTEP = 0.01;

template <class T>
struct Point {
	T x;
	T y;
	template <class U> friend std::ostream& operator<<(std::ostream& os, const Point<U>& p);
};

template <class T>
std::ostream& operator<<(std::ostream& os, const Point<T>& p) {
	os << "(" << p.x << ", " << p.y << ")";
	return os;
}

template <class T>
bool below_y(Point<T> r, Point<T> v) {
	return r.y <= 0.0;
}

template <class T>
Point<T> earth_gravity(Point<T> r, Point<T> v) {
	Point<T> f;
	f.x = 0;
	f.y = -1;
	return f;
}

template <class T>
void euler_simulate(Point<T> r_0, Point<T> v_0, Point<T>(*force)(Point<T>, Point<T>), bool(*terminate)(Point<T>, Point<T>), T step_size, std::ostream & os) {
	Point<T> r = r_0;
	Point<T> v = v_0;
	Point<T> f;
	os << "# x\ty" << std::endl;
	for(int count=0; count<EULER_LIMIT; ++count) {
		f = force(r, v);
		os << r.x << " " << r.y << std::endl;
		if(terminate(r, v)) {
			std::cout << "After " << count << " steps of " << EULER_TIMESTEP << ":" << std::endl;
			std::cout << "Position: " << r << std::endl;
			std::cout << "Velocity: " << v << std::endl;
			std::cout << "Force:    " << f << std::endl;
			return;
		}

		r.x = r.x + step_size*v.x;
		r.y = r.y + step_size*v.y;

		v.x = v.x + step_size*f.x;
		v.y = v.y + step_size*f.y;
	}
	std::cout << "Hit limit of " << EULER_LIMIT << " timesteps of " << EULER_TIMESTEP << ": " << std::endl;
	std::cout << "Position: " << r << std::endl;
	std::cout << "Velocity: " << v << std::endl;
	std::cout << "Force:    " << f << std::endl;

}

template <class T>
void ballistic(T x0, T y0, T v0, T angle, std::ostream & os) {
	Point<T> r_0, v_0;
	r_0.x = x0;
	r_0.y = y0;
	v_0.x = v0 * std::cos(M_PI * (angle / 180.0));
	v_0.y = v0 * std::sin(M_PI * (angle / 180.0));

	euler_simulate(r_0, v_0, earth_gravity, below_y, EULER_TIMESTEP, os);
}

int main() {

	std::fstream traj_a45("./traj_x0_y1_v1_a45.dat", std::ios_base::out);
	ballistic<double>(0, 1, 1, 45, traj_a45);

	std::fstream traj_a0("./traj_x0_y1_v1_a0.dat", std::ios_base::out);
	ballistic<double>(0, 1, 1, 0, traj_a0);

	return 0;
}









