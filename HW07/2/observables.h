#ifndef OBSERVABLES_H
#define OBSERVABLES_H

#include <vector>

#include "lattice.h"

struct Observable_v {
    virtual void calculate(Lattice * lat);
    virtual void update(Lattice * lat, int i);
    virtual double measure();
};

template <class T>
struct Observable: public Observable_v {
    T quantity;
    std::vector<double> measured_quantities;
    void (*calculate_)(T &, Lattice *);
    void (*update_)(T &, Lattice *, int);

    void calculate(Lattice * lat);
    void update(Lattice * lat, int i);
    double measure();
};

struct Observables {
    std::vector<Observable_v> obs;
    void calculate();
    void update();
    void measure();
};

#endif
