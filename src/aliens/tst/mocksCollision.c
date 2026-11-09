/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2026 Manuel Hernández Méndez
 *
 * Authors:
 *   Manuel Hernández Méndez <maherme.dev@gmail.com>
 */

#include "mocksCollision.h"
#include "aliens.h"
#include "collision.h"
#include "graph.h"
#include <cmocka.h>
#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>

static size_t expected_alien_index = 0;
static size_t expected_collider_calls = 0;

size_t
mocksCollisionGetExpectedAlienIndex(void)
{
    return expected_alien_index;
}

void
mocksCollisionResetExpectedAlienIndex(void)
{
    expected_alien_index = 0;
}

size_t
mocksCollisionGetExpectedColliderCalls(void)
{
    return expected_collider_calls;
}

void
mocksCollisionResetExpectedColliderCalls(void)
{
    expected_collider_calls = 0;
}

collision_handle_t
__wrap_collisionRegisterCollider(collider_t *collider)
{
    assert_non_null(collider);

    collision_handle_t *handle = mock_ptr_type(collision_handle_t *);

    alien_t alien = helperUT_aliensGetAlienInstance(expected_alien_index);
    sprite_t *sprite = helperUT_alienGetSprite(alien);

    assert_ptr_equal(collider->x, &sprite->x);
    assert_ptr_equal(collider->y, &sprite->y);
    assert_int_equal(collider->width, sprite->width);
    assert_int_equal(collider->height, sprite->height);
    assert_ptr_equal(collider->pixel_data_ref, (const char **)&sprite->image);
    assert_int_equal(collider->entity, ENTITY_ALIEN);

    expected_collider_calls++;
    function_called();

    expected_alien_index++;

    return *handle;
}

int
__wrap_collisionUnregisterCollider(collision_handle_t handle)
{
    check_expected_any(handle);
    function_called();
    return (int)mock();
}
