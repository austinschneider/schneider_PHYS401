// sqrt(2)
// Author: Austin Schneider

#include <cmath>
#include <iostream>
#include <iomanip>

const int BISECT_LIMIT = 10000;
const int NEWTON_LIMIT = BISECT_LIMIT;

template <class T> // Function that has root sqrt(2): f(x)=x^2-2
T func_0(T x) {
	return std::pow(x, (T) 2.0) - 2;
}

template <class T> // Derivative of func_0
T deriv_0(T x) {
	return 2.0*x;
}

template <class T>
T newton(T(*func)(T), T(*deriv)(T), T x, T epsilon) {
	T f_x;
	T d_x;
	for(int count=0; count<NEWTON_LIMIT; ++count) {
		f_x = func(x);
		if(std::abs(f_x) < epsilon) {
			std::cout << "Took " << count << " steps with epsilon = " << epsilon << std::endl;
			return x;
		}
		x -= f_x/deriv(x);
	}

}

int main() {
	long double epsilon = 1e-15;

	std::cout << "Newton's method:" << std::endl;

	std::cout << "sqrt(2) = " << std::setprecision(16) << newton<long double>(func_0<long double>, deriv_0<long double>, 1.0, epsilon) << std::endl;

}