#include "omp.h"

#include <stdio.h>

#include <iostream>

int main() {
    int n_cores = omp_get_num_procs();
    int n_threads = n_cores - 2;
    std::cout << "Number of cores: " << n_cores << std::endl;
    std::cout << "Parallel section will use " << n_threads << " threads." << std::endl;
    omp_set_dynamic(n_threads);

    #pragma omp parallel
    {
        #pragma omp for
        for(int i=0; i<n_threads; ++i) {
            printf("Hello from thread %d\n", omp_get_thread_num());
        }
        
    }

    long int N = 1000000;
    long int sum = 0;
    #pragma omp parallel for reduction(+:sum)
    for(int i=0; i<1000000; ++i) {
        sum += i;
    }

    std::cout << 2*sum/(N-1) << std::endl;
}
