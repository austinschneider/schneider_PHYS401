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

	std::cout << std::endl;

	std::fstream f_romberg("romberg.dat", std::ios_base::out);
	std::cout << "---Testing Romberg's Method---" << std::endl;
	I_result = romberg<double>(I<double>,epsilon,0.0,1.0);
	error = std::abs(I_result-I_actual);
	std::cout << "Result: " << std::setprecision(15) << I_result << std::endl;
    std::cout << "Error: " << std::setprecision(15) << error << std::endl;
}

