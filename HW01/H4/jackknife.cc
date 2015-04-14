// Jackknife:
// Computes a re-sampled mean using the jackknife method
// Author: Austin Schneider

#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <algorithm>
#include <math.h>

template<class T> // Calculate the mean of an array
double mean_(T * array, int size) {
	double total = 0;
	for(int i=0; i<size; ++i) {
		total += array[i];
	}
	total /= size;
	return total;
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
double std_dev_(T * array, int size) {
	double mean = mean_(array, size);
	double total = 0;
	double tmp;
	for(int i=0; i<size; ++i) {
		tmp = array[i] - mean;
		total += tmp * tmp;
	}
	total /= size;
	return sqrt(total);
}

template <class T> // Read a list from a file and store in a vector
std::vector<T> * read_file(std::istream & is) {
	std::vector<T> * sample_data = new std::vector<T>;
	T sample_member;
	while(is) {
		is >> sample_member;
		if(is)
			sample_data->push_back(sample_member);
		else
			break;
	}
	return sample_data;
}

int main(int argc, char * argv[]) {
	std::string file_name;
	if(argc > 1) {
		file_name = std::string(argv[1]);
	}
	else {
		std::cout << "Enter data file name:" << std::endl;
		std::cin >> file_name;
	}

	// Open the file
	std::fstream file(file_name.c_str());

	// Read in the numbers
	std::vector<double> * sample_data_p = read_file<double>(file);
	std::vector<double> & sample_data = *sample_data_p;

	int sample_size = sample_data.size();
	double total = 0;
	double * means = new double[sample_size];

	// Calculate sum of all numbers
	for(long int i=0; i<sample_size; ++i) {
		total += (double) sample_data[i];
	}

	// Calculate individual means
	for(long int i=0; i<sample_size; ++i) {
		means[i] = (total - sample_data[i]) / (sample_size - 1);
	}
	
	double mean = mean_(means, sample_size);
	double std_dev = std_dev_(means, sample_size);

	std::cout << "Mean = " << mean << " +/- " << std_dev << std::endl;
	
}
