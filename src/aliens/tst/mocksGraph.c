/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2026 Manuel Hernández Méndez
 *
 * Authors:
 *   Manuel Hernández Méndez <maherme.dev@gmail.com>
 */

#include "aliens.h"
#include "graph.h"
#include "mocksCollision.h"
#include <cmocka.h>
#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>

void
__wrap_graphUpdateTimeSprite(sprite_t *sprite)
{
    size_t expected_alien_index = mocksCollisionGetExpectedAlienIndex();
    alien_t alien = helperUT_aliensGetAlienInstance(expected_alien_index);
    sprite_t *expected_sprite = helperUT_alienGetSprite(alien);

    assert_ptr_equal(sprite, expected_sprite);
    function_called();
}
