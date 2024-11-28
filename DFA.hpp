#ifndef DFAHPP
#define DFAHPP

#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <algorithm>
#include <math.h>

#ifdef _OPENMP
#include <omp.h>
#endif

using namespace std;
#define AlPHASIZE 255

struct DFA {
    vector<vector<int>> table;
    set<int> fstates;
    vector<char> alphabet;

    void read() {
        ifstream file("dfa.txt");
        int nfstates;
        file >> nfstates;

        for (int i = 0; i < nfstates; ++i) {
            int x;
            file >> x;
            fstates.insert(x);
        }

        int nstates, nalpha;
        file >> nstates >> nalpha;
        table.reserve(nstates);
        alphabet.reserve(nalpha);

        for (int i = 0; i < nalpha; ++i) {
            char x;
            file >> x;
            alphabet.emplace_back(x);
        }

        for (int i = 0; i < nstates; ++i) {
            vector<int> row(AlPHASIZE, -1);
            for (int j = 0; j < nalpha; ++j) {
                int x;
                file >> x;
                row[(int)alphabet[j]] = x;
            }
            table.emplace_back(row);
        }
    }

    void print() {
        cout << "------ Transition table ------" << endl;
        // Print alphabet
        cout << string(floor(9 + log10(table.size())), ' ');
        for (int i = 0; i < alphabet.size(); ++i) {
            cout << alphabet[i] << " ";
        } cout << endl;
        
        for (int i = 0; i < table.size(); ++i) {
            cout << "state(" << i << ") ";
            for (auto b : table[i]) {
                if (b != -1) {
                    cout << b << " ";
                }
            }
            cout << endl;
        }

        cout << "------ Final states ------" << endl;
        for (auto a : fstates) {
            cout << a << " "; 
        }
        cout << endl;
    }

    bool exec(const string& s) {
        int curr_state = 0;
        for (int i = 0; i < s.size(); ++i) {
            char c = s[i];
            int cidx = (int)c;
            curr_state = table[curr_state][cidx];
            if (curr_state == -1) {
                break;
            }
        }
        auto it = fstates.find(curr_state);
        
        if (it != fstates.end()) {
            cout << "Accepted: " << s << endl;
            return true;
        }
        cout << "Rejected: " << s << endl;
        return false;
    }

    bool is_state(int state) {
        return (state < table.size() && state >= 0);
    }

    
    bool parallel_rem(const string& s, int num_threads) {
        if (num_threads > s.size()) {
            throw runtime_error("Number of threads mustn't be greater than input");
        }
        #ifdef _OPENMP
            omp_set_num_threads(num_threads);
        #endif
        double t1, t2;
        vector<vector<vector<int>>> I(num_threads);

        #ifdef _OPENMP
        t1 = omp_get_wtime();
        #endif
        

        int res = s.size() % num_threads;
        #pragma omp parallel 
        {
            int tid;
            set<int> S, L; // Possible initail states, Possible final states

            #ifdef _OPENMP
            tid = omp_get_thread_num();
            #endif
            
            int start_pos = tid*(s.size()/num_threads);
            string pi_input;
            if (tid == num_threads - 1) {
                pi_input = s.substr(start_pos, s.size()/num_threads + res);
            } else {
                pi_input = s.substr(start_pos, s.size()/num_threads);
            }

            // Print input for each processor
            // #pragma omp critical
            // {
            //     cout << "Thread " << tid << ": " << pi_input << endl;
            // }

            // Find possible initial states
            for (int q = 0; q < table.size(); ++q) {
                int idx_start_c = (int)pi_input[0];
                int idx_last_c = (int)s[start_pos-1];
                
                // validate if the states exist
                if (is_state(table[q][idx_start_c])) {
                    S.insert(q);
                }
                if (is_state(table[q][idx_last_c])) {
                    L.insert(table[q][idx_last_c]);
                }
            }

            set<int> R;
            set_intersection(S.begin(), S.end(), L.begin(), L.end(), inserter(R, R.begin()));
            
            int found = 0;
            if (tid == 0) {
                vector<int> Rr(pi_input.size());
                int curr_state = 0;
                for (int i = 0; i < Rr.size(); ++i) {
                    int cidx = (int)pi_input[i];
                    if (fstates.find(table[curr_state][cidx]) != fstates.end()) {
                        ++found;
                    }
                    curr_state = table[curr_state][cidx];
                    Rr[i] = curr_state;
                }
                I[tid].push_back(Rr);
            } else {
                for (auto r : R) {
                    vector<int> Rr(pi_input.size());
                    for (int i = 0; i < Rr.size(); ++i) {
                        int cidx = (int)pi_input[i];
                        if (fstates.find(table[r][cidx]) != fstates.end()) {
                            ++found;
                        }
                        // #pragma omp critical
                        // {
                        //     cout << "Thread " << tid << ": " << pi_input[i] << endl;
                        // }
                        r = table[r][cidx];
                        Rr[i] = r;
                    }
                    I[tid].push_back(Rr);
                }
            }
        }

        // Perform reduction I
        
        // Print I
        // for (int i = 0; i < I.size(); ++i) {
        //     cout << "Thread " << i << ": " << endl;
        //     for (size_t j = 0; j < I[i].size(); j++)
        //     {
        //         cout << "Rr[" << j << "] = ";
        //         for (size_t k = 0; k < I[i][j].size(); k++)
        //         {
        //             cout << I[i][j][k] << " ";
        //         } cout << endl;
        //     }
        // }


        vector<int> last_route_indexes = {0};
        // For each process i from 1 to p-1
        for (int i = 1; i < I.size(); ++i) {
            bool connected = false;
            // For each possible route
            for (int j = 0; j < I[i].size(); ++j) {
                int last_index_route = last_route_indexes.back();
                int last_state_of_prev_proc = I[i-1][last_index_route][I[i-1][last_index_route].size()-1];
                int first_char = s[i*s.size()/num_threads]; 
                int initial_state = I[i][j][0];
                if (table[last_state_of_prev_proc][first_char] == initial_state) {
                    connected = true;
                    last_route_indexes.push_back(j);
                }
            }
            if (!connected) {
                cout << "Rejected: " << s << endl;
                return false;
            }
        }

        for (int i : last_route_indexes) {
            int fstate = I[I.size()-1][i][I[I.size()-1][i].size()-1];
            if (fstates.find(fstate) != fstates.end()) {
                cout << "Accepted: " << s << endl;
                return true;
            }
        }

        cout << "Rejected: " << s << endl;
        return false;
    }
    
};


#endif
