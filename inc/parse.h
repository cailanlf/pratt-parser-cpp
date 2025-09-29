#pragma once

#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <memory>
#include <tuple>
#include <optional>

#include "lex.h"
#include "nodes.h"

namespace PrattParser {
    class Parser {
        const std::vector<Token>& tokens;
        size_t position;

        static const int Precedence_Assignment = 0;
        static const int Precedence_Addition = 1;
        static const int Precedence_Multiplication = 2;
        static const int Precedence_Negation = 3;
        static const int Precedence_Exponentiation = 4;

    public:
        Parser(const std::vector<Token>& tokens) : tokens(tokens), position(0) {};
        std::unique_ptr<RootNode> parse();
    private:
        std::unique_ptr<ExprNode> parse_expression(int min_bp = 0);
        std::unique_ptr<ExprNode> parse_atomic();

        static int get_prefix_precedence(const Token& token);
        static int get_postfix_precedence(const Token& token);
        static std::pair<int, int> get_binary_precedence(const Token& token);

        static constexpr std::pair<int, int> left_associative(int level);
        static constexpr std::pair<int, int> right_associative(int level);
        static constexpr int unary(int level);

        const Token& peek() const noexcept;
        bool peek_is(TokenType type, const char* content) const noexcept;
        bool peek_is(TokenType type) const noexcept;
        const Token& consume();
        const Token& expect(TokenType type, const char *content);
        const Token& expect(TokenType type);
    };
}