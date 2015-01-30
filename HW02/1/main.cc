//Bisection and Newton Method
// Author: Austin Schneider

#include <cmath>
#include <iostream>

const int BISECT_LIMIT = 10000;
const int NEWTON_LIMIT = BISECT_LIMIT;

template <class T> // Function to perform bisection on: f(x)=x^3-4x+2
T func_0(T x) {
	return std::pow(x, 3.0) - 4*x + 2;
}

template <class T> // Derivative of func_0
T deriv_0(T x) {
	return 3.0*std::pow(x, 2.0) - 4;
}

template <class T> // Performs bisection on a function given a bracket and an epsilon bound for convergence
T bisect(T(*func)(T), T a, T b, T epsilon) {
	T f_a = func(a);
	T f_b = func(b);
	T f_c;
	T c;
	T zero;
	bool sign_a = f_a < 0;
	bool sign_b = f_b < 0;
	bool sign_c;
	bool is_in_bounds;

	int count = 0;
	if(sign_a^sign_b) {
		for(int count=0; count<BISECT_LIMIT; ++count) {
			c = (a + b) / 2.0;
			f_c = func(c);
			sign_c = f_c < 0;
			if(sign_a^sign_c) {
				b = c;
				f_b = f_c;
			}
			else if(sign_b^sign_c) {
				a = c;
				f_a = f_c;
			}
			if(std::abs(f_c) < epsilon) {
				zero = c;
				std::cout << "Took " << count << " steps with epsilon = " << epsilon << std::endl;
				return zero;
			}
		}
	}
	return -1;
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
	double epsilon = 1e-4;

	std::cout << "Bisection method:" << std::endl;

	std::cout << "zero: " << bisect<double>(func_0<double>, -3.0, -2.0, epsilon) << std::endl;
	std::cout << "zero: " << bisect<double>(func_0<double>, 0.0, 1.0, epsilon) << std::endl;
	std::cout << "zero: " << bisect<double>(func_0<double>, 1.0, 2.0, epsilon) << std::endl;

	std::cout << "Newton's method:" << std::endl;

	epsilon = 1e-14;
	std::cout << "zero: " << newton<double>(func_0<double>, deriv_0<double>, -2.0, epsilon) << std::endl;
	std::cout << "zero: " << newton<double>(func_0<double>, deriv_0<double>, 0.0, epsilon) << std::endl;
	std::cout << "zero: " << newton<double>(func_0<double>, deriv_0<double>, 2.0, epsilon) << std::endl;

}