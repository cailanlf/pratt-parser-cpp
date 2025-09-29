#pragma once

#include <string>
#include <variant>
#include <memory>
#include <iomanip>
#include <iostream>

namespace PrattParser {
    const int INDENT_DEPTH = 8;

    struct Node {
        virtual void pretty_print(int indent) = 0;
    };

    struct ExprNode : Node {

    };

    struct RootNode : Node {
        std::unique_ptr<ExprNode> value;

        RootNode(std::unique_ptr<ExprNode> value)
            : value(std::move(value)){}

        void pretty_print(int indent) override {
            std::cout << std::setw(indent) << " " << "root" << std::endl;
            std::cout << std::setw(indent) << " " << "- expr:" << std::endl;
            value->pretty_print(indent + INDENT_DEPTH);
        }
    };

    enum BinaryOperator {
        ADDITION,
        SUBTRACTION,
        MULTIPLICATION,
        DIVISION,
    };

    BinaryOperator str_to_binary_op(std::string str);
    std::string binary_op_to_str(BinaryOperator op);

    struct BinaryOperatorNode : ExprNode {
        BinaryOperator operation;
        std::unique_ptr<ExprNode> left;
        std::unique_ptr<ExprNode> right;

        BinaryOperatorNode(BinaryOperator op, std::unique_ptr<ExprNode> l, std::unique_ptr<ExprNode> r) 
            : operation(op), left(std::move(l)), right(std::move(r)) {};

        void pretty_print(int indent) override {
            std::cout << std::setw(indent) << " " << "binary-op" << std::endl;
            std::cout << std::setw(indent) << " " << "- op: " << binary_op_to_str(operation) << std::endl;
            std::cout << std::setw(indent) << " " << "- left:" << std::endl;
            left->pretty_print(indent + INDENT_DEPTH);
            std::cout << std::setw(indent) << " " << "- right:" << std::endl;
            right->pretty_print(indent + INDENT_DEPTH);
        }
    };

    enum UnaryOperator {
        NEGATIVE,
        POSITIVE,
    };

    std::string unary_op_to_str(UnaryOperator op);
    UnaryOperator str_to_prefix_op(std::string str);
    UnaryOperator str_to_postfix_op(std::string str);
    
    struct UnaryOperatorNode : ExprNode {
        UnaryOperator operation;
        std::unique_ptr<ExprNode> operand;

        UnaryOperatorNode(UnaryOperator op, std::unique_ptr<ExprNode> expr) 
            : operation(op), operand(std::move(expr)) {};

        void pretty_print(int indent) override {
            std::cout << std::setw(indent) << " " << "unary-op" << std::endl;
            std::cout << std::setw(indent) << " " <<  "- op: " << unary_op_to_str(operation) << std::endl;
            std::cout << std::setw(indent) << " " <<  "- operand:" << std::endl;
            operand->pretty_print(indent + INDENT_DEPTH);
        }
    };

    struct NumberNode : ExprNode {
        NumberNode(std::string value) noexcept : value(value) {}; 
        std::string value;
        void pretty_print(int indent) override {
            std::cout << std::setw(indent) << " " << "number " << value << std::endl;
        }
    };
}