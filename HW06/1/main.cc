
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>

#include <unistd.h>

#include "mtrand.h"

long seedgen() {
    long int    seconds;
    return ((time(&seconds)*181)*((getpid() - 83)*359))%104729;
}

template <class T, class U>
T calc_pi(U & functor, T N) {
	T count = 0;
	T a, b;
	for(T i=0; i<N; ++i) {
		a = functor();
		b = functor();
		if(std::sqrt(std::pow(a, (T)2)+std::pow(b,(T)2)) <= (T)1) {
			count += 1;
		}
	}
	return (count/N)*((T)4);
}

template <class T, class U>
void test_calc_pi(std::ostream & os, U & functor, int k_max, int n) {
	const T pi = 3.141592653589793238462643383279502884197169399375105820974944592307816406286;
	T estimate[k_max]; std::memset(estimate, 0, sizeof(estimate));
	T N[k_max];	std::memset(N, 0, sizeof(N));
	
	for(int k=1; k<=k_max; ++k) {
		N[k-1] = std::pow((T)10, (T)k);
		for(int i=0; i<n; ++i) {
			estimate[k-1] += calc_pi<T>(functor, N[k-1]);
		}
		estimate[k-1] /= n;
		os << std::setprecision(15) << N[k-1] << " " << std::setprecision(15) << std::abs(pi-estimate[k-1]) << std::endl;
	}
}

int main() {
	MTRand_closed mtr;
	mtr.seed(seedgen());
	std::fstream out("output.dat", std::ios_base::out);
	out << "#N pi_estimate" << std::endl;
	test_calc_pi<double>(out, mtr, 8, 100);
}
