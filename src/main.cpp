#include <string>
#include <iostream>
#include <iomanip>

#include "lex.h"
#include "parse.h"
#include "interpret.h"

using namespace PrattParser;

int main(int argc, char *argv[]) {
    std::stringstream s;

    for (int i = 1; i < argc; i++) {
        std::cout << argv[i] << std::endl;
        s << argv[i] << " ";
    }

    Lexer lexer { s.str() };
    auto tokens = lexer.lex();

    for (const Token& token : *tokens) {
        std::cout 
            << PrattParser::tokentype_to_str(token.type) << " "
            << "`" << token.content << "` "
            << "(" << token.pos.start << ", " << token.pos.end << ")"
            << std::endl;
    }

    Parser parser { *tokens };
    auto root = parser.parse();
    root->pretty_print(0);

    std::cout << interpret(*root->value) << std::endl;
}