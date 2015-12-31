#include <stdio.h>
#include <iostream>
#include "Lexer.h"

int main(int argc, const char * argv[]) {
    Lexer lexer("../dataset/PerLoc.aql");
    std::vector<Token> x = lexer.scan();
    for (int i = 0; i < x.size(); i++) {
        std::cout << x[i].value
        << ":"
        << x[i].type
        << "("
        << x[i].from
        << ","
        << x[i].to
        << ")"
        << std::endl;
    }
    return 0;
}
