
build: Char.o Token.o Lexer.o
	g++ -Wall -pedantic -c -o leela.o Char.o Token.o

clean:
	\rm -f *.o

rebuild:
	make clean
	make build

Char.o: Char.h Char.cpp
	g++ -Wall -pedantic -c -o Char.o Char.cpp

Token.o: Token.h Token.cpp
	g++ -Wall -pedantic -c -o Token.o Token.cpp

Lexer.o: Lexer.h Lexer.cpp
	g++ -Wall -pedantic -c -o Lexer.o Lexer.cpp

