//
//  Parser.h
//  compiler
//
//  Created by 牛仁鹏 on 15/12/21.
//  Copyright © 2015年 牛仁鹏. All rights reserved.
//

#ifndef Parser_h
#define Parser_h
#include "Lexer.h"
class Parser {
private:
    vector<Token> tokens;
public:
    Parser(string path);
    void error(string);
};

#endif /* Parser_h */
