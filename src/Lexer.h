#pragma once
#include<string>
#include<vector>
#include "Tokenizer.h"

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
} Type;

struct Token {
    int from, to;
    Type type;
    std::string value;
    Token(const std::string & s, const Type & t, const int & f, const int & x):
        value(s), type(t), from(f), to(x) { }
};

class Lexer {
private:
    std::vector<Token> tokens;
    std::vector<Text_token> text_tokens;
public:
    Lexer(const std::string &);
    std::vector<Token> scan();
    Type isKey(const Text_token &) const;
    inline Type idOrNum(const Text_token & x) const {
        return (x.value[0] >= '0' && x.value[0] <= '9') ? NUM : ID;
    }
};

