#pragma once

#include <vector>
#include <memory>
#include <string>
#include <stdexcept>

namespace PrattParser {
    enum TokenType {
        OPERATOR,
        NUMBER,
        PARENTHESIS,
        EOI,
    };

    std::string tokentype_to_str(TokenType type);

    struct Position {
        int start;
        int end;
    };

    struct Token {
        TokenType type;
        Position pos;
        std::string content;
    };

    class Lexer {
        std::string source;
        size_t position;

    public:
        Lexer(const std::string& src) : source(src), position(0) {}
        std::unique_ptr<std::vector<Token>> lex();

    private:
        char peek() const noexcept;
        char consume();
        Token lex_number();
        static bool is_digit(char c);
        static bool is_alpha(char c);
        static bool is_whitespace(char c);
        static Token make_token(TokenType type, int start, std::string content);
    };
}