/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2026 Manuel Hernández Méndez
 *
 * Authors:
 *   Manuel Hernández Méndez <maherme.dev@gmail.com>
 */

#include "testCollision.h"
#include "collision.h"
#include "entity_types.h"
#include "events.h"
#include "graph.h"
#include <cmocka.h>
#include <limits.h>
#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

int
teardown(void **state)
{
    (void)state;
    helperUT_clearCollisionState();
    memset(&last_collision_event, 0, sizeof(last_collision_event));
    return 0;
}

static void
checkGraphGetSpriteCoordinates(sprite_t *sprite, int x1, int x2, int y1, int y2)
{
    expect_uint_value(__wrap_graphGetSpriteCoordinates, sprite, (uintptr_t)sprite);
    will_return(__wrap_graphGetSpriteCoordinates, x1); // x1
    will_return(__wrap_graphGetSpriteCoordinates, x2); // x2
    will_return(__wrap_graphGetSpriteCoordinates, y1); // y1
    will_return(__wrap_graphGetSpriteCoordinates, y2); // y2
    expect_function_call(__wrap_graphGetSpriteCoordinates);
}

void
testCollisionInit(void **status)
{
    (void)status;

    collisionInit();

    unsigned int max_colliders = helperUT_getMaxColliders();
    assert_int_equal(helperUT_getFreeTop(), (int)max_colliders - 1);

    for (unsigned int i = 0; i < max_colliders; i++)
    {
        const collider_t *collider = helperUT_getCollider(i);

        assert_uint_equal(helperUT_getFreeSlot(i), i);

        assert_false(helperUT_isColliderActive(i));
        assert_false(helperUT_isColliderPendingRemove(i));
        assert_uint_equal(helperUT_getColliderGeneration(i), 0);

        assert_null(collider->x);
        assert_null(collider->y);
        assert_int_equal(collider->width, 0);
        assert_int_equal(collider->height, 0);
        assert_null(collider->pixel_data_ref);
        assert_int_equal(collider->entity, 0);
        assert_null(collider->owner);
    }
}

void
testCollisionRegisterCollider(void **status)
{
    (void)status;

    unsigned int max_colliders = helperUT_getMaxColliders();

    helperUT_setFreeTop((int)max_colliders - 1);
    helperUT_setFreeSlot(max_colliders - 1, 0);

    int x = 100;
    int y = 200;

    collider_t collider = {
        .x = &x,
        .y = &y,
        .width = 32,
        .height = 16,
        .pixel_data_ref = NULL,
        .entity = ENTITY_SPACESHIP,
        .owner = NULL,
    };

    collision_handle_t handle = collisionRegisterCollider(&collider);

    assert_uint_equal(handle.index, 0);
    assert_uint_equal(handle.generation, 0);

    assert_uint_equal(helperUT_getFreeTop(), helperUT_getMaxColliders() - 2);

    assert_true(helperUT_isColliderActive(handle.index));
    assert_false(helperUT_isColliderPendingRemove(handle.index));

    const collider_t *registered = helperUT_getCollider(handle.index);

    assert_ptr_equal(registered->x, &x);
    assert_ptr_equal(registered->y, &y);
    assert_int_equal(registered->width, 32);
    assert_int_equal(registered->height, 16);
    assert_null(registered->pixel_data_ref);
    assert_int_equal(registered->entity, ENTITY_SPACESHIP);
    assert_null(registered->owner);
}

void
testCollisionRegisterColliderPoolFull(void **status)
{
    (void)status;

    helperUT_setFreeTop(-1);

    collider_t collider = {0};

    collision_handle_t handle = collisionRegisterCollider(&collider);

    assert_uint_equal(handle.index, UINT_MAX);
    assert_uint_equal(handle.generation, 0);
    assert_int_equal(helperUT_getFreeTop(), -1);
}

void
testCollisionUnregisterColliderValid(void **status)
{
    (void)status;
    const unsigned int index = 0;
    const unsigned int generation = 0;

    helperUT_setColliderState(index, true, false, generation);

    collision_handle_t handle = {.index = index, .generation = generation};

    assert_int_equal(collisionUnregisterCollider(handle), 0);
    assert_true(helperUT_isColliderActive(index));
    assert_true(helperUT_isColliderPendingRemove(index));
}

