/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2026 Manuel Hernández Méndez
 *
 * Authors:
 *   Manuel Hernández Méndez <maherme.dev@gmail.com>
 */

#include "testAliens.h"
#include <cmocka.h>
#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>

int
main(void)
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test_setup(testAliensCreate, setup),
        cmocka_unit_test_setup(testAlienAliveNullParameter, setup),
        cmocka_unit_test_setup(testAlienAliveFalse, setup),
        cmocka_unit_test_setup(testAlienAliveTrue, setup),
        cmocka_unit_test_setup(testAliensGetAlienInstanceInvalidIndex, setup),
        cmocka_unit_test_setup(testAliensGetAlives, setup),
        cmocka_unit_test_setup(testAliensGetShooterFormationEmpty, setup),
        cmocka_unit_test_setup(testAliensGetShooterFormationOneAlien, setup),
        cmocka_unit_test_setup(testAliensGetShooterFirstInColumn, setup),
        cmocka_unit_test_setup(testAliensMoveRight, setup),
        cmocka_unit_test_setup(testAliensMoveLeft, setup),
        cmocka_unit_test_setup(testAliensMoveMaxLeft, setup),
        cmocka_unit_test_setup(testAliensMoveMaxRight, setup),
        cmocka_unit_test_setup(testAliensOnCollisionEntityAAlien, setup),
        cmocka_unit_test_setup(testAliensOnCollisionEntityBAlien, setup),
        cmocka_unit_test_setup(testAliensOnCollisionNoAlien, setup),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
