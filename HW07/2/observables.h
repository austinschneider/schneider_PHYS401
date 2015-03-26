#ifndef OBSERVABLES_H
#define OBSERVABLES_H

#include <vector>

#include "lattice.h"

struct Observables;

struct Observable_v {
  Observables * obs;
  inline virtual void calculate(Lattice * lat) {};
  inline virtual void update(Lattice * lat, double delta_e, int n) {};
  inline virtual double measure() {};
  inline virtual double peek() {};
  inline virtual void average() {};
  inline virtual double mean() {};
  inline virtual Observable_v & operator+=(Observable_v & o) {};
};

template <class T>
struct Observable: public Observable_v {
  T quantity;
  std::vector<double> measured_quantities;
  void (*calculate_)(T &, Lattice *);
  void (*update_)(T &, Lattice *, double, int);

  void calculate(Lattice * lat);
  void update(Lattice * lat, double delta_e, int n);
  double measure();
  double peek();
  void average();
  double mean();
  Observable_v & operator+=(Observable_v & o);
};

struct Observables {
  std::vector<Observable_v> obs;
  Lattice * lat;
  void calculate();
  void update(double delta_e, int n);
  void measure();
  void add(Observable_v o);
  void average();
  Observables mean();

  Observables & operator+=(Observables & o);
  Observables & operator=(Observables & o);
  Observables & shallow_copy(Observables & o);
};

#include "observables.tcc"

#endif
