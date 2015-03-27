#ifndef LATTICE_H
#define LATTICE_H

struct Lattice {
    int * spins;
    int * * lookup_table;
    int N;
    int d;
};

#endif