void
testCollisionUnregisterColliderNonExisting(void **status)
{
    (void)status;
    collision_handle_t collision_handle = {.index = 1, .generation = 0};

    int result = collisionUnregisterCollider(collision_handle);

    assert_int_equal(result, -1);
}

void
testCollisionUnregisterColliderWrongGeneration(void **status)
{
    (void)status;
    const unsigned int index = 0;
    collision_handle_t handle = {.index = index, .generation = 0};

    helperUT_setColliderState(index, true, false, 1);

    assert_int_equal(collisionUnregisterCollider(handle), -1);
}

void
testCollisionUpdateNoActiveColliders(void **status)
{
    (void)status;

    collisionUpdate();
}

void
testCollisionUpdatePendingRemoveCollider(void **status)
{
    (void)status;

    const unsigned int index = 0;

    helperUT_setColliderState(index, true, true, 0);

    collisionUpdate();

    assert_false(helperUT_isColliderActive(index));
    assert_false(helperUT_isColliderPendingRemove(index));
    assert_uint_equal(helperUT_getColliderGeneration(index), 1);
    assert_int_equal(helperUT_getFreeTop(), 0);
    assert_uint_equal(helperUT_getFreeSlot(0), index);
}

void
testCollisionUpdateInactiveSlotB(void **status)
{
    (void)status;

    int x_a = 0;
    int y_a = 0;

    helperUT_setColliderState(0, true, false, 0);
    helperUT_setCollider(0, &x_a, &y_a, 10, 10, NULL, ENTITY_SPACESHIP, NULL);

    helperUT_setColliderState(1, false, false, 0);

    collisionUpdate();
}

void
testCollisionUpdateActiveSlotB(void **status)
{
    (void)status;

    int x_a = 0;
    int y_a = 0;

    helperUT_setColliderState(0, true, false, 0);
    helperUT_setCollider(0, &x_a, &y_a, 10, 10, NULL, ENTITY_SPACESHIP, NULL);

    helperUT_setColliderState(1, true, true, 0);

    collisionUpdate();
}

void
testCollisionUpdateEntitiesShouldNotCollide(void **status)
{
    (void)status;

    int x_a = 0;
    int y_a = 0;
    int x_b = 0;
    int y_b = 0;

    helperUT_setColliderState(0, true, false, 0);
    helperUT_setCollider(0, &x_a, &y_a, 10, 10, NULL, ENTITY_SPACESHIP, NULL);

    helperUT_setColliderState(1, true, false, 0);
    helperUT_setCollider(1, &x_b, &y_b, 10, 10, NULL, ENTITY_ALIEN, NULL);

    collisionUpdate();
}

void
testCollisionUpdateNoBoxCollision(void **status)
{
    (void)status;

    int x_a = 0;
    int y_a = 0;
    int x_b = 20;
    int y_b = 0;

    helperUT_setColliderState(0, true, false, 0);
    helperUT_setCollider(0, &x_a, &y_a, 10, 10, NULL, ENTITY_SPACESHIP, NULL);

    helperUT_setColliderState(1, true, false, 0);
    helperUT_setCollider(1, &x_b, &y_b, 10, 10, NULL, ENTITY_ALIEN_BULLET, NULL);

    collisionUpdate();
}

void
testCollisionUpdateNoBoxCollisionLeft(void **status)
{
    (void)status;

    int x_a = 20;
    int y_a = 0;
    int x_b = 0;
    int y_b = 0;

    helperUT_setColliderState(0, true, false, 0);
    helperUT_setCollider(0, &x_a, &y_a, 10, 10, NULL, ENTITY_SPACESHIP, NULL);

    helperUT_setColliderState(1, true, false, 0);
    helperUT_setCollider(1, &x_b, &y_b, 10, 10, NULL, ENTITY_ALIEN_BULLET, NULL);

    collisionUpdate();
}

