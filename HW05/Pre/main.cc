#include "romberg.h"

// A test function
template <class T>
T thingy(T x) {
    return ((T)1.0)/((T)std::pow(x,2.0));
}

int main() {
    std::cout << romberg<double>(thingy, 0.000000001, 1.0, 2.0) << std::endl;
    return 0;
}
