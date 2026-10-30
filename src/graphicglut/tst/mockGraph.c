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

void
__wrap_graphCreateImage(sprite_t *sprite)
{
    check_expected_ptr(sprite);
    function_called();
}

void
__wrap_graphPrintImage(const sprite_t *const sprite)
{
    check_expected_ptr(sprite);
    function_called();
}

void
__wrap_graphDestroyImage(sprite_t *sprite)
{
    check_expected_ptr(sprite);
    function_called();
}

void
__wrap_graphPrintBorder(int x, int y, int w, int h)
{
    check_expected_int(x);
    check_expected_int(y);
    check_expected_int(w);
    check_expected_int(h);
    function_called();
}
