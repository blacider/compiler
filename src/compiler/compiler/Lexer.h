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
#include "Tokenizer.h"
using namespace std;

typedef enum {
    CREATE, VIEW, AS, OUTPUT, SELECT, FROM, EXTRACT, REGEX,
    ON, RETURN, GROUP, AND, TOKEN, PATTERN,
    DOT,
    COMMA,// , 逗号
    SEMICOLON, //;　分号
    OPENPAREN, CLOSEPAREN, //()　圆括号
    LESSTHAN, GREATETHAN, // <>
    OPENBRACE, CLOSEBRACE, //{} 花括号
    ERROR ,// 错误类型
    REGEX_ ,// 正则表达式
    ID, NUM
}Type;

struct Token {
    int from, to;
    Type type;
    string value;
    Token(string s, Type t, int f, int x){
        value = s;
        type = t;
        from = f;
        to = x;
    }
};

class Lexer {
private:
    vector<Token> tokens;
    vector<Text_token> text_tokens;
public:
    Lexer(string);
    vector<Token> scan();
    Type isKey(Text_token);
    Type idOrNum(Text_token);
};
#endif /* Lexer_h */
