//
//  main.cpp
//  compiler
//
//  Created by 牛仁鹏 on 15/12/30.
//  Copyright © 2015年 牛仁鹏. All rights reserved.
//

#include <stdio.h>
#include "Tokenizer.h"
#include "Parser.h"
int main(int argc, const char * argv[]) {
	cout << endl << argc << endl;
	if (argc == 1) {
		Tokenizer tokenizer("../dataset/weather/weather.input", false);//输入文本
    	Parser parser("../dataset/weather.aql", tokenizer);//aql 文件，输入文本的 tokenizer
    	parser.program();
	} else if(argc == 2){
		if (argv[1][0] == '1') {
   			//测试用例1：Perloc
    		Tokenizer tokenizer("../dataset/perloc/PerLoc.input", false);
    		Parser parser("../dataset/PerLoc.aql", tokenizer);
    		parser.program();
		} else if (argv[1][0] == '2'){
			//测试用例2：revenue
    		Tokenizer tokenizer("../dataset/revenue/Revenue.input", false);//输入文本
    		Parser parser("../dataset/Revenue.aql", tokenizer);//aql 文件，输入文本的 tokenizer
    		parser.program();
		} else {
			cout << "wrong argument 2!" << endl;
		}
	} else {
		cout << "wrong argument muti!" << endl;
	}
    return 0;
}
