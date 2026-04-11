/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2026 Manuel Hernández Méndez
 *
 * Authors:
 *   Manuel Hernández Méndez <maherme.dev@gmail.com>
 */

#include "testBullet.h"
#include "bullet.h"
#include "collision.h"
#include "entity_types.h"
#include <cmocka.h>
#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

int
setup(void **state)
{
    (void)state;
    helperUT_bulletInitPool();
    captured_death_count = 0;
    memset(captured_deaths, 0, sizeof(captured_deaths));
    return 0;
}

int
__wrap_rand(void)
{
    function_called();
    return (int)mock();
}

static entity_type_t bullets[] = {ENTITY_SPACESHIP_BULLET, ENTITY_ALIEN_BULLET};

void
testBulletCreate(void **status)
{
    (void)status;

    for (size_t i = 0; i < sizeof(bullets) / sizeof(bullets[0]); i++)
    {
        if (bullets[i] != ENTITY_SPACESHIP_BULLET)
        {
            will_return(__wrap_rand, 1);
            expect_function_call(__wrap_rand);
        }

        expect_function_call(__wrap_graphRegisterPrint);
        expect_function_call(__wrap_collisionRegisterCollider);

        bulletCreate(0, 0, bullets[i]);
    }
}

void
testBulletCreateAllTypeBulletAlien(void **status)
{
    (void)status;

    /* crackle */
    will_return(__wrap_rand, 0);
    expect_function_call(__wrap_rand);

    expect_function_call(__wrap_graphRegisterPrint);
    expect_function_call(__wrap_collisionRegisterCollider);

    bulletCreate(0, 0, ENTITY_ALIEN_BULLET);

    /* coil */
    will_return(__wrap_rand, 50);
    expect_function_call(__wrap_rand);

    expect_function_call(__wrap_graphRegisterPrint);
    expect_function_call(__wrap_collisionRegisterCollider);

    bulletCreate(0, 0, ENTITY_ALIEN_BULLET);

    /* plasma */
    will_return(__wrap_rand, 80);
    expect_function_call(__wrap_rand);

    expect_function_call(__wrap_graphRegisterPrint);
    expect_function_call(__wrap_collisionRegisterCollider);

    bulletCreate(0, 0, ENTITY_ALIEN_BULLET);
}

void
testBulletCreateTwoBulletSpaceship(void **status)
{
    (void)status;

    expect_function_call(__wrap_graphRegisterPrint);
    expect_function_call(__wrap_collisionRegisterCollider);

    bulletCreate(0, 0, ENTITY_SPACESHIP_BULLET);
    bulletCreate(0, 0, ENTITY_SPACESHIP_BULLET);
}

void
testBulletCreateInvalidEntity(void **status)
{
    (void)status;

    bulletCreate(0, 0, ENTITY_SPACESHIP);
}

void
testBulletDestroyNullParameter(void **status)
{
    (void)status;

    bulletDestroy(NULL);
}

void
testBulletDestroy(void **status)
{
    (void)status;
    bullet_t bullet = helperUT_bulletInjectInPool(0, ENTITY_SPACESHIP_BULLET);

    expect_function_call(__wrap_graphGetSprite);
    expect_function_call(__wrap_graphUnregisterPrint);
    expect_function_call(__wrap_collisionUnregisterCollider);

    bulletDestroy(bullet);
}

void
testBulletUsedNullParameter(void **status)
{
    (void)status;

    assert_false(bulletUsed(NULL));
}

void
testBulletUsedFalse(void **status)
{
    (void)status;
    bullet_t bullet = helperUT_bulletInjectInPool(0, ENTITY_SPACESHIP_BULLET);
    helperUT_bulletSetUsed(bullet, false);

    assert_false(bulletUsed(bullet));
}

void
testBulletUsedTrue(void **status)
{
    (void)status;
    bullet_t bullet = helperUT_bulletInjectInPool(0, ENTITY_SPACESHIP_BULLET);

    assert_true(bulletUsed(bullet));
}

