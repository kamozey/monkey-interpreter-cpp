output: main.o ast.o lexer.o parser.o precedence.o token.o evaluator.o object.o environment.o move 
	g++ -I ./*.cpp -I ./build/*.o -o output
	rm -rf ./build
	./output

main.o: main.cpp
	g++ -c main.cpp

ast.o : ./ast/*.*
	g++ -c ./ast/*.h

lexer.o: ./lexer/*.*
	g++ -c ./lexer/*.cpp ./lexer/*.h

parser.o: ./parser/*.*
	g++ -c ./parser/*.cpp ./parser/*.h

precedence.o : ./precedence/*.*
	g++ -c ./precedence/*.h

token.o: ./token/*.*
	g++ -c ./token/*.cpp ./token/*.h

evaluator.o: ./evaluator/*.cpp ./evaluator/*.h
	g++ -c ./evaluator/*.cpp ./evaluator/*.h

object.o: ./object/object.h
	g++ -c ./object/object.h

environment.o: ./object/environment.h
	g++ -c ./object/environment.h

move:
	mkdir -p ./build
	mv *.o ./build/

.PHONY: clean

clean:
	find . -name "*.o" -type f -delete
	find . -name "*.exe" -type f -delete
	find . -name "output" -delete
	find . -name "*.h.gch" -type f -delete
	rm -rf ./build