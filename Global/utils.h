#ifndef UTILS_H
#define UTILS_H

#include <vector>

namespace utils {

long seedgen();

template<class T> // Calculate the mean of a vector
double mean(std::vector<T, std::allocator<T> > & vec);

template <class T> // Calculate the standard deviation of a vector
double std_dev(std::vector<T, std::allocator<T> > & vec);

template<class T> // Calculate the mean of an array
double mean(T * a, int size);

template<class T> // Calculate the mean of an array
double std_dev(T * a, int size);

}

#include "utils.tcc"

#endif
