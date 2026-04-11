/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2026 Manuel Hernández Méndez
 *
 * Authors:
 *   Manuel Hernández Méndez <maherme.dev@gmail.com>
 */

#include "explosion.h"
#include "graph.h"
#include <cmocka.h>
#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>

static sprite_t fake_sprite;

void
helperUT_explosionsSetStaticFrameSize(int width, int height)
{
    fake_sprite.width = width;
    fake_sprite.height = height;
}

sprite_t *
__wrap_explosionsGetStaticFrame(entity_type_t type, int num_frame)
{
    function_called();

    check_expected_uint(type);
    check_expected_int(num_frame);

    return &fake_sprite;
}
