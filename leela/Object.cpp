/**
 * \file   leela/Object.cpp
 * \author Jan Milík <milikjan@fit.cvut.cz>
 * \date   2011-03-27
 *
 * \brief  Implementation of the Object class.
 */

#include "Object.h"

Object::RingItem* Object::_ring = NULL;

Object::Object()
{
	_refCount = 0;
}

Object::~Object()
{
	_refCount = 0;
}

void Object::claim()
{
	_refCount++;
}

Object* Object::release(Object* obj)
{
	if (--(obj->_refCount) <= 0) {
		delete obj;
		return NULL;
	}
	return obj;
}

void Object::deleteAll()
{
}

