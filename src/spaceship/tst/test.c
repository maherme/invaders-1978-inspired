/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2025 Manuel Hernández Méndez
 *
 * Authors:
 *   Manuel Hernández Méndez <maherme.dev@gmail.com>
 */

#include "testSpaceship.h"
#include <cmocka.h>
#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>

int
main(void)
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(testSpaceshipCreate),
        cmocka_unit_test(testSpaceshipAliveNullParameter),
        cmocka_unit_test(testSpaceshipAliveFalse),
        cmocka_unit_test(testSpaceshipAliveTrue),
        cmocka_unit_test(testSpaceshipOnCollisionEntityA),
        cmocka_unit_test(testSpaceshipOnCollisionEntityB),
        cmocka_unit_test(testSpaceshipOnCollisionNoSpaceship),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
