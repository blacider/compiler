//
//  testParser.cpp
//  compiler
//
//  Created by 牛仁鹏 on 15/12/21.
//  Copyright © 2015年 牛仁鹏. All rights reserved.
//

#include <iostream>
#include "Parser.h"

using namespace std;

int main(int argc, const char * argv[]) {
    //Parser parser("../dataset/Revenue.aql");
    Parser parser("../dataset/PerLoc.aql");
    parser.program();
    return 0;
}