#include <cmath>
#include <fstream>
#include <iostream>

template <class T>
T s_up(int power) {
	int max = std::pow(10, power);
	T sum = 0;
	for(int n = 1; n <= max; ++n) {
		sum += (T) n;
	}
	return sum;
}

template <class T>
T s_down(int power) {
	int max = std::pow(10, power);
	T sum = 0;
	for(int n = max; n >= 1; --n) {
		sum += (T) n;
	}
	return sum;
}

template <class T, class U>
void do_diff(T(*f_0)(int), U(*f_1)(int), std::ostream & out) {
	out << "# power\tdiff" << std::endl;
	for(int power = 2; power <= 8; ++power) {
		out << power << "\t" << std::abs(f_0(power) - f_1(power)) << std::endl;
	}
}

template <class T>
void test_precision(std::ostream & out) {
	do_diff(s_up<T>, s_down<T>, out);
}

int main() {
	std::fstream float_file("./float_precision_data.dat", std::ios_base::out);
	test_precision<float>(float_file);
	
	std::fstream double_file("./double_precision_data.dat", std::ios_base::out);
	test_precision<double>(double_file);
	
	std::fstream sup_err_file("./single_sup_err.dat", std::ios_base::out);
	do_diff(s_up<float>, s_up<double>, sup_err_file);
	
	std::fstream sdown_err_file("./single_sdown_err.dat", std::ios_base::out);
	do_diff(s_down<float>, s_up<double>, sdown_err_file);
}