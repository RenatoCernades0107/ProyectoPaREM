#ifndef IMPNFAHPP
#define IMPNFAHPP
#include "imp_visitor.hpp"
#include "NFA.hpp"


struct ImpNFA : public ImpVisitor {
    NFA* final_nfa;

    ImpNFA() {
        final_nfa = new NFA();
    }
    
    ~ImpNFA() {
        delete final_nfa;
    }

    NFA* constructNFA(AST* e) {
        auto nfa = (NFA*)e->expl->accept(this);
        return nfa;  
    }

    void* visit(ExpList* e) override {
        NFA* nfa = new NFA();
        for (auto it = e->expl.begin(); it != e->expl.end(); ++it) {;
            (*it)->accept(this);
        }
        return (void*)nfa;
    }

    void* visit(BorExp* e) override {
        auto nfa_l = (NFA*)e->left->accept(this);
        auto nfa_r = (NFA*)e->right->accept(this);

        NFA* nfa = new NFA(nfa_l, nfa_r, "|");
        // nfa->print();
        return (void*)nfa;
    }

    void* visit(CharExp* e) override {
        NFA* nfa = new NFA();
        for (auto &c : e->c) {
            nfa->add_charexp(c);
        }
        return (void*)nfa;
    }

    void* visit(GroupExp* e) override {
        NFA* nfa = new NFA();
        for (auto exp : e->expl->expl) {
            exp->accept(this);
        }
        return (void*)nfa;
    }

    void* visit(PlusExp* e) override {
        auto nfa = (NFA*)e->e->accept(this);
        nfa->plus_op();        
        return (void*)nfa;
    }

    void* visit(AsteriskExp* e) override {
        NFA* nfa = new NFA();
        e->e->accept(this);
        return (void*)nfa;
    }

    void* visit(NrintervalExp* e) override {
        NFA* nfa = new NFA();
        // cout << e->a;
        // cout << e->b;
        return (void*)nfa;
    }

    void* visit(QmarkExp* e) override {
        NFA* nfa = new NFA();
        e->e->accept(this);
        return (void*)nfa;
    }

};

#endif