void
testCollisionUpdateNoBoxCollisionBelow(void **status)
{
    (void)status;

    int x_a = 0;
    int y_a = 0;
    int x_b = 0;
    int y_b = 20;

    helperUT_setColliderState(0, true, false, 0);
    helperUT_setCollider(0, &x_a, &y_a, 10, 10, NULL, ENTITY_SPACESHIP, NULL);

    helperUT_setColliderState(1, true, false, 0);
    helperUT_setCollider(1, &x_b, &y_b, 10, 10, NULL, ENTITY_ALIEN_BULLET, NULL);

    collisionUpdate();
}

void
testCollisionUpdateNoBoxCollisionAbove(void **status)
{
    (void)status;

    int x_a = 0;
    int y_a = 20;
    int x_b = 0;
    int y_b = 0;

    helperUT_setColliderState(0, true, false, 0);
    helperUT_setCollider(0, &x_a, &y_a, 10, 10, NULL, ENTITY_SPACESHIP, NULL);

    helperUT_setColliderState(1, true, false, 0);
    helperUT_setCollider(1, &x_b, &y_b, 10, 10, NULL, ENTITY_ALIEN_BULLET, NULL);

    collisionUpdate();
}

void
testCollisionUpdateNoPixelCollisionNullImage(void **status)
{
    (void)status;

    int x_a = 0;
    int y_a = 0;
    int x_b = 0;
    int y_b = 0;
    const char *image_a = NULL;
    const char *image_b = NULL;

    helperUT_setColliderState(0, true, false, 0);
    helperUT_setCollider(0, &x_a, &y_a, 10, 10, &image_a, ENTITY_SPACESHIP, NULL);

    helperUT_setColliderState(1, true, false, 0);
    helperUT_setCollider(1, &x_b, &y_b, 10, 10, &image_b, ENTITY_ALIEN_BULLET, NULL);

    collisionUpdate();
}

void
testCollisionUpdateNoPixelCollisionSecondImageNull(void **status)
{
    (void)status;

    int x_a = 0;
    int y_a = 0;
    int x_b = 0;
    int y_b = 0;

    static const uint8_t pixel_a[] = {255, 255, 255, 255};
    const char *image_a = (const char *)pixel_a;
    const char *image_b = NULL;

    helperUT_setColliderState(0, true, false, 0);
    helperUT_setCollider(0, &x_a, &y_a, 1, 1, &image_a, ENTITY_SPACESHIP, NULL);

    helperUT_setColliderState(1, true, false, 0);
    helperUT_setCollider(1, &x_b, &y_b, 1, 1, &image_b, ENTITY_ALIEN_BULLET, NULL);

    collisionUpdate();
}

void
testCollisionUpdateNoPixelCollisionTransparent(void **status)
{
    (void)status;

    int x_a = 0;
    int y_a = 0;
    int x_b = 0;
    int y_b = 0;

    static const uint8_t transparent_pixel[] = {255, 255, 255, 0};

    const char *image_a = (const char *)transparent_pixel;
    const char *image_b = (const char *)transparent_pixel;

    helperUT_setColliderState(0, true, false, 0);
    helperUT_setCollider(0, &x_a, &y_a, 1, 1, &image_a, ENTITY_SPACESHIP, NULL);

    helperUT_setColliderState(1, true, false, 0);
    helperUT_setCollider(1, &x_b, &y_b, 1, 1, &image_b, ENTITY_ALIEN_BULLET, NULL);

    collisionUpdate();
}

void
testCollisionUpdatePixelCollisionFirstOpaqueSecondTransparent(void **status)
{
    (void)status;

    int x_a = 0;
    int y_a = 0;
    int x_b = 0;
    int y_b = 0;

    static const uint8_t opaque_pixel[] = {255, 255, 255, 255};

    static const uint8_t transparent_pixel[] = {255, 255, 255, 0};

    const char *image_a = (const char *)opaque_pixel;
    const char *image_b = (const char *)transparent_pixel;

    helperUT_setColliderState(0, true, false, 0);
    helperUT_setCollider(0, &x_a, &y_a, 1, 1, &image_a, ENTITY_SPACESHIP, NULL);

    helperUT_setColliderState(1, true, false, 0);
    helperUT_setCollider(1, &x_b, &y_b, 1, 1, &image_b, ENTITY_ALIEN_BULLET, NULL);

    collisionUpdate();
}

