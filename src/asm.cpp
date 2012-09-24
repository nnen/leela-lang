/**
 * \file   asm.cpp
 * \author Jan Milík <milikjan@fit.cvut.cz>
 * \date   2011-04-16
 *
 * \brief  
 */

#include <iostream>
#include <fstream>
#include <unistd.h>

#include "Assembler.h"
#include "Input.h"

int main(int argc, const char *argv[])
{
	bool dumpFlag = false;
	
	int c;
	while ((c = getopt(argc, const_cast<char * const *>(argv), "d")) != -1) {
		switch (c) {
		case 'd':
			dumpFlag = true;
			break;
		default: break;
		}
	}
	
	vector<Ref<Input> > inputFiles;
	
	if (optind < argc) {
		while (optind < argc)
			inputFiles.push_back(new FileInput(argv[optind++]));
	} else {
		inputFiles.push_back(new StreamInput());
	}
	
	if (dumpFlag) {
		foreach(file, inputFiles) {
			Bytecode bytecode;
			bytecode.read((*file)->stream());
			bytecode.dump(std::cout);
		}
		return 0;
	}
	
	Assembler assembler;
	
	foreach(file, inputFiles) {
		Ref<Bytecode> bytecode = assembler.assemble((*file)->stream());
		bytecode->write(std::cout);
	}
	
	return 0;
}

