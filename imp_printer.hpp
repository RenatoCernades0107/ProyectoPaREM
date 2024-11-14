#ifndef PRINTVISITOR
#define PRINTVISITOR
#include "imp_visitor.hpp"

struct ImpPrinter : public ImpVisitor {
    void print(AST* e) {
        e->expl->accept(this);
        return;  
    }

    int visit(ExpList* e) override {
        for (auto it = e->expl.begin(); it != e->expl.end(); ++it) {;
            (*it)->accept(this);
        }
        return 0;
    }

    int visit(BorExp* e) override {

        e->left->accept(this);
        cout << "|";
        e->right->accept(this);
        return 0;
    }

    int visit(CharExp* e) override {
        cout << e->c;
        return 0;
    }

    int visit(GroupExp* e) override {
        cout << "(";
        for (auto exp : e->expl->expl) {
            exp->accept(this);
        }
        cout << ")";
        return 0;
    }

    int visit(PlusExp* e) override {
        
        e->e->accept(this);
        cout << "+";
        return 0;
    }

    int visit(AsteriskExp* e) override {
    
        e->e->accept(this);

        cout << "*";
        return 0;
    }

    int visit(NrintervalExp* e) override {
        
        cout << "[";
        cout << e->a;
        cout << "...";
        cout << e->b;
        cout << "]";
        return 0;
    }

    int visit(QmarkExp* e) override {
        
        e->e->accept(this);
        cout << "?";
        return 0;
    }


};


#endif