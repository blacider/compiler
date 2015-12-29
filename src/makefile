testAll: testAQL testTokenizer testLexer testParser
testTokenizer:testTokenizer.cpp Tokenizer.cpp
	g++ -g testTokenizer.cpp Tokenizer.cpp -o testTokenizer.o
testLexer:testLexer.cpp Lexer.cpp Tokenizer.cpp
	g++ -g testLexer.cpp Lexer.cpp Tokenizer.cpp -o testLexer.o
testParser:testParser.cpp Parser.cpp Lexer.cpp Tokenizer.cpp
	g++ -g testParser.cpp Parser.cpp Lexer.cpp Tokenizer.cpp -o testParser.o
testAQL:testAQL.cpp AQL.cpp Tokenizer.cpp regex.cpp Pattern.cpp
	g++ -g testAQL.cpp AQL.cpp Tokenizer.cpp regex.cpp Pattern.cpp -o testAQL.o
clean:
	rm -rf *.o *.out
