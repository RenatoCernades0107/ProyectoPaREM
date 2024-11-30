#include <iostream>
#include <vector>
#include <fstream>

#include "DFA.hpp"
using namespace std;



int main() {
    DFA dfa;    
    dfa.read();
    dfa.print();
    // a*bc
    string *s = new string(6e8, 'a');
    s->append({'b'});
    // bool a = dfa.exec(s);
    for (int i = 1; i < 10; i++) {
        bool b = dfa.parallel_rem(s, i);    
    }
    delete s;
}