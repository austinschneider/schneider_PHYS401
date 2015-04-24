
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>

#include <unistd.h>
#include <stdlib.h>

#include "omp.h"

long seedgen() {
    long int    seconds;
    return ((time(&seconds)*181)*((getpid() - 83)*359))%104729;
}

template <class T, class U>
T calc_pi(U & functor, T N) {
	int count = 0;
	T a, b;
    int NN = N;
    #pragma omp parallel
    {    
    #pragma omp for
	for(int i=0; i<NN; ++i) {
		a = functor();
		b = functor();
		if(std::sqrt(std::pow(a, (T)2)+std::pow(b,(T)2)) <= (T)1) {
			count += 1;
		}
	}
    }
	return (((T)count)/N)*((T)4);
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

int main(int argc, char * argv[]) {
    int n_cores = omp_get_num_procs();
    int n_threads = n_cores - 2;
    if(argc > 1)
        n_threads = atoi(argv[1]);
    std::cout << "Number of cores: " << n_cores << std::endl;
    std::cout << "Parallel section will use " << n_threads << " threads." << std::endl;
    omp_set_dynamic(n_threads); 

	//MTRand_closed mtr;
	//mtr.seed(seedgen());
	std::fstream out("output.dat", std::ios_base::out);
	out << "#N pi_estimate" << std::endl;
	test_calc_pi<double>(out, rand, 6, 100);
}
