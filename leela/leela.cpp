/**
 * \file   leela/leela.cpp
 * \author Jan Milík <milikjan@fit.cvut.cz>
 * \date   2011-03-27
 *
 * \brief  Main file. Contains the main() function.
 */

#include <iostream>

#include "leela.h"
#include "Lexer.h"

int main()
{
	Lexer lexer(&std::cin);
	Token token;
	
	while ((token = lexer.getToken()).type != Token::END) {
		std::cout << token << std::endl;
	}
	std::cout << token << std::endl;
	
	return 0;
}

