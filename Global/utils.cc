#include "utils.h"

#include <time.h>
#include <unistd.h>

#include <cmath>
#include <vector>

long utils::seedgen() {
    long int 	seconds;
    return ((time(&seconds)*181)*((getpid() - 83)*359))%104729;
}

template<class T> // Calculate the mean of a vector
double utils::mean(std::vector<T, std::allocator<T> > vec) {
	double total = 0;
	for(int i=0; i<vec.size(); ++i) {
		total += vec[i];
	}
	total /= vec.size();
	return total;
}

template <class T> // Calculate the standart deviation of an array
double utils::std_dev(std::vector<T, std::allocator<T> > vec) {
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
