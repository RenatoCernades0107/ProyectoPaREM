#include <ostream>
#include "parser.hpp"
#include "imp_printer.hpp"
#include "imp_nfa.hpp"
using namespace std;

int main(int argc, char const *argv[])
{
    string reg = "(a|b)?c*[0...3]b+";
    Scanner *scanner = new Scanner(reg);
    ImpNFA* nfabuilder = new ImpNFA();

    Token tk;
    while(tk.type != Token::END){
        tk = scanner->nextToken();
        cout << tk.to_string() << endl;
    }

    Parser *parser = new Parser(reg);

    AST* ast = parser->parse();
    ImpPrinter* printer = new ImpPrinter();
    auto x = nfabuilder->constructNFA(ast);

    printer->print(ast);

    cout << endl;
    if(ast){
        cout << "Accepted" << endl;
    }else{
        cout << "Rejected" << endl;
    }

    delete scanner;
    delete parser;
    delete ast;
}