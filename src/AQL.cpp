#include <cstdio>
#include <string>
#include <vector>
#include <sstream>
#include "AQL.h"

extern std::vector< std::vector<int> >
findall(const char *regex, const char *content);

View::View(const std::string & name_) : name(name_) { }

void View::output(const std::string & x) const {
    std::ios::sync_with_stdio(false);
    if (x == "")
        std::cout << "View: " << name << std::endl;
    else
        std::cout << "View: " << x << std::endl;

    std::vector<int> longestEachCol;

    // pre-process
    int columnsSize = columns.size();
    int i;
    int row = 0;
    int span_length;
    bool finish = false;
    std::stringstream stream;
    std::string string_temp;
    for (i = 0; i < columnsSize; ++i)
        longestEachCol.push_back(columns[i].name.size());
    while (!finish) {
        finish = true;
        for (i = 0; i < columnsSize; ++i)
            if (row < columns[i].spans.size()) {
                finish = false;
                span_length =
                    columns[i].spans[row].to - columns[i].spans[row].from;
                stream.str("");
                string_temp.clear();
                stream << columns[i].spans[row].from
                       << columns[i].spans[row].to;
                string_temp = stream.str();
                span_length += string_temp.length();
                span_length += 6;
                if (span_length > longestEachCol[i])
                    longestEachCol[i] = span_length;
            }
        row++;
    }
    output_frame(columnsSize, longestEachCol);

    // output name
    int j;
    std::cout << "|";
    for (i = 0; i < columnsSize; ++i) {
        std::cout << " " << columns[i].name;
        for (j = longestEachCol[i] - columns[i].name.size() - 1; j > 0; --j)
            std::cout << " ";
        std::cout << "|";
    }
    std::cout << std::endl;

    output_frame(columnsSize, longestEachCol);

    finish = false;
    int rowSize((columnsSize > 0) ? columns[0].spans.size() : 0);
    std::stringstream stream_from, stream_to;
    std::string temp_span;
    for (row = 0; row < rowSize; ++row) {
        std::cout << "|";
        finish = true;
        for (i = 0; i < columnsSize; ++i) {
            if (row < columns[i].spans.size()) {
                stream_from.str("");
                stream_to.str("");
                string_temp.clear();
                stream_from << columns[i].spans[row].from;
                stream_to << columns[i].spans[row].to;
                temp_span = " " + columns[i].spans[row].value + ":(" +
                            stream_from.str() + "," + stream_to.str() +")";
                std::cout << temp_span;
                for (j = 0; j < longestEachCol[i] - temp_span.size(); ++j)
                    std::cout << " ";
            }
            std::cout << "|";
        }
        std::cout << std::endl;
    }
    output_frame(columnsSize, longestEachCol);
    std::cout << rowSize << " rows in set" << std::endl << std::endl;

    std::ios::sync_with_stdio(true);
}

void View::extract_regex(
    const std::string & regex,
    const std::vector<int> & groups,
    const std::vector<std::string> & column_names,
    const std::string & text) {
    std::vector< std::vector<int> > result =
        findall(regex.c_str(), text.c_str());
    columns.clear();
    int col, i, from, to;
    for (col = 0; col < groups.size(); col++) {
        columns.push_back(Column(column_names[col]));
        for (i = 0; i < result.size(); i++) {
            from = result[i][groups[col] << 1];
            to = result[i][(groups[col] << 1) + 1];
            columns[col].spans.push_back(
                Span(text.substr(from, to - from), from, to));
        }
    }
}

void View::select(const std::vector<Column> & columns_,
                  const std::vector<std::string> & column_names) {
    columns.clear();
    int i;
    for (i = 0; i < column_names.size(); i++) {
        columns.push_back(columns_[i]);
        columns[i].name = column_names[i];
    }
}

void View::output_frame(const int & columnsSize,
                        const std::vector<int> & longestEachCol) const {
    int i, j;

    // print kuangtu
    for (i = 0; i < columnsSize; ++i) {
        std::cout << '+';
        for (j = 0; j < longestEachCol[i]; ++j)
            std::cout << '-';
    }
    std::cout << '+' << std::endl;
}

void View::extract_pattern(
    const std::vector<Atom> & atoms,
    const std::vector<int> & groups,
    const std::vector<std::string> & column_names,
    const std::vector<Text_token> & tokens,
    const std::string & text) {

    std::vector<Atom> cols;
    for (std::vector<Atom>::const_iterator i = atoms.begin();
         i != atoms.end();
         ++i)
        if (i->type == COLUMN) {
            cols.push_back(*i);
        } else {
            View temp("temp");
            temp.extract_regex(
                i->regex,
                std::vector<int>(1, 0),
                std::vector<std::string>(1, "temp"),
                text);
            cols.push_back(
                Atom(COLUMN, i->interval_from, i->interval_to, "",
                    temp.columns.front()) );
        }

    std::vector<Column> newCols;
    for (std::vector<std::string>::const_iterator i = column_names.begin();
         i != column_names.end();
         ++i)
        newCols.push_back(Column(*i));
    std::vector<Span> s;
    dfs(cols.begin(), cols.end(), s, groups, newCols, tokens, text);

    for (std::vector<Column>::const_iterator i = newCols.begin();
         i != newCols.end();
         ++i)
        columns.push_back(*i);
}

void View::dfs(
    const std::vector<Atom>::const_iterator & now,
    const std::vector<Atom>::const_iterator & end,
    std::vector<Span> & deque,
    const std::vector<int> & groups,
    std::vector<Column> & newCols,
    const std::vector<Text_token> & tokens,
    const std::string & text) {

    if (now == end) {
        unsigned p, q, k, h;
        for (unsigned i = 0; i < newCols.size(); ++i) {
            newCols[i].spans.push_back(
                Span("", deque[groups[i << 1]].from,
                    deque[groups[(i << 1) + 1] - 1].to) );
            for (unsigned j = groups[i << 1]; j < groups[(i << 1) + 1]; ++j) {
                if (j > groups[i << 1]) {
                    q = query(tokens, deque[j-1].to - 1);
                    p = query(tokens, deque[j].from);
                    for (k = q + 1; k < p; ++k) {
                        for (h = tokens[k-1].to; h < tokens[k].from; ++h)
                            newCols[i].spans.back().value.push_back(text[h]);
                        newCols[i].spans.back().value += tokens[k].value;
                    }
                    for (h = tokens[p-1].to; h < tokens[p].from; ++h)
                        newCols[i].spans.back().value.push_back(text[h]);
                }
                newCols[i].spans.back().value += deque[j].value;
            }
        }
    } else {
        unsigned p, q;
        if (!deque.empty())
            q = query(tokens, deque.back().to - 1);
        for (std::vector<Span>::const_iterator i = now->column.spans.begin();
             i != now->column.spans.end();
             ++i) {
            p = query(tokens, i->from);
            if (deque.empty() || p - q > (now - 1)->interval_from
                && p - q - 1 <= (now - 1)->interval_to) {
                deque.push_back(*i);
                dfs(now + 1, end, deque, groups, newCols, tokens, text);
                deque.pop_back();
            }
        }
    }
}

unsigned View::query(
    const std::vector<Text_token> & tokens, const unsigned & x) const {
    unsigned l = 0, r = tokens.size() - 1, mid;
    while (true) {
        mid = (l + r) >> 1;
        if (tokens[mid].from > x)
            r = mid - 1;
        else if (tokens[mid].to <= x)
            l = mid + 1;
        else
            return mid;
    }
}

Column View::getColumn(const std::string & id) {
    for (int i = 0; i < columns.size(); i++) {
        if (columns[i].name == id) {
            return columns[i];
        }
    }
    return Column("");
}

