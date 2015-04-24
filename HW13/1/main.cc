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
#include <cmath>

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
double mean_(std::vector<T, std::allocator<T> > & vec) {
	double total = 0;
	for(int i=0; i<vec.size(); ++i) {
		total += vec[i];
	}
	total /= vec.size();
	return total;
}

template <class RandomAccessIterator>
double mean_(RandomAccessIterator a, RandomAccessIterator b) {
	double total = 0;
	unsigned int N = 0;
	for(; a != b; ++a) {
		total += *a;
		++N;
	}
	total /= (double) N;
	return total;
}

template <class T> // Calculate the standart deviation of an array
void m_s(T * array, int size, T & mean, T & stdev) {
	mean = mean_(array, size);
	double total = 0;
	double tmp;
	for(int i=0; i<size; ++i) {
		tmp = array[i] - mean;
		total += tmp * tmp;
	}
	total /= (double) size;
	stdev = sqrt(total);
}

template <class T> // Calculate the standart deviation of an array
void m_s(std::vector<T, std::allocator<T> > & vec, T & mean, T & stdev) {
	mean = mean_(vec);
	double total = 0;
	double tmp;
	for(int i=0; i<vec.size(); ++i) {
		tmp = vec[i] - mean;
		total += tmp * tmp;
	}
	total /= (double) vec.size();
	stdev = sqrt(total);
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
	total /= (double) size;
	return sqrt(total);
}

template <class T> // Calculate the standart deviation of an array
double std_dev_(std::vector<T, std::allocator<T> > & vec) {
	double mean = mean_(vec);
	double total = 0;
	double tmp;
	for(int i=0; i<vec.size(); ++i) {
		tmp = vec[i] - mean;
		total += tmp * tmp;
	}
	total /= (double) vec.size();
	return sqrt(total);
}

