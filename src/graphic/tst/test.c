/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2025 Manuel Hernández Méndez
 *
 * Authors:
 *   Manuel Hernández Méndez <maherme.dev@gmail.com>
 */

#include "testGraph.h"
#include <cmocka.h>
#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>

int
main(void)
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(testGraphCreateImageNullParameter),
        cmocka_unit_test(testGraphCreateImageSuccess),
        cmocka_unit_test(testGraphDestroyImageNullParameter),
        cmocka_unit_test(testGraphDestroyImage),
        cmocka_unit_test(testGraphGetSpriteFail),
        cmocka_unit_test(testGraphGetSpriteSuccess),
        cmocka_unit_test(testGraphGetSpriteCoordinatesNullParameters),
        cmocka_unit_test(testGraphGetSpriteCoordinates),
        cmocka_unit_test(testGraphUpdateImageToPrintNullParameter),
        cmocka_unit_test(testGraphUpdateImageToPrintOneFrame),
        cmocka_unit_test(testGraphUpdateImageToPrintMoreOneFrame),
        cmocka_unit_test(testGraphUpdateTimeSpriteNullParameter),
        cmocka_unit_test(testGraphUpdateTimeSprite),
        cmocka_unit_test(testGraphPrintImageNullParameter),
        cmocka_unit_test(testGraphPrintImageVisibleFalse),
        cmocka_unit_test(testGraphPrintImageSuccess),
        cmocka_unit_test(testGraphUpdateSpriteNullParameter),
        cmocka_unit_test(testGraphUpdateSpriteSameDimensions),
        cmocka_unit_test(testGraphUpdateSpriteDifferentHeight),
        cmocka_unit_test(testGraphUpdateSpriteDifferentWidth),
        cmocka_unit_test(testGraphPrintGameBorder),
        cmocka_unit_test(testGraphMakeFootprintSpriteNullArguments),
        cmocka_unit_test(testGraphMakeFootprintSpriteInvalidDimensions),
        cmocka_unit_test(testGraphMakeFootprintSpriteNoHorizontalOverlap),
        cmocka_unit_test(testGraphMakeFootprintSpriteNoVerticalOverlap),
        cmocka_unit_test(testGraphMakeFootprintSpriteOverlap),
        cmocka_unit_test(testGraphMakeFootprintSpriteOpaquePixels),
        cmocka_unit_test(testGraphMakeFootprintSpriteTransparentSpritePixel),
        cmocka_unit_test(testGraphMakeFootprintSpriteTransparentMaskPixel),
        cmocka_unit_test(testGraphMakeFootprintSpriteSemiTransparentPixels),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
