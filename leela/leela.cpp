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

Ref<FileInput> input;
Ref<Output>    output;

int main(int argc, const char * argv[])
{
	int c;
	bool dumpTokens       = false;
	bool dumpBytecodeFlag = false;
	bool parseOnly        = false;
	bool compileOnly      = false;
	
	output = NULL;
	
	while ((c = getopt(argc, const_cast<char * const *>(argv), "tdo:pc")) != -1) {
		switch (c) {
		case 't': dumpTokens = true;               break;
		case 'd': dumpBytecodeFlag = true;         break;
		case 'o': output = new FileOutput(optarg); break;
		case 'p': parseOnly = true;                break;
		case 'c': compileOnly = true;              break;
		default:
			abort();
		}
	}
	
	if (optind >= argc) {
		std::cerr << "No input file given." << endl;
		return 1;
	}
	
	input = new FileInput(argv[optind++]);
	
	if (dumpTokens) {
		Lexer lexer(input);
		lexer.dumpTokens(output);
	} else if (input->getExtension() == "leela") {
		if (!parse()) return 1;
		if (parseOnly) return 0;
		inputFromOutput();
		if (!compile()) return 1;
		if (compileOnly) return 0;
		inputFromOutput();
		run();
	} else if (input->getExtension() == "lasm") {
		if (!compile()) return 1;
		if (compileOnly) return 0;
		inputFromOutput();
		run();
	} else if (input->getExtension() == "leelac") {
		if (dumpBytecodeFlag)
			dumpBytecode();
		else
			run();
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

void normalizeOutput(string extension)
{
	output = normalizeOutput(input, output, extension);
}

void inputFromOutput()
{
	Ref<FileOutput> foutput = output.as<FileOutput>();
	if (foutput.isNull()) {
		input = NULL;
		output = NULL;
		return;
	}
	string fileName = foutput->getFileName();
	foutput = NULL;
	output  = NULL;
	input = new FileInput(fileName);
}

bool parse()
{
	output = normalizeOutput(input, output, "lasm");
	
	Parser parser;
	return parser.parse(input, output);
}

bool compile()
{
	output = normalizeOutput(input, output, "leelac");
	
	Assembler assembler;
	return assembler.assemble(input, output);
}

void run()
{
	Machine machine;
	machine.loadBytecode(input);
	machine.run();
}

void dumpBytecode()
{
	if (output.isNull()) output = new StdOutput();
	
	Ref<Bytecode> code = new Bytecode();
	code->read(input);
	code->dump(output->stream());
}

