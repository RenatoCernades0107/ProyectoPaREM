#ifndef PARSER
#define PARSER

#include <iostream>
#include "scanner.cpp"
#include "AST.hpp"

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
        if (current.type != Token::END) {
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

        ExpList *expl = parseRE();

        if (current.type != Token::END) {
            std::cout << "Error: " << current.lexema << std::endl;
            return nullptr;
        }

        return new AST(expl);
    }

    ExpList* parseRE() {
        ExpList *expl = new ExpList();
        Exp* e = parseBorExp();
        
        while (e != nullptr) {
            expl->add(e);
            e = parseBorExp();
        }
        return expl;
    }

    Exp *parseBorExp() {
        Exp *leftexp, *rightexp;
        leftexp = parseUnaryExp();
        BorExp *bor = nullptr;
        if (match(Token::UNION)) {
            rightexp = parseBorExp(); 
            bor = new BorExp(leftexp, rightexp);    
        }
        if (bor == nullptr) {
            return leftexp;
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
            ExpList *exp = parseRE();
            if (!match(Token::RPAREN)) {
                printError("Right parenthesis is missing");
                exit(0);
            }
            
            return new GroupExp(exp);

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
        }

        return nullptr;
    }

    
};

#endif