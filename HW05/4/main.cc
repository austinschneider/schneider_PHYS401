#include <cmath>
#include <vector>
#include <cstring>
#include <fstream>
#include <iostream>

#include <unistd.h>

#include "mtrand.h"
#include "helmutrand.h"

long seedgen() {
    long int    seconds;
    return ((time(&seconds)*181)*((getpid() - 83)*359))%104729;
}

template <class T, class U>
T compute_X(U & functor, int N) {
    T total = 0;

    for(int i=0; i<N; ++i) {
        total += ((T)functor() - (T)0.5);
    }

    total *= std::sqrt(((T)12)/((T)N));
    return total;
}

template <class T, class U>
void compute_X_moments(U & functor, int n, T * moments, int k_max, int N) {
    T total[k_max];
    std::memset(total, 0, sizeof(total));
    T x;
    for(int i=0; i<n; ++i) {
        x = compute_X<T>(functor, N);
        for(int k_i=1; k_i<=k_max; ++k_i) {
            total[k_i-1] += (T) std::pow(x,(T)k_i);
        }
    }
    for(int i=0; i<k_max; ++i) {
        moments[i] = total[i]/((T)n);
    }
}

template <class T, class U>
void test_moments(std::ostream & os, U & functor, int n, int k, int N) {
    T moments[k];
    compute_X_moments(functor, n, moments, k, N);
    for(int i=0; i<k; ++i) {
        os << "<X^" << i+1 << "> = " << moments[i+1] << std::endl;
    }
}


struct TFunctor {
    double operator()();
};

struct mtrand_functor: TFunctor {
    MTRand_open mtr;
    
    double operator()() {
        return (mtr());
    }
};

struct helmut_functor: TFunctor {
    double operator()() {
        return helmutrand();
    }
};

int main() {
    long seed = seedgen();
    mtrand_functor m; m.mtr.seed(seed);
    helmut_functor h;
    sethelmutrand(seed);

    std::fstream out("output.txt", std::ios_base::out);

    out << "---Mersenne Twister---" << std::endl;
    test_moments<double>(out, m, std::pow(10.0,6.0), 6, 12);
    out << std::endl;
    test_moments<double>(out, m, std::pow(10.0,6.0), 6, 12000);
    out << std::endl;

    out << "---Helmut Rand---" << std::endl;
    test_moments<double>(out, h, std::pow(10.0,6.0), 6, 12);
    out << std::endl;
    test_moments<double>(out, m, std::pow(10.0,6.0), 6, 12000);
    out << std::endl;



}
