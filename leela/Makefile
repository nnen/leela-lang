
CXX      = g++
CXXFLAGS = -Wall -g -pedantic -D __DEBUG__

build: Dependencies
	make leela
	make leela-asm

clean:
	\rm -f Dependencies
	\rm -f leela
	\rm -f leela-asm
	\rm -f tags
	\rm -f *.o

rebuild:
	make clean
	make build

test: leela-test
	clear; ./leela-test

Dependencies: *.cpp
	g++ -MM *.cpp > Dependencies

.PHONY: build clean rebuild test

tags: *.cpp *.h
	ctags --c++-kinds=-vm *.cpp *.h

leela: leela.o Input.o Char.o Token.o Parser.o Lexer.o Object.o Value.o Function.o grammar.o Instruction.o Bytecode.o Machine.o
	$(CXX) $(CXXFLAGS) -o leela $^

leela-asm: asm.o Object.o Input.o Instruction.o AsmScanner.o Assembler.o Bytecode.o
	$(CXX) $(CXXFLAGS) -o leela-asm $^

leela-test: leela-test.o Object.o Token.o Value.o Char.o Grammar.o GSymbol.o Conflict.o
	$(CXX) $(CXXFLAGS) -o leela-test $^

include Dependencies

