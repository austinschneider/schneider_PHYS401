
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
	if(functor() > 0.5)
        x += 1;
    else
        x -= 1;
}

template <class T>
struct y_sample {
    T y;
    T y2;
};

template <class T, class U>
std::vector<y_sample<T> > random_walk(U & functor, int N_steps, int sample_interval) {
    T y_total = 0;
    T y2_total = 0;
    T y = 0;
    y_sample<T> ss;
    std::vector<y_sample<T> > samples;
    for(int i=0; i<N_steps; ++i) {
        random_step(y, functor);
        y_total += y;
        y2_total += std::pow(y, (T)2.0);
        if((i+1) % sample_interval == 0) {
            ss.y = y_total/((T)(i+1));
            ss.y2 = y2_total/((T)(i+1));
            samples.push_back(ss);
        }
    }
    return samples;
}

template <class T, class U>
void test_1d_rwalk(std::ostream & os) {

}

int main() {
	MTRand_closed mtr;
	mtr.seed(seedgen());
	std::fstream out("output.dat", std::ios_base::out);
	out << "#N estimate" << std::endl;
	test_mc_integrate<double>(out, x_pow<double, 3>, 0.0, 1.0, mtr, 8, 100, 1.0/(3.0+1.0));
}
