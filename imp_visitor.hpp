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
    virtual void* visit(ExpList* e) = 0;
    virtual void* visit(BorExp* e) = 0;
    virtual void* visit(CharExp* e) = 0;
    virtual void* visit(GroupExp* e) = 0;
    virtual void* visit(PlusExp* e) = 0;
    virtual void* visit(NrintervalExp* e) = 0;
    virtual void* visit(AsteriskExp* e) = 0;
    virtual void* visit(QmarkExp* e) = 0;    
};

#endif