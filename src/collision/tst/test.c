/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2026 Manuel Hernández Méndez
 *
 * Authors:
 *   Manuel Hernández Méndez <maherme.dev@gmail.com>
 */

#include "testCollision.h"
#include <cmocka.h>
#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>

int
main(void)
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test_teardown(testCollisionInit, teardown),
        cmocka_unit_test_teardown(testCollisionRegisterCollider, teardown),
        cmocka_unit_test_teardown(testCollisionRegisterColliderPoolFull, teardown),
        cmocka_unit_test_teardown(testCollisionUnregisterColliderValid, teardown),
        cmocka_unit_test_teardown(testCollisionUnregisterColliderNonExisting, teardown),
        cmocka_unit_test_teardown(testCollisionUnregisterColliderWrongGeneration, teardown),
        cmocka_unit_test_teardown(testCollisionUpdateNoActiveColliders, teardown),
        cmocka_unit_test_teardown(testCollisionUpdatePendingRemoveCollider, teardown),
        cmocka_unit_test_teardown(testCollisionUpdateInactiveSlotB, teardown),
        cmocka_unit_test_teardown(testCollisionUpdateActiveSlotB, teardown),
        cmocka_unit_test_teardown(testCollisionUpdateEntitiesShouldNotCollide, teardown),
        cmocka_unit_test_teardown(testCollisionUpdateNoBoxCollision, teardown),
        cmocka_unit_test_teardown(testCollisionUpdateNoBoxCollisionLeft, teardown),
        cmocka_unit_test_teardown(testCollisionUpdateNoBoxCollisionBelow, teardown),
        cmocka_unit_test_teardown(testCollisionUpdateNoBoxCollisionAbove, teardown),
        cmocka_unit_test_teardown(testCollisionUpdateNoPixelCollisionNullImage, teardown),
        cmocka_unit_test_teardown(testCollisionUpdateNoPixelCollisionSecondImageNull, teardown),
        cmocka_unit_test_teardown(testCollisionUpdateNoPixelCollisionTransparent, teardown),
        cmocka_unit_test_teardown(testCollisionUpdatePixelCollisionFirstOpaqueSecondTransparent, teardown),
        cmocka_unit_test_teardown(testCollisionUpdatePixelCollision, teardown),
        cmocka_unit_test(testCollisionCheckBorderNullParameter),
        cmocka_unit_test(testCollisionCheckBorderUpTrue),
        cmocka_unit_test(testCollisionCheckBorderDownTrue),
        cmocka_unit_test(testCollisionCheckBorderRightTrue),
        cmocka_unit_test(testCollisionCheckBorderLeftTrue),
        cmocka_unit_test(testCollisionCheckBorderInvalidDir),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
