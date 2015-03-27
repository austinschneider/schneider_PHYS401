#ifndef OBSERVABLES_TCC
#define OBSERVABLES_TCC

#include <string>
#include "../../Global/utils.h"

void Observable::calculate(Lattice * lat) {
    calculate_(quantity, lat, obs);
}

void Observable::update(Lattice * lat, double delta_e, int n) {
    if(!cpu_save)
        update_(quantity, lat, obs, delta_e, n);
}

double Observable::measure() {
  if(cpu_save)
    calculate(obs->lat);
  measured_quantities.push_back((double)quantity);
  return quantity;
}

double Observable::peek() {
    return quantity;
}

void Observable::average() {
  quantity = utils::mean(measured_quantities);
  measured_quantities.clear();
}

double Observable::mean() {
  if(cpu_save)
      calculate(obs->lat);
  return utils::mean(measured_quantities);
}

Observable & Observable::operator+=(Observable & o) {
  measured_quantities.push_back(o.peek());
  return * this;
}

Observable & Observable::shallow_copy(Observable & o) {
  obs = o.obs;
  quantity = o.quantity;
  cpu_save = o.cpu_save;
  update_ = o.update_;
  calculate_ = o.calculate_;
  return *this;
}

////////////////////////////////////////////////////////////////////////////////

void Observables::calculate() {
  for(int i=0; i<obs.size(); ++i) {
    obs[i].obs = this;
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

void Observables::add(std::string n, Observable o) {
  names.push_back(n);
  obs.push_back(o);
  obs[obs.size()].obs = this;
}

void Observables::average() {
  for(int i=0, size=obs.size(); i<size; ++i) {
    obs[i].average();
  }
}

Observables Observables::mean() {
  Observables obs_new;
  obs_new.shallow_copy(*this);
  obs_new.average();
  return obs_new;
}

Observables & Observables::operator+=(Observables & o) {
  if(obs.size() == o.obs.size()) {
    for(int i=0; i<obs.size(); ++i) {
      obs[i] += o.obs[i];
    }
  }
  return *this;
}

Observables & Observables::operator=(Observables & o) {
  obs.clear();
  lat = o.lat;
  for(int i=0; i<o.obs.size(); ++i) {
    obs.push_back(o.obs[i]);
    obs[i].obs = this;
  }
  return *this;
}

Observables & Observables::shallow_copy(Observables & o) {
  obs.clear();
  for(int i=0; i<o.obs.size(); ++i) {
    obs.push_back(Observable());
    obs[i].shallow_copy(o.obs[i]);
  }

  return *this;
}

#endif
