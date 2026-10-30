/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2026 Manuel Hernández Méndez
 *
 * Authors:
 *   Manuel Hernández Méndez <maherme.dev@gmail.com>
 */

#include "testExplosion.h"
#include "entity_types.h"
#include "events.h"
#include "explosion.h"
#include "graph.h"
#include <cmocka.h>
#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <time.h>

int
teardown(void **state)
{
    (void)state;
    helperUT_explosionResetList();
    return 0;
}

int
__wrap_clock_gettime(clockid_t clockid, struct timespec *tp)
{
    assert_int_equal(clockid, CLOCK_MONOTONIC);
    assert_non_null(tp);
    tp->tv_sec = (time_t)mock_type(time_t);
    tp->tv_nsec = (long)mock_type(long);
    function_called();
    return 0;
}

static void
registerExplosionBullet(void (*callback)(void))
{
    expect_function_call(__wrap_utilsCalloc);
    expect_function_call(__wrap_utilsCalloc);
    will_return(__wrap_clock_gettime, 0); /* tv_sec */
    will_return(__wrap_clock_gettime, 0); /* tv_nsec */
    expect_function_call(__wrap_clock_gettime);
    expect_function_call(__wrap_graphRegisterDrawFunction);

    explosionCreate(0, 0, ENTITY_SPACESHIP_BULLET, callback);
}

static void
registerExplosionSpaceship(void (*callback)(void))
{
    expect_function_call(__wrap_utilsCalloc);
    expect_function_call(__wrap_utilsCalloc);
    will_return(__wrap_clock_gettime, 0); /* tv_sec */
    will_return(__wrap_clock_gettime, 0); /* tv_nsec */
    expect_function_call(__wrap_clock_gettime);
    will_return(__wrap_clock_gettime, 0); /* tv_sec */
    will_return(__wrap_clock_gettime, 0); /* tv_nsec */
    expect_function_call(__wrap_clock_gettime);
    expect_function_call(__wrap_graphRegisterDrawFunction);

    explosionCreate(0, 0, ENTITY_SPACESHIP, callback);
}

static entity_type_t explosions[] = {ENTITY_SPACESHIP_BULLET, ENTITY_ALIEN_BULLET, ENTITY_SPACESHIP, ENTITY_UFO, ENTITY_ALIEN};

static void
checkExplosionCreate(entity_type_t type)
{
    expect_function_call(__wrap_utilsCalloc);
    expect_function_call(__wrap_utilsCalloc);
    if (type == ENTITY_SPACESHIP)
    {
        will_return(__wrap_clock_gettime, 0); /* tv_sec */
        will_return(__wrap_clock_gettime, 0); /* tv_nsec */
        expect_function_call(__wrap_clock_gettime);
    }
    will_return(__wrap_clock_gettime, 0); /* tv_sec */
    will_return(__wrap_clock_gettime, 0); /* tv_nsec */
    expect_function_call(__wrap_clock_gettime);
    expect_function_call(__wrap_graphRegisterDrawFunction);
}

void
testExplosionCreate(void **status)
{
    (void)status;

    for (size_t i = 0; i < sizeof(explosions) / sizeof(explosions[0]); i++)
    {
        checkExplosionCreate(explosions[i]);
        explosionCreate(0, 0, explosions[i], NULL);
    }
}

void
testExplosionsDestroyListEmpty(void **status)
{
    (void)status;

    explosionsDestroy();
}

void
testExplosionsDestroyTimeoutFalse(void **status)
{
    (void)status;
    registerExplosionBullet(NULL);

    will_return(__wrap_utilsCheckTimeout, false);
    expect_function_call(__wrap_utilsCheckTimeout);
    explosionsDestroy();
}

void
testExplosionsDestroyTimeoutTrue(void **status)
{
    (void)status;
    registerExplosionBullet(NULL);

    will_return(__wrap_utilsCheckTimeout, true);
    expect_function_call(__wrap_utilsCheckTimeout);
    expect_function_call(__wrap_graphUnregisterDrawFunction);

    explosionsDestroy();
}

static void
foo(void)
{
    function_called();
}

