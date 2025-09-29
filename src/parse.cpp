#include "parse.h"

namespace PrattParser {

    std::unique_ptr<RootNode> Parser::parse() {
        auto expr = parse_expression();
        // expect(TokenType::EOI);
        return std::make_unique<RootNode>(RootNode {
            std::move(expr)
        });
    }

    std::unique_ptr<ExprNode> Parser::parse_expression(int min_bp) {
        int prefix_prec = get_prefix_precedence(peek());
        std::unique_ptr<ExprNode> left;

        if (prefix_prec >= min_bp) {
            auto op = str_to_prefix_op(consume().content);
            auto right = parse_expression(prefix_prec);

            left = std::make_unique<UnaryOperatorNode>(UnaryOperatorNode {
                op,
                std::move(right)
            });
        } else {
            left = std::move(parse_atomic());
        }

        while (true) {
            int postfix_prec = get_postfix_precedence(peek());
            
            if (postfix_prec < min_bp) break;

            auto op = str_to_postfix_op(consume().content);
            left = std::make_unique<UnaryOperatorNode>(UnaryOperatorNode {
                op,
                std::move(left)
            });
        }

        while (true) {
            auto[left_prec, right_prec] = get_binary_precedence(peek());

            if (left_prec < min_bp) break;

            auto op = str_to_binary_op(consume().content);
            auto right = parse_expression(right_prec);

            left = std::make_unique<BinaryOperatorNode>( BinaryOperatorNode {
                op,
                std::move(left),
                std::move(right)
            });
        }

        return left;
    }

    std::unique_ptr<ExprNode> Parser::parse_atomic() {
        if (peek_is(TokenType::NUMBER)) {
            return std::make_unique<NumberNode>(NumberNode { consume().content });
        }
        else if (peek_is(TokenType::PARENTHESIS, "(")) {
            expect(TokenType::PARENTHESIS, "(");
            auto expr = parse_expression();
            expect(TokenType::PARENTHESIS, ")");
            return std::make_unique<ParenthesisExprNode>(ParenthesisExprNode {
                std::move(expr)
            });
        }
        else {
            std::string message { "Encountered unexpected token " };
            message += tokentype_to_str(peek().type);
            throw std::logic_error { message };
        }
    }


    int Parser::get_prefix_precedence(const Token& token) {
        if (token.type == TokenType::OPERATOR && (token.content == "+" || token.content == "-")) {
            return unary(Precedence_Negation);
        }
        else return -1;
    }

    int Parser::get_postfix_precedence(const Token& token) {
        return -1;
    }

    std::pair<int, int> Parser::get_binary_precedence(const Token& token) {
        if (token.type == TokenType::OPERATOR && (token.content == "+" || token.content == "-")) {
            return left_associative(Precedence_Addition);
        }
        else if (token.type == TokenType::OPERATOR && (token.content == "*" || token.content == "/")) {
            return left_associative(Precedence_Multiplication);
        }
        else return std::make_pair(-1, -1);
    }


    constexpr std::pair<int, int> Parser::right_associative(int level) {
        return std::make_pair((level + 1) * 2, (level + 1) * 2 - 1);
    }

    constexpr std::pair<int, int> Parser::left_associative(int level) {
        return std::make_pair((level + 1) * 2 - 1, (level + 1) * 2);
    }

    constexpr int Parser::unary(int level) {
        return (level + 1) * 2;
    }


    const Token& Parser::peek() const noexcept {
        if (position > tokens.size()) {
            return tokens.back();
        } else {
            return tokens[position];
        }
    }

    bool Parser::peek_is(TokenType type, const char *content) const noexcept {
        const Token& peeked = peek();
        return peeked.type == type && peeked.content == content;
    }

    bool Parser::peek_is(TokenType type) const noexcept {
        const Token& peeked = peek();
        return peeked.type == type;
    }

    const Token& Parser::consume() {
        if (position > tokens.size()) {
            throw std::overflow_error("Consumed beyond end of token stream");
        } else {
            return tokens[position++];
        }
    }

    const Token& Parser::expect(TokenType type, const char *content) {
        if (!peek_is(type, content)) {
            std::stringstream s;
            s 
                << "Expected token of type " << tokentype_to_str(type)
                << " with content '" << content
                << "', but got token of type" << tokentype_to_str(peek().type)
                << " with content '" << peek().content << "'\n";
            throw std::logic_error(s.str());
        }
        return consume();
    }

    const Token& Parser::expect(TokenType type) {
        if (!peek_is(type)) {
            std::stringstream s;
            s 
                << "Expected token of type " << tokentype_to_str(type)
                << ", but got token of type " << tokentype_to_str(peek().type)
                << "\n";
        }

        return consume();
    }
}