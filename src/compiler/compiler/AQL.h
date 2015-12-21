//
//  AQL.h
//  compiler
//
//  Created by 牛仁鹏 on 15/12/21.
//  Copyright © 2015年 牛仁鹏. All rights reserved.
//
#ifndef AQL_h
#define AQL_h

#include <iostream>
#include <vector>
#include "Tokenizer.h"
using namespace std;

struct Span {
    int from, to;
    string value;
    Span(string x, int f, int t) {
        value = x;
        from = f;
        to = t;
    }
};
struct Column {
    string name;
    vector<Span> spans;
};

class View {
private:
    string name;
    vector<Column> columns;
public:
    View(string);
    void output();
    void extract_regex(string regex,
                       vector<int> groups,
                       vector<string> column_names,
                       string path);
    void extract_pattern();
    void select(vector<View> from_list,
                vector<string> column_names_from,
                vector<string> column_names);
    void output(string name = "");
};


#endif /* AQL_h */
