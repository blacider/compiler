//
//  testLexer.cpp
//  compiler
//
//  Created by 牛仁鹏 on 15/12/21.
//  Copyright © 2015年 牛仁鹏. All rights reserved.
//

#include <stdio.h>
#include "Lexer.h"
#include <iostream>
int main(int argc, const char * argv[]) {
    Lexer lexer("../dataset/PerLoc.aql");
    vector<Token> x = lexer.scan();
    for (int i = 0; i < x.size(); i++) {
        cout << x[i].value
        << ":"
        << x[i].type
        << "("
        << x[i].from
        << ","
        << x[i].to
        << ")"
        << endl;
    }
    return 0;
}
