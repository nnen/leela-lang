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
#include "Assembler.h"
#include "Machine.h"

int main(int argc, const char * argv[])
{
	int c;
	bool dumpTokens       = false;
	bool dumpBytecodeFlag = false;
	
	Ref<Output> output = NULL;

	while ((c = getopt(argc, const_cast<char * const *>(argv), "tdo:")) != -1) {
		switch (c) {
		case 't': dumpTokens = true;               break;
		case 'd': dumpBytecodeFlag = true;         break;
		case 'o': output = new FileOutput(optarg); break;
		default:
			abort();
		}
	}
	
	if (optind >= argc) {
		std::cerr << "No input file given." << endl;
		return 1;
	}
	
	Ref<FileInput> input(new FileInput(argv[optind++]));
	
	if (dumpTokens) {
		Lexer lexer(input);
		lexer.dumpTokens(output);
	} else if (input->getExtension() == "leela") {
		if (!parse(input, NULL)) return 1;
	} else if (input->getExtension() == "lasm") {
		if (!compile(input, NULL)) return 1;
	} else if (input->getExtension() == "leelac") {
		if (dumpBytecodeFlag)
			dumpBytecode(input, output);
		else
			run(input);
	} else {
		std::cerr << "Unknown input file extension." << endl;
		return 1;
	}
	
	return 0;
}

Ref<Output> normalizeOutput(Ref<Input> input,
                            Ref<Output> output,
                            string fileNameExtension)
{
	if (!output.isNull()) return output;
	
	Ref<FileInput> fileInput = input.as<FileInput>();
	
	if (fileInput.isNull())
		return new StdOutput();
	
	return new FileOutput(fileInput->getBasename() + "." + fileNameExtension);
}

bool parse(Ref<Input> input, Ref<Output> output)
{
	output = normalizeOutput(input, output, "lasm");
	
	Parser parser;
	return parser.parse(input, output);
}

bool compile(Ref<Input> input, Ref<Output> output)
{
	output = normalizeOutput(input, output, "leelac");
	
	Assembler assembler;
	return assembler.assemble(input, output);
}

void run(Ref<Input> input)
{
	Machine machine;
	machine.loadBytecode(input);
	machine.run();
}

void dumpBytecode(Ref<Input> input, Ref<Output> output)
{
	output = normalizeOutput(input, output, "codedump");

	Ref<Bytecode> code = new Bytecode();
	code->read(input);
	code->dump(output->stream());
}

