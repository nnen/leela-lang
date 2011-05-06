
CXX      = g++
CXXFLAGS = -Wall -g -pedantic -D __DEBUG__

BASE_FILES = Object.o \
             Value.o \
             Exception.o \
             Input.o \
             Output.o

LEXER_FILES = Char.o \
              Token.o \
              Lexer.o

PARSER_FILES = Parser.o \
               Parser.rules.o \
               Context.o \
               StringTable.o \
               AsmWriter.o

build: Dependencies
	make leela
	make leela-asm

clean:
	\rm -f Dependencies
	\rm -f leela
	\rm -f leela-asm
	\rm -f leela-test
	\rm -f tags
	\rm -f *.o *.a

rebuild:
	make clean
	make build

test: leela-test
	clear; ./leela-test | tee test.txt

Dependencies: *.cpp
	g++ -MM *.cpp > Dependencies

.PHONY: build clean rebuild test

tags: *.cpp *.h
	ctags --c++-kinds=-vm *.cpp *.h

Parser.rules.cpp: leela_grammar.txt Parser.rules.tpl.cpp
	cat leela_grammar.txt | python grammar.py

Parser.rules.h: leela_grammar.txt Parser.rules.tpl.h
	cat leela_grammar.txt | python grammar.py

base.a: base.a(Object.o Value.o Exception.o Input.o Output.o)
	ranlib $@

lexer.a: lexer.a(Char.o Token.o Lexer.o)
	ranlib $@

parser.a: parser.a(Parser.o Parser.rules.o Context.o StringTable.o AsmWriter.o)
	ranlib $@

leela: leela.o ${BASE_FILES} ${LEXER_FILES} ${PARSER_FILES} AsmScanner.o Function.o Instruction.o Bytecode.o Machine.o
	$(CXX) $(CXXFLAGS) -o leela $^

leela-asm: asm.o Object.o Input.o Instruction.o AsmScanner.o Assembler.o Bytecode.o
	$(CXX) $(CXXFLAGS) -o leela-asm $^

leela-test: leela-test.o Object.o Token.o Value.o Char.o Grammar.o GSymbol.o Conflict.o
	$(CXX) $(CXXFLAGS) -o leela-test $^

include Dependencies