void
testBulletGetTypeNullParameters(void **status)
{
    (void)status;
    bullet_t bullet = (bullet_t)0xdeadbeef;
    entity_type_t *type = (entity_type_t *)0xdeadbeef;

    assert_int_equal(-1, bulletGetType(NULL, NULL));
    assert_int_equal(-1, bulletGetType(bullet, NULL));
    assert_int_equal(-1, bulletGetType(NULL, type));
}

void
testBulletGetType(void **status)
{
    (void)status;
    bullet_t bullet = helperUT_bulletInjectInPool(0, ENTITY_ALIEN_BULLET);
    entity_type_t type;

    assert_int_equal(0, bulletGetType(bullet, &type));
    assert_int_equal(ENTITY_ALIEN_BULLET, type);
}

void
testBulletCallFunctionForEachNullParameter(void **status)
{
    (void)status;

    bulletCallFunctionForEach(NULL);
}

static void
foo(bullet_t bullet)
{
    (void)bullet;
    function_called();
}

void
testBulletCallFunctionForEachBulletPoolEmpty(void **status)
{
    (void)status;

    bulletCallFunctionForEach(foo);
}

void
testBulletCallFunctionForEach(void **status)
{
    (void)status;

    helperUT_bulletInjectInPool(0, ENTITY_SPACESHIP_BULLET);

    expect_function_call(foo);

    bulletCallFunctionForEach(foo);
}

void
testBulletAlienInhibit(void **status)
{
    (void)status;

    bulletAlienInhibit(true);
    bulletCreate(0, 0, ENTITY_ALIEN_BULLET);
    assert_true(bulletNoneUsed());
}

void
testBulletNoneUsedTrue(void **status)
{
    (void)status;

    assert_true(bulletNoneUsed());
}

void
testBulletNoneUsedFalse(void **status)
{
    (void)status;

    helperUT_bulletInjectInPool(0, ENTITY_SPACESHIP_BULLET);

    assert_false(bulletNoneUsed());
}

static bool
cmp_foo(const sprite_t *const sprite1, const sprite_t *const sprite2)
{
    (void)sprite1, (void)sprite2;
    function_called();
    return (bool)mock();
}

void
testBulletSpaceshipHitBulletAliensNoBullets(void **status)
{
    (void)status;

    assert_false(bulletSpaceshipHitBulletAliens(cmp_foo));
}

void
testBulletSpaceshipHitBulletAliensCmpFalse(void **status)
{
    (void)status;

    will_return(cmp_foo, false);
    expect_function_call(cmp_foo);

    helperUT_bulletInjectInPool(0, ENTITY_SPACESHIP_BULLET);
    helperUT_bulletInjectInPool(1, ENTITY_ALIEN_BULLET);

    assert_false(bulletSpaceshipHitBulletAliens(cmp_foo));
}

void
testBulletSpaceshipHitBulletAliensCmpTrue(void **status)
{
    (void)status;

    will_return(cmp_foo, true);
    expect_function_call(cmp_foo);

    /* Called bulletDestroy expected */
    expect_function_call(__wrap_graphGetSprite);
    expect_function_call(__wrap_graphUnregisterPrint);
    expect_function_call(__wrap_collisionUnregisterCollider);

    helperUT_bulletInjectInPool(0, ENTITY_SPACESHIP_BULLET);
    helperUT_bulletInjectInPool(1, ENTITY_ALIEN_BULLET);

    assert_true(bulletSpaceshipHitBulletAliens(cmp_foo));
}

void
testBulletOnCollisionNonBulletEntities(void **status)
{
    (void)status;

    collision_event_t collision = {
        .entity_a = ENTITY_ALIEN,
        .entity_b = ENTITY_BUNKER,
        .owner_a = NULL,
        .owner_b = NULL,
        .bounds_a = {.x = 10, .y = 20, .width = 8, .height = 8},
        .bounds_b = {.x = 30, .y = 40, .width = 10, .height = 5},
    };

    bulletOnCollision(&collision);

    assert_int_equal(captured_death_count, 0);
}

