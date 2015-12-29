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
Parser::Parser(string path) {
    Lexer lexer(path);
    tokens = lexer.scan();
    look = 0;
    end = (int)tokens.size()+1;
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
    match(ID);
    match(AS);
    view_stmt();
    match(SEMICOLON);
    // TODO create
}
void Parser::view_stmt() {
    if (tokens[look].type == SELECT) {
        select_stmt();
    } else {
        extract_stmt();
    }
}
void Parser::output_stmt() {
    match(OUTPUT);
    match(VIEW);
    string id = tokens[look].value;
    match(ID);
    alias();
    match(SEMICOLON);
    //TODO OUTPUT
}
void Parser::alias() {
    if (tokens[look].type == AS) {
        match(AS);
        string id = tokens[look].value;
        match(ID);
        // TODO ID
    }
}
void Parser::select_stmt() {
    match(SELECT);
    select_list();
    match(FROM);
    from_list();
    //TODO SELECT
}
void Parser::select_list() {
    while (tokens[look].type != FROM) {
        select_item();
    }
    //TODO RETURN LIST
}
void Parser::select_item() {
    match(ID);
    match(DOT);
    match(ID);
    alias();
    //TODO ID.ID AS
}
void Parser::from_list() {
    from_item();
    while (tokens[look].type != SEMICOLON) {
        match(COMMA);
        from_item();
    }
    //todo LIST
}
void Parser::from_item() {
    match(ID);
    match(ID);
    //TODO ID
}
void Parser::extract_stmt() {
    match(EXTRACT);
    extract_spec();
    match(FROM);
    from_list();
    //TODO
}
void Parser::extract_spec() {
    if (tokens[look].type == REGEX) {
        regex_spec();
    } else {
        pattern_spec();
    }
    //TODO
}
void Parser::regex_spec() {
    match(REGEX);
    match(REGEX_);
    match(ON);
    column();
    name_spec();
    //TODO
}
void Parser::column() {
    match(ID);
    match(DOT);
    match(ID);
    //TODO
}
void Parser::name_spec() {
    if (tokens[look].type == AS) {
        match(AS);
        match(ID);
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
    //TODO
}
void Parser::single_group() {
    match(GROUP);
    match(NUM);
    match(AS);
    match(ID);
    //TODO
}
void Parser::pattern_spec() {
    match(PATTERN);
    pattern_expr();
    name_spec();
    //TODO
}
void Parser::pattern_expr() {
    while (tokens[look].type != AS
           && tokens[look].type != RETURN && tokens[look].type != CLOSEPAREN) {
        pattern_pkg();
    }
    //TODO
}
void Parser::pattern_pkg() {
    if (tokens[look].type == OPENPAREN) {
        pattern_group();
    } else {
        atom();
        if (tokens[look].type == OPENBRACE) {
            match(OPENBRACE);
            match(NUM);
            match(COMMA);
            match(NUM);
            match(CLOSEBRACE);
        }
    }
    //TODO
}
void Parser::atom() {
    if (tokens[look].type == REGEX_) {
        match(REGEX_);
    } else {
        match(LESSTHAN);
        if (tokens[look].type == TOKEN) {
            match(TOKEN);
        } else {
            column();
        }
        match(GREATETHAN);
    }
    //TODO
}
void Parser::pattern_group() {
    match(OPENPAREN);
    pattern_expr();
    match(CLOSEPAREN);
}
