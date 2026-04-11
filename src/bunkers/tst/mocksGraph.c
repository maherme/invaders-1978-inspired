/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2026 Manuel Hernández Méndez
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

static sprite_t captured_mask;
static const sprite_t *captured_footprint_sprite;
static const sprite_t *captured_updated_sprite;

const sprite_t *
helperUT_getCapturedMask(void)
{
    return &captured_mask;
}

const sprite_t *
helperUT_getCapturedFootprintSprite(void)
{
    return captured_footprint_sprite;
}

const sprite_t *
helperUT_getCapturedUpdatedSprite(void)
{
    return captured_updated_sprite;
}

void
__wrap_graphMakeFootprintSprite(const sprite_t *const sprite, const sprite_t *const mask)
{
    function_called();

    check_expected_ptr(sprite);
    check_expected_ptr(mask);

    captured_footprint_sprite = sprite;
    captured_mask = *mask;
}

void
__wrap_graphUpdateSprite(const sprite_t *const sprite)
{
    function_called();

    check_expected_ptr(sprite);

    captured_updated_sprite = sprite;
}
