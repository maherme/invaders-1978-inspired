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
testGraphRegisterDrawFunctionFailNotInitialized(void **status);

void
testGraphRegisterDrawFunctionFailNullParameter(void **status);

void
testGraphRegisterDrawFunctionFailNegativeLayer(void **status);

void
testGraphRegisterDrawFunctionFailLayerOutOfRange(void **status);

void
testGraphRegisterDrawFunctionUniqueIds(void **status);

void
testGraphRegisterDrawFunctionCallback(void **status);

void
testGraphUnregisterDrawFunctionFailNotInitialized(void **status);

void
testGraphUnregisterDrawFunctionSuccess(void **status);

void
testGraphUnregisterDrawFunctionNotFound(void **status);

void
testGraphUnregisterDrawFunctionInvalidId(void **status);

void
testGraphUnregisterDrawFunctionCallback(void **status);

void
testGraphUnregisterDrawFunctionKeepsOtherDrawables(void **status);

void
testGraphGlutDisplayDrawLayersOrder(void **status);

void
testGraphGlutDisplayDrawLayersNullCallback(void **status);

void
testGraphGlutDisplayDrawLayersEmpty(void **status);

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
