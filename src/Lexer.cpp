#include <cstdio>
#include "Lexer.h"
#include "Tokenizer.h"

std::string keyword[] = {
    "create", "view", "as", "output", "select",  "from", "extract", "regex",
    "on", "return",  "group",  "and",  "Token",  "pattern", ".", ",", ";",
    "(", ")", "<", ">", "{", "}"
};

Lexer::Lexer(const std::string & path) {
    Tokenizer *tokenizer = new Tokenizer(path, true);
    text_tokens = tokenizer->scan();
}

std::vector<Token> Lexer::scan() {
    int i, from;
    std::string tmp;
    Type type;
    for (i = 0; i < text_tokens.size(); i++) {
        type = isKey(text_tokens[i]);
        if (type != ERROR) {
            tokens.push_back(
                Token(text_tokens[i].value,
                      type,
                      text_tokens[i].from,
                      text_tokens[i].to));
        } else if (text_tokens[i].value == "/") {
            from = text_tokens[i++].from + 1;
            tmp.clear();
            while (text_tokens[i].value != "/")
                tmp += text_tokens[i++].value;
            tokens.push_back(
                Token(tmp, REGEX_, from, text_tokens[i].to-1));
        } else {
            type = idOrNum(text_tokens[i]);
            tokens.push_back(
                Token(text_tokens[i].value,
                      type,
                      text_tokens[i].from,
                      text_tokens[i].to));
        }
    }
    return tokens;
}

Type Lexer::isKey(const Text_token & token) const  {
    for (int i = 0; i < 23; i++)
        if (token.value == keyword[i])
            return static_cast<Type>(i);
    return ERROR;
}

