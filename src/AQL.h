#pragma once

#include <iostream>
#include <vector>
#include <string>
#include "Tokenizer.h"

struct Span {
    int from, to;
    std::string value;
    Span(const std::string & x, const int & f, const int & t) :
        value(x), from(f), to(t) { }
};

struct Column {
    std::string name;
    std::vector<Span> spans;
    explicit Column(const std::string & n = "") : name(n) { }
};

typedef enum {
    REG, COLUMN
} Type_Atom;

struct Atom {
    Column column;
    int interval_from;
    int interval_to;
    Type_Atom type;
    std::string regex;
    Atom(const Type_Atom & t, const int & from, const int & to,
         const std::string & r, const Column & c) :
        type(t), interval_from(from), interval_to(to), regex(r), column(c) { }
};

class View {
 private:
    std::string name;
    std::vector<Column> columns;
    void output_frame(const int &, const std::vector<int> &) const;
    unsigned query(const std::vector<Text_token> &, const unsigned &) const;
    void dfs(
        const std::vector<Atom>::const_iterator &,
        const std::vector<Atom>::const_iterator &,
        std::vector<Span> &,
        const std::vector<int> &,
        std::vector<Column> &,
        const std::vector<Text_token> &,
        const std::string &);

 public:
    Column getColumn(const std::string &);

    inline const std::vector<Column> & getColumns() const {
        return columns;
    }

    inline std::vector<Column> & getColumns() {
        return columns;
    }

    explicit View(const std::string & = "");
    void output(const std::string &) const;
    void extract_regex(
        const std::string &,
        const std::vector<int> &,
        const std::vector<std::string> &,
        const std::string &);
    void extract_pattern(
        const std::vector<Atom> &,
        const std::vector<int> &,
        const std::vector<std::string> &,
        const std::vector<Text_token> &,
        const std::string &);
    void select(
        const std::vector<Column> &,
        const std::vector<std::string> &);
};

