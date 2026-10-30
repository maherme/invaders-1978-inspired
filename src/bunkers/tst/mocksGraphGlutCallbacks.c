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
    (void)drawable;
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
