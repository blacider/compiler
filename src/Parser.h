#pragma once

#include <map>
#include <string>
#include <sstream>
#include <vector>
#include "Lexer.h"
#include "AQL.h"

inline int Number(const std::string & x) {
    std::stringstream ss;
    ss << x;
    int num_;
    ss >> num_;
    return num_;
}

class Parser {
 public:
    Parser(const std::string &, Tokenizer);
        // 第一个是 aql 文件 path，第二个是文本 text
    void error(const std::string &);
    void program();

 private:
    std::vector<Text_token> Text_tokens;
    std::map<std::string, View> views;
    std::vector<Token> tokens;
    std::map<std::string, std::string> from_lists;
    std::map<std::string, std::string> select_lists;
    std::map<std::string, std::string> aliass;
    std::vector<std::string> from_items;
    std::vector<std::string> select_items;
    std::vector<int> groups;
    std::vector<int> groups_pattern;
    std::vector< std::vector<int> > groups_paren;   // 捕获块
    std::vector<std::string> col_names;
    std::vector<Atom> atoms;
    std::vector<std::string> col;
    std::map<std::string, std::string> cols;
    int look, end;
    // parser
    inline void move() {
        ++look;
    }
    inline void match(const Type & type) {
        if (tokens[look].type == type)
            move();
        else
            error("syntax error");
    }
    // aql
    inline void aql_stmt() {
        if (tokens[look].type == CREATE)
            create_stmt();
        else
            output_stmt();
    }

    inline void create_stmt() {
        match(CREATE);
        match(VIEW);
        std::string id = tokens[look].value;
        views[id] = View(id);
        match(ID);
        match(AS);
        view_stmt(id);
        match(SEMICOLON);
    }

    inline void view_stmt(const std::string & id) {
        if (tokens[look].type == SELECT)
            select_stmt(id);
        else
            extract_stmt(id);
    }

    inline void output_stmt() {
        match(OUTPUT);
        match(VIEW);
        std::string id = tokens[look].value;
        match(ID);
        std::string tmp = alias();
        match(SEMICOLON);
        views[id].output(tmp);
    }

    void select_stmt(const std::string &);
    void extract_stmt(const std::string &);

    inline std::string alias() {
        if (tokens[look].type == AS) {
            match(AS);
            std::string id = tokens[look].value;
            match(ID);
            return id;
        }
        return "";
    }

    // Select Statement
    void select_list();
    void from_list();

    inline void from_item() {
        from_items.clear();
        from_items.push_back(tokens[look].value);
        match(ID);
        from_items.push_back(tokens[look].value);
        match(ID);
    }

    inline void select_item() {
        aliass.clear();
        select_items.push_back(tokens[look].value);
        match(ID);
        match(DOT);
        std::string id2 = tokens[look].value;
        select_items.push_back(tokens[look].value);
        match(ID);
        std::string tmp = alias();
        if (!tmp.empty())
            aliass[tmp] = id2;
        else
            aliass[id2] = id2;
    }

    // Extract Statement--regular
    void extract_spec(const std::string &);
    void regex_spec(const std::string &);

    inline void column() {
        col.clear();
        col.push_back(tokens[look].value);
        match(ID);
        match(DOT);
        col.push_back(tokens[look].value);
        match(ID);
    }

    inline void name_spec() {
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
    }

    void group_spec();

    inline void single_group() {
        match(GROUP);
        std::string num = tokens[look].value;
        match(NUM);
        match(AS);
        std::string id = tokens[look].value;;
        match(ID);
        groups.push_back(Number(num));
        col_names.push_back(id);
    }

    // Extract Statement--pattern
    inline void pattern_spec(const std::string & id) {
        match(PATTERN);
        groups_pattern.clear();
        pattern_expr();
        name_spec();
    }

    void pattern_expr();
    void pattern_pkg();
    void atom();
    void pattern_group();

    std::string text;
};

