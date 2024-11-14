
#include "AST.cpp"

struct ImpVisitor {
    virtual int visit(Exp* e) = 0;
    virtual int visit(ExpList* e) = 0;
    virtual int visit(BorExp* e) = 0;
    virtual int visit(CharExp* e) = 0;
    virtual int visit(GroupExp* e) = 0;
    virtual int visit(PlusExp* e) = 0;
    virtual int visit(NrintervalExp* e) = 0;
    virtual int visit(AsteriskExp* e) = 0;
    virtual int visit(QmarkExp* e) = 0;    
};