void
testCollisionUpdatePixelCollision(void **status)
{
    (void)status;

    int x_a = 10;
    int y_a = 20;
    int x_b = 10;
    int y_b = 20;

    static const uint8_t opaque_pixel[] = {255, 255, 255, 255};

    const char *image_a = (const char *)opaque_pixel;
    const char *image_b = (const char *)opaque_pixel;

    helperUT_setColliderState(0, true, false, 0);
    helperUT_setCollider(0, &x_a, &y_a, 1, 1, &image_a, ENTITY_SPACESHIP, (void *)0x1234);

    helperUT_setColliderState(1, true, false, 0);
    helperUT_setCollider(1, &x_b, &y_b, 1, 1, &image_b, ENTITY_ALIEN_BULLET, (void *)0x5678);

    expect_uint_value(__wrap_eventEmit, type, EVENT_COLLISION);
    expect_function_call(__wrap_eventEmit);

    collisionUpdate();

    assert_int_equal(last_collision_event.bounds_a.x, 10);
    assert_int_equal(last_collision_event.bounds_a.y, 20);
    assert_int_equal(last_collision_event.bounds_a.width, 1);
    assert_int_equal(last_collision_event.bounds_a.height, 1);

    assert_int_equal(last_collision_event.bounds_b.x, 10);
    assert_int_equal(last_collision_event.bounds_b.y, 20);
    assert_int_equal(last_collision_event.bounds_b.width, 1);
    assert_int_equal(last_collision_event.bounds_b.height, 1);

    assert_int_equal(last_collision_event.entity_a, ENTITY_SPACESHIP);
    assert_int_equal(last_collision_event.entity_b, ENTITY_ALIEN_BULLET);

    assert_ptr_equal(last_collision_event.owner_a, (void *)0x1234);
    assert_ptr_equal(last_collision_event.owner_b, (void *)0x5678);
}

void
testCollisionCheckBorderNullParameter(void **status)
{
    (void)status;

    assert_false(collisionCheckBorder(NULL, RIGHT));
}

void
testCollisionCheckBorderUpTrue(void **status)
{
    (void)status;
    sprite_t sprite = {
        .max_movement.up = 10,
    };

    checkGraphGetSpriteCoordinates(&sprite, 0, 0, 0, sprite.max_movement.up);
    assert_true(collisionCheckBorder(&sprite, UP));
}

void
testCollisionCheckBorderDownTrue(void **status)
{
    (void)status;
    sprite_t sprite = {
        .max_movement.down = 10,
    };

    checkGraphGetSpriteCoordinates(&sprite, 0, 0, sprite.max_movement.down, 0);
    assert_true(collisionCheckBorder(&sprite, DOWN));
}

void
testCollisionCheckBorderRightTrue(void **status)
{
    (void)status;
    sprite_t sprite = {
        .max_movement.right = 10,
    };

    checkGraphGetSpriteCoordinates(&sprite, 0, sprite.max_movement.right, 0, 0);
    assert_true(collisionCheckBorder(&sprite, RIGHT));
}

void
testCollisionCheckBorderLeftTrue(void **status)
{
    (void)status;
    sprite_t sprite = {
        .max_movement.left = 10,
    };

    checkGraphGetSpriteCoordinates(&sprite, sprite.max_movement.left, 0, 0, 0);
    assert_true(collisionCheckBorder(&sprite, LEFT));
}

void
testCollisionCheckBorderInvalidDir(void **status)
{
    (void)status;
    sprite_t sprite = {
        .max_movement.left = 10,
    };

    checkGraphGetSpriteCoordinates(&sprite, 0, 0, 0, 0);
    assert_false(collisionCheckBorder(&sprite, INVALID_DIR));
}