void
testExplosionsDestroyTimeoutCallback(void **status)
{
    (void)status;
    registerExplosionBullet(foo);

    will_return(__wrap_utilsCheckTimeout, true);
    expect_function_call(__wrap_utilsCheckTimeout);
    expect_function_call(__wrap_graphUnregisterDrawFunction);
    expect_function_call(foo);

    explosionsDestroy();
}

void
testExplosionsDestroyTwoExplosions(void **status)
{
    (void)status;
    int number_explosions = 2;

    for (int i = 0; i < number_explosions; i++)
    {
        registerExplosionBullet(NULL);
    }

    /* first registered explosion */
    will_return(__wrap_utilsCheckTimeout, false);
    expect_function_call(__wrap_utilsCheckTimeout);

    /* second registered explosion */
    will_return(__wrap_utilsCheckTimeout, true);
    expect_function_call(__wrap_utilsCheckTimeout);
    expect_function_call(__wrap_graphUnregisterDrawFunction);

    explosionsDestroy();
}

void
testExplosionDestroyMoreOneFrame(void **status)
{
    (void)status;

    /* Only EXPLOSION_SPACESHIP has more than one frame */
    registerExplosionSpaceship(NULL);

    will_return(__wrap_utilsCheckTimeout, true);
    expect_function_call(__wrap_utilsCheckTimeout);
    expect_function_call(__wrap_graphNextImageToPrint);
    will_return(__wrap_clock_gettime, 0); /* tv_sec */
    will_return(__wrap_clock_gettime, 0); /* tv_nsec */
    expect_function_call(__wrap_clock_gettime);
    will_return(__wrap_utilsCheckTimeout, false);
    expect_function_call(__wrap_utilsCheckTimeout);

    explosionsDestroy();
}

void
testExplosionDestroyMoreOneFrameTimeoutFalse(void **status)
{
    (void)status;

    /* Only EXPLOSION_SPACESHIP has more than one frame */
    registerExplosionSpaceship(NULL);

    will_return(__wrap_utilsCheckTimeout, false);
    expect_function_call(__wrap_utilsCheckTimeout);
    will_return(__wrap_utilsCheckTimeout, false);
    expect_function_call(__wrap_utilsCheckTimeout);

    explosionsDestroy();
}

void
testExplosionAllFinishedTrue(void **status)
{
    (void)status;

    assert_true(explosionsAllFinished());
}

void
testExplosionAllFinishedFalse(void **status)
{
    (void)status;

    registerExplosionBullet(NULL);
    assert_false(explosionsAllFinished());
}

static const unsigned int expected_explosion_heights[] = {
    [ENTITY_SPACESHIP_BULLET] = 8,
    [ENTITY_ALIEN_BULLET] = 8,
    [ENTITY_SPACESHIP] = 8,
    [ENTITY_UFO] = 8,
    [ENTITY_ALIEN] = 8,
};

static const unsigned int expected_explosion_widths[] = {
    [ENTITY_SPACESHIP_BULLET] = 8,
    [ENTITY_ALIEN_BULLET] = 6,
    [ENTITY_SPACESHIP] = 16,
    [ENTITY_UFO] = 24,
    [ENTITY_ALIEN] = 14,
};

void
testExplosionsGetExplosionHeight(void **status)
{
    (void)status;

    for (size_t i = 0; i < sizeof(explosions) / sizeof(explosions[0]); i++)
    {
        assert_uint_equal(expected_explosion_heights[explosions[i]], explosionsGetExplosionHeight(explosions[i]));
    }
}

static void
foo_callback(void)
{
    /* do nothing, dummy callback */
}

static entity_type_t explosions_on_finish_action[] = {ENTITY_ALIEN, ENTITY_UFO, ENTITY_SPACESHIP};

void
testExplosionOnEntityDiedOnFinishAction(void **status)
{
    (void)status;

    for (size_t i = 0; i < sizeof(explosions_on_finish_action) / sizeof(explosions_on_finish_action[0]); i++)
    {
        entity_death_event_t death_data = {.x = 10,
                                           .y = 10,
                                           .width = 20,
                                           .entity_type = explosions_on_finish_action[i],
                                           .on_finish = foo_callback};

        checkExplosionCreate(death_data.entity_type);

        explosionsOnEntityDied(&death_data);

        explosion_t explosion = helperUT_getExplosionFromPoolByType(explosions_on_finish_action[i]);
        sprite_t explosion_sprite = helperUT_getSpriteFromExplosion(explosion);

        assert_uint_equal(death_data.entity_type, helperUT_getEntityTypeFromExplosion(explosion));
        assert_int_equal(death_data.x + death_data.width / 2 - expected_explosion_widths[explosions_on_finish_action[i]] / 2,
                         explosion_sprite.x);
        assert_int_equal(death_data.y, explosion_sprite.y);
        assert_ptr_equal(death_data.on_finish, helperUT_getCallbackFromExplosion(explosion));
    }
}

