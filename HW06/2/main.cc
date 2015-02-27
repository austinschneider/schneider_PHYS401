
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
T mc_integrate(T(*func)(T), T a, T b, U & functor, T N) {
	if(b < a) return mc_integrate(func, b, a, functor, N);
	T total = 0;
	T range = b-a;
	T x;
	for(T i=0; i<N; ++i) {
		x = a + functor()*range;
		total += func(x);
	}
	return total / N;
}

template <class T, class U>
void test_mc_integrate(std::ostream & os, T(*func)(T), T a, T b, U & functor, int k_max, int n, T actual) {
	T estimate[k_max]; std::memset(estimate, 0, sizeof(estimate));
	T N[k_max];	std::memset(N, 0, sizeof(N));
	
	for(int k=1; k<=k_max; ++k) {
		N[k-1] = std::pow((T)10, (T)k);
		for(int i=0; i<n; ++i) {
			estimate[k-1] += mc_integrate<T>(func, a, b, functor, N[k-1]);
		}
		estimate[k-1] /= n;
		os << std::setprecision(15) << N[k-1] << " " << std::setprecision(15) << std::abs(actual-estimate[k-1]) << std::endl;
	}
}

template <class T, int n>
T x_pow(T x) {
	const T k = n;
	return std::pow(x, k);
}

int main() {
	MTRand_closed mtr;
	mtr.seed(seedgen());
	std::fstream out("output.dat", std::ios_base::out);
	out << "#N estimate" << std::endl;
	test_mc_integrate<double>(out, x_pow<double, 3>, 0.0, 1.0, mtr, 8, 100, 1.0/(3.0+1.0));
}
