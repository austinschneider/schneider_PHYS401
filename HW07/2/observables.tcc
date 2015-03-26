#ifndef OBSERVABLES_TCC
#define OBSERVABLES_TCC

template <class T>
void Observable<T>::calculate(Lattice * lat) {
    calculate_(quantity, lat);
}

template <class T>
void Observable<T>::update(Lattice * lat, int i) {
    update_(quantity, lat, i);
}

template <class T>
double Observable<T>::measure() {
    measured_quantities.push_back((double)quantity);
    return quantity;
}

void Observables::calculate() {
    for(int i=0, size=obs.size(); i<size; ++i) {
        obs[i].calculate();
    }
}

void Observables::update() {
    for(int i=0, size=obs.size(); i<size; ++i) {
        obs[i].update();
    }
}

void Observables::measure() {
    for(int i=0, size=obs.size(); i<size; ++i) {
        obs[i].measure();
    }
}

#endif
