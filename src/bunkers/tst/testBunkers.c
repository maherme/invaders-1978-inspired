/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2026 Manuel Hernández Méndez
 *
 * Authors:
 *   Manuel Hernández Méndez <maherme.dev@gmail.com>
 */

#include "testBunkers.h"
#include "bunkers.h"
#include "collision.h"
#include "mocksCollision.h"
#include "mocksExplosion.h"
#include "mocksGraph.h"
#include <cmocka.h>
#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>

int
setup(void **state)
{
    (void)state;
    helperUT_bunkersInitPool();
    return 0;
}

void
testBunkersCreate(void **status)
{
    (void)status;

    helperUT_resetCapturedBunkerOwners();

    for (size_t i = 0; i < NUM_TEST_BUNKERS; i++)
    {
        expect_function_call(__wrap_graphRegisterDrawFunction);
        expect_function_call(__wrap_collisionRegisterCollider);
    }

    bunkersCreate();

    for (size_t i = 0; i < NUM_TEST_BUNKERS; i++)
    {
        assert_non_null(helperUT_getCapturedBunkerOwner(i));
    }
}

static void
runBunkersOnCollisionBulletTest(entity_type_t bullet_type, bool bunker_is_entity_a)
{
    bunker_t bunker = helperUT_bunkersInjectInPool(0);
    sprite_t *bunker_sprite = helperUT_bunkersGetSprite(bunker);

    helperUT_explosionsSetStaticFrameSize(6, 8);

    collision_event_t collision = {
        .entity_a = bunker_is_entity_a ? ENTITY_BUNKER : bullet_type,
        .entity_b = bunker_is_entity_a ? bullet_type : ENTITY_BUNKER,
        .owner_a = bunker_is_entity_a ? bunker : NULL,
        .owner_b = bunker_is_entity_a ? NULL : bunker,
        .bounds_a =
            {
                .x = 40,
                .y = 60,
                .width = 4,
                .height = 8,
            },
        .bounds_b =
            {
                .x = 80,
                .y = 100,
                .width = 10,
                .height = 12,
            },
    };

    const collision_bounds_t *bullet_bounds = bunker_is_entity_a ? &collision.bounds_b : &collision.bounds_a;

    expect_uint_value(__wrap_explosionsGetStaticFrame, type, bullet_type);
    expect_int_value(__wrap_explosionsGetStaticFrame, num_frame, 0);
    expect_function_call(__wrap_explosionsGetStaticFrame);

    expect_uint_value(__wrap_graphMakeFootprintSprite, sprite, (uintptr_t)bunker_sprite);
    expect_any(__wrap_graphMakeFootprintSprite, mask);
    expect_function_call(__wrap_graphMakeFootprintSprite);

    expect_uint_value(__wrap_graphUpdateSprite, sprite, (uintptr_t)bunker_sprite);
    expect_function_call(__wrap_graphUpdateSprite);

    bunkersOnCollision(&collision);

    const sprite_t *mask = helperUT_getCapturedMask();

    /* mask width / 2 = 3 */
    assert_int_equal(mask->x, bullet_bounds->x - 3);

    int expected_y = bullet_bounds->y;

    if (bullet_type == ENTITY_ALIEN_BULLET)
    {
        expected_y -= bullet_bounds->height / 2;
    }

    assert_int_equal(mask->y, expected_y);
    assert_int_equal(mask->width, 6);
    assert_int_equal(mask->height, 8);

    assert_ptr_equal(helperUT_getCapturedFootprintSprite(), bunker_sprite);
    assert_ptr_equal(helperUT_getCapturedUpdatedSprite(), bunker_sprite);
}

void
testBunkersOnCollisionShipBulletEntityA(void **status)
{
    (void)status;
    runBunkersOnCollisionBulletTest(ENTITY_SPACESHIP_BULLET, true);
}

void
testBunkersOnCollisionAlienBulletEntityA(void **status)
{
    (void)status;
    runBunkersOnCollisionBulletTest(ENTITY_ALIEN_BULLET, true);
}

void
testBunkersOnCollisionShipBulletEntityB(void **status)
{
    (void)status;
    runBunkersOnCollisionBulletTest(ENTITY_SPACESHIP_BULLET, false);
}

void
testBunkersOnCollisionAlienBulletEntityB(void **status)
{
    (void)status;
    runBunkersOnCollisionBulletTest(ENTITY_ALIEN_BULLET, false);
}

void
testBunkersOnCollisionNoBunker(void **status)
{
    (void)status;

    collision_event_t collision = {
        .entity_a = ENTITY_ALIEN,
        .entity_b = ENTITY_SPACESHIP_BULLET,
        .owner_a = NULL,
        .owner_b = NULL,
    };

    bunkersOnCollision(&collision);
}

void
testBunkersOnCollisionNullOwner(void **status)
{
    (void)status;

    collision_event_t collision = {
        .entity_a = ENTITY_BUNKER,
        .entity_b = ENTITY_SPACESHIP_BULLET,
        .owner_a = NULL,
        .owner_b = NULL,
    };

    bunkersOnCollision(&collision);
}

void
testBunkersOnCollisionUnsupportedEntity(void **status)
{
    (void)status;

    bunker_t bunker = helperUT_bunkersInjectInPool(0);

    collision_event_t collision = {
        .entity_a = ENTITY_BUNKER,
        .entity_b = ENTITY_ALIEN,
        .owner_a = bunker,
        .owner_b = NULL,
    };

    bunkersOnCollision(&collision);
}
