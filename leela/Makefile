
CXX      = g++
CXXFLAGS = -Wall -g -pedantic -D __DEBUG__

build: Dependencies
	make leela

clean:
	\rm -f Dependencies
	\rm -f leela
	\rm -f *.o

rebuild:
	make clean
	make build

Dependencies: *.cpp
	g++ -MM *.cpp > Dependencies

.PHONY: build clean rebuild

leela: leela.o Char.o Token.o Parser.o Lexer.o Object.o Value.o grammar.o
	$(CXX) $(CXXFLAGS) -o leela $^

include Dependencies

