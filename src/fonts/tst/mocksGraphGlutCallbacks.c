/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2026 Manuel Hernández Méndez
 *
 * Authors:
 *   Manuel Hernández Méndez <maherme.dev@gmail.com>
 */

#include "graphGlutCallbacks.h"
#include <cmocka.h>
#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>

graph_draw_id_t
__wrap_graphRegisterDrawFunction(graph_drawable_t *drawable)
{
    check_expected_ptr(drawable->on_register_data);
    check_expected_ptr(drawable->on_draw_data);
    check_expected_ptr(drawable->on_unregister_data);
    function_called();
    return 0;
}

int
__wrap_graphUnregisterDrawFunction(graph_draw_id_t id)
{
    (void)id;
    function_called();
    return 0;
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
