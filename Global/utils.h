#ifndef UTILS_H
#define UTILS_H

#include <vector>

namespace utils {

long seedgen();

template<class T> // Calculate the mean of a vector
double mean(std::vector<T, std::allocator<T> > vec);

template <class T> // Calculate the standard deviation of an array
double std_dev(std::vector<T, std::allocator<T> > vec);

}

#endif
