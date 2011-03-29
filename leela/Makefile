
CXX      = g++
CXXFLAGS = -Wall -pedantic -D __DEBUG__

build:
	make leela

clean:
	\rm -f leela
	\rm -f *.o

rebuild:
	make clean
	make build

leela: leela.o Char.o Token.o Lexer.o Object.o Value.o
	$(CXX) $(CXXFLAGS) -o leela $^

#Char.o: Char.h Char.cpp
#	$(CXX) $(CXXFLAGS) -c -o Char.o Char.cpp
#
#Token.o: Token.h Token.cpp
#	g++ -Wall -pedantic -c -o Token.o Token.cpp
#
#Lexer.o: Lexer.h Lexer.cpp
#	g++ -Wall -pedantic -c -o Lexer.o Lexer.cpp
#
#leela: leela.cpp leela.h Char.o Token.o Lexer.o
#	g++ -Wall -pedantic -o leela leela.cpp Char.o Token.o Lexer.o

