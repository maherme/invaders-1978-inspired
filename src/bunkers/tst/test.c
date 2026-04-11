/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2026 Manuel Hernández Méndez
 *
 * Authors:
 *   Manuel Hernández Méndez <maherme.dev@gmail.com>
 */

#include "testBunkers.h"
#include <cmocka.h>
#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>

int
main(void)
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test_setup(testBunkersCreate, setup),
        cmocka_unit_test_setup(testBunkersOnCollisionShipBulletEntityA, setup),
        cmocka_unit_test_setup(testBunkersOnCollisionAlienBulletEntityA, setup),
        cmocka_unit_test_setup(testBunkersOnCollisionShipBulletEntityB, setup),
        cmocka_unit_test_setup(testBunkersOnCollisionAlienBulletEntityB, setup),
        cmocka_unit_test_setup(testBunkersOnCollisionNoBunker, setup),
        cmocka_unit_test_setup(testBunkersOnCollisionNullOwner, setup),
        cmocka_unit_test_setup(testBunkersOnCollisionUnsupportedEntity, setup),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
