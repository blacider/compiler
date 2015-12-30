testAll: testAQL testTokenizer testLexer main
main:main.cpp Tokenizer.cpp Lexer.cpp Parser.cpp AQL.cpp regex.cpp
	g++ main.cpp Tokenizer.cpp Lexer.cpp Parser.cpp AQL.cpp regex.cpp -o main.o
testTokenizer:testTokenizer.cpp Tokenizer.cpp
	g++ testTokenizer.cpp Tokenizer.cpp -o testTokenizer.o
testLexer:testLexer.cpp Lexer.cpp Tokenizer.cpp
	g++ testLexer.cpp Lexer.cpp Tokenizer.cpp -o testLexer.o
testAQL:testAQL.cpp AQL.cpp Tokenizer.cpp regex.cpp
	g++ testAQL.cpp AQL.cpp Tokenizer.cpp regex.cpp -o testAQL.o
clean:
	rm -rf *.o *.out
