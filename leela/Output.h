/**
 * \file   Output.h
 * \author Jan Milík <milikjan@fit.cvut.cz>
 * \date   2011-04-28
 *
 * \brief  
 */

#ifndef OUTPUT_H_12DFD019DTGBFV128957DF35AZ65G
#define OUTPUT_H_12DFD019DTGBFV128957DF35AZ65G

#include <ostream>

#include "Object.h"

class Output : public Object {
public:
	Output() {}
	virtual ~Output() {}
	
	virtual ostream& stream() = 0;
};

#endif /* end of include guard: OUTPUT_H_12DFD019DTGBFV128957DF35AZ65G */

