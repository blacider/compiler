//
//  AQL.cpp
//  compiler
//
//  Created by 牛仁鹏 on 15/12/21.
//  Copyright © 2015年 牛仁鹏. All rights reserved.
//

#include <stdio.h>
#include "AQL.h"
extern vector<vector<int> > findall(const char *regex, const char *content);
View::View(string name_) {
    name = name_;
}
void View::output(string x) {
    
}
void View::extract_regex(string regex,
                         vector<int> groups,
                         vector<string> column_names,
                         string text) {
    vector<vector<int> > result = findall(regex.c_str(), text.c_str());
    columns.clear();
    for (int col = 0; col < groups.size(); col++) {
        columns.push_back(Column(column_names[col]));
        for (int i = 0; i < result.size(); i++) {
            int from = result[i][groups[col]*2]
            ,to = result[i][groups[col]*2+1];
            columns[col].spans.push_back(Span(text.substr(from, to-from), from, to));
        }
    }
}

void extract_pattern(vector<Atom> atoms,
                     vector<int> groups,
                     vector<string> columns_names){
    
}

void View::select(vector<Column> columns_, vector<string> column_names) {
    columns.clear();
    for (int i = 0; i < column_names.size(); i++) {
        columns.push_back(columns_[i]);
        columns[i].name = column_names[i];
    }
}
