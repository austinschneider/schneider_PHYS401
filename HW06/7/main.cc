
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

template <class T>
struct y_sample {
    T y;
    T y2;
    y_sample<T> & operator+=(y_sample<T> & x) {
        this->y += x.y;
        this->y2 += x.y2;
        return *this;
    }
    y_sample<T> & operator/=(T x) {
        this->y /= x;
        this->y2 /= x;
        return *this;
    }
};

template <class T, class U>
std::vector<y_sample<T> > random_walk(U & functor, int N_steps, int sample_interval) {
    T y = 0;
    y_sample<T> ss;
    std::vector<y_sample<T> > samples;
    for(int i=0; i<N_steps; ++i) {
        random_step(y, functor);
        if((i+1) % sample_interval == 0) {
            ss.y = y;
            ss.y2 = std::pow((T)y,(T)2.0);
            samples.push_back(ss);
        }
    }
    return samples;
}

template <class T, class U>
void test_1d_rwalk(std::ostream & os, U & functor, int N_tries, int N_steps, int sample_interval) {
    y_sample<T> empty;
    empty.y = 0;
    empty.y2 = 0;
    std::vector<y_sample<T> > total(N_steps / sample_interval, empty);
    std::vector<y_sample<T> > sample;
    for(int i=0; i<N_tries; ++i) {
        sample = random_walk<T>(functor, N_steps, sample_interval);
        for(int j=0; j<N_steps; ++j) {
            total[j] += sample[j];
        }
    }
    os << "#sample, <y>, <y^2>" << std::endl;
    for(int i=0; i<(N_steps / sample_interval); ++i) {
        total[i] /= N_tries;
        os << (i+1)*sample_interval << ", " << total[i].y << ", " << total[i].y2 << std::endl;
    }
}

int main() {
	MTRand_closed mtr;
	mtr.seed(seedgen());
	std::fstream out("output.dat", std::ios_base::out);
	test_1d_rwalk<double>(out, mtr, (int) std::pow(10.0, 6.0), 1000, 10);
}
