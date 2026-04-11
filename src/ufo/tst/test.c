/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2026 Manuel Hernández Méndez
 *
 * Authors:
 *   Manuel Hernández Méndez <maherme.dev@gmail.com>
 */

#include "testUfo.h"
#include <cmocka.h>
#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>

int
main(void)
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(testUfoCreateLeftDirection),
        cmocka_unit_test(testUfoCreateRightDirection),
        cmocka_unit_test(testUfoDestroyNullParameter),
        cmocka_unit_test(testUfoDestroy),
        cmocka_unit_test(testUfoAliveNullParameter),
        cmocka_unit_test(testUfoAliveFalse),
        cmocka_unit_test(testUfoAliveTrue),
        cmocka_unit_test(testUfoGetDirectionNullParameter),
        cmocka_unit_test(testUfoCheckForMovingNullParameter),
        cmocka_unit_test(testUfoCheckForMovingAliveFalse),
        cmocka_unit_test(testUfoCheckForMovingTimeoutFalse),
        cmocka_unit_test(testUfoCheckForMovingTrue),
        cmocka_unit_test(testUfoCheckForMovingTrueMoreCalls),
        cmocka_unit_test(testUfoGetPoints),
        cmocka_unit_test(testUfoPointsByDangerLevel),
        cmocka_unit_test(testUfoOnCollisionEntityA),
        cmocka_unit_test(testUfoOnCollisionEntityB),
        cmocka_unit_test(testUfoOnCollisionNoUfo),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
