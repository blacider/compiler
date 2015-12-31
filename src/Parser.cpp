#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <string>
#include <sstream>
#include "Parser.h"
#include "Lexer.h"

Parser::Parser(const std::string & path, Tokenizer tokenizer) :
    look(0),
    tokens(Lexer(path).scan()),
    end(tokens.size() + 1),
    Text_tokens(tokenizer.scan()),
    text(tokenizer.getText()) { }

void Parser::error(const std::string & x) {
    std::cerr << "near position " << tokens[look].from
              << ":" << x << std::endl;
    exit(1);
}

void Parser::program() {
    while (look != end - 1) {
        aql_stmt();
    }
}

void Parser::select_stmt(const std::string & id) {
    match(SELECT);
    select_list();
    match(FROM);
    from_list();
    std::vector<Column> cols;
    std::vector<std::string> names;
    std::string view;
    std::map<std::string, std::string>::const_iterator it;
    for (it = aliass.begin(); it != aliass.end(); it++) {
        names.push_back(it->first);
        view = from_lists[select_lists[it->second]];
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

void Parser::extract_stmt(const std::string & id) {
    groups_paren.clear();
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
        int i;
        for (i = 0; i < groups.size(); i++) {
            if (groups[i] == 0) {
                groups_pattern.push_back(0);
                groups_pattern.push_back(atoms.size());
            } else {
                groups_pattern.push_back(groups_paren[groups[i]-1][0]);
                groups_pattern.push_back(groups_paren[groups[i]-1][1]);
            }
        }
        std::string view;
        for (i = 0; i < atoms.size(); i++) {
            if (atoms[i].type == COLUMN) {
                view = from_lists[cols[atoms[i].regex]];
                atoms[i].column = views[view].getColumn(atoms[i].regex);
            }
        }
        views[id].extract_pattern(
            atoms, groups_pattern, col_names, Text_tokens, text);
    }
}

void Parser::group_spec() {
    single_group();
    while (tokens[look].type != FROM) {
        match(AND);
        single_group();
    }
}

void Parser::pattern_expr() {
    while (tokens[look].type != AS
           && tokens[look].type != RETURN
           && tokens[look].type != CLOSEPAREN)
        pattern_pkg();
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
    std::vector<int> paren;
    int pos = groups_paren.size();
    groups_paren.push_back(paren);
    paren.push_back(atoms.size());
    pattern_expr();
    paren.push_back(atoms.size());
    match(CLOSEPAREN);
    groups_paren[pos] = paren;
}

void Parser::extract_spec(const std::string & id) {
    if (tokens[look].type == REGEX)
        regex_spec(id);
    else
        pattern_spec(id);
}

void Parser::regex_spec(const std::string & id) {
    match(REGEX);
    std::string regex = tokens[look].value;
    match(REGEX_);
    match(ON);
    column();
    name_spec();
    views[id].extract_regex(regex, groups, col_names, text);
}

