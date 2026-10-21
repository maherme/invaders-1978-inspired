/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2025 Manuel Hernández Méndez
 *
 * Authors:
 *   Manuel Hernández Méndez <maherme.dev@gmail.com>
 */

#ifndef __TESTGRAPH_H__
#define __TESTGRAPH_H__

void
testGraphCreateImageNullParameter(void **status);

void
testGraphCreateImageSuccess(void **status);

void
testGraphDestroyImageNullParameter(void **status);

void
testGraphDestroyImage(void **status);

void
testGraphGetSpriteFail(void **status);

void
testGraphGetSpriteSuccess(void **status);

void
testGraphGetSpriteCoordinatesNullParameters(void **status);

void
testGraphGetSpriteCoordinates(void **status);

void
testGraphUpdateImageToPrintNullParameter(void **status);

void
testGraphUpdateImageToPrintOneFrame(void **status);

void
testGraphUpdateImageToPrintMoreOneFrame(void **status);

void
testGraphUpdateTimeSpriteNullParameter(void **status);

void
testGraphUpdateTimeSprite(void **status);

void
testGraphPrintImageNullParameter(void **status);

void
testGraphPrintImageVisibleFalse(void **status);

void
testGraphPrintImageSuccess(void **status);

void
testGraphUpdateSpriteNullParameter(void **status);

void
testGraphUpdateSpriteSameDimensions(void **status);

void
testGraphUpdateSpriteDifferentHeight(void **status);

void
testGraphUpdateSpriteDifferentWidth(void **status);

void
testGraphPrintGameBorder(void **status);

void
testGraphMakeFootprintSpriteNullArguments(void **status);

void
testGraphMakeFootprintSpriteInvalidDimensions(void **status);

void
testGraphMakeFootprintSpriteNoHorizontalOverlap(void **status);

void
testGraphMakeFootprintSpriteNoVerticalOverlap(void **status);

void
testGraphMakeFootprintSpriteOverlap(void **status);

void
testGraphMakeFootprintSpriteOpaquePixels(void **status);

void
testGraphMakeFootprintSpriteTransparentSpritePixel(void **status);

void
testGraphMakeFootprintSpriteTransparentMaskPixel(void **status);

void
testGraphMakeFootprintSpriteSemiTransparentPixels(void **status);

#endif
