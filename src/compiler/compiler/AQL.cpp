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
void View::output(string x) {
	if (x == "") {
		std::cout << "View: " << name << endl;
	}
	else {
		std::cout << "View: " << x << endl;
	}
	vector<int> longestEachCol;
	//pre-process
	int columnsSize = columns.size();
	int i = 0;
	int row = 0;
	bool finish = false;
	while (i < columnsSize) {
		longestEachCol.push_back(0);
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
		rowSize = columns[0].spans.size();
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
	std::cout << rowSize << " rows in set" << endl;
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