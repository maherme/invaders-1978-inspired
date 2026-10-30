/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2024 Manuel Hernández Méndez
 *
 * Authors:
 *   Manuel Hernández Méndez <maherme.dev@gmail.com>
 */

#ifndef __TESTGRAPHGLUTCALLBACKS_H__
#define __TESTGRAPHGLUTCALLBACKS_H__

int
setup(void **state);

int
teardown(void **state);

void
testGraphRegisterDrawFunctionFailNullParameter(void **status);

void
testGraphRegisterDrawFunctionSuccess(void **status);

void
testGraphGlutDisplay(void **status);

void
testGraphGlutReshapeScaleLowerThanOne(void **status);

void
testGraphGlutReshapeScaleHigherThanOne(void **status);

void
testGraphRegisterDrawSprite(void **status);

void
testGraphDrawSprite(void **status);

void
testGraphUnregisterDrawSprite(void **status);

#endif /* __TESTGRAPHGLUTCALLBACKS_H__ */
