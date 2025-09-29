#include "interpret.h"

namespace PrattParser {
    long long interpret(const ExprNode& node) {
        if (auto *a = dynamic_cast<const NumberNode *>(&node)) {
            long long value = std::stoll(a->value);
            return value;
        }
        else if (auto *a = dynamic_cast<const UnaryOperatorNode *>(&node)) {
            auto value = interpret(*a->operand);
            switch (a->operation) {
                case UnaryOperator::NEGATIVE:
                    return -value;
                case UnaryOperator::POSITIVE:
                    return +value;
                default: 
                    throw std::logic_error("interpret: Encountered unexpected unary operator (value) " + a->operation);
            }
        }
        else if (auto *a = dynamic_cast<const BinaryOperatorNode *>(&node)) {
            auto left = interpret(*a->left);
            auto right = interpret(*a->right);
            switch (a->operation) {
                case BinaryOperator::ADDITION:
                    return left + right;
                case BinaryOperator::SUBTRACTION:
                    return left - right;
                case BinaryOperator::MULTIPLICATION:
                    return left * right;
                case BinaryOperator::DIVISION:
                    return left / right;
                default: 
                    throw std::logic_error("interpret: Encountered unexpected binary operator (value) " + a->operation);
            }
        }
        else {
            throw std::logic_error("interpret: Encountered unexpected node type");
        }
    }
}