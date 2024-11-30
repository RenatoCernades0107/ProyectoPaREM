#ifndef PRINTVISITOR
#define PRINTVISITOR
#include "imp_visitor.hpp"

struct ImpPrinter : public ImpVisitor {
    void* print(AST* e) {
        e->expl->accept(this);
        return nullptr;
    }

    void* visit(ExpList* e) override {
        for (auto it = e->expl.begin(); it != e->expl.end(); ++it) {;
            (*it)->accept(this);
        }
        return nullptr;
    }

    void* visit(BorExp* e) override {
        e->left->accept(this);
        cout << "|";
        e->right->accept(this);
        return nullptr;
    }

    void* visit(CharExp* e) override {
        cout << e->c;
        return nullptr;
    }

    void* visit(GroupExp* e) override {
        cout << "(";
        for (auto exp : e->expl->expl) {
            exp->accept(this);
        }
        cout << ")";
        return nullptr;
    }

    void* visit(PlusExp* e) override {
        e->e->accept(this);
        cout << "+";
        return nullptr;
    }

    void* visit(AsteriskExp* e) override {
        e->e->accept(this);
        cout << "*";
        return nullptr;
    }

    void* visit(NrintervalExp* e) override {
        cout << "[";
        cout << e->a;
        cout << "...";
        cout << e->b;
        cout << "]";
        return nullptr;
    }

    void* visit(QmarkExp* e) override {
        e->e->accept(this);
        cout << "?";
        return nullptr;
    }
};


#endif