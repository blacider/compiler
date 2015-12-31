#include <fstream>
#include <map>
#include <vector>
#include <iostream>
#include <cstdlib>
#include "Tokenizer.h"

Tokenizer::Tokenizer(const std::string & path_, const bool & isAql) :
    path(path_), isAQL(isAql) { }

std::vector<Text_token> Tokenizer::scan() {
    std::ifstream file(path.c_str());
    if (!file.is_open()) {
        std::cerr << "error, wrong path" << std::endl;
        exit(1);
    }

    char tmp;
    std::string str;
    int pos = 0;
    while (file.get(tmp)) {
        if (isAQL && tmp == '/') {
            str.clear();
            tokens.push_back(Text_token("/", pos - 1, pos));
            pos++;
            for  (file.get(tmp); tmp != '/'; file.get(tmp), ++pos)
                str.push_back(tmp);
        }
        text.push_back(tmp);
        if (isBlock(tmp)) {
            if (str.length() != 0) {
                tokens.push_back(Text_token(str, pos-str.length(), pos));
                str.clear();
            }
            if (!(tmp == ' ' || tmp == '\n'
                || tmp == '\t' || tmp == '\r')) {
                str.push_back(tmp);
                tokens.push_back(Text_token(str, pos, pos + 1));
                str.clear();
            }
        } else {
            str += tmp;
        }
        ++pos;
    }
    file.close();
    return tokens;
}

