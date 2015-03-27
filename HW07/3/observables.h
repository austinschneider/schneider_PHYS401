#ifndef OBSERVABLES_H
#define OBSERVABLES_H

#include <vector>
#include <string>

#include "lattice.h"
#include "../../Global/utils.h"

struct Observables;

struct Observable {
  Observables * obs;
  double quantity;
  std::vector<double> measured_quantities;
  void (*calculate_)(double &, Lattice *, Observables *);
  void (*update_)(double &, Lattice *, Observables *, double, int);

  void calculate(Lattice * lat);
  void update(Lattice * lat, double delta_e, int n);
  double measure();
  double peek();
  void average();
  double mean();
  Observable & operator+=(Observable & o);
  Observable & shallow_copy(Observable & o);
};

struct Observables {
  std::vector<Observable> obs;
  std::vector<std::string> names;
  Lattice * lat;
  void calculate();
  void update(double delta_e, int n);
  void measure();
  void add(std::string n, Observable o);
  void average();
  Observables mean();

  Observables & operator+=(Observables & o);
  Observables & operator=(Observables & o);
  Observables & shallow_copy(Observables & o);
};

#include "observables.tcc"

#endif
