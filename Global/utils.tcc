#ifndef UTILS_TCC
#define UTILS_TCC

#include "utils.h"

#include <cmath>
#include <vector>

namespace utils {

template<class T> // Calculate the mean of a vector
double mean(std::vector<T, std::allocator<T> > & vec) {
	double total = 0;
	for(int i=0; i<vec.size(); ++i) {
		total += vec[i];
	}
	total /= vec.size();
	return total;
}

template <class T> // Calculate the standart deviation of an array
double std_dev(std::vector<T, std::allocator<T> > & vec) {
	double mean = utils::mean(vec);
	double total = 0;
	double tmp;
	for(int i=0; i<vec.size(); ++i) {
		tmp = vec[i] - mean;
		total += tmp * tmp;
	}
	total /= vec.size();
	return sqrt(total);
}

template<class T> // Calculate the mean of a vector
double mean(T * a, int size) {
    double total = 0;
    for(int i=0; i<size; ++i) {
        total += a[i];
    }
    total /= size;
    return total;
}

template <class T> // Calculate the standart deviation of an array
double std_dev(T * a, int size) {
    double mean = utils::mean(a, size);
    double total = 0;
    for(int i=0; i<size; ++i) {
        total += std::pow(std::abs(mean - a[i]), 2.0);
    }
    total /= size;
    return sqrt(total);
}

}

#endif
