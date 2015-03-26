#ifndef OBSERVABLES_TCC
#define OBSERVABLES_TCC

#include "../../Global/utils.h"

template <class T>
void Observable<T>::calculate(Lattice * lat) {
    calculate_(quantity, lat);
}

template <class T>
void Observable<T>::update(Lattice * lat, double delta_e, int n) {
    update_(quantity, lat, delta_e, n);
}

template <class T>
double Observable<T>::measure() {
    measured_quantities.push_back((double)quantity);
    return quantity;
}

template <class T>
double Observable<T>::peek() {
    return quantity;
}

template <class T>
void Observable<T>::average() {
  quantity = utils::mean(measured_quantities);
  measured_quantities.clear();
}

template <class T>
double Observable<T>::mean() {
  return utils::mean(measured_quantities);
}

template <class T>
Observable_v & Observable<T>::operator+=(Observable_v & o) {
  measured_quantities.push_back(o.peek());
  return * this;
}

////////////////////////////////////////////////////////////////////////////////

void Observables::calculate() {
  for(int i=0, size=obs.size(); i<size; ++i) {
    obs[i].calculate(lat);
  }
}

void Observables::update(double delta_e, int n) {
  for(int i=0, size=obs.size(); i<size; ++i) {
    obs[i].update(lat, delta_e, n);
  }
}

void Observables::measure() {
  for(int i=0, size=obs.size(); i<size; ++i) {
    obs[i].measure();
  }
}

void Observables::add(Observable_v o) {
  obs.push_back(o);
  obs[obs.size()].obs = this;
}

void Observables::average() {
  for(int i=0, size=obs.size(); i<size; ++i) {
    obs[i].average();
  }
}

Observables Observables::mean() {
  Observables obs_new = *this;
  obs_new.average();
  return obs_new;
}

Observables & Observables::operator+=(Observables & o) {
  if(obs.size() == o.obs.size()) {
    for(int i=0; i<obs.size(); ++i) {
      obs[i] += o.obs[i];
    }
  }
}

Observables & Observables::operator=(Observables & o) {
  obs = o.obs;
  lat = o.lat;
  for(int i=0; i<obs.size(); ++i) {
    obs[i].obs = this;
  }
}

#endif