void
testBulletOnCollisionNullOwnerVsBunker(void **status)
{
    (void)status;

    collision_event_t collision = {
        .entity_a = ENTITY_SPACESHIP_BULLET,
        .entity_b = ENTITY_BUNKER,
        .owner_a = NULL,
        .owner_b = NULL,
        .bounds_a = {.x = 10, .y = 20, .width = 1, .height = 4},
        .bounds_b = {.x = 10, .y = 20, .width = 10, .height = 5},
    };

    bulletOnCollision(&collision);

    assert_int_equal(captured_death_count, 0);
}

static void
assertDeathEvent(const entity_death_event_t *actual, const entity_death_event_t *expected)
{
    assert_int_equal(actual->x, expected->x);
    assert_int_equal(actual->y, expected->y);
    assert_int_equal(actual->width, expected->width);
    assert_int_equal(actual->height, expected->height);
    assert_int_equal(actual->entity_type, expected->entity_type);
}

void
testBulletOnCollisionBulletVsBullet(void **status)
{
    (void)status;

    bullet_t spaceship_bullet = helperUT_bulletInjectInPool(0, ENTITY_SPACESHIP_BULLET);
    bullet_t alien_bullet = helperUT_bulletInjectInPool(1, ENTITY_ALIEN_BULLET);

    collision_event_t collision = {
        .entity_a = ENTITY_SPACESHIP_BULLET,
        .entity_b = ENTITY_ALIEN_BULLET,
        .owner_a = spaceship_bullet,
        .owner_b = alien_bullet,
        .bounds_a = {.x = 10, .y = 20, .width = 1, .height = 4},
        .bounds_b = {.x = 30, .y = 40, .width = 3, .height = 7},
    };

    /* Two bullets are destroyed. */
    expect_uint_value(__wrap_eventEmit, type, EVENT_ENTITY_DIED);
    expect_function_call(__wrap_eventEmit);
    expect_function_call(__wrap_graphGetSprite);
    expect_function_call(__wrap_graphUnregisterPrint);
    expect_function_call(__wrap_collisionUnregisterCollider);

    expect_uint_value(__wrap_eventEmit, type, EVENT_ENTITY_DIED);
    expect_function_call(__wrap_eventEmit);
    expect_function_call(__wrap_graphGetSprite);
    expect_function_call(__wrap_graphUnregisterPrint);
    expect_function_call(__wrap_collisionUnregisterCollider);

    bulletOnCollision(&collision);

    assert_false(bulletUsed(spaceship_bullet));
    assert_false(bulletUsed(alien_bullet));

    assert_int_equal(captured_death_count, 2);

    const entity_death_event_t expected_spaceship = {
        .x = 10,
        .y = 20,
        .width = 1,
        .height = 4,
        .entity_type = ENTITY_SPACESHIP_BULLET,
    };

    const entity_death_event_t expected_alien = {
        .x = 30,
        .y = 40,
        .width = 3,
        .height = 7,
        .entity_type = ENTITY_ALIEN_BULLET,
    };

    assertDeathEvent(&captured_deaths[0], &expected_spaceship);
    assertDeathEvent(&captured_deaths[1], &expected_alien);
}

void
testBulletOnCollisionSpaceshipBulletVsAlien(void **status)
{
    (void)status;

    bullet_t spaceship_bullet = helperUT_bulletInjectInPool(0, ENTITY_SPACESHIP_BULLET);

    collision_event_t collision = {
        .entity_a = ENTITY_SPACESHIP_BULLET,
        .entity_b = ENTITY_ALIEN,
        .owner_a = spaceship_bullet,
        .owner_b = NULL,
        .bounds_a = {.x = 10, .y = 20, .width = 1, .height = 4},
        .bounds_b = {.x = 50, .y = 60, .width = 8, .height = 8},
    };

    expect_function_call(__wrap_graphGetSprite);
    expect_function_call(__wrap_graphUnregisterPrint);
    expect_function_call(__wrap_collisionUnregisterCollider);

    bulletOnCollision(&collision);

    assert_false(bulletUsed(spaceship_bullet));
    assert_int_equal(captured_death_count, 0);
}