template <class RandomAccessIterator> // Calculate the standart deviation of an array
double std_dev_(RandomAccessIterator a, RandomAccessIterator b) {
	double mean = mean_(a, b);
	double total = 0;
	double tmp;
	unsigned int N = 0;
	for(; a != b; ++b) {
		tmp = *a - mean;
		total += tmp * tmp;
		++N;
	}
	total /= (double) N;
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

template <class T>
void jackknife(std::ostream & os, std::vector<T> sample_data) {
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

    os << "Mean = " << mean << " +/- " << std_dev << std::endl;
}

template <class T>
void bootstrap_avg(std::vector<T> sample_data, double & mean, double & error, unsigned int n_sub_samples = 1000) {
    unsigned int sample_size = sample_data.size();
    unsigned int sub_sample_size = sample_size;
    T * sub_sample = new T[sub_sample_size];
    T medians[n_sub_samples];

    // Iterate over sub samples
    for(int i=0; i<n_sub_samples; ++i) {
        // Re sample data
        for(int j=0; j<sub_sample_size; ++j) {
            sub_sample[j] = sample_data[rand() % sample_size];
        }
        medians[i] = mean_(sub_sample, sub_sample_size);
    }

    // Find mean of medians
    T median_mean = 0;
    for(int i=0; i<n_sub_samples; ++i) {
        median_mean += medians[i];
    }

    // Find standard deviation of medians
    median_mean /= n_sub_samples;
    T median_stdev = 0;
    for(int i=0; i<n_sub_samples; ++i) {
        T tmp = medians[i] - median_mean;
        median_stdev += tmp * tmp;
    }
    median_stdev /= n_sub_samples;
    median_stdev = sqrt(median_stdev);

    mean = median_mean;
    error = median_stdev;

    delete[] sub_sample;
}


template <class T>
void bootstrap(std::vector<T> sample_data, double(*func)(T *, int), double & mean, double & error, unsigned int n_sub_samples = 1000) {
	unsigned int sample_size = sample_data.size();
	unsigned int sub_sample_size = sample_size;
	T * sub_sample = new T[sub_sample_size];
	std::vector<T> medians(n_sub_samples, 0);

	// Iterate over sub samples
	for(int i=0; i<n_sub_samples; ++i) {
		// Re sample data
		for(int j=0; j<sub_sample_size; ++j) {
			sub_sample[j] = sample_data[rand() % sample_size];
		}
		medians[i] = func(sub_sample, sub_sample_size);
	}

    /*	
	// Find mean of medians
	T median_mean = 0;
	for(int i=0; i<n_sub_samples; ++i) {
		median_mean += medians[i];
	}
	
	// Find standard deviation of medians
	median_mean /= n_sub_samples;
	T median_stdev = 0;
	for(int i=0; i<n_sub_samples; ++i) {
		T tmp = medians[i] - median_mean;
		median_stdev += tmp * tmp;
	}
	median_stdev /= n_sub_samples;
	median_stdev = sqrt(median_stdev);
    */
    
    bootstrap_avg(medians, mean, error, n_sub_samples);

	delete[] sub_sample;
}

template <class T>
double median_(T * array, int size) {
	std::sort(array, array + size);
	T median;
	if(size % 2 == 1)
		median = array[size / 2 + 1];
	else
		median = (array[size / 2] + array[size / 2 + 1])/2;
	return median;
}

template<class T> // Calculate the mean of a vector
void s_k(std::vector<T, std::allocator<T> > & vec, T & skew, T & kurtosis) {
	double mean;
	double stdev;
	m_s(vec, mean, stdev);
	double total_s = 0;
	double total_k = 0;
	for(int i=0; i<vec.size(); ++i) {
		double tmp = (vec[i] - mean)/stdev;
		total_s += std::pow(tmp, (T)3.0);
		total_k += std::pow(tmp, (T)4.0);
	}
	total_s /= (double) vec.size();
	total_k /= (double) vec.size();
	skew = total_s;
	kurtosis = total_k;
}

template<class T> // Calculate the mean of a vector
void s_k(T * array, int size, T & skew, T & kurtosis) {
	double mean;
	double stdev;
	m_s(array, size, mean, stdev);
	double total_s = 0;
	double total_k = 0;
	for(int i=0; i<size; ++i) {
		double tmp = (array[i] - mean)/stdev;
		total_s += std::pow(tmp, (T)3.0);
		total_k += std::pow(tmp, (T)4.0);
	}
	total_s /= (double) size;
	total_k /= (double) size;
	skew = total_s;
	kurtosis = total_k;
}

template<class T> // Calculate the mean of a vector
double skew(T * array, int size) {
	double mean;
	double stdev;
	m_s(array, size, mean, stdev);
	double total_s = 0;
	for(int i=0; i<size; ++i) {
		double tmp = (array[i] - mean)/stdev;
		total_s += std::pow(tmp, (T)3.0);
	}
	total_s /= (double) size;
	return total_s;
}

template<class T> // Calculate the mean of a vector
double kurtosis(T * array, int size) {
	double mean;
	double stdev;
	m_s(array, size, mean, stdev);
	double total_s = 0;
	for(int i=0; i<size; ++i) {
		double tmp = (array[i] - mean)/stdev;
		total_s += std::pow(tmp, (T)4.0);
	}
	total_s /= (double) size - 3.0;
	return total_s;
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

	std::cout << "Jackknife:" << std::endl;
	jackknife(std::cout, sample_data);
	std::cout << std::endl;
	std::cout << "Bootstrap:" << std::endl;
	double mean;
	double mean_e;
    bootstrap_avg(sample_data, mean, mean_e, 100);
    std::cout << "Mean = " << mean << " +/- " << mean_e << std::endl;

    double stdev;
	double stdev_e;
    bootstrap(sample_data, std_dev_, stdev, stdev_e, 100);
    std::cout << "Standard deviation = " << stdev << " +/- " << stdev_e << std::endl;

    double sskew;
	double sskew_e;
    bootstrap(sample_data, skew, sskew, sskew_e, 100);
    std::cout << "Skew = " << sskew << " +/- " << sskew_e << std::endl;

    double kurt;
	double kurt_e;
    bootstrap(sample_data, kurtosis, kurt, kurt_e, 100);
    std::cout << "Kurtosis = " << kurt << " +/- " << kurt_e << std::endl;
}
