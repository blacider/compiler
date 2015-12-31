#include <cstdio>
#include "Tokenizer.h"
#include "Parser.h"
int main(int argc, const char * argv[]) {
    std::cout << std::endl << argc << std::endl;
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
                } else if (argv[1][0] == '3') {
                Tokenizer tokenizer("../dataset/weather/weather2.input", false);//输入文本
                Parser parser("../dataset/weather.aql", tokenizer);//aql 文件，输入文本的 tokenizer
                parser.program();
		} else {
            std::cout << "wrong argument 2!" << std::endl;
		}
	} else {
        std::cout << "wrong argument muti!" << std::endl;
	}
    return 0;
}

