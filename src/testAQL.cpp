#include <stdio.h>
#include "AQL.h"
#include <iostream>
#include <string>
#include<vector>

int main() {
    //tokennizer
    Tokenizer tokenizer("../dataset/perloc/PerLoc.input", false);
    std::vector<Text_token> text_tokens = tokenizer.scan();
    //测试 extract_regex
    /*
     create view cap as
     extract regex /[A-Z][a-z]* /
     on D.text
     return group 0 as Cap
     from Document D;
     */
    View cap("cap");
    std::vector<int> groups;
    groups.push_back(0);
    std::vector<std::string> column_names;
    column_names.push_back("Cap");
    std::string text = tokenizer.getText();
    cap.extract_regex("[A-Z][a-z]*", groups, column_names, text);
    cap.output("");
    //测试 extract_regex
    /*
     create view loc as
     extract regex /Washington|Georgia|Virginia/
     on D.text
     return group 0 as loc
     from Document D;
     */
    View loc("loc");
    groups.clear();
    groups.push_back(0);
    column_names.clear();
    column_names.push_back("loc");
    loc.extract_regex("Washington|Georgia|Virginia",
                      groups,
                      column_names,
                      text);
    loc.output("");
    //测试 select
    /*
     create view perloc as
      select c.Cap as col1
      from cap c;
     */
    View perloc("perloc");
    std::vector<std::string> column_names2;
    column_names2.push_back("col1");
    std::vector<Column> columns2;
    columns2.push_back(cap.getColumns()[0]);
    perloc.select(columns2, column_names2);
	perloc.output("");
    //测试 extract_pattern
    /*
     create view pl as
     extract pattern (<c.cap>) /,/ (<l.loc>)
     return group 0 as Loc
     and group 1 as cap
     and group 2 as stt
     from cat c, loc l;
     */
    View pl("pl");
    std::vector<Atom> atoms;
    atoms.push_back(Atom(
                         COLUMN,
                         0,
                         0,
                         "",
                         cap.getColumns()[0]
                    ));
    atoms.push_back(Atom(
                         REG,
                         0,
                         0,
                         ",",
                         Column()
                         ));
    atoms.push_back(Atom(
                         COLUMN,
                         0,
                         0,
                         "",
                         loc.getColumns()[0]
                        ));
    std::vector<int> groups_;
    std::vector<std::string> names(1, "123");
    groups_.push_back(0);
    groups_.push_back(atoms.size());
    
    pl.extract_pattern(atoms,
                            groups_,
                            names,
                            text_tokens,
                            text
                            );
    pl.output("");
}

