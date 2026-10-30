/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2026 Manuel Hernández Méndez
 *
 * Authors:
 *   Manuel Hernández Méndez <maherme.dev@gmail.com>
 */

#include "testUfo.h"
#include "collision.h"
#include "entity_types.h"
#include "events.h"
#include "ufo.h"
#include <cmocka.h>
#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

int
__wrap_rand(void)
{
    function_called();
    return (int)mock();
}

void
testUfoCreateLeftDirection(void **status)
{
    (void)status;

    will_return(__wrap_rand, 1);
    expect_function_call(__wrap_rand);
    expect_function_call(__wrap_graphUpdateTimeSprite);
    expect_function_call(__wrap_graphRegisterDrawFunction);
    expect_function_call(__wrap_collisionRegisterCollider);

    ufo_t ufo = ufoCreate();
    assert_int_equal(LEFT, ufoGetDirection(ufo));
}

void
testUfoCreateRightDirection(void **status)
{
    (void)status;

    will_return(__wrap_rand, RAND_MAX);
    expect_function_call(__wrap_rand);
    expect_function_call(__wrap_graphUpdateTimeSprite);
    expect_function_call(__wrap_graphRegisterDrawFunction);
    expect_function_call(__wrap_collisionRegisterCollider);

    ufo_t ufo = ufoCreate();
    assert_int_equal(RIGHT, ufoGetDirection(ufo));
}

void
testUfoDestroyNullParameter(void **status)
{
    (void)status;

    ufoDestroy(NULL);
}

void
testUfoDestroy(void **status)
{
    (void)status;
    ufo_t ufo = helperUT_ufoGetInstance();

    expect_function_call(__wrap_graphUnregisterDrawFunction);
    expect_function_call(__wrap_collisionUnregisterCollider);

    ufoDestroy(ufo);

    assert_false(ufoAlive(ufo));
}

void
testUfoAliveNullParameter(void **status)
{
    (void)status;

    assert_false(ufoAlive(NULL));
}

void
testUfoAliveFalse(void **status)
{
    (void)status;
    ufo_t ufo = helperUT_ufoGetInstance();
    helperUT_ufoSetAlive(ufo, false);

    assert_false(ufoAlive(ufo));
}

void
testUfoAliveTrue(void **status)
{
    (void)status;
    ufo_t ufo = helperUT_ufoGetInstance();

    assert_true(ufoAlive(ufo));
}

void
testUfoGetDirectionNullParameter(void **status)
{
    (void)status;

    assert_int_equal(INVALID_DIR, ufoGetDirection(NULL));
}

void
testUfoCheckForMovingNullParameter(void **status)
{
    (void)status;

    assert_false(ufoCheckForMoving(NULL));
}

void
testUfoCheckForMovingAliveFalse(void **status)
{
    (void)status;

    ufo_t ufo = helperUT_ufoGetInstance();
    helperUT_ufoSetAlive(ufo, false);

    assert_false(ufoCheckForMoving(ufo));
}

void
testUfoCheckForMovingTimeoutFalse(void **status)
{
    (void)status;

    ufo_t ufo = helperUT_ufoGetInstance();
    helperUT_ufoSetTimeToAppear(ufo, 1);

    will_return(__wrap_utilsCheckTimeout, false);
    expect_function_call(__wrap_utilsCheckTimeout);

    assert_false(ufoCheckForMoving(ufo));
}

void
testUfoCheckForMovingTrue(void **status)
{
    (void)status;

    ufo_t ufo = helperUT_ufoGetInstance();
    helperUT_ufoSetTimeToAppear(ufo, 1);

    will_return(__wrap_utilsCheckTimeout, true);
    expect_function_call(__wrap_utilsCheckTimeout);

    assert_true(ufoCheckForMoving(ufo));
}

void
testUfoCheckForMovingTrueMoreCalls(void **status)
{
    (void)status;

    ufo_t ufo = helperUT_ufoGetInstance();
    helperUT_ufoSetTimeToAppear(ufo, 1);

    will_return(__wrap_utilsCheckTimeout, true);
    expect_function_call(__wrap_utilsCheckTimeout);

    assert_true(ufoCheckForMoving(ufo));
    /* no more calls to utilsCheckTimeout are expected from here */
    assert_true(ufoCheckForMoving(ufo));
}

void
testUfoGetPoints(void **status)
{
    (void)status;

    unsigned int expected_points = 50;
    helperUT_ufoSetPoints(expected_points);

    assert_uint_equal(expected_points, ufoGetPoints());
}

void
testUfoPointsByDangerLevel(void **status)
{
    (void)status;
    danger_level_t danger_lv = DANGER_LOW;
    unsigned int expected_points = 50;

    ufoOnDangerLevelChanged(&danger_lv);
    assert_uint_equal(expected_points, helperUT_ufoGetPoints());

    danger_lv = DANGER_MID;
    expected_points = 150;

    ufoOnDangerLevelChanged(&danger_lv);
    assert_uint_equal(expected_points, helperUT_ufoGetPoints());

    danger_lv = DANGER_HIGH;
    expected_points = 300;

    ufoOnDangerLevelChanged(&danger_lv);
    assert_uint_equal(expected_points, helperUT_ufoGetPoints());
}

void
testUfoOnCollisionEntityA(void **status)
{
    (void)status;

    ufo_t ufo = helperUT_ufoGetInstance();
    unsigned int expected_points = 150;
    helperUT_ufoSetPoints(expected_points);

    collision_event_t collision_data = {
        .entity_a = ENTITY_UFO,
        .owner_a = ufo,
        .bounds_a =
            {
                .x = 10,
                .y = 20,
                .width = 16,
                .height = 8,
            },
        .entity_b = ENTITY_SPACESHIP_BULLET,
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

    expect_function_call(__wrap_graphUnregisterDrawFunction);
    expect_function_call(__wrap_collisionUnregisterCollider);

    ufoOnCollision(&collision_data);

    assert_int_equal(10, death_event_data.x);
    assert_int_equal(20, death_event_data.y);
    assert_int_equal(16, death_event_data.width);
    assert_int_equal(8, death_event_data.height);
    assert_uint_equal(expected_points, death_event_data.points);
    assert_int_equal(ENTITY_UFO, death_event_data.entity_type);
}

void
testUfoOnCollisionEntityB(void **status)
{
    (void)status;

    ufo_t ufo = helperUT_ufoGetInstance();
    unsigned int expected_points = 300;
    helperUT_ufoSetPoints(expected_points);

    collision_event_t collision_data = {
        .entity_a = ENTITY_SPACESHIP_BULLET,
        .owner_a = NULL,
        .bounds_a =
            {
                .x = 100,
                .y = 200,
                .width = 32,
                .height = 16,
            },
        .entity_b = ENTITY_UFO,
        .owner_b = ufo,
        .bounds_b =
            {
                .x = 30,
                .y = 40,
                .width = 16,
                .height = 8,
            },
    };

    expect_uint_value(__wrap_eventEmit, type, EVENT_ENTITY_DIED);
    expect_function_call(__wrap_eventEmit);

    expect_function_call(__wrap_graphUnregisterDrawFunction);
    expect_function_call(__wrap_collisionUnregisterCollider);

    ufoOnCollision(&collision_data);

    assert_int_equal(30, death_event_data.x);
    assert_int_equal(40, death_event_data.y);
    assert_int_equal(16, death_event_data.width);
    assert_int_equal(8, death_event_data.height);
    assert_uint_equal(expected_points, death_event_data.points);
    assert_int_equal(ENTITY_UFO, death_event_data.entity_type);
}

void
testUfoOnCollisionNoUfo(void **status)
{
    (void)status;

    collision_event_t collision_data = {
        .entity_a = ENTITY_COUNT,
        .owner_a = NULL,
        .entity_b = ENTITY_COUNT,
        .owner_b = NULL,
    };

    ufoOnCollision(&collision_data);
}
