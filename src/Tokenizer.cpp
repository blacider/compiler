//
//  Tokenizer.cpp
//  compiler
//
//  Created by 牛仁鹏 on 15/12/20.
//  Copyright © 2015年 牛仁鹏. All rights reserved.
//

#include "Tokenizer.h"
#include <fstream>
#include <cstdio>
#include <iostream>
#include <cstdlib>
Tokenizer::Tokenizer(string path_) {
    path = path_;
}
vector<Text_token> Tokenizer::scan() {
	FILE *file = fopen(path.c_str(), "r");
    if (file == NULL) {
        cout << "error, wrong path" << endl;
        exit(1);
    }
    char tmp;
    string str;
    int pos = 0;
    while ((tmp = fgetc(file)) != EOF) {
        text += tmp;
        if (isBlock(tmp)) {
            if (str.length() != 0) {
                tokens.push_back(Text_token(str, pos-(int)str.length(), pos));
                str = "";
            }
            if (!(tmp == ' ' || tmp == '\n'
                || tmp == '\t' || tmp == '\r')) {
                str += tmp;
                tokens.push_back(Text_token(str, pos, pos+1));
                str = "";
            }
        } else {
            str += tmp;
        }
        
        pos++;
    }
    return tokens;
}
bool Tokenizer::isBlock(char c) {
    return !(((c >= 'a' && c <= 'z')  ||
              (c >= 'A' && c <= 'Z')) ||
              (c >= '0' && c <= '9'));
}
