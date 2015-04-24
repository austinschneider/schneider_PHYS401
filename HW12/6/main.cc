#include <iostream>
#include <tgmath.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include <omp.h>

struct rule_set {
    unsigned int size;
    unsigned int rule_size;
    bool * rules;
};

void next_state(bool * current, bool * next, unsigned int size, rule_set & rs) {
    unsigned int id;
    #pragma omp parallel
    {
    #pragma omp for private(id)
    for(int i=0; i<size; ++i) {
        id = 0;
        id |= current[(i+size-1) % size] << 0x2;
        id |= current[i] << 0x1;
        id |= current[(i+1) % size] << 0x0;
        next[i] = rs.rules[id];
    }
    }
}

void print(std::ostream & os, bool * state, unsigned int size) {
    for(int i=0; i<size; ++i) {
        if(state[i])
            os << '#';
        else
            os << ' ';
    }
    os << std::endl;
}

rule_set make_rules(unsigned int size, unsigned int rule_num) {
    rule_set r;
    r.size = size;
    r.rule_size = log2(size);
    r.rules = new bool[size];
    for(int i=0; i<size; ++i) {
        r.rules[i] = (rule_num >> i) & 0x1;
    }
    return r;
}

void test_rules(std::ostream & os, unsigned int n_rules, unsigned int rule_num, unsigned int lattice_size, unsigned int n_iterations) {
    bool * l0 = new bool[lattice_size]; memset(l0, 0, sizeof(l0));
    bool * l1 = new bool[lattice_size]; memset(l1, 0, sizeof(l1));
    bool * current = l0;
    bool * next = l1;
    current[lattice_size/2] = true;

    rule_set rs = make_rules(n_rules, rule_num);

    for(int i=0; i<n_iterations; ++i) {
        print(os, current, lattice_size);
        next_state(current, next, lattice_size, rs);
        bool * t = current;
        current = next;
        next = t;
        usleep(25000);
    }
    print(os, current, lattice_size);
}


int main(int argc, char * argv[]) {
    int n_cores = omp_get_num_procs();
    int n_threads = n_cores - 2;
    omp_set_dynamic(n_threads); 

    unsigned int rule_num = 222;
    unsigned int n_iterations = 100;
    unsigned int rule_size = 8;
    unsigned int lattice_size = 74;
    if(argc <= 1) {
        std::cout << "# Usage:" << std::endl;
        std::cout << "main rule_num [n_iterations] [rule_size] [lattice_size]" << std::endl;
        return 1;
    }
    if(argc > 1)
        rule_num = atoi(argv[1]);
    if(argc > 2)
        n_iterations = atoi(argv[2]);
    if(argc > 3)
        rule_size = atoi(argv[3]);
    if(argc > 4)
        lattice_size = atoi(argv[4]);
    test_rules(std::cout, rule_size, rule_num, lattice_size, n_iterations);
    return 0;
}
