/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2026 Manuel Hernández Méndez
 *
 * Authors:
 *   Manuel Hernández Méndez <maherme.dev@gmail.com>
 */

#include "testAliens.h"
#include "aliens.h"
#include "collision.h"
#include "events.h"
#include "graph.h"
#include "mocksCollision.h"
#include "mocksGraphGlutCallbacks.h"
#include "physic.h"
#include <cmocka.h>
#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>

int
setup(void **state)
{
    (void)state;
    helperUT_alienInitPool();
    return 0;
}

int
__wrap_rand(void)
{
    function_called();
    return (int)mock();
}

#define EXPECTED_OCTOPUS_WIDTH 12
#define EXPECTED_OCTOPUS_HEIGHT 8
#define EXPECTED_CRAB_WIDTH 11
#define EXPECTED_CRAB_HEIGHT 8
#define EXPECTED_SQUID_WIDTH 8
#define EXPECTED_SQUID_HEIGHT 8

static int
check_collision_handle_struct(CMockaValueData value, CMockaValueData check_data)
{
    collision_handle_t *expected = (collision_handle_t *)check_data.ptr;
    collision_handle_t *actual = (collision_handle_t *)value.ptr;

    return (actual->generation == expected->generation && actual->index == expected->index);
}

static collision_handle_t expected_collision_handle;

static void
checkCollisionUnregisterCollider(collision_handle_t handle)
{
    expected_collision_handle = handle;

    expect_check_data(__wrap_collisionUnregisterCollider,
                      handle,
                      check_collision_handle_struct,
                      cast_ptr_to_cmocka_value(&expected_collision_handle));

    will_return(__wrap_collisionUnregisterCollider, 0);
    expect_function_call(__wrap_collisionUnregisterCollider);
}

void
testAliensCreate(void **status)
{
    (void)status;

    static const int expected_widths[ALIENS_ROWS] = {
        EXPECTED_OCTOPUS_WIDTH,
        EXPECTED_OCTOPUS_WIDTH,
        EXPECTED_CRAB_WIDTH,
        EXPECTED_CRAB_WIDTH,
        EXPECTED_SQUID_WIDTH,
    };

    static const int expected_heights[ALIENS_ROWS] = {
        EXPECTED_OCTOPUS_HEIGHT,
        EXPECTED_OCTOPUS_HEIGHT,
        EXPECTED_CRAB_HEIGHT,
        EXPECTED_CRAB_HEIGHT,
        EXPECTED_SQUID_HEIGHT,
    };

    mocksGraphGlutCallbacksResetExpectedDrawCalls();
    mocksCollisionResetExpectedColliderCalls();
    mocksCollisionResetExpectedAlienIndex();

    collision_handle_t fake_collision_handle = {0};

    for (int i = 0; i < ALIENS_INITIAL_NUMBER; i++)
    {
        alien_t alien = helperUT_aliensGetAlienInstance(i);
        assert_non_null(alien);

        expect_function_call(__wrap_graphUpdateTimeSprite);
        expect_function_call(__wrap_graphRegisterDrawFunction);

        will_return(__wrap_collisionRegisterCollider, &fake_collision_handle);
        expect_function_call(__wrap_collisionRegisterCollider);
    }

    aliensCreate();

    assert_int_equal(mocksGraphGlutCallbacksGetExpectedDrawCalls(), ALIENS_INITIAL_NUMBER);
    assert_int_equal(mocksCollisionGetExpectedColliderCalls(), ALIENS_INITIAL_NUMBER);

    for (int i = 0; i < ALIENS_INITIAL_NUMBER; i++)
    {
        int row = i / ALIENS_COLS;
        alien_t alien = helperUT_aliensGetAlienInstance(i);
        sprite_t *sprite = helperUT_alienGetSprite(alien);

        assert_non_null(alien);
        assert_int_equal(sprite->width, expected_widths[row]);
        assert_int_equal(sprite->height, expected_heights[row]);
    }
}

void
testAlienAliveNullParameter(void **status)
{
    (void)status;

    assert_false(alienAlive(NULL));
}

void
testAlienAliveFalse(void **status)
{
    (void)status;
    alien_t alien = helperUT_alienInjectInPool(0, 0, NULL);
    helperUT_alienSetAlive(alien, false);

    assert_false(alienAlive(alien));
}

