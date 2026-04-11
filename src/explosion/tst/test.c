/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2026 Manuel Hernández Méndez
 *
 * Authors:
 *   Manuel Hernández Méndez <maherme.dev@gmail.com>
 */

#include "testExplosion.h"
#include <cmocka.h>
#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>

int
main(void)
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test_teardown(testExplosionCreate, teardown),
        cmocka_unit_test_teardown(testExplosionsDestroyListEmpty, teardown),
        cmocka_unit_test_teardown(testExplosionsDestroyTimeoutFalse, teardown),
        cmocka_unit_test_teardown(testExplosionsDestroyTimeoutTrue, teardown),
        cmocka_unit_test_teardown(testExplosionsDestroyTimeoutCallback, teardown),
        cmocka_unit_test_teardown(testExplosionsDestroyTwoExplosions, teardown),
        cmocka_unit_test_teardown(testExplosionDestroyMoreOneFrame, teardown),
        cmocka_unit_test_teardown(testExplosionDestroyMoreOneFrameTimeoutFalse, teardown),
        cmocka_unit_test_teardown(testExplosionAllFinishedTrue, teardown),
        cmocka_unit_test_teardown(testExplosionAllFinishedFalse, teardown),
        cmocka_unit_test_teardown(testExplosionOnEntityDiedOnFinishAction, teardown),
        cmocka_unit_test_teardown(testExplosionOnEntityDiedOnFinishNull, teardown),
        cmocka_unit_test(testExplosionOnEntityDiedNothingToDo),
        cmocka_unit_test(testExplosionsGetExplosionHeight),
        cmocka_unit_test(testExplosionsGetStaticFrameEntityReturnNull),
        cmocka_unit_test(testExplosionGetStaticFrameSpaceshipBullet),
        cmocka_unit_test(testExplosionGetStaticFrameAlienBullet),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
