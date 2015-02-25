#include <cmath>
#include <vector>
#include <iostream>

#include <unistd.h>

#include "mtrand.h"
#include "helmutrand.h"

long seedgen() {
    long int    seconds;
    return ((time(&seconds)*181)*((getpid() - 83)*359))%104729;
}

template<class T> // Calculate the mean of a vector
double mean_(std::vector<T, std::allocator<T> > vec) {
    double total = 0;
    for(int i=0; i<vec.size(); ++i) {
        total += vec[i];
    }
    total /= vec.size();
    return total;
}

template <class T> // Calculate the standart deviation of an array
double std_dev_(std::vector<T, std::allocator<T> > vec) {
    double mean = mean_(vec);
    double total = 0;
    double tmp;
    for(int i=0; i<vec.size(); ++i) {
        tmp = vec[i] - mean;
        total += tmp * tmp;
    }
    total /= vec.size();
    return sqrt(total);
}

template <class T, class U>
T compute_X(U & functor, int N) {
    T total = 0;

    for(int i=0; i<N; ++i) {
        total += (functor() - 0.5);
    }

    total *= std::sqrt(((T)12)/((T)N));
    return total;
}

template <class T, class U>
T compute_X_moment(U & functor, int n, T k, int N) {
    T total = 0;
    for(int i=0; i<n; ++i) {
        total += ((T)std::pow(compute_X<T>(functor, N),(T)k));
    }
    total *= ((T)1)/((T)N);
    return total;
}

struct mtrand_functor {
    MTRand_open mtr;
    
    double operator()() {
        return (mtr());
    }
};

int main() {
    mtrand_functor m; m.mtr.seed(seedgen());

    std::cout << compute_X_moment<double>(m, 12, 1, std::pow(10.0, 6.0)) << std::endl;
}
