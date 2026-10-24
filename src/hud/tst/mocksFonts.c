/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2026 Manuel Hernández Méndez
 *
 * Authors:
 *   Manuel Hernández Méndez <maherme.dev@gmail.com>
 */

#include "fonts.h"
#include <cmocka.h>
#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>

fonts_handler_t *
__wrap_fontsCreateFontsHandler(size_t size)
{
    check_expected_uint(size);
    function_called();
    return mock_type(fonts_handler_t *);
}

void
__wrap_fontsInitLayout(fonts_handler_t *h, int x, int y)
{
    check_expected_ptr(h);
    check_expected_int(x);
    check_expected_int(y);
    function_called();
}

void
__wrap_fontsDraw(const char *const text, fonts_handler_t *fonts_handler)
{
    check_expected_any(text);
    check_expected_ptr(fonts_handler);
    function_called();
}

void
__wrap_fontsDrawNumber(int num, fonts_handler_t *fonts_handler)
{
    check_expected_int(num);
    check_expected_ptr(fonts_handler);
    function_called();
}
