/**
 * \file   leela/Object.cpp
 * \author Jan Milík <milikjan@fit.cvut.cz>
 * \date   2011-03-27
 *
 * \brief  Implementation of the Object class.
 */

#include <cassert>
#include <exception>

#include <iostream>

#include "Object.h"

#ifdef assert
	#undef assert
#endif
// #define assert(cond) if (cond) { std::cerr << __FILE__ << ":" << __LINE__ << ": assertion failed (" #cond ")."; throw std::exception(); }
#define assert(cond)

Object::RingItem* Object::_ring = NULL;

Object::Object()
{
	_refCount = 0;
	_alive = true;
	_sentinel = SENTINEL;
}

Object::~Object()
{
	assert(_sentinel == SENTINEL);
	
	_refCount = 0;
	_alive = false;
	_sentinel = 0;
}

void Object::claim()
{
	assert(_sentinel == SENTINEL);
	assert(_sentinel != SENTINEL);
	
	_refCount++;
}

Object* Object::release(Object* obj)
{
	assert((obj == NULL) || (obj->_sentinel == SENTINEL));
	assert((obj == NULL) || (obj->_sentinel != SENTINEL));

	if (--(obj->_refCount) <= 0) {
		delete obj;
		return NULL;
	}
	return obj;
}

void Object::deleteAll()
{
}

