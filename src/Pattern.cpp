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
                    COLUMN, i->interval_from, i->interval_to, "", temp.columns
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
    dfs(cols.beign(), cols.end(), s, groups, newCols);
}

void View::dfs(
        const std::vector<Atom>::const_iterator & now,
        const std::vector<Atom>::const_iterator & end,
        std::vector<Span> & deque,
        const std::vector<int> & groups,
        std::vector<Column> & newCols
) {
    if (now == end)
        for (unsigned i = 0; i < newCols.size(); ++i) {
            newCols[i].spans.push_back(
                Span(
                    "",
                    deque[groups[i << 1]].from,
                    deque[groups[(i << 1) + 1] - 1].to
                )
            );
            for (unsigned j = groups[i << 1]; j < groups[(i << 1) + 1]; ++j)
                newCols[i].spans.back.value += deque[j].value;
        }
    else
        for (
            std::vector<Span>::const_iterator i = now->column.spans.begin();
            i != now->column.spans.end();
            ++i
        )
            if (
                deque.empty()
                 || i->from - deque.back().end > (now - 1)->interval_from
                 && i->from - deque.back().end - 1 <= (now - 1)->interval_to
            ) {
                deque.push_back(*i);
                dfs(next, end, deque, groups, newCols);
                deque.pop_back();
            }
}

