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

template <class T>
void test_precision(std::ostream & out) {
	out << "# power\tdiff" << std::endl;
	for(int power = 2; power <= 8; ++power) {
		out << power << "\t" << std::abs(s_up<T>(power) - s_down<T>(power)) << std::endl;
	}
}

int main() {
	std::fstream float_file("./float_precision_data.dat", std::ios_base::out);
	std::fstream double_file("./double_precision_data.dat", std::ios_base::out);
	if(float_file)
		test_precision<float>(float_file);
	else
		std::cout << "float file error!" << std::endl;
		
	if(double_file)
		test_precision<double>(double_file);
	else
		std::cout << "double file error!" << std::endl;
}