/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2025 Manuel Hernández Méndez
 *
 * Authors:
 *   Manuel Hernández Méndez <maherme.dev@gmail.com>
 */

#include "testSpaceship.h"
#include "collision.h"
#include "events.h"
#include "spaceship.h"
#include <cmocka.h>
#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>

static void
foo_cb(void)
{
    function_called();
}

void
testSpaceshipCreate(void **status)
{
    (void)status;
    int x_pos = 10;
    int y_pos = 20;

    expect_function_call(__wrap_graphRegisterPrint);
    expect_function_call(__wrap_collisionRegisterCollider);

    spaceship_t spaceship = spaceshipCreate(x_pos, y_pos, foo_cb);

    assert_true(spaceshipAlive(spaceship));
}

void
testSpaceshipAliveNullParameter(void **status)
{
    (void)status;

    assert_false(spaceshipAlive(NULL));
}

void
testSpaceshipAliveFalse(void **status)
{
    (void)status;
    spaceship_t spaceship = helperUT_spaceshipGetInstance();
    helperUT_spaceshipSetAlive(spaceship, false);

    assert_false(spaceshipAlive(spaceship));
}

void
testSpaceshipAliveTrue(void **status)
{
    (void)status;
    spaceship_t spaceship = helperUT_spaceshipGetInstance();

    assert_true(spaceshipAlive(spaceship));
}

void
testSpaceshipOnCollisionEntityA(void **status)
{
    (void)status;

    spaceship_t spaceship = helperUT_spaceshipGetInstance();

    collision_event_t collision_data = {
        .entity_a = ENTITY_SPACESHIP,
        .owner_a = spaceship,
        .bounds_a =
            {
                .x = 10,
                .y = 20,
                .width = 13,
                .height = 8,
            },
        .entity_b = ENTITY_ALIEN_BULLET,
        .bounds_b =
            {
                .x = 100,
                .y = 200,
                .width = 32,
                .height = 16,
            },
    };

    expect_uint_value(__wrap_eventEmit, type, EVENT_ENTITY_DIED);
    expect_function_call(__wrap_eventEmit);

    expect_function_call(__wrap_graphGetSprite);
    expect_function_call(__wrap_graphUnregisterPrint);
    expect_function_call(__wrap_collisionUnregisterCollider);

    spaceshipOnCollision(&collision_data);

    assert_int_equal(10, death_event_data.x);
    assert_int_equal(20, death_event_data.y);
    assert_int_equal(13, death_event_data.width);
    assert_int_equal(8, death_event_data.height);
    assert_int_equal(ENTITY_SPACESHIP, death_event_data.entity_type);
}

void
testSpaceshipOnCollisionEntityB(void **status)
{
    (void)status;

    spaceship_t spaceship = helperUT_spaceshipGetInstance();

    collision_event_t collision_data = {
        .entity_a = ENTITY_ALIEN_BULLET,
        .owner_a = NULL,
        .bounds_a =
            {
                .x = 100,
                .y = 200,
                .width = 32,
                .height = 16,
            },
        .entity_b = ENTITY_SPACESHIP,
        .owner_b = spaceship,
        .bounds_b =
            {
                .x = 30,
                .y = 40,
                .width = 13,
                .height = 8,
            },
    };

    expect_uint_value(__wrap_eventEmit, type, EVENT_ENTITY_DIED);
    expect_function_call(__wrap_eventEmit);

    expect_function_call(__wrap_graphGetSprite);
    expect_function_call(__wrap_graphUnregisterPrint);
    expect_function_call(__wrap_collisionUnregisterCollider);

    spaceshipOnCollision(&collision_data);

    assert_int_equal(30, death_event_data.x);
    assert_int_equal(40, death_event_data.y);
    assert_int_equal(13, death_event_data.width);
    assert_int_equal(8, death_event_data.height);
    assert_int_equal(ENTITY_SPACESHIP, death_event_data.entity_type);
}

void
testSpaceshipOnCollisionNoSpaceship(void **status)
{
    (void)status;

    collision_event_t collision_data = {
        .entity_a = ENTITY_COUNT,
        .owner_a = NULL,
        .entity_b = ENTITY_COUNT,
        .owner_b = NULL,
    };

    spaceshipOnCollision(&collision_data);
}
