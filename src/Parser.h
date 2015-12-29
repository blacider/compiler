//
//  Parser.h
//  compiler
//
//  Created by 牛仁鹏 on 15/12/21.
//  Copyright © 2015年 牛仁鹏. All rights reserved.
//

#ifndef Parser_h
#define Parser_h
#include "Lexer.h"
class Parser {
private:
    map<string, View> views;
    vector<Token> tokens;
    int look, end;
    //parser
    void move();
    void match(Type);
    //aql
    void aql_stmt();
    void create_stmt();
    void output_stmt();
    void view_stmt();
    void select_stmt();
    void extract_stmt();
    void alias();
    //Select Statement
    void select_list();
    void from_list();
    void select_item();
    void from_item();
    //Extract Statement--regular
    void extract_spec();
    void regex_spec();
    void column();
    void name_spec();
    void group_spec();
    void single_group();
    //Extract Statement--pattern
    void pattern_spec();
    void pattern_expr();
    void pattern_pkg();
    void atom();
    void pattern_group();
public:
    Parser(string);
    void error(string);
    void program();
};

#endif /* Parser_h */
