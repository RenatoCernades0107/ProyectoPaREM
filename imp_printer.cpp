#include "imp_visitor.cpp"

struct ImpVisitor : public ImpVisitor {
    void print(ExpList* e) {
        e->accept(this);
        return;  
    }

    int visit(ExpList* e) {
        for (int i = 0; i < e->expl.size(); i++) {
            e->expl[i]->accept(this);
        }
        return 0;
    }

    int visit(Exp* e) {
        e->exp->accept(this);
        return 0;
    }

    int visit(BorExp* e) {
        e->exp-left->accept(this);
        cout << " | ";
        e->exp-left->accept(this);
        return 0;
    }

    int visit(CharExp* e) {
        cout << e->c;
        return 0;
    }

    int visit(GroupExp* e) {
        cout << "(";
        e->e->accept(this);
        cout << ")";
        return 0;
    }

    int visit(PlusExp* e) {
        e->exp->accept(this);
        cout << "+";
        return 0;
    }

    int visiti(AsteriskExp* e) {
        e->exp->accept(this);
        cout << "*";
        return 0;
    }

    int visit(NintervalExp* e) {
        cout << "[";
        cout << e->a;
        cout << "...";
        cout << e->b;
        cout << "]";
        return 0;
    }

    int visit(AsteriskExp* e) {
        e->e->accept(this);
        cout << "*";
        return 0;
    }

    int visit(QmarkExp* e) {
        e->e->accept(this);
        cout << "?";
        return 0;
    }

    
};
