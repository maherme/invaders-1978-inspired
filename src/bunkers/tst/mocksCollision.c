/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2026 Manuel Hernández Méndez
 *
 * Authors:
 *   Manuel Hernández Méndez <maherme.dev@gmail.com>
 */

#include "collision.h"
#include "testBunkers.h"
#include <cmocka.h>
#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>

static collision_handle_t fake_collision_handle;
static void *captured_bunker_owners[NUM_TEST_BUNKERS];
static size_t captured_bunker_count;

void
helperUT_resetCapturedBunkerOwners(void)
{
    captured_bunker_count = 0;
}

void *
helperUT_getCapturedBunkerOwner(size_t index)
{
    assert_true(index < captured_bunker_count);
    return captured_bunker_owners[index];
}

collision_handle_t
__wrap_collisionRegisterCollider(collider_t *collider)
{
    function_called();

    assert_non_null(collider);

    if (captured_bunker_count < NUM_TEST_BUNKERS)
    {
        captured_bunker_owners[captured_bunker_count++] = collider->owner;
    }

    return fake_collision_handle;
}
