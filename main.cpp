#include <ostream>
#include "parser.cpp"
using namespace std;

int main(int argc, char const *argv[])
{
    string reg = "(a|b)?c*[0...3]b+";
    Scanner *scanner = new Scanner(reg);

    Token tk;
    while(tk.type != Token::END){
        tk = scanner->nextToken();
        cout << tk.to_string() << endl;
    }

    Parser *parser = new Parser(reg);

    if(parser->parse()){
        cout << "Accepted" << endl;
    }else{
        cout << "Rejected" << endl;
    }

    

    delete scanner;
    delete parser;

}