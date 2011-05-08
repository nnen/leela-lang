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

#include "Object.h"
#include "Input.h"
#include "Output.h"

#define var(name, expr) typeof(expr) name = (expr)

#define foreach(iterator, collection) for (var(iterator, (collection).begin()); (iterator) != (collection).end(); (iterator)++)

typedef uint32_t UInteger;
typedef UInteger Address;
typedef int32_t  Integer;

Ref<Output> normalizeOutput(Ref<Input> input,
                            Ref<Output> output,
                            string fileNamExtension);
bool parse(Ref<Input> input, Ref<Output> output);
bool compile(Ref<Input> input, Ref<Output> output);
void run(Ref<Input> input);
void dumpBytecode(Ref<Input> input, Ref<Output> output);

#endif /* end of include guard: LEELA_H_SD5756UJU65423 */

