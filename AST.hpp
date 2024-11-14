#ifndef ASTREE
#define ASTREE

#include "imp_visitor.hpp"
#include <iostream>
#include <vector>
using namespace std;


struct Exp {
    virtual void print(ostream& os) const = 0;
    virtual ~Exp() {}
    virtual int accept(ImpVisitor* v) = 0;
};

struct ExpList {
    vector<Exp*> expl;

    ExpList() {}

    void add(Exp *e) {
        expl.push_back(e);
    }

    void print(ostream& os) const {
        for (auto e : expl) {
            e->print(os);
        }
    }

    int accept(ImpVisitor* v) {
        return v->visit(this);
    }

    ~ExpList() {
        for (auto e : expl) {
            delete e;
        }
    }
};

struct BorExp : public Exp {
    Exp *left, *right;
    
    BorExp(Exp *left, Exp *right) : left(left), right(right) {}
    
    void print(ostream& os) const {
        os << "(";
        left->print(os);
        os << "|";
        right->print(os);
        os << ")";
    }

    int accept(ImpVisitor* v) override {
        return v->visit(this);
    }
    
    ~BorExp() {
        delete left;
        delete right;
    }
};

struct CharExp : public Exp {
    string c;

    CharExp(string c) : c(c) {}

    void print(ostream& os) const {
        os << c;
    }

    int accept(ImpVisitor* v) override {
        return v->visit(this);
    }

    ~CharExp() {}
};

struct GroupExp : public Exp {
   ExpList *expl;

   GroupExp(ExpList *e) : expl(e) {}

    void print(ostream& os) const {
        os << "(";
        for (auto e : expl->expl) {
            e->print(os);
        }
        os << ")";
    }

    int accept(ImpVisitor* v) override {
        return v->visit(this);
    }

    ~GroupExp() {
        delete expl;
    }
};

struct PlusExp : public Exp {
    Exp *e;

    PlusExp(Exp *e) : e(e) {}

    void print(ostream& os) const {
        e->print(os);
        os << "+";
    }

    int accept(ImpVisitor* v)  override {
        return v->visit(this);
    }

    ~PlusExp() {
        delete e;
    }
};

struct NrintervalExp : public Exp {
    int a, b;

    NrintervalExp(int a, int b) : a(a), b(b) {}

    void print(ostream& os) const {
        os << "[" << a << "..." << b << "]";
    }

    int accept(ImpVisitor* v) override {
        return v->visit(this);
    }
};

struct AsteriskExp : public Exp {
    Exp *e;

    AsteriskExp(Exp *e) : e(e) {}

    void print(ostream& os) const {
        e->print(os);
        os << "*";
    }

    int accept(ImpVisitor* v) override {
        return v->visit(this);
    }

    ~AsteriskExp() {
        delete e;
    }
};

struct QmarkExp : public Exp {
    Exp *e;

    QmarkExp(Exp *e) : e(e) {}

    void print(ostream& os) const {
        e->print(os);
        os << "?";
    }

    int accept(ImpVisitor* v) override {
        return v->visit(this);
    }

    ~QmarkExp() {
        delete e;
    }
};



struct AST {
    ExpList *expl;
    AST(ExpList *expl) : expl(expl) {}
};


#endif