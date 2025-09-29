#include "nodes.h"

namespace PrattParser {
    BinaryOperator str_to_binary_op(std::string str) {
        if (str == "+") {
            return BinaryOperator::ADDITION;
        }
        else if (str == "-") {
            return BinaryOperator::SUBTRACTION;
        }
        else if (str == "*") {
            return BinaryOperator::MULTIPLICATION;
        }
        else if (str == "/") {
            return BinaryOperator::DIVISION;
        }
        else {
            std::string message { "Unrecognized binary operator " };
            message += str;
            throw std::range_error { message };
        }
    }

    std::string binary_op_to_str(BinaryOperator op) {
        switch (op) {
            case BinaryOperator::ADDITION: return "+";
            case BinaryOperator::SUBTRACTION: return "-";
            case BinaryOperator::MULTIPLICATION: return "*";
            case BinaryOperator::DIVISION: return "/";
            default: return "?";
        }
    }

    UnaryOperator str_to_prefix_op(std::string str) {
        if (str == "+") {
            return UnaryOperator::POSITIVE;
        }
        else if (str == "-") {
            return UnaryOperator::NEGATIVE;
        }
        else {
            std::string message { "Unrecognized prefix operator " };
            message += str;
            throw std::range_error { message };
        }
    }

    UnaryOperator str_to_postfix_op(std::string str) {
        std::string message { "Unrecognized postfix operator " };
        message += str;
        throw std::range_error { message };
    }

    std::string unary_op_to_str(UnaryOperator op) {
        switch (op) {
            case UnaryOperator::NEGATIVE: return "-";
            case UnaryOperator::POSITIVE: return "+";
            default: 
                std::string message { "Cannot get string representation of unrecognized unary operator value "};
                message += std::to_string(op);
                throw std::logic_error("Can't get string rep for unary operator value");
        }
    }
}