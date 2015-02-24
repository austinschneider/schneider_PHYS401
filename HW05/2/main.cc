#include <iostream>
#include <cmath>
#include <vector>
#include <fstream>
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/resource.h>

#include "mtrand.h"
#include "helmutrand.h"

long seedgen() {
    long int 	seconds;
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

int main() {
	MTRand_open mtr;

	long seed = seedgen();

	std::cout << "---Mersenne Twister Test---" << std::endl;
	std::fstream twister("twister.dat", std::ios_base::out);
	for(int k=0; k<8; ++k) {
		mtr.seed(seed);
		std::vector<double> items;
		int max = std::pow(10.0,k);
		for(int i=0; i<max; ++i) {
			items.push_back(mtr());
		}
		double mean = mean_(items);
		twister << max << " " << std::abs(mean - 0.5) << std::endl;
		std::cout << "n = 2^" << k << "\nmean = " << mean << "\nerror = " << std::abs(mean-0.5) << "\n" << std::endl;
	}

	std::cout << "\n---Helmutrand Test---" << std::endl;
	std::fstream helmut("helmut.dat", std::ios_base::out);
	for(int k=0; k<8; ++k) {
		sethelmutrand(seed);
		std::vector<double> items;
		int max = std::pow(10.0,k);
		for(int i=0; i<max; ++i) {
			items.push_back(helmutrand());
		}
		double mean = mean_(items);
		helmut << max << " " << std::abs(mean - 0.5) << std::endl;
		std::cout << "n = 2^" << k << "\nmean = " << mean << "\nerror = " << std::abs(mean-0.5) << "\n" << std::endl;
	}

	std::fstream twister_3d("twister_3d.dat", std::ios_base::out);
	mtr.seed(seed);
	for(int i=0; i<10000; ++i) {
		twister_3d << mtr() << " " << mtr() << " " << mtr() << std::endl;
	}

	std::fstream helmut_3d("helmut_3d.dat", std::ios_base::out);
	sethelmutrand(seed);
	for(int i=0; i<10000; ++i) {
		helmut_3d << helmutrand() << " " << helmutrand() << " " << helmutrand() << std::endl;
	}

	return 0;
}
