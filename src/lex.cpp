#include "lex.h"

namespace PrattParser {
    std::string tokentype_to_str(TokenType type) {
        switch (type) {
        case TokenType::EOI:
            return "EOI";
        case TokenType::NUMBER:
            return "NUMBER";
        case TokenType::OPERATOR:
            return "OPERATOR";
        case TokenType::PARENTHESIS:
            return "PARENTHESIS";
        default:
            return "?";
        }
    }
    
    std::unique_ptr<std::vector<Token>> Lexer::lex() {
        auto tokens = std::make_unique<std::vector<Token>>();

        while (true) {
            char peeked = peek();

            if (peeked == '\0') {
                tokens->push_back(make_token(TokenType::EOI, position, ""));
                return tokens;
            } 
            else if (peeked == '+' || peeked == '-' || peeked == '/' || peeked == '*') {
                Token token = make_token(TokenType::OPERATOR, position, std::string { consume() } );
                tokens->push_back(token);
            }
            else if (peeked == '(' || peeked == ')') {
                Token token = make_token(TokenType::PARENTHESIS, position, std::string { consume() });
                tokens->push_back(token);
            }
            else if (is_digit(peeked)) {
                Token token = lex_number();
                tokens->push_back(token);
            }
            else if (is_whitespace(peeked)) {
                while (is_whitespace(peek())) {
                    consume();
                }
            } else {
                std::string message { "Unrecognized character "}; 
                message += peeked;
                throw std::logic_error(message);
            }
        }
    }

    char Lexer::peek() const noexcept {
        if (this->position > this->source.length()) {
            return '\0';
        } else {
            return this->source[this->position];
        }
    }

    char Lexer::consume() {
        if (this->position > this->source.length()) {
            throw std::overflow_error("Tried to consume beyond length of input");
        } else {
            return this->source[this->position++];
        }
    }

    Token Lexer::lex_number() {
        auto start = position;

        while (is_digit(peek())) {
            consume();
        }

        if (start == position) {
            throw std::logic_error("Lexed number of length 0.");
        }

        return make_token(TokenType::NUMBER, start, source.substr(start, position - start));
    }

    bool Lexer::is_digit(char c) {
        return c >= '0' && c <= '9';
    }

    bool Lexer::is_alpha(char c) {
        return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'); 
    }

    bool Lexer::is_whitespace(char c) {
        return c == '\t' || c == '\n' || c == ' ' || c == '\r';
    }

    Token Lexer::make_token(TokenType type, int start, std::string content) {
        return {
            .type = type,
            .pos = {
                .start = start,
                .end = start + (int)content.length()
            },
            .content = content,
        };
    }
}
