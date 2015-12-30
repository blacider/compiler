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
#include "AQL.h"
#include<map>
class Parser {
private:
    vector<Text_token> Text_tokens;
    map<string, View> views;
    vector<Token> tokens;
    map<string, string> from_lists;
    map<string, string> select_lists;
    map<string, string> aliass;
    vector<string> from_items;
    vector<string> select_items;
    vector<int> groups;
    vector<int> groups_pattern;
    vector<vector<int> > groups_paren;//捕获块
    vector<string> col_names;
    vector<Atom> atoms;
    vector<string> col;
    map<string, string> cols;
    int look, end;
    //parser
    void move();
    void match(Type);
    //aql
    void aql_stmt();
    void create_stmt();
    void output_stmt();
    void view_stmt(string);
    void select_stmt(string);
    void extract_stmt(string);
    string alias();
    //Select Statement
    void select_list();
    void from_list();
    void select_item();
    void from_item();
    //Extract Statement--regular
    void extract_spec(string);
    void regex_spec(string);
    void column();
    void name_spec();
    void group_spec();
    void single_group();
    //Extract Statement--pattern
    void pattern_spec(string);
    void pattern_expr();
    void pattern_pkg();
    void atom();
    void pattern_group();
    string text;
public:
    Parser(string, Tokenizer);//第一个是 aql 文件 path，第二个是文本 text
    void error(string);
    void program();
};

#endif /* Parser_h */
