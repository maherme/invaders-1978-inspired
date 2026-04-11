/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2025 Manuel Hernández Méndez
 *
 * Authors:
 *   Manuel Hernández Méndez <maherme.dev@gmail.com>
 */

#include "graph.h"
#include <cmocka.h>
#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>

int
__wrap_graphGetSpriteCoordinates(const sprite_t *const sprite, sprite_coordinates_t *coordinates)
{
    check_expected_ptr(sprite);
    coordinates->x1 = (int)mock_type(int);
    coordinates->x2 = (int)mock_type(int);
    coordinates->y1 = (int)mock_type(int);
    coordinates->y2 = (int)mock_type(int);
    function_called();
    return 0;
}