void
testAlienAliveTrue(void **status)
{
    (void)status;
    alien_t alien = helperUT_alienInjectInPool(0, 0, NULL);

    assert_true(alienAlive(alien));
}

void
testAliensGetAlienInstanceInvalidIndex(void **status)
{
    (void)status;

    assert_null(aliensGetAlienInstance(ALIENS_INITIAL_NUMBER + 1));
}

void
testAliensGetAlienInstanceValidIndex(void **status)
{
    (void)status;

    alien_t expected_alien = helperUT_alienInjectInPool(0, 0, NULL);
    alien_t alien = aliensGetAlienInstance(0);

    assert_ptr_equal(expected_alien, alien);
}

void
testAliensGetAlives(void **status)
{
    (void)status;
    int expected_num_aliens = 0;

    for (int col = 0; col < ALIENS_COLS; col++)
    {
        for (int row = 0; row < ALIENS_ROWS; row++)
        {
            helperUT_alienInjectInPool(row, col, NULL);
            expected_num_aliens++;
            assert_uint_equal(expected_num_aliens, aliensGetAlives());
        }
    }
}

void
testAliensGetShooterFormationEmpty(void **status)
{
    (void)status;

    will_return(__wrap_rand, 1);
    expect_function_call(__wrap_rand);

    assert_null(aliensGetShooter());
}

void
testAliensGetShooterFormationOneAlien(void **status)
{
    (void)status;

    alien_t alien = helperUT_alienInjectInPool(0, 0, NULL);

    will_return(__wrap_rand, 1);
    expect_function_call(__wrap_rand);

    assert_ptr_equal(alien, aliensGetShooter());
}

void
testAliensGetShooterFirstInColumn(void **status)
{
    (void)status;
    alien_t injected[ALIENS_ROWS];

    for (int i = 0; i < ALIENS_ROWS; i++)
    {
        injected[i] = helperUT_alienInjectInPool(i, 0, NULL);
    }

    alien_t expected = injected[0];

    will_return(__wrap_rand, 1);
    expect_function_call(__wrap_rand);
    assert_ptr_equal(expected, aliensGetShooter());
}

void
testAliensMoveRight(void **status)
{
    (void)status;
    sprite_t sprite_alien1 = {
        .x = 64,
        .y = 100,
    };

    sprite_t sprite_alien2 = {
        .x = 32,
        .y = 132,
    };

    helperUT_alienSetCurrentDirection(RIGHT);
    helperUT_alienInjectInPool(0, 1, &sprite_alien1);
    helperUT_alienInjectInPool(1, 0, &sprite_alien2);

    /* Each alien is moved */
    expect_uint_value(__wrap_eventEmit, type, EVENT_ALIENS_HEIGHT);
    expect_function_call(__wrap_eventEmit);
    expect_uint_value(__wrap_physicMoveSprite, direction, RIGHT);
    expect_function_call(__wrap_physicMoveSprite);
    expect_uint_value(__wrap_physicMoveSprite, direction, RIGHT);
    expect_function_call(__wrap_physicMoveSprite);

    aliensMove();
}

void
testAliensMoveLeft(void **status)
{
    (void)status;
    sprite_t sprite = {
        .x = 100,
        .y = 100,
    };

    helperUT_alienSetCurrentDirection(LEFT);
    helperUT_alienInjectInPool(0, 0, &sprite);

    expect_uint_value(__wrap_eventEmit, type, EVENT_ALIENS_HEIGHT);
    expect_function_call(__wrap_eventEmit);
    expect_uint_value(__wrap_physicMoveSprite, direction, LEFT);
    expect_function_call(__wrap_physicMoveSprite);

    aliensMove();
}

void
testAliensMoveMaxLeft(void **status)
{
    (void)status;
    sprite_t sprite = {
        .x = 0,
        .y = 100,
    };

    helperUT_alienSetCurrentDirection(LEFT);
    helperUT_alienInjectInPool(0, 0, &sprite);

    expect_uint_value(__wrap_eventEmit, type, EVENT_ALIENS_HEIGHT);
    expect_function_call(__wrap_eventEmit);
    expect_uint_value(__wrap_physicMoveSprite, direction, DOWN);
    expect_function_call(__wrap_physicMoveSprite);

    aliensMove();
}

