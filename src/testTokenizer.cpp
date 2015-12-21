//
//  testTokenizer.cpp
//  compiler
//
//  Created by 牛仁鹏 on 15/12/21.
//  Copyright © 2015年 牛仁鹏. All rights reserved.
//

#include <stdio.h>
#include "Tokenizer.h"
#include <iostream>
int main(int argc, const char * argv[]) {
    Tokenizer tokenizer("PerLoc.aql");
    vector<Text_token> x = tokenizer.scan();
    for (int i = 0; i < x.size(); ++i)
    {
    	/* code */
    	cout << x[i].value
    		 << x[i].from
    		 << x[i].to
    		 << endl;
    }
    return 0;
}