void
testBulletOnCollisionSpaceshipBulletVsBunker(void **status)
{
    (void)status;

    bullet_t spaceship_bullet = helperUT_bulletInjectInPool(0, ENTITY_SPACESHIP_BULLET);

    collision_event_t collision = {
        .entity_a = ENTITY_SPACESHIP_BULLET,
        .entity_b = ENTITY_BUNKER,
        .owner_a = spaceship_bullet,
        .owner_b = NULL,
        .bounds_a = {.x = 10, .y = 20, .width = 1, .height = 4},
        .bounds_b = {.x = 10, .y = 20, .width = 10, .height = 5},
    };

    expect_uint_value(__wrap_eventEmit, type, EVENT_ENTITY_DIED);
    expect_function_call(__wrap_eventEmit);

    expect_function_call(__wrap_graphGetSprite);
    expect_function_call(__wrap_graphUnregisterPrint);
    expect_function_call(__wrap_collisionUnregisterCollider);

    bulletOnCollision(&collision);

    assert_false(bulletUsed(spaceship_bullet));
    assert_int_equal(captured_death_count, 1);

    const entity_death_event_t expected = {
        .x = 10,
        .y = 20,
        .width = 1,
        .height = 4,
        .entity_type = ENTITY_SPACESHIP_BULLET,
    };

    assertDeathEvent(&captured_deaths[0], &expected);
}

void
testBulletOnCollisionBunkerVsSpaceshipBullet(void **status)
{
    (void)status;

    bullet_t spaceship_bullet = helperUT_bulletInjectInPool(0, ENTITY_SPACESHIP_BULLET);

    collision_event_t collision = {
        .entity_a = ENTITY_BUNKER,
        .entity_b = ENTITY_SPACESHIP_BULLET,
        .owner_a = NULL,
        .owner_b = spaceship_bullet,
        .bounds_a = {.x = 30, .y = 40, .width = 10, .height = 5},
        .bounds_b = {.x = 10, .y = 20, .width = 1, .height = 4},
    };

    expect_uint_value(__wrap_eventEmit, type, EVENT_ENTITY_DIED);
    expect_function_call(__wrap_eventEmit);
    expect_function_call(__wrap_graphGetSprite);
    expect_function_call(__wrap_graphUnregisterPrint);
    expect_function_call(__wrap_collisionUnregisterCollider);

    bulletOnCollision(&collision);

    assert_false(bulletUsed(spaceship_bullet));
    assert_int_equal(captured_death_count, 1);

    const entity_death_event_t expected = {
        .x = 10,
        .y = 20,
        .width = 1,
        .height = 4,
        .entity_type = ENTITY_SPACESHIP_BULLET,
    };

    assertDeathEvent(&captured_deaths[0], &expected);
}

void
testBulletOnCollisionAlienBulletVsAlien(void **status)
{
    (void)status;

    bullet_t alien_bullet = helperUT_bulletInjectInPool(1, ENTITY_ALIEN_BULLET);

    collision_event_t collision = {
        .entity_a = ENTITY_ALIEN_BULLET,
        .entity_b = ENTITY_ALIEN,
        .owner_a = alien_bullet,
        .owner_b = NULL,
        .bounds_a = {.x = 30, .y = 40, .width = 3, .height = 7},
        .bounds_b = {.x = 50, .y = 60, .width = 8, .height = 8},
    };

    expect_function_call(__wrap_graphGetSprite);
    expect_function_call(__wrap_graphUnregisterPrint);
    expect_function_call(__wrap_collisionUnregisterCollider);

    bulletOnCollision(&collision);

    assert_false(bulletUsed(alien_bullet));
    assert_int_equal(captured_death_count, 0);
}

