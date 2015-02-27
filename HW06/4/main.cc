
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
T mc_integrate(T(*func)(T*), T * a, T * b, U & functor, T N, int d) {
	if(b < a) return mc_integrate(func, b, a, functor, N, d);
	T total = 0;
	T range[d];
	for(int i=0; i<d; ++i)
		range[i] = b[i] - a[i];
	T x[d];
	for(T i=0; i<N; ++i) {
		for(int d_i=0; d_i<d; ++d_i)

			x[d_i] = a[d_i] + range[d_i] * functor();
		total += func(x);
	}
	return total / N;
}

template <class T, class U>
void test_mc_integrate(std::ostream & os, T(*func)(T*), T * a, T * b, U & functor, int d, int k_max, int n, T actual) {
	T estimate[k_max]; std::memset(estimate, 0, sizeof(estimate));
	T N[k_max];	std::memset(N, 0, sizeof(N));
	
	for(int k=1; k<=k_max; ++k) {
		N[k-1] = std::pow((T)10, (T)k);
		for(int i=0; i<n; ++i) {
			estimate[k-1] += mc_integrate<T>(func, a, b, functor, N[k-1], d);
		}
		estimate[k-1] /= n;
		os << std::setprecision(15) << N[k-1] << " " << std::setprecision(15) << std::abs(actual-estimate[k-1]) << std::endl;
	
	}
}

template <class T, int n>
T squared_nd(T * x) {
	T total = 0;
	for(int i=0; i<n; ++i) {
		total += x[i];
	}
	return std::pow(total, (T)2.0);
}

int main() {
	MTRand_closed mtr;
	mtr.seed(seedgen());
	std::fstream out("output.dat", std::ios_base::out);
	out << "#N estimate" << std::endl;
	double a[10];
	double b[10];
	for(int i=0; i<10; ++i) {
		a[i] = 0;
		b[i] = 1;
	}
	test_mc_integrate<double>(out, squared_nd<double, 10>, a, b, mtr, 10, 8, 100, 155.0/6.0);
}
