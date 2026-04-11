/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2026 Manuel Hernández Méndez
 *
 * Authors:
 *   Manuel Hernández Méndez <maherme.dev@gmail.com>
 */

#include "testBullet.h"
#include <cmocka.h>
#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>

int
main(void)
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test_setup(testBulletCreate, setup),
        cmocka_unit_test_setup(testBulletCreateAllTypeBulletAlien, setup),
        cmocka_unit_test_setup(testBulletCreateTwoBulletSpaceship, setup),
        cmocka_unit_test_setup(testBulletCreateInvalidEntity, setup),
        cmocka_unit_test_setup(testBulletDestroyNullParameter, setup),
        cmocka_unit_test_setup(testBulletDestroy, setup),
        cmocka_unit_test_setup(testBulletUsedNullParameter, setup),
        cmocka_unit_test_setup(testBulletUsedFalse, setup),
        cmocka_unit_test_setup(testBulletUsedTrue, setup),
        cmocka_unit_test_setup(testBulletGetTypeNullParameters, setup),
        cmocka_unit_test_setup(testBulletGetType, setup),
        cmocka_unit_test_setup(testBulletCallFunctionForEachNullParameter, setup),
        cmocka_unit_test_setup(testBulletCallFunctionForEachBulletPoolEmpty, setup),
        cmocka_unit_test_setup(testBulletCallFunctionForEach, setup),
        cmocka_unit_test_setup(testBulletAlienInhibit, setup),
        cmocka_unit_test_setup(testBulletNoneUsedTrue, setup),
        cmocka_unit_test_setup(testBulletNoneUsedFalse, setup),
        cmocka_unit_test_setup(testBulletSpaceshipHitBulletAliensNoBullets, setup),
        cmocka_unit_test_setup(testBulletSpaceshipHitBulletAliensCmpFalse, setup),
        cmocka_unit_test_setup(testBulletSpaceshipHitBulletAliensCmpTrue, setup),
        cmocka_unit_test_setup(testBulletOnCollisionNonBulletEntities, setup),
        cmocka_unit_test_setup(testBulletOnCollisionNullOwnerVsBunker, setup),
        cmocka_unit_test_setup(testBulletOnCollisionBulletVsBullet, setup),
        cmocka_unit_test_setup(testBulletOnCollisionSpaceshipBulletVsAlien, setup),
        cmocka_unit_test_setup(testBulletOnCollisionSpaceshipBulletVsBunker, setup),
        cmocka_unit_test_setup(testBulletOnCollisionBunkerVsSpaceshipBullet, setup),
        cmocka_unit_test_setup(testBulletOnCollisionAlienBulletVsAlien, setup),
        cmocka_unit_test_setup(testBulletOnCollisionAlienBulletVsSpaceshipBullet, setup),
        cmocka_unit_test_setup(testBulletOnCollisionAlienBulletVsBunker, setup),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
