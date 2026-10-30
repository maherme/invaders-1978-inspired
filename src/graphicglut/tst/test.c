/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2024 Manuel Hernández Méndez
 *
 * Authors:
 *   Manuel Hernández Méndez <maherme.dev@gmail.com>
 */

#include "testGraphGlut.h"
#include "testGraphGlutCallbacks.h"
#include <cmocka.h>
#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>

int
main(void)
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(testGraphInitGlut),
        cmocka_unit_test_setup_teardown(testGraphRegisterDrawFunctionFailNotInitialized, setup, teardown),
        cmocka_unit_test_setup_teardown(testGraphRegisterDrawFunctionFailNullParameter, setup, teardown),
        cmocka_unit_test_setup_teardown(testGraphRegisterDrawFunctionFailNegativeLayer, setup, teardown),
        cmocka_unit_test_setup_teardown(testGraphRegisterDrawFunctionFailLayerOutOfRange, setup, teardown),
        cmocka_unit_test_setup_teardown(testGraphRegisterDrawFunctionUniqueIds, setup, teardown),
        cmocka_unit_test_setup_teardown(testGraphRegisterDrawFunctionCallback, setup, teardown),
        cmocka_unit_test_setup_teardown(testGraphUnregisterDrawFunctionFailNotInitialized, setup, teardown),
        cmocka_unit_test_setup_teardown(testGraphUnregisterDrawFunctionSuccess, setup, teardown),
        cmocka_unit_test_setup_teardown(testGraphUnregisterDrawFunctionNotFound, setup, teardown),
        cmocka_unit_test_setup_teardown(testGraphUnregisterDrawFunctionInvalidId, setup, teardown),
        cmocka_unit_test_setup_teardown(testGraphUnregisterDrawFunctionCallback, setup, teardown),
        cmocka_unit_test_setup_teardown(testGraphUnregisterDrawFunctionKeepsOtherDrawables, setup, teardown),
        cmocka_unit_test_setup_teardown(testGraphGlutDisplayDrawLayersOrder, setup, teardown),
        cmocka_unit_test_setup_teardown(testGraphGlutDisplayDrawLayersNullCallback, setup, teardown),
        cmocka_unit_test_setup_teardown(testGraphGlutDisplayDrawLayersEmpty, setup, teardown),
        cmocka_unit_test(testGraphGlutReshapeScaleLowerThanOne),
        cmocka_unit_test(testGraphGlutReshapeScaleHigherThanOne),
        cmocka_unit_test(testGraphRegisterDrawSprite),
        cmocka_unit_test(testGraphDrawSprite),
        cmocka_unit_test(testGraphUnregisterDrawSprite),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