void
testBulletOnCollisionAlienBulletVsSpaceshipBullet(void **status)
{
    (void)status;

    bullet_t alien_bullet = helperUT_bulletInjectInPool(0, ENTITY_ALIEN_BULLET);
    bullet_t spaceship_bullet = helperUT_bulletInjectInPool(1, ENTITY_SPACESHIP_BULLET);

    collision_event_t collision = {
        .entity_a = ENTITY_ALIEN_BULLET,
        .entity_b = ENTITY_SPACESHIP_BULLET,
        .owner_a = alien_bullet,
        .owner_b = spaceship_bullet,
        .bounds_a = {.x = 30, .y = 40, .width = 3, .height = 7},
        .bounds_b = {.x = 10, .y = 20, .width = 1, .height = 4},
    };

    /* Alien bullet is destroyed. */
    expect_uint_value(__wrap_eventEmit, type, EVENT_ENTITY_DIED);
    expect_function_call(__wrap_eventEmit);
    expect_function_call(__wrap_graphGetSprite);
    expect_function_call(__wrap_graphUnregisterPrint);
    expect_function_call(__wrap_collisionUnregisterCollider);

    /* Spaceship bullet is destroyed. */
    expect_uint_value(__wrap_eventEmit, type, EVENT_ENTITY_DIED);
    expect_function_call(__wrap_eventEmit);
    expect_function_call(__wrap_graphGetSprite);
    expect_function_call(__wrap_graphUnregisterPrint);
    expect_function_call(__wrap_collisionUnregisterCollider);

    bulletOnCollision(&collision);

    assert_false(bulletUsed(alien_bullet));
    assert_false(bulletUsed(spaceship_bullet));
    assert_int_equal(captured_death_count, 2);

    const entity_death_event_t expected_alien = {
        .x = 30,
        .y = 40,
        .width = 3,
        .height = 7,
        .entity_type = ENTITY_ALIEN_BULLET,
    };

    const entity_death_event_t expected_spaceship = {
        .x = 10,
        .y = 20,
        .width = 1,
        .height = 4,
        .entity_type = ENTITY_SPACESHIP_BULLET,
    };

    assertDeathEvent(&captured_deaths[0], &expected_alien);
    assertDeathEvent(&captured_deaths[1], &expected_spaceship);
}

void
testBulletOnCollisionAlienBulletVsBunker(void **status)
{
    (void)status;

    bullet_t alien_bullet = helperUT_bulletInjectInPool(1, ENTITY_ALIEN_BULLET);

    collision_event_t collision = {
        .entity_a = ENTITY_ALIEN_BULLET,
        .entity_b = ENTITY_BUNKER,
        .owner_a = alien_bullet,
        .owner_b = NULL,
        .bounds_a = {.x = 30, .y = 40, .width = 3, .height = 7},
        .bounds_b = {.x = 30, .y = 40, .width = 10, .height = 5},
    };

    expect_uint_value(__wrap_eventEmit, type, EVENT_ENTITY_DIED);
    expect_function_call(__wrap_eventEmit);
    expect_function_call(__wrap_graphGetSprite);
    expect_function_call(__wrap_graphUnregisterPrint);
    expect_function_call(__wrap_collisionUnregisterCollider);

    bulletOnCollision(&collision);

    assert_false(bulletUsed(alien_bullet));
    assert_int_equal(captured_death_count, 1);

    const entity_death_event_t expected = {
        .x = 30,
        .y = 37, /* 40 - 7 / 2 */
        .width = 3,
        .height = 7,
        .entity_type = ENTITY_ALIEN_BULLET,
    };

    assertDeathEvent(&captured_deaths[0], &expected);
}
