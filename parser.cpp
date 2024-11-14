#include <iostream>
#include "scanner.cpp"
#include "AST.cpp"

struct Parser { 
    Scanner *scanner;
    Token current, previous;
    int counter = 0;
    
    Parser(const std::string input) {
        scanner = new Scanner(input);
    }

    Parser(Scanner* scanner) : scanner(scanner) {}

    void printError(const std::string &s) {
        cout << "Error in chr " + to_string(counter) + ": " + s << endl;
    }

    bool check(Token::Type type) {
        if (current.type == Token::END) {
            return false;
        }
        return current.type == type;
    }

    bool advance() {
        if (!current.type == Token::END) {
            Token tmp = current;
            current = scanner->nextToken();
            previous = tmp;
            if (check(Token::ERR)) {
                std::cout << "Error: " << current.lexema << std::endl;
                exit(0);
            }
            counter++;
            return true;
        }
        return false;
    }

    bool match(Token::Type type) {
        if (check(type)) {
            advance();
            return true;
        }
        return false;
    }

    AST* parse() {  
        current = scanner->nextToken();
        if (check(Token::ERR)) {
            std::cout << "Error: " << current.lexema << std::endl;
            exit(0);
        }

        AST *ast = parseRE();

        if (current.type != Token::END) {
            std::cout << "Error: " << current.lexema << std::endl;
            return nullptr;
        }

        return ast;
    }

    AST* parseRE() {
        ExpList *expl = new ExpList();
        expl->add(parseBorExp());
        while (!match(Token::END)) {
            expl->add(parseBorExp());
        }
        return new AST(expl);
    }

    BorExp *parseBorExp() {
        Exp *leftexp = parseUnaryExp();
        BorExp *bor;
        while (match(Token::UNION)) {
            Exp *rightexp = parseUnaryExp();
            bor = new BorExp(leftexp, rightexp);    
        }
        return bor;
    }

    Exp* parseUnaryExp() {
        Exp *e = parseExp();
        if (match(Token::ASTERISK)) {
            return new AsteriskExp(e);
        } else if (match(Token::PLUS)) {
            return new PlusExp(e);
        } else if (match(Token::QMARK)) {
            return new QmarkExp(e);
        }
        return e;
    }

    Exp* parseExp() {

        if (match(Token::LPAREN)) {
            Exp *exp = parseExp();
            if (!match(Token::RPAREN)) {
                printError("Right parenthesis is missing");
                exit(0);
            }
            
            return exp;

        } else if (match(Token::LBRACKET)) {

            if (match(Token::INT)) {
                int start = stoi(previous.lexema);
                if (!match(Token::POINT)) {
                    printError("Point or points are missing");
                    exit(0);
                }
                if (!match(Token::INT)) {
                    printError("End of interval is missing");
                    exit(0);
                }
                int end = stoi(previous.lexema);
                if (!match(Token::RBRACKET)) {
                    printError("Right bracket is missing");
                    exit(0);
                }
                return new NrintervalExp(start, end);
                
            } else {
                printError("No match for token " + current.to_string() + " after left bracket");
                exit(0);
            }

        } else if (match(Token::CHAR)) {

            return new CharExp(previous.lexema);
            
        } else {
            std::cout << "No match for token: " << current.to_string() << std::endl;
            exit(0);  
        }
    }

    
};

