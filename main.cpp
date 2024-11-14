#include <ostream>
#include "parser.hpp"
#include "imp_printer.hpp"
using namespace std;

int main(int argc, char const *argv[])
{
    string reg = "(a|b|(c*[1..9]+))?c*[0...3]b+";
    Scanner *scanner = new Scanner(reg);

    Token tk;
    while(tk.type != Token::END){
        tk = scanner->nextToken();
        cout << tk.to_string() << endl;
    }

    Parser *parser = new Parser(reg);

    AST* ast = parser->parse();
    ImpPrinter* printer = new ImpPrinter();;

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