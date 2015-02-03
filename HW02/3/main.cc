// sqrt(2)
// Author: Austin Schneider

#include <cmath>
#include <iostream>
#include <iomanip>

const int SECANT_LIMIT = 10000;
const int NEWTON_LIMIT = SECANT_LIMIT;
const int FIXED_POINT_LIMIT = SECANT_LIMIT;

template <class T> // Function that has root sqrt(2): f(x)=x^2-2
T func_0(T x) {
	return std::pow(x, (T) 2.0) - 2;
}

template <class T> // Derivative of func_0
T deriv_0(T x) {
	return 2.0*x;
}

template <class T> // Fixed point root approximation
T root_iteration(T a, T x) {
    return ((T) 0.5) * (a / x + x);
}

template <class T> // Sqrt(2) fixed point approximation
T root2_iteration(T x) {
    return root_iteration((T) 2.0, x);
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

template <class T>
T secant(T(*func)(T), T x0, T x1, T epsilon) {
	T f_x0 = func(x0);
	T f_x1 = func(x1);
	T x2;
	for(int count=0; count<SECANT_LIMIT; ++count) {
		x2 = (x0*f_x1 - x1*f_x0) / (f_x1 - f_x0);

		x0 = x1;
		x1 = x2;
		f_x0 = f_x1;
		f_x1 = func(x2);

		if(std::abs(f_x1) < epsilon) {
			std::cout << "Took " << count + 1 << " steps with epsilon = " << epsilon << std::endl;
			return x1;
		}
	}
}

template <class T>
T fixed_point_iteration(T(*func)(T), T x, T epsilon) {
    T prev;

    for(int count=0; count<FIXED_POINT_LIMIT; ++count) {
        prev = x;
        x = func(x);
        if(std::abs(prev - x) < epsilon) {
            std::cout << "Took " << count << " steps with epsilon = " << epsilon << std::endl;
            return x;
        }
    }
}

int main() {
	long double epsilon = 1e-15;

	std::cout << "Newton's method:" << std::endl;
	std::cout << "sqrt(2) = " << std::setprecision(16) << newton<long double>(func_0<long double>, deriv_0<long double>, 1.0, epsilon) << std::endl;

	std::cout << "Secant method:" << std::endl;
	std::cout << "sqrt(2) = " << std::setprecision(16) << secant<long double>(func_0<long double>, 1.0, 1.1, epsilon) << std::endl;

    std::cout << "Fixed point method:" << std::endl;
    std::cout << "sqrt(2) = " << std::setprecision(16) << fixed_point_iteration<long double>(root2_iteration<long double>, 1.0, epsilon) << std::endl;

}
