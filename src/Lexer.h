//
//  Lexer.h
//  compiler
//
//  Created by 牛仁鹏 on 15/12/20.
//  Copyright © 2015年 牛仁鹏. All rights reserved.
//

#ifndef Lexer_h
#define Lexer_h
#include<string>
#include<vector>
using namespace std;

typedef enum {
    CREATE, VIEW, AS, OUTPUT, SELECT, FROM, EXTRACT, REGEX,
    ON, RETURN, GROUP, AND, TOKEN, PATTERN,
    ID, NUM, DOT,
    COMMA,// , 逗号
    SEMICOLON, //;　分号
    OPENPAREN, CLOSEPAREN, //()　圆括号
    OPENBRACE, CLOSEBRACE, //{} 花括号
    OPENBRACKET, CLOSEBRACKET // []
}Type;

struct Token {
    int from, to;
    Type type;
    string value;
    Token(string s, Type t){
        value = s;
        type = t;
    }
};

class Lexer {
private:
    vector<Token> tokens;
    
public:
    Lexer();
    vector<Token> scan();
};
#endif /* Lexer_h */
