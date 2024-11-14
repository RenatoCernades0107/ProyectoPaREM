#ifndef VISITOR
#define VISITOR
#include "AST.hpp"

struct Exp;
struct ExpList;
struct BorExp;
struct CharExp;
struct GroupExp;
struct PlusExp;
struct NrintervalExp;
struct AsteriskExp;
struct QmarkExp;



struct ImpVisitor {
    virtual int visit(ExpList* e) = 0;
    virtual int visit(BorExp* e) = 0;
    virtual int visit(CharExp* e) = 0;
    virtual int visit(GroupExp* e) = 0;
    virtual int visit(PlusExp* e) = 0;
    virtual int visit(NrintervalExp* e) = 0;
    virtual int visit(AsteriskExp* e) = 0;
    virtual int visit(QmarkExp* e) = 0;    
};

#endif