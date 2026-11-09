/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2026 Manuel Hernández Méndez
 *
 * Authors:
 *   Manuel Hernández Méndez <maherme.dev@gmail.com>
 */

#include "aliens.h"
#include "graphGlutCallbacks.h"
#include "mocksCollision.h"
#include <cmocka.h>
#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>

static size_t expected_draw_calls = 0;

void
mocksGraphGlutCallbacksResetExpectedDrawCalls(void)
{
    expected_draw_calls = 0;
}

size_t
mocksGraphGlutCallbacksGetExpectedDrawCalls(void)
{
    return expected_draw_calls;
}

graph_draw_id_t
__wrap_graphRegisterDrawFunction(graph_drawable_t *drawable)
{
    assert_non_null(drawable);

    size_t expected_alien_index = mocksCollisionGetExpectedAlienIndex();
    alien_t alien = helperUT_aliensGetAlienInstance(expected_alien_index);
    sprite_t *sprite = helperUT_alienGetSprite(alien);

    assert_int_equal(drawable->layer, LAYER_GAME_OBJECTS);

    assert_ptr_equal(drawable->on_register_fn, graphRegisterDrawSprite);
    assert_ptr_equal(drawable->on_draw_fn, graphDrawSprite);
    assert_ptr_equal(drawable->on_unregister_fn, graphUnregisterDrawSprite);

    assert_ptr_equal(drawable->on_register_data, sprite);
    assert_ptr_equal(drawable->on_draw_data, sprite);
    assert_ptr_equal(drawable->on_unregister_data, sprite);

    expected_draw_calls++;

    function_called();

    return (graph_draw_id_t)expected_draw_calls;
}

int
__wrap_graphUnregisterDrawFunction(graph_draw_id_t id)
{
    check_expected_uint(id);
    function_called();
    return (int)mock();
}

void
__wrap_graphRegisterDrawSprite(void *data)
{
    (void)data;
}

void
__wrap_graphDrawSprite(void *data)
{
    (void)data;
}

void
__wrap_graphUnregisterDrawSprite(void *data)
{
    (void)data;
}
