/**
 * \file   leela/leela.cpp
 * \author Jan Milík <milikjan@fit.cvut.cz>
 * \date   2011-03-27
 *
 * \brief  Main file. Contains the main() function.
 */

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <unistd.h>

#include "leela.h"
#include "Input.h"
#include "Lexer.h"
#include "Machine.h"
#include "grammar.h"

int main(int argc, const char * argv[])
{
	/*
	Lexer lexer(&std::cin);
	Token token;
	
	while ((token = lexer.getToken()).type != Token::END) {
		std::cout << token << std::endl;
	}
	std::cout << token << std::endl;
	*/

	bool dumpGrammarFlag = false;
	bool tokenizeFlag = false;

	initGrammar();
	// dumpGrammar(std::cout);

	int c;

	while ((c = getopt(argc, const_cast<char * const *>(argv), "gt")) != -1) {
		switch (c) {
		case 'g':
			dumpGrammarFlag = true;
			break;
		case 't':
			tokenizeFlag = true;
			break;
		default:
			abort();
		}
	}

	if (dumpGrammarFlag) {
		dumpGrammar(std::cout);
		return 0;
	}

	if (optind >= argc) {
		std::cerr << "No input file given.";
		return 1;
	}
	
	if (tokenizeFlag) {
		std::ifstream f(argv[optind], std::ifstream::in);
		Lexer lexer(&f);
		lexer.dumpTokens(std::cout);
		f.close();
	} else {
		while (optind < argc) {
			Ref<FileInput> file(new FileInput(argv[optind++]));
			
			if (file->getExtension() == "cleela") {
				Machine machine;
				machine.loadBytecode(file);
				cout << "Running " << argv[optind - 1] << "..." << endl;
				machine.run();
			} else {
				Parser parser;
				parser.parse(file->stream());
			}
		}
	}
	
	/*
	std::cout << std::endl << std::endl << "OUTPUT:" << std::endl;


	if (argc > 1) {
		if (strcmp(argv[1], "--tokens") == 0) {
		} else {
			Parser parser;
			std::ifstream f(argv[1], std::ifstream::in);
			parser.parse(f);
			f.close();
		}
	}*/ /* else
		parser.parse(); */
	
	return 0;
}

