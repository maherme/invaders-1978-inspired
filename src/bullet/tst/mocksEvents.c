/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2026 Manuel Hernández Méndez
 *
 * Authors:
 *   Manuel Hernández Méndez <maherme.dev@gmail.com>
 */

#include "mocksEvents.h"
#include "events.h"
#include <cmocka.h>
#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>

entity_death_event_t captured_deaths[MAX_CAPTURED_DEATHS];
size_t captured_death_count;

void
__wrap_eventEmit(event_type_t type, void *data)
{
    function_called();
    check_expected_uint(type);

    assert_non_null(data);
    assert_true(captured_death_count < MAX_CAPTURED_DEATHS);

    if (type == EVENT_ENTITY_DIED)
    {
        captured_deaths[captured_death_count++] = *(const entity_death_event_t *)data;
    }
}