static entity_type_t explosions_on_finish_null[] = {ENTITY_SPACESHIP_BULLET, ENTITY_ALIEN_BULLET};

void
testExplosionOnEntityDiedOnFinishNull(void **status)
{
    (void)status;

    for (size_t i = 0; i < sizeof(explosions_on_finish_null) / sizeof(explosions_on_finish_null[0]); i++)
    {
        entity_death_event_t death_data = {.x = 10, .y = 10, .entity_type = explosions_on_finish_null[i], .on_finish = NULL};

        checkExplosionCreate(death_data.entity_type);

        explosionsOnEntityDied(&death_data);

        explosion_t explosion = helperUT_getExplosionFromPoolByType(explosions_on_finish_null[i]);
        sprite_t explosion_sprite = helperUT_getSpriteFromExplosion(explosion);

        assert_uint_equal(death_data.entity_type, helperUT_getEntityTypeFromExplosion(explosion));
        assert_int_equal(death_data.x - expected_explosion_widths[explosions_on_finish_null[i]] / 2, explosion_sprite.x);
        assert_int_equal(death_data.y, explosion_sprite.y);
        assert_ptr_equal(death_data.on_finish, helperUT_getCallbackFromExplosion(explosion));
    }
}

void
testExplosionOnEntityDiedNothingToDo(void **status)
{
    (void)status;

    entity_death_event_t death_data = {.entity_type = ENTITY_BUNKER};
    explosionsOnEntityDied(&death_data);

    death_data.entity_type = ENTITY_COUNT;
    explosionsOnEntityDied(&death_data);
}

void
testExplosionsGetStaticFrameEntityReturnNull(void **status)
{
    (void)status;

    sprite_t *returned_sprite = explosionsGetStaticFrame(ENTITY_ALIEN, 0);
    assert_null(returned_sprite);

    returned_sprite = explosionsGetStaticFrame(ENTITY_ALIEN, 0);
    assert_null(returned_sprite);

    returned_sprite = explosionsGetStaticFrame(ENTITY_UFO, 0);
    assert_null(returned_sprite);

    returned_sprite = explosionsGetStaticFrame(ENTITY_BUNKER, 0);
    assert_null(returned_sprite);

    returned_sprite = explosionsGetStaticFrame(ENTITY_SPACESHIP, 0);
    assert_null(returned_sprite);

    returned_sprite = explosionsGetStaticFrame(ENTITY_COUNT, 0);
    assert_null(returned_sprite);
}

void
testExplosionGetStaticFrameSpaceshipBullet(void **status)
{
    (void)status;

    sprite_t *returned_sprite = explosionsGetStaticFrame(ENTITY_SPACESHIP_BULLET, 0);
    assert_int_equal(returned_sprite->width, helperUT_expectedExplosionWidth(ENTITY_SPACESHIP_BULLET));
    assert_int_equal(returned_sprite->height, helperUT_expectedExplosionHeight(ENTITY_SPACESHIP_BULLET));
    assert_ptr_equal(returned_sprite->image, helperUT_expectedExplosionImage(ENTITY_SPACESHIP_BULLET));
}

void
testExplosionGetStaticFrameAlienBullet(void **status)
{
    (void)status;

    sprite_t *returned_sprite = explosionsGetStaticFrame(ENTITY_ALIEN_BULLET, 0);
    assert_int_equal(returned_sprite->width, helperUT_expectedExplosionWidth(ENTITY_ALIEN_BULLET));
    assert_int_equal(returned_sprite->height, helperUT_expectedExplosionHeight(ENTITY_ALIEN_BULLET));
    assert_ptr_equal(returned_sprite->image, helperUT_expectedExplosionImage(ENTITY_ALIEN_BULLET));
}
