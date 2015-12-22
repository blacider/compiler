//
//  testAQL.cpp
//  compiler
//
//  Created by 牛仁鹏 on 15/12/22.
//  Copyright © 2015年 牛仁鹏. All rights reserved.
//

#include <stdio.h>
#include "AQL.h"
#include <iostream>
#include <string>
#include<vector>

int main() {
    //tokennizer
    Tokenizer tokenizer("../dataset/perloc/PerLoc.input");
    vector<Text_token> text_tokens = tokenizer.scan();
    //测试 extract_regex
    View cap("cap");
    vector<int> groups;
    groups.push_back(0);
    groups.push_back(1);
    vector<string> column_names;
    column_names.push_back("Cap");
    column_names.push_back("col2");
    string text = tokenizer.text;
    cap.extract_regex("([A-Z])[a-z]*", groups, column_names, text);
    cap.output("");
    //测试 select
    View perloc("perloc");
    vector<string> column_names2;
    column_names2.push_back("col1");
    vector<Column> columns2;
    columns2.push_back(cap.columns[1]);
    perloc.select(columns2, column_names2);
    //测试 extract_pattern
    
}