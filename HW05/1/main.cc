#include <iostream>
#include <iomanip>
#include <fstream>

#include "../Pre/integrate.h"

template <class T>
T I(T x) {
	return 1/(((T)1.0)+x);
}

int main() {
	double I_actual = 0.6931471805599453094172321;
	double epsilon  = 0.0000000001;
	double I_result = 0;
	double error = 0;
	double h = 0;
	double i = 0;

	std::fstream f_trap("trapezium.dat", std::ios_base::out);
	std::cout << "---Testing Trapezium Method---" << std::endl;
	i = 0;
	while(true) {
		I_result = trap<double>(I<double>,std::pow(2.0,i),0.0,1.0);
		error = std::abs(I_result-I_actual);
		std::cout << "Result: " << std::setprecision(15) << I_result << std::endl;
		f_trap << (1.0/std::pow(2.0,i)) << " " << error << std::endl;

		if(error < epsilon) {
			std::cout << "Success at n=2^" << i << "\nError: " << std::setprecision(15) << error << std::endl;
			break;
		}
		++i;
	}

	std::cout << std::endl;

	std::fstream f_simpson("simpson.dat", std::ios_base::out);
	std::cout << "---Testing Simpson's Method---" << std::endl;
	i = 0;
	while(true) {
		I_result = simpson<double>(I<double>,std::pow(2.0,i),0.0,1.0);
		error = std::abs(I_result-I_actual);
		std::cout << "Result: " << std::setprecision(15) << I_result << std::endl;
		f_simpson << (1.0/std::pow(2.0,i)) << " " << error << std::endl;

		if(error < epsilon) {
			std::cout << "Success at n=2^" << i << "\nError: " << std::setprecision(15) << error << std::endl;
			break;
		}
		++i;
	}
}