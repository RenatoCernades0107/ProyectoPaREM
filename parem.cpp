#include <iostream>
#include <vector>
#include <fstream>
#include "DFA.hpp"
using namespace std;



int main() {
    DFA dfa;    
    dfa.read();
    dfa.print();
    string s = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaab";
    bool a = dfa.exec(s);
    bool b = dfa.parallel_rem(s, 10);
}