/**
 * \file   registers.h
 * \author Jan Milík <milikjan@fit.cvut.cz>
 * \date   2011-05-12
 *
 * \brief  Contains virtual machine register definitions.
 */

#ifndef R
	#define R( name, attr, type, desc )
#endif

R( CA, address, Address, closure address )
R( FA, address, Address, function address )

#undef R