void
testAliensMoveMaxRight(void **status)
{
    (void)status;
    sprite_t sprite = {
        .x = GAME_WIDTH,
        .y = 100,
    };

    helperUT_alienSetCurrentDirection(RIGHT);
    helperUT_alienInjectInPool(0, 0, &sprite);

    expect_uint_value(__wrap_eventEmit, type, EVENT_ALIENS_HEIGHT);
    expect_function_call(__wrap_eventEmit);
    expect_uint_value(__wrap_physicMoveSprite, direction, DOWN);
    expect_function_call(__wrap_physicMoveSprite);

    aliensMove();
}

void
testAliensOnCollisionEntityAAlien(void **status)
{
    (void)status;

    alien_t alien = helperUT_alienInjectInPool(0, 0, NULL);
    helperUT_aliensSetCollisionHandle(alien, expected_collision_handle);
    graph_draw_id_t expected_draw_id = 3;
    helperUT_aliensSetDrawId(alien, expected_draw_id);
    helperUT_aliensSetType(alien, SQUID);

    collision_event_t collision = {
        .entity_a = ENTITY_ALIEN,
        .entity_b = ENTITY_SPACESHIP_BULLET,
        .owner_a = alien,
        .bounds_a =
            {
                .x = 10,
                .y = 20,
                .width = 8,
                .height = 8,
            },
    };

    entity_death_event_t expected_death = {
        .x = 10,
        .y = 20,
        .width = 8,
        .height = 8,
        .points = 30,
        .entity_type = ENTITY_ALIEN,
    };

    expect_uint_value(__wrap_eventEmit, type, EVENT_ENTITY_DIED);
    expect_function_call(__wrap_eventEmit);

    will_return(__wrap_graphUnregisterDrawFunction, 0);
    expect_uint_value(__wrap_graphUnregisterDrawFunction, id, expected_draw_id);
    expect_function_call(__wrap_graphUnregisterDrawFunction);
    checkCollisionUnregisterCollider(expected_collision_handle);

    aliensOnCollision(&collision);

    assert_memory_equal(&captured_death_event, &expected_death, sizeof(expected_death));

    assert_false(helperUT_alienGetAlive(alien));
    assert_uint_equal(0, helperUT_aliensGetAlives());
}

void
testAliensOnCollisionEntityBAlien(void **status)
{
    (void)status;

    alien_t alien = helperUT_alienInjectInPool(0, 0, NULL);
    helperUT_aliensSetCollisionHandle(alien, expected_collision_handle);
    graph_draw_id_t expected_draw_id = 3;
    helperUT_aliensSetDrawId(alien, expected_draw_id);
    helperUT_aliensSetType(alien, CRAB);

    collision_event_t collision = {
        .entity_a = ENTITY_SPACESHIP_BULLET,
        .entity_b = ENTITY_ALIEN,
        .owner_b = alien,
        .bounds_b =
            {
                .x = 40,
                .y = 50,
                .width = 11,
                .height = 8,
            },
    };

    entity_death_event_t expected_death = {
        .x = 40,
        .y = 50,
        .width = 11,
        .height = 8,
        .points = 20,
        .entity_type = ENTITY_ALIEN,
    };

    expect_uint_value(__wrap_eventEmit, type, EVENT_ENTITY_DIED);
    expect_function_call(__wrap_eventEmit);

    will_return(__wrap_graphUnregisterDrawFunction, 0);
    expect_uint_value(__wrap_graphUnregisterDrawFunction, id, expected_draw_id);
    expect_function_call(__wrap_graphUnregisterDrawFunction);
    checkCollisionUnregisterCollider(expected_collision_handle);

    aliensOnCollision(&collision);

    assert_memory_equal(&captured_death_event, &expected_death, sizeof(expected_death));

    assert_false(helperUT_alienGetAlive(alien));
    assert_uint_equal(0, helperUT_aliensGetAlives());
}

void
testAliensOnCollisionNoAlien(void **status)
{
    (void)status;

    collision_event_t collision = {
        .entity_a = ENTITY_COUNT,
        .entity_b = ENTITY_COUNT,
    };

    aliensOnCollision(&collision);
}
