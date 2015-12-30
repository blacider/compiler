//
//  main.cpp
//  compiler
//
//  Created by 牛仁鹏 on 15/12/30.
//  Copyright © 2015年 牛仁鹏. All rights reserved.
//

#include <stdio.h>
#include "Tokenizer.h"
#include "Parser.h"
int main() {
    Tokenizer tokenizer("../dataset/revenue/Revenue.input");
    Parser parser("../dataset/Revenue.aql", tokenizer);
    //Tokenizer tokenizer("../dataset/perloc/PerLoc.input");
    //Parser parser("../dataset/PerLoc.aql", tokenizer);
    parser.program();
    return 0;
}