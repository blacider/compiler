//
//  AQL.cpp
//  compiler
//
//  Created by 牛仁鹏 on 15/12/21.
//  Copyright © 2015年 牛仁鹏. All rights reserved.
//

#include <stdio.h>
#include "AQL.h"
#include <sstream>
extern vector<vector<int> > findall(const char *regex, const char *content);
View::View(string name_) {
    name = name_;
}
View::View() {
    name = "";
}
void View::output(string x) {
	if (x == "") {
		std::cout << "View: " << name << endl;
	}
	else {
		std::cout << "View: " << x << endl;
	}
	vector<int> longestEachCol;
	//pre-process
	int columnsSize = (int)columns.size();
	int i = 0;
	int row = 0;
	bool finish = false;
	while (i < columnsSize) {
		longestEachCol.push_back(columns[i].name.size());
		i++;
	}
	while (!finish) {
		i = 0;
		finish = true;
		while (i < columnsSize) {
			if (row < columns[i].spans.size()) {
				finish = false;
				int span_length = columns[i].spans[row].to - columns[i].spans[row].from;
				stringstream stream;
				string string_temp;
				stream << columns[i].spans[row].from;
				stream << columns[i].spans[row].to;
				string_temp = stream.str();
				span_length += string_temp.length();
				span_length += 6;
				if (span_length > longestEachCol[i]) {
					longestEachCol[i] = span_length;
				}
			}
			i++;
		}
		row++;
	}
	output_frame(columnsSize, longestEachCol);

	//output name
	i = 0;
	std::cout << "|";
	while (i < columnsSize) {
		std::cout << columns[i].name;
		int j = 0;
		while (j < longestEachCol[i] - columns[i].name.size()) {
			std::cout << " ";
			j++;
		}
		std::cout << "|";
		i++;
	}
	std::cout << endl;
	
	output_frame(columnsSize, longestEachCol);

	i = 0;
	finish = false;
	row = 0;
	int rowSize;
	if (columnsSize > 0) {
		rowSize = (int)columns[0].spans.size();
	}
	while (row < rowSize) {
		i = 0;
		std::cout << "|";
		finish = true;
		while (i < columnsSize) {
			if (row < columns[i].spans.size()) {
				stringstream stream_from;
				stringstream stream_to;
				string string_temp;
				stream_from << columns[i].spans[row].from;
				stream_to << columns[i].spans[row].to;
				string temp_span = " " + columns[i].spans[row].value + ":(" + stream_from.str() + "," + stream_to.str() +")";
				std::cout << temp_span;
				int j = 0;
				while (j < longestEachCol[i] - temp_span.size()) {
					std::cout << " ";
					j++;
				}
			}
			std::cout << "|";
			i++;
		}
		row++;
		std::cout << endl;
	}
	output_frame(columnsSize, longestEachCol);
	std::cout << rowSize << " rows in set" << endl << endl;
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



void View::select(vector<Column> columns_, vector<string> column_names) {
    columns.clear();
    for (int i = 0; i < column_names.size(); i++) {
        columns.push_back(columns_[i]);
        columns[i].name = column_names[i];
    }
}
void View::output_frame(int columnsSize, vector<int> longestEachCol) {
	int i = 0;

	//print kuangtu
	while (i < columnsSize) {
		cout << "+";
		int j = 0;
		while (j < longestEachCol[i]) {
			cout << "-";
			j++;
		}
		i++;
	}
	cout << "+" << endl;
}
#include <vector>
#include <string>
#include "AQL.h"

void View::extract_pattern(
                           const std::vector<Atom> & atoms,
                           const std::vector<int> & groups,
                           const std::vector<std::string> & column_names,
                           const std::vector<Text_token> & tokens,
                           const std::string & text
                           ) {
    std::vector<Atom> cols;
    for (
         std::vector<Atom>::const_iterator i = atoms.begin();
         i != atoms.end();
         ++i
         )
        if (i->type == COLUMN) {
            cols.push_back(*i);
        } else {
            View temp("temp");
            temp.extract_regex(
                               i->regex,
                               std::vector<int>(1, 0),
                               std::vector<std::string>(1, "temp"),
                               text
                               );
            cols.push_back(
                           Atom(
                                COLUMN,
                                i->interval_from,
                                i->interval_to,
                                "",
                                temp.columns.front()
                                )
                           );
        }
    
    std::vector<Column> newCols;
    for (
         std::vector<string>::const_iterator i = column_names.begin();
         i != column_names.end();
         ++i
         )
        newCols.push_back(Column(*i));
    std::vector<Span> s;
    dfs(cols.begin(), cols.end(), s, groups, newCols, tokens);
    
    for (
         std::vector<Column>::const_iterator i = newCols.begin();
         i != newCols.end();
         ++i
         )
        columns.push_back(*i);
}

void View::dfs(
               const std::vector<Atom>::const_iterator & now,
               const std::vector<Atom>::const_iterator & end,
               std::vector<Span> & deque,
               const std::vector<int> & groups,
               std::vector<Column> & newCols,
               const std::vector<Text_token> & tokens
               ) {
    if (now == end) {
        for (unsigned i = 0; i < newCols.size(); ++i) {
            newCols[i].spans.push_back(
                                       Span(
                                            "",
                                            deque[groups[i << 1]].from,
                                            deque[groups[(i << 1) + 1] - 1].to
                                            )
                                       );
            for (unsigned j = groups[i << 1]; j < groups[(i << 1) + 1]; ++j)
                newCols[i].spans.back().value += deque[j].value;
        }
    } else {
        unsigned p, q;
        if (!deque.empty())
            q = query(tokens, deque.back().to - 1);
        for (
             std::vector<Span>::const_iterator i = now->column.spans.begin();
             i != now->column.spans.end();
             ++i
             ) {
            p = query(tokens, i->from);
            if (
                deque.empty()
                || p - q > (now - 1)->interval_from
                && p - q - 1 <= (now - 1)->interval_to
                ) {
                deque.push_back(*i);
                dfs(now + 1, end, deque, groups, newCols, tokens);
                deque.pop_back();
            }
        }
    }
}

unsigned View::query(
                     const std::vector<Text_token> & tokens,
                     const unsigned & x
                     ) {
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
Column View::getColumn(string id) {
    for (int i = 0; i < columns.size(); i++) {
        if (columns[i].name == id) {
            return columns[i];
        }
    }
    return Column("");
}
