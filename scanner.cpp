#include <iostream>

struct Token {
    enum Type {
        LPAREN = 0,
        RPAREN,
        PLUS,
        ASTERISK,
        QMARK,
        CHAR,
        INT,
        POINT,
        LBRACKET,
        RBRACKET,
        UNION,
        END,
        ERR,
    };
    Type type;
    std::string lexema;

    const std::string to_string() {
        if (type == Type::LPAREN) {
            return "Left parenthesis";
        } else if (type == Type::RPAREN) {
            return "Right parenthesis";
        } else if (type == Type::PLUS) {
            return "Plus";
        } else if (type == Type::ASTERISK) {
            return "Asterisk";
        } else if (type == Type::QMARK) {
            return "Question mark";
        } else if (type == Type::CHAR) {
            if (lexema.empty()) {
                return "CHAR (With empty lexema)";
            } else {
                return "CHAR (" + lexema + ")";
            }
        } else if (type == Type::INT) {
            if (lexema.empty()) {
                return "INT (With empty lexema)";
            } else {
                return "INT (" + lexema + ")";
            }
        } else if (type == Type::POINT) {
            return "Point";
        } else if (type == Type::LBRACKET) {
            return "Left bracket ";
        } else if (type == Type::RBRACKET) {
            return "Right bracket";
        } else if (type == Type::UNION) {
            return "Union";
        } else if (type == Type::END) {
            return "End";
        } else if (type == Type::ERR) {
            return "Error";
        } 
        return "No token";
    }
    
    Token() = default;
    Token(Type type): type(type) {};
    Token(Type type, const std::string lexema): type(type), lexema(lexema) {};
};


struct Scanner {
    std::string input;
    int index = 0;
    char current;

    Scanner(const std::string &input): input(input) {
        current = input[index];
    }

    void advance() {
        index++;
        if (index < input.size()) {
            current = input[index];
        } else {
            current = '\0';
        }
    }

    Token nextToken() {
        while (current != '\0') {
            switch (current) {
                case '(': advance(); return Token(Token::LPAREN);
                case ')': advance(); return Token(Token::RPAREN);
                case '+': advance(); return Token(Token::PLUS);
                case '*': advance(); return Token(Token::ASTERISK);
                case '[': advance(); return Token(Token::LBRACKET);
                case ']': advance(); return Token(Token::RBRACKET);
                case '|': advance(); return Token(Token::UNION);
                case '?': advance(); return Token(Token::QMARK);
                case ' ': advance(); return Token(Token::ERR);
                default:
                    if (isdigit(current)) {
                        std::string lexema;
                        while (isdigit(current)) {
                            lexema += current;
                            advance();
                        }
                        return Token(Token::INT, lexema);
                    }
                    else if (current == '.') {
                        while (current == '.') {
                            advance();
                        }
                        return Token(Token::POINT);
                    }
                    else if (isalpha(current)) {
                        std::string lexema;
                        while (isalpha(current)) {
                            lexema += current;
                            advance();
                        }
                        return Token(Token::CHAR, lexema);
                    } else {
                        advance();
                        return Token(Token::ERR);
                    }
            }
        }
        return Token(Token::END);
    }
};
