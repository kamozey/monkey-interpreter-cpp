output: main.o ast.o lexer.o parser.o precedence.o token.o
	g++ main.o lexer.o parser.o token.o -o output

main.o: main.cpp
	g++ -c main.cpp

ast.o : ./ast/ast.h
	g++ -c ./ast/ast.h

lexer.o: ./lexer/lexer.cpp ./lexer/lexer.h
	g++ -c ./lexer/lexer.cpp ./lexer/lexer.h

parser.o: ./parser/parser.cpp ./parser/parser.h
	g++ -c ./parser/parser.cpp ./parser/parser.h

precedence.o : ./precedence/precedence.h
	g++ -c ./precedence/precedence.h

token.o: ./token/token.cpp ./token/token.h
	g++ -c ./token/token.cpp ./token/token.h

clean:
	find . -name \*.o -type f -delete
	find . -name output.exe -type f -delete
	find . -name \*.h.gch -type f -delete