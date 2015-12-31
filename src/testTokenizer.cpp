
#include <stdio.h>
#include <iostream>
#include "Tokenizer.h"
int main(int argc, const char * argv[]) {
    Tokenizer tokenizer("../dataset/PerLoc.aql", true);
    std::vector<Text_token> x = tokenizer.scan();
    for (int i = 0; i < x.size(); i++) {
        std::cout << x[i].value
             << "("
             << x[i].from
             << ","
             << x[i].to
             << ")"
            << std::endl;
    }
    return 0;
}
