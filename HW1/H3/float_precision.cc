#include <cmath>
#include <fstream>
#include <iostream>

template <class T> // 1/n
T func_0(int n) {
	return (T) (1.0 / n);
}

template <class T> // Taylor series terms for ln(2), alternating series of 1/n
T func_1(int n) {
	return ((T) std::pow((double)-1, (double) n + 1)) / ((T) n);
}

template <class T> // Sum series from n=1 to n=10^power
T s_up(int power, T(*func)(int) = func_0) {
	int max = std::pow((double)10, (double)power);
	T sum = 0;
	for(int n = 1; n <= max; ++n) {
		sum += func(n);
	}
	return sum;
}

template <class T> // Sum series from n=10^power to n=1
T s_down(int power, T(*func)(int) = func_0) {
	int max = std::pow((double)10, (double)power);
	T sum = 0;
	for(int n = max; n >= 1; --n) {
		sum += func(n);
	}
	return sum;
}

template <class T, class U> // Print difference between two functions for a range [min, max]
void print_diff(T(*f_0)(int, T(*)(int)), U(*f_1)(int, U(*)(int)), T(*ff_0)(int), U(*ff_1)(int), int min, int max, std::ostream & out) {
	out << "# power\tdiff" << std::endl;
	for(int power = min; power <= max; ++power) {
		out << power << "\t" << std::abs(f_0(power, ff_0) - f_1(power, ff_1)) << std::endl;
	}
}

template <class T> // Test the precision of s_up vs s_down
void test_precision(std::ostream & out) {
	print_diff(s_up<T>, s_down<T>, func_0<T>, func_0<T>, 2, 8, out);
}

template <class T>
T ln2(int power, T(*func)(int) = func_0) {
	return (T) std::log(2);
}

int main() {
	std::fstream single_file("./single_precision_data.dat", std::ios_base::out);
	test_precision<float>(single_file); // Test precision of float
	
	std::fstream double_file("./double_precision_data.dat", std::ios_base::out);
	test_precision<double>(double_file); // Test precision of double
	
	std::fstream sup_err_file("./single_sup_err.dat", std::ios_base::out);
	print_diff(s_up<float>, s_up<double>, func_0, func_0, 2, 8, sup_err_file); // Test s_down<float> vs s_down<double>
	
	std::fstream sdown_err_file("./single_sdown_err.dat", std::ios_base::out);
	print_diff(s_down<float>, s_up<double>, func_0, func_0, 2, 8, sdown_err_file); // Test s_up<float> vs s_down<double>
	
	std::fstream alter_single_err_file("./alter_single_sup_err.dat", std::ios_base::out);
	print_diff(s_up<float>, ln2<double>, func_0, func_0, 2, 9, alter_single_err_file); // Test s_up<float> vs library function for ln(2)
	
	std::fstream alter_double_err_file("./alter_double_sup_err.dat", std::ios_base::out);
	print_diff(s_up<double>, ln2<double>, func_1, func_1, 2, 9, alter_double_err_file); // Test s_up<double> vs library function for ln(2)
}