/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2026 Manuel Hernández Méndez
 *
 * Authors:
 *   Manuel Hernández Méndez <maherme.dev@gmail.com>
 */

#include "events.h"
#include <cmocka.h>
#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>

entity_death_event_t captured_death_event;

void
__wrap_eventEmit(event_type_t type, void *data)
{
    function_called();
    check_expected_uint(type);

    assert_non_null(data);
    if (type == EVENT_ENTITY_DIED)
    {
        captured_death_event = *(entity_death_event_t *)data;
    }
}
