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
#include "Parser.h"
#include "Machine.h"

int main(int argc, const char * argv[])
{
	int c;

	Ref<Output> output = new StdOutput();

	while ((c = getopt(argc, const_cast<char * const *>(argv), "o:")) != -1) {
		switch (c) {
		case 'o':
			output = new FileOutput(optarg);
			break;
		default:
			abort();
		}
	}
	
	if (optind >= argc) {
		std::cerr << "No input file given." << endl;
		return 1;
	}
	
	Ref<FileInput> file(new FileInput(argv[optind++]));
		
	if (file->getExtension() == "leela") {
		Parser parser;
		parser.parse(file, output);
	} else if (file->getExtension() == "cleela") {
		Machine machine;
		machine.loadBytecode(file);
		cout << "Running " << argv[optind - 1] << "..." << endl;
		machine.run();
	} else {
		std::cerr << "Unknown input file extension." << endl;
		return 1;
	}
	
	return 0;
}

