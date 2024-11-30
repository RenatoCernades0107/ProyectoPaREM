#include <iostream>
#include <vector>
#include <fstream>
#include <omp.h>

#include "DFA.hpp"
using namespace std;



int main() {
    DFA dfa;    
    dfa.read();
    dfa.print();
    // Language: a*bc?
    const std::vector<size_t> NUMBER_OF_PROCESSES = {1, 2, 4, 8};
    #ifdef DEBUG
    const std::vector<double> INPUT_LENGTHS = { 6.69e+07};
    #else
    const std::vector<double> INPUT_LENGTHS = { 6.69e+07, 1.34e+08,  2.68e+08, 5.36e+08, 1.07e+09};
    #endif

    for (const auto n : INPUT_LENGTHS) {
        std::vector<double> measurements;;
        string *s = new string(n, 'a');
        s->append({'b'});
        for (const auto p : NUMBER_OF_PROCESSES) {
            double t1, t2;
            #ifdef _OPENMP
            t1 = omp_get_wtime();
            #endif
            bool b = dfa.parallel_rem(s, p); 
            #ifdef _OPENMP
            t2 = omp_get_wtime();
            #endif
            measurements.push_back(t2-t1);
        }
         delete s;
    }
    
   
}