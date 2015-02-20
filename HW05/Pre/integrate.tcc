#ifndef INTEGRATE_TCC
#define INTEGRATE_TCC

#include "integrate.h"

#include <cmath>
#include <vector>
#include <iostream>

// Trapezoidal integration estimate
template <class T>
T trap(T(*f)(T), int n, T a, T b) {
	if(a>b)
		return -trap(f,n,b,a);
	T div = (b - a) / ((T)n);
	T total = (f(a)+f(b))/((T)2.0)*div;
	for(int i=1; i<n; ++i) {
		total += f(a+((T)i)*div)*div;
	}
	return total;
}

template <class T>
T simpson(T(*f)(T), int n, T a, T b) {
    if(a>b)
        return -simpson(f,n,b,a);
    T div = (b - a)/((T)n);
    T total = div / ((T)(3.0)) * (f(a)+f(b));
    for(int i=1; i<n; ++i) {
        total += ((T)(2+2*(i%2))) * f(a+((T)i)*div) * div/((T)(3.0));
    }
    return total;
}

// Romberg numeric integration method
template <class T>
T romberg(T(*f)(T), T epsilon, T a, T b) {
	std::vector<T*> table;
	int n=0;
	table.push_back(new T[n+1]);
	table[n][0] = trap(f,std::pow((double)2,(int)n),a,b);
	++n;
	for(int ii=0; ii<ROMBERG_LIMIT; ++ii) {
		table.push_back(new T[n+1]);
		table[n][0] = trap(f,std::pow((double)2,n),a,b);
		for(int i=1; i<n+1; ++i) {
			table[n][i] = (((T)std::pow((double)4.0,i))*table[n][i-1]-table[n-1][i-1])/((T)std::pow((double)4.0,i)-((T)1));
		}
		if(std::abs(table[n][n]-table[n-1][n-1])<epsilon) {
			for(int i=0; i<n+1; ++i) {
				for(int j=0; j<=i; ++j) {
					std::cout << table[i][j] << " ";
				}
				std::cout << std::endl;
			}
			return table[n][n];
		}
		++n;
	}
	
	for(int i=0; i<n; ++i) {
		for(int j=0; j<=i; ++j) {
			std::cout << table[i][j] << " ";
		}
		std::cout << std::endl;
	}
	
	return table[n-1][n-1];
}

#endif

