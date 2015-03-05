
#include <cmath>
#include <vector>
#include <fstream>
#include <iomanip>
#include <iostream>

#include <string.h>
#include <unistd.h>

#include "mtrand.h"

long seedgen() {
    long int    seconds;
    return ((time(&seconds)*181)*((getpid() - 83)*359))%104729;
}

template <class T, class U>
void random_step(T & x, U & functor) {
	if(functor() >= 0.5)
        x += 1;
    else
        x -= 1;
}

template <class T, class U>
int random_walk(U & functor, T a, T b) {
    a = std::abs(a);
    b = std::abs(b);
    T y = 0;
    while(true) {
        random_step(y, functor);
        if(y >= a)
            return 1;
        if(y <= -b)
            return -1;
    }
    return 0;
}

template <class T, class U>
void test_1d_rwalk(std::ostream & os, U & functor, T a, T b, int k_max) {
    for(int k=1; k<=k_max; ++k) {
        T N_tries = std::pow((T)10.0, (T)k);
        T escape_count = 0;
        for(int i=0; i<N_tries; ++i) {
            if(random_walk<T>(functor, a, b) > 0)
                ++escape_count;
        }
        os << N_tries << ", " << std::abs((((T)1.0)/((T)3.0)) - (((T)escape_count) / ((T)N_tries))) << std::endl;
    }
}

int main() {
	MTRand_open mtr;
	mtr.seed(seedgen());
	std::fstream out("output.dat", std::ios_base::out);
    out << "#N_tries, escape_p" << std::endl;
    test_1d_rwalk<double>(out, mtr, 6.0, 3.0, 8);
}
