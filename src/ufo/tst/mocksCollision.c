/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2026 Manuel Hernández Méndez
 *
 * Authors:
 *   Manuel Hernández Méndez <maherme.dev@gmail.com>
 */

#include "collision.h"
#include <cmocka.h>
#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>

static collision_handle_t fake_collision_handle;

collision_handle_t
__wrap_collisionRegisterCollider(collider_t *collider)
{
    (void)collider;
    function_called();
    return fake_collision_handle;
}

int
__wrap_collisionUnregisterCollider(collision_handle_t handle)
{
    (void)handle;
    function_called();
    return 0;
}
