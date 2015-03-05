
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>

#include <string.h>
#include <unistd.h>

#include "mtrand.h"

const long double IROOT2 = std::pow(2.0, -0.5);

long seedgen() {
    long int    seconds;
    return ((time(&seconds)*181)*((getpid() - 83)*359))%104729;
}

template <class T, class U>
T calc_pi(U & functor, T N) {
	T count = 0;
	T a = 0;
    T b = 0;
    T a_t;
    T b_t;
    T da;
    T db;
    double p_count = 0;
	for(T i=0; i<N; ++i) {
		da = functor() * (58.0/99.0);
		db = functor() * (58.0/99.0);
        if(functor() > 0.5)
            da *= -1.0;
        if(functor() > 0.5)
            db *= -1.0;
        a_t = a + da;
        b_t = b + db;
        if(a_t >= 0 && a_t <= 1 && b_t >=0 && b_t <= 1) {
            a = a_t;
            b = b_t;
            ++p_count; 
        }

		if(std::sqrt(std::pow(a, (T)2)+std::pow(b,(T)2)) <= (T)1) {
			count += 1;
		}
	}
	return (count/N)*((T)4);
}

template <class T, class U>
void test_calc_pi(std::ostream & os, U & functor, int k_max, int n) {
	const T pi = 3.141592653589793238462643383279502884197169399375105820974944592307816406286;
	T estimate[k_max]; memset(estimate, 0, sizeof(estimate));
	T N[k_max];	memset(N, 0, sizeof(N));
	
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
