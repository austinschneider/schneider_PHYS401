#ifndef INTEGRATE_H
#define INTEGRATE_H

// Limit number of steps in Romberg calculation
const int ROMBERG_LIMIT = 12;

template <class T>
T trap(T(*f)(T), int n, T a, T b);

template <class T>
T romberg(T(*f)(T), T epsilon, T a, T b);

template <class T>
T simpson(T(*f)(T), int n, T a, T b);

#include "integrate.tcc"
#endif

