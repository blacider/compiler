//
//  Lexer.cpp
//  compiler
//
//  Created by 牛仁鹏 on 15/12/20.
//  Copyright © 2015年 牛仁鹏. All rights reserved.
//

#include <stdio.h>
#include "Lexer.h"
string keyword[] = {
    "create", "view", "as", "output", "select",  "from", "extract", "regex",  "on", "return",  "group",  "and",  "Token",  "pattern", ".", ",", ";", "(", ")", "<", ">", "{", "}"
};
#include "Tokenizer.h"
Lexer::Lexer(string path) {
    Tokenizer *tokenizer = new Tokenizer(path);
    text_tokens = tokenizer->scan();
}
vector<Token> Lexer::scan() {
    for (int i = 0; i < text_tokens.size(); i++) {
        Type type = isKey(text_tokens[i]);
        if (type != ERROR) {
            tokens.push_back(Token(text_tokens[i].value,
                                   type,
                                   text_tokens[i].from,
                                   text_tokens[i].to));
        } else if (text_tokens[i].value == "/") {
            string tmp;
            int from = text_tokens[i].from+1;
            i++;
            while (text_tokens[i].value != "/") {
                tmp += text_tokens[i].value;
                i++;
            }
            tokens.push_back(Token(tmp,
                                   REGEX_,
                                   from,
                                   text_tokens[i].to-1));
        } else {
            type = idOrNum(text_tokens[i]);
            tokens.push_back(Token(text_tokens[i].value,
                                   type,
                                   text_tokens[i].from,
                                   text_tokens[i].to));
        }
    }
    return tokens;
}
Type Lexer::isKey(Text_token token) {
    for (int i = 0; i < 23; i++) {
        if (token.value == keyword[i]) {
            Type result = (Type)i;
            return result;
        }
    }
    return ERROR;
}
Type Lexer::idOrNum(Text_token x) {
    if (x.value[0] >= '0' && x.value[0] <= '9')
        return NUM;
    else return ID;
}