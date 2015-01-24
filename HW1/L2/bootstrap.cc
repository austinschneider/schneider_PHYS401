// Bootstrap:
// Computes a re-sampled median using the bootstrap method
// Author: Austin Schneider

#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <algorithm>
#include <math.h>

int main(int argc, char * argv[]) {
	std::string file_name;
	if(argc > 1) {
		file_name = std::string(argv[1]);
	}
	else {
		std::cout << "Enter data file name:" << std::endl;
		std::cin >> file_name;
	}

	std::fstream file(file_name.c_str());
	
	std::vector<int> sample_data;
	int sample_member;
	while(file) {
		file >> sample_member;
		if(file)
			sample_data.push_back(sample_member);
		else
			break;
	}

	int sample_size = sample_data.size();
	int sub_sample_size = sample_size / 10;
	int sub_sample[sub_sample_size];

	int n_sub_samples = 1000;
	int medians[n_sub_samples];

	for(int i=0; i<n_sub_samples; ++i) {
		for(int j=0; j<sub_sample_size; ++j) {
			sub_sample[j] = sample_data[rand() % sample_size];
		}
		std::sort(sub_sample, sub_sample + sub_sample_size);
		double median;
		if(sub_sample_size % 2 == 1)
			median = sub_sample[sub_sample_size / 2 + 1];
		else
			median = (sub_sample[sub_sample_size / 2] + sub_sample[sub_sample_size / 2 + 1])/2;
		medians[i] = median;
	}
	
	double median_mean = 0;
	for(int i=0; i<n_sub_samples; ++i) {
		median_mean += medians[i];
	}
	
	median_mean /= n_sub_samples;
	double median_stdev = 0;
	for(int i=0; i<n_sub_samples; ++i) {
		double tmp = medians[i] - median_mean;
		median_stdev += tmp * tmp;
	}
	median_stdev /= n_sub_samples;
	median_stdev = sqrt(median_stdev);

	std::cout << "Median = " << median_mean << " +/- " << median_stdev << std::endl;
	
}
