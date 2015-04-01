#include <cmath>
#include <limits>
#include <vector>
#include <fstream>
#include <iostream>

#include <string.h>
#include <stdlib.h>

#include "mtrand.h"
#include "../../Global/utils.h"

MTRand drand;
MTRand_int32 irand;

template <class Sub>
struct Load {
    
};

template <class Sub, class LoadSub>
struct Individual {
    typedef LoadSub Load;
    LoadSub * load;
    Individual() {
        static_cast<Sub*>(this)->init();
    }
    Individual(Load * l) {
        load = l;
        static_cast<Sub*>(this)->init(l);
    }
    double calc_fitness() {
        return static_cast<Sub*>(this)->calc_fitness();
    }
    static void breed(Sub & p0, Sub & p1, Sub & c, double mutation_rate) {
        Sub::breed(p0, p1, c, mutation_rate);
    }
};


template <class T>
struct Population {
    struct Pop_ {
        std::vector<T> pop;
        double min_fitness;
        double total_fitness;
        Pop_ () {
            total_fitness = 0;
            min_fitness = std::numeric_limits<double>::max();
        }
        void clear() {
            pop.clear();
            total_fitness = 0;
            min_fitness = std::numeric_limits<double>::max();
        }
    };
    Pop_ * pop;
    Pop_ * next_pop;
    Pop_ pop0;
    Pop_ pop1;
    typename T::Load * load;
    int pop_size;
    double mutation_rate;
    double min_fitness;
    T min;

    Population(typename T::Load * l, int size, double m) {
        load = l;
        pop_size = size;
        pop = &pop0;
        next_pop = &pop1;
        mutation_rate = m;
        min_fitness = std::numeric_limits<int>::max();
        for(int i=0; i<pop_size; ++i) {
            add(T(load), pop);
        }
    }
    
    void choose_parents(int & p0, int & p1) {
        double roll = (pop->total_fitness - pop->min_fitness*pop->pop.size()) * drand();
        double accumulator = 0;
        int i;
        for(i=0; accumulator <= roll && i<pop->pop.size(); i++)
        {
            accumulator += pop->pop.at(i).calc_fitness() - pop->min_fitness;
        }
        if(i>0)
            p0 = i-1;
        else
            p0 = i;
        roll = (pop->total_fitness - pop->pop.at(p0).calc_fitness() + min_fitness) * drand();
        accumulator = 0;
        for(i=0; accumulator <= roll && i<pop->pop.size(); i++)
        {
            if(i == p0)
                continue;
            accumulator += pop->pop.at(i).calc_fitness() - pop->min_fitness;
        }
        p1 = i-1;
    }

    void add(T i, Pop_ * p) {
        /*for(int ii=0; ii<i.load->size; ++ii) {
            std::cout << i.I[ii] << " ";
        }
        std::cout << std::endl;*/
        if(i.calc_fitness() < p->min_fitness) {
            p->min_fitness = i.calc_fitness();
        }
        p->total_fitness += p->min_fitness;
        p->pop.push_back(i);
        if(i.calc_fitness() < min_fitness) {
            min_fitness = i.calc_fitness();
            min = i;
        }
    }

    void breed() {
        int p0, p1;
        choose_parents(p0, p1);
        T c0(pop->pop.at(p0).load);
        T c1(pop->pop.at(p1).load);
        c0.is_child = true;
        c1.is_child = true;
        T::breed(pop->pop[p0], pop->pop[p1], c0, mutation_rate);
        T::breed(pop->pop[p1], pop->pop[p0], c1, mutation_rate);
        add(c0, next_pop);
        add(c1, next_pop);
    }

    void next_gen() {
        int N_Iter = pop_size / 2;
        for(int i=0; i<N_Iter; ++i) {
            breed();
        }
        Pop_ * p_temp = pop;
        pop = next_pop;
        next_pop = p_temp;
        next_pop->clear();
    }
};

struct PartLoad: Load<PartLoad> {
    double * S;
    double sum;
    int size;
    int n_partitions;

    PartLoad(std::istream & is, int n_p) {
        n_partitions = n_p;
        std::vector<double> temp;
        double t;
        while(is) {
            is >> t;
            if(is)
                temp.push_back(t);
        }
        size = temp.size();
        S = new double[size];
        sum = 0;
        for(int i=0; i<size; ++i) {
            sum += temp[i];
            S[i] = temp[i];
        }
    }
};

struct PartIndividual: Individual<PartIndividual, PartLoad> {
    bool is_fit;
    bool is_child;
    double fitness;
    int * I;
    
    PartIndividual(): Individual<PartIndividual, PartLoad>() {
        
    };

    PartIndividual(Load * l): Individual<PartIndividual, PartLoad>(l) {

    };

    init() {
        is_fit = false;
        is_child = false;
        I = NULL;
    }
    init(PartLoad * l) {
        is_fit = false;
        is_child = false;
        I = new int[load->size];
    }

    void rand_init() {
        if(I == NULL)
            if(load != NULL)
                I = new int[load->size];
            else
                return;
        for(int i=0; i<load->size; ++i) {
            I[i] = irand() % load->n_partitions;
        }
    }

    double calc_fitness() {
        if(!is_fit) {
            if(!is_child) {
                rand_init();
                is_child = true;
            }
            double sum = load->sum;
            double H = 0;
            int n = load->n_partitions;
            double sums[n][n]; memset(sums, 0, sizeof(sums));
            
            for(int i=0; i<load->size; ++i) {
                for(int j=0; j<n; ++j) {
                    if(j > I[i])
                        sums[I[i]][j] += load->S[i];
                    else if(j < I[i])
                        sums[j][I[i]] -= load->S[i];
                }
            }
            for(int i=0; i<n; ++i) {
                for(int j=i+1; j<n; ++j) {
                    H += std::abs(sums[i][j]);
                }
            }
            H = H * 2.0/(n*(n-1.0));
            fitness = sum - H;
        }
        return fitness;   
    }
    static void breed(PartIndividual p0, PartIndividual p1, PartIndividual c, double mutation_rate) {
        int cut_point = irand() % p0.load->size;
        for(int i=0; i<p0.load->size; i++) {
            if(drand() < mutation_rate)
                c.I[i] = irand() % p0.load->n_partitions;
            else if(i<cut_point)
                c.I[i] = p0.I[i];
            else
                c.I[i] = p1.I[i];
        }
    }
};

int main() {
    drand.seed(utils::seedgen());
    irand.seed(utils::seedgen());
    std::ifstream in("load.txt", std::ios_base::in);
    PartLoad load(in, 10);
    Population<PartIndividual> pop(&load, 10, 0.05);
    for(int i=0; i<1000; ++i) {
        pop.next_gen();
        std::cout << pop.min_fitness << std::endl;
        std::vector<double> sets[load.n_partitions];
        double sums[load.n_partitions]; memset(sums, 0, sizeof(sums));
        for(int i=0; i<load.size; ++i) {
            sets[pop.min.I[i]].push_back(load.S[i]);
            sums[pop.min.I[i]] += load.S[i];
        }
        for(int i=0; i<load.n_partitions; ++i) {
            std::cout << sums[i] << ":\t ";
            for(int j=0; j<sets[i].size(); ++j) {
                std::cout << sets[i][j] << " ";
            }
            std::cout << std::endl;
        }
    }
}
