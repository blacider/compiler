#pragma once

#include <string>
#include <vector>

struct Text_token {
    int from, to;
    std::string value;
    Text_token(const std::string & s, const int & f, const int & t) :
        value(s), from(f), to(t) { }
};

class Tokenizer {
private:
    bool isAQL;
    std::vector<Text_token> tokens;
    std::string path, text;
    inline bool isBlock(const char & c) {
        return !(((c >= 'a' && c <= 'z')  ||
                (c >= 'A' && c <= 'Z')) ||
                (c >= '0' && c <= '9'));
    }
public:
    Tokenizer(const std::string &, const bool &);
    std::vector<Text_token> scan();
    inline const std::string & getText() const {
        return text;
    }
    inline std::string & getText() {
        return text;
    }
};

