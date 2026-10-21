/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2026 Manuel Hernández Méndez
 *
 * Authors:
 *   Manuel Hernández Méndez <maherme.dev@gmail.com>
 */

#include "testFonts.h"
#include <cmocka.h>
#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>

int
main(void)
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(testFontsCreateFontsHandler),
        cmocka_unit_test(testFontsDestroyFontsHandler),
        cmocka_unit_test(testFontsInitLayout),
        cmocka_unit_test(testFontsDraw),
        cmocka_unit_test(testFontsDrawNotSupported),
        cmocka_unit_test(testFontsDrawBiggerBuffer),
        cmocka_unit_test(testFontsDrawSmallerBuffer),
        cmocka_unit_test(testFontsDrawMixedCharacters),
        cmocka_unit_test(testFontsDrawNumber),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
