/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2026 Manuel Hernández Méndez
 *
 * Authors:
 *   Manuel Hernández Méndez <maherme.dev@gmail.com>
 */

#include "collision.h"
#include "events.h"
#include <cmocka.h>
#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

collision_event_t last_collision_event;

void
__wrap_eventEmit(event_type_t type, void *data)
{
    check_expected_uint(type);
    memcpy(&last_collision_event, data, sizeof(last_collision_event));
    function_called();
}
