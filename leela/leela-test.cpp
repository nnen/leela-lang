/**
 * \file   leela-test.cpp
 * \author Jan Milík <milikjan@fit.cvut.cz>
 * \date   2011-04-23
 *
 * \brief  
 */

#include <iostream>

#include "leela.h"

#include "Grammar.h"
#include "GSymbol.h"
#include "Conflict.h"

using namespace std;

int main(int argc, const char *argv[])
{
	Grammar g;
	g.dump(cout);
	return 0;
}

