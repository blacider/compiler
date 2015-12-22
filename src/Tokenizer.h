//
//  Tokenizer.hpp
//  compiler
//
//  Created by 牛仁鹏 on 15/12/20.
//  Copyright © 2015年 牛仁鹏. All rights reserved.
//

#ifndef Tokenizer_h
#define Tokenizer_h

#include <string>
#include <vector>
using namespace std;
struct Text_token {
    int from, to;
    string value;
    Text_token(string s, int f, int t) {
        value = s;
        from = f;
        to = t;
    }
};

class Tokenizer {
private:
    vector<Text_token> tokens;
    string path;
    bool isBlock(char c); // 判断是否非数字也非字母
public:
    Tokenizer(string);
    vector<Text_token> scan();
    string text;
};

#endif /* Tokenizer_h */
