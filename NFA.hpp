#ifndef NFAHPP
#define NFAHPP

#include <iostream>
#include <vector>
#include <unordered_set>
#include <unordered_map>
using namespace std;

#define ALPHABET_SIZE 52
#define EPS '\0'

struct NFA {
    using state = int*; 
    int n_states;
    vector<bool> *alphabet;
    vector<state> *final_states;
    int* initial_state;
    unordered_map<pair<state, int>*, vector<state>*> *transitions; 

    NFA() {
        transitions = new unordered_map<pair<state, int>*, vector<state>*>();
        final_states = new vector<state>();
        alphabet = new vector<bool>(ALPHABET_SIZE, false);
        n_states = 0;
   }

   NFA(NFA* a, NFA *b, const string &binaryop = "") {
        if (binaryop == "|") {
            this->alphabet = new vector<bool>(ALPHABET_SIZE, false);
            this->transitions = new unordered_map<pair<state, int>*, vector<state>*>();
            this->initial_state = create_state();

            for (auto transition : *(a->transitions)) {
                if (transition.first->first == a->initial_state) {
                    auto p = new pair<state, int>(this->initial_state, transition.first->second);
                    this->transitions->insert({p, transition.second});
                } else {
                    this->transitions->insert(transition);
                }
            }

            for (auto transition : *(b->transitions)) {
                if (transition.first->first == b->initial_state) {
                    auto p = new pair<state, int>(this->initial_state, transition.first->second);
                    this->transitions->insert({p, transition.second});
                } else {
                    this->transitions->insert(transition);
                }
            }

            for (int i = 0; i < ALPHABET_SIZE; ++i)
            {
                this->alphabet->at(i) = (a->alphabet->at(i) || b->alphabet->at(i));    
            }

            this->n_states += a->n_states + b->n_states + 1;
            if (a->final_states->size() > b->final_states->size()) {
                this->final_states = a->final_states;
                this->final_states->insert(this->final_states->end(), b->final_states->begin(), b->final_states->end());
            } else {
                this->final_states = b->final_states;
                this->final_states->insert(this->final_states->end(), a->final_states->begin(), a->final_states->end());
            }
        }
        else {
            for (int i = 0; i < ALPHABET_SIZE; ++i)
            {
                this->alphabet->at(i) = (a->alphabet->at(i) || b->alphabet->at(i));    
            }

            if (a->transitions->size() > b->transitions->size()) {
                this->transitions = a->transitions;
            } else {
                this->transitions = b->transitions;
            }

            this->final_states = b->final_states;
            // Concatenate final state of a with initial state of b
            for (state fstate : *a->final_states) {
                auto p = new pair<state, int>(fstate, EPS);
                auto v = new vector<int*>(b->initial_state);
                this->transitions->insert({p, v});
            }
            this->n_states = a->n_states + b->n_states;
        }
   }

   NFA(NFA* a, const string &unaryop) {
        // Apply unary operator
        if (unaryop == "*") {
            // Kleene star
        }
        else if (unaryop == "+") {
            // Plus
        }
        else if (unaryop == "?") {
            // Optional
        }
   }

   state create_state() {
        state new_s = new int(n_states);
        ++n_states;
        return new_s;
   }

   void add_final_state(state q) {
        final_states->push_back(q);
   }

   int push_alphabet(char c) {
        alphabet->at(c - 'a') = true;
        return c - 'a';
   }



   void add_charexp(char c) {
        initial_state = create_state();
        int charindex = push_alphabet(c);
        state final_state = create_state();
        add_final_state(final_state);

        auto p = new pair<state, int>(initial_state, charindex);
        auto v = new vector<state>();
        v->push_back(final_state);
        transitions->insert({p, v});
   }

    void plus_op() {
        if (initial_state == nullptr || n_states < 1 || final_states->empty()) {
            throw runtime_error("states must be inizialized to apply plus operator");
        }
        auto new_initial_state = create_state();
        auto new_final_state = create_state();
        //TODO
    }

    void destroy(){
        delete alphabet;
        delete final_states;
        delete initial_state;
        for (auto transition : *transitions) {
            delete transition.first;
            delete transition.second;
        }
        delete transitions;
    }

    void print_alphabet() {
        for (int i = 0; i < ALPHABET_SIZE; ++i) {
            if (alphabet->at(i)) {
                cout << char(i + 'a') << endl;
            }
        }
    }

    void print() {
        cout << "Initial state: " << *initial_state << endl;
        cout << "Final states: ";
        for (auto state : *final_states) {
            cout << *state << " ";
        }
        cout << endl;
        for (auto transition : *transitions) {
            if (transition.first->second == '\0') {
                cout << "(" << *(transition.first->first) << ", " << "ε" << ") -> ";
            } else {
                cout << "(" << *(transition.first->first) << ", " << char(transition.first->second + 'a') << ") -> ";
            }
            cout << "{";
            for (auto state : *transition.second) {
                cout << *state << " ";
            }
            cout << "}" << endl;
        }
        cout << endl;
    }


    ~NFA() {
        delete alphabet;
    }
};

#endif // !NFAHPP

