/**
 * \file   leela/leela.h
 * \author Jan Milík <milikjan@fit.cvut.cz>
 * \date   2011-03-26
 *
 * \brief  Common header file for Leela.
 */

#ifndef LEELA_H_SD5756UJU65423
#define LEELA_H_SD5756UJU65423

#include <stdint.h>

#define var(name, expr) typeof(expr) name = (expr)

#define foreach(iterator, collection) for (var(iterator, (collection).begin()); (iterator) != (collection).end(); (iterator)++)

typedef uint32_t UInteger;
typedef UInteger Address;
typedef int32_t  Integer;

#endif /* end of include guard: LEELA_H_SD5756UJU65423 */

