//
//  parser.cpp
//  compiler
//
//  Created by 牛仁鹏 on 15/12/21.
//  Copyright © 2015年 牛仁鹏. All rights reserved.
//

#include <cstdio>
#include <iostream>
#include <cstdlib>
#include "Parser.h"
#include <sstream>
#include <string>
int Number(string x) {
    stringstream ss;
    ss << x;
    int num_;
    ss >> num_;
    return num_;
}
Parser::Parser(string path, Tokenizer tokenizer) {
    Lexer lexer(path);
    tokens = lexer.scan();
    look = 0;
    end = (int)tokens.size()+1;
    Text_tokens = tokenizer.scan();
    text = tokenizer.text;
}
void Parser::move() {
    look++;
}
void Parser::match(Type type) {
    if (tokens[look].type == type) {
        move();
    } else {
        error("syntax error");
    }
}
void Parser::error(string x) {
    cout << "near position " << tokens[look].from
    << ":" << x << endl;
    exit(1);
}
void Parser::program() {
    while (look != end-1) {
        aql_stmt();
    }
    cout << "success" << endl;
}
void Parser::aql_stmt() {
    if (tokens[look].type == CREATE) {
        create_stmt();
    } else {
        output_stmt();
    }
}
void Parser::create_stmt() {
    match(CREATE);
    match(VIEW);
    string id = tokens[look].value;
    views[id] = View(id);
    match(ID);
    match(AS);
    view_stmt(id);
    match(SEMICOLON);
}
void Parser::view_stmt(string id) {
    if (tokens[look].type == SELECT) {
        select_stmt(id);
    } else {
        extract_stmt(id);
    }
}
void Parser::output_stmt() {
    match(OUTPUT);
    match(VIEW);
    string id = tokens[look].value;
    match(ID);
    string tmp = alias();
    match(SEMICOLON);
    views[id].output(tmp);
}
string Parser::alias() {
    if (tokens[look].type == AS) {
        match(AS);
        string id = tokens[look].value;
        match(ID);
        return id;
    }
    return "";
}
void Parser::select_stmt(string id) {
    match(SELECT);
    select_list();
    match(FROM);
    from_list();
    //TODO SELECT
    vector<Column> cols;
    vector<string> names;
    map<string, string>::iterator it;
    for (it = aliass.begin(); it != aliass.end(); it++) {
        names.push_back(it->first);
        string view = from_lists[select_lists[it->second]];
        cols.push_back(views[view].getColumn(it->second));
    }
    views[id].select(cols, names);
    
}
void Parser::select_list() {
    select_lists.clear();
    while (tokens[look].type != FROM) {
        select_items.clear();
        select_item();
        select_lists[select_items[1]] = select_items[0];
    }
}
void Parser::select_item() {
    aliass.clear();
    select_items.push_back(tokens[look].value);
    match(ID);
    match(DOT);
    string id2 = tokens[look].value;
    select_items.push_back(tokens[look].value);
    match(ID);
    string tmp = alias();
    if (tmp != "") {
        aliass[tmp] = id2;
    } else {
        aliass[id2] = id2;
    }
}
void Parser::from_list() {
    from_lists.clear();
    from_item();
    from_lists[from_items[1]] = from_items[0];
    while (tokens[look].type != SEMICOLON) {
        match(COMMA);
        from_item();
        from_lists[from_items[1]] = from_items[0];
    }
}
void Parser::from_item() {
    from_items.clear();
    from_items.push_back(tokens[look].value);
    match(ID);
    from_items.push_back(tokens[look].value);
    match(ID);
}
void Parser::extract_stmt(string id) {
    match(EXTRACT);
    atoms.clear();
    bool ifPattern = true;
    if (tokens[look].type == REGEX) {
        ifPattern = false;
    }
    extract_spec(id);
    match(FROM);
    from_list();
    if (ifPattern) {
        //TODO(1)根据 groups_paren 求 groups_pattern，
        for (int i = 0; i < groups.size(); i++) {
            if (groups[i] == 0) {
                groups_pattern.push_back(0);
                groups_pattern.push_back((int)atoms.size());
            } else {
                groups_pattern.push_back(groups_paren[groups[i]-1][0]);
                groups_pattern.push_back(groups_paren[groups[i]-1][1]);
            }
        }
        for (int i = 0; i < atoms.size(); i++) {
            if (atoms[i].type == COLUMN) {
                string view = from_lists[cols[atoms[i].regex]];
                atoms[i].column = views[view].getColumn(atoms[i].regex);
            }
        }
        views[id].extract_pattern(atoms, groups_pattern, col_names, Text_tokens, text);
    }
}
void Parser::extract_spec(string id) {
    if (tokens[look].type == REGEX) {
        regex_spec(id);
    } else {
        pattern_spec(id);
    }
}
void Parser::regex_spec(string id) {
    match(REGEX);
    string regex = tokens[look].value;
    match(REGEX_);
    match(ON);
    column();
    name_spec();
    views[id].extract_regex(regex, groups, col_names, text);
}
void Parser::column() {
    col.clear();
    col.push_back(tokens[look].value);
    match(ID);
    match(DOT);
    col.push_back(tokens[look].value);
    match(ID);
}
void Parser::name_spec() {
    groups.clear();
    col_names.clear();
    if (tokens[look].type == AS) {
        match(AS);
        col_names.push_back(tokens[look].value);
        match(ID);
        groups.push_back(0);
    } else {
        match(RETURN);
        group_spec();
    }
    //TODO
}
void Parser::group_spec() {
    single_group();
    while (tokens[look].type != FROM) {
        match(AND);
        single_group();
    }
}
void Parser::single_group() {
    match(GROUP);
    string num = tokens[look].value;
    match(NUM);
    match(AS);
    string id = tokens[look].value;;
    match(ID);
    groups.push_back(Number(num));
    col_names.push_back(id);
}
void Parser::pattern_spec(string id) {
    match(PATTERN);
    groups_pattern.clear();
    pattern_expr();
    name_spec();
    
}
void Parser::pattern_expr() {
    while (tokens[look].type != AS
           && tokens[look].type != RETURN && tokens[look].type != CLOSEPAREN) {
        pattern_pkg();
    }
}
void Parser::pattern_pkg() {
    if (tokens[look].type == OPENPAREN) {
        pattern_group();
    } else {
        atom();
        if (tokens[look].type == OPENBRACE) {
            match(OPENBRACE);
            atoms[atoms.size()-1].interval_from = Number(tokens[look].value);
            match(NUM);
            match(COMMA);
            atoms[atoms.size()-1].interval_to = Number(tokens[look].value);
            match(NUM);
            match(CLOSEBRACE);
        }
    }
}
void Parser::atom() {
    if (tokens[look].type == REGEX_) {
        atoms.push_back(Atom(REG, 0, 0, tokens[look].value, Column()));
        match(REGEX_);
    } else {
        match(LESSTHAN);
        if (tokens[look].type == TOKEN) {
            match(TOKEN);
        } else {
            column();
            cols[col[1]] = col[0];
            atoms.push_back(Atom(COLUMN, 0, 0, col[1], Column()));
        }
        match(GREATETHAN);
    }
}
void Parser::pattern_group() {
    match(OPENPAREN);
    vector<int> paren;
    int pos = (int)groups_paren.size();
    groups_paren.push_back(paren);
    paren.push_back((int)atoms.size());
    pattern_expr();
    paren.push_back((int)atoms.size());
    match(CLOSEPAREN);
    groups_paren[pos] = paren;
}
