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
#include "graph.h"
#include "physic.h"
#include <limits.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#define MAX_COLLIDERS 512

typedef struct
{
    collider_t collider;
    unsigned int generation;
    bool active;
    bool pending_remove;
} collider_slot_t;

static collider_slot_t collidersPool[MAX_COLLIDERS];
static unsigned int free_slots[MAX_COLLIDERS];
static int free_top = -1;

static const collision_handle_t COLLISION_INVALID_HANDLE = {
    .index = UINT_MAX,
    .generation = 0,
};

void
collisionInit(void)
{
    for (int i = 0; i < MAX_COLLIDERS; i++)
    {
        free_slots[i] = i;
        collidersPool[i].active = false;
        collidersPool[i].pending_remove = false;
        collidersPool[i].generation = 0;
        memset(&collidersPool[i].collider, 0, sizeof(collider_t));
    }

    free_top = MAX_COLLIDERS - 1;
}

collision_handle_t
collisionRegisterCollider(collider_t *collider)
{
    if (free_top < 0)
    {
        return COLLISION_INVALID_HANDLE;
    }

    unsigned int idx = free_slots[free_top--];
    collider_slot_t *slot = &collidersPool[idx];

    memcpy(&slot->collider, collider, sizeof(collider_t));
    slot->active = true;
    slot->pending_remove = false;

    collision_handle_t handle = {.index = idx, .generation = slot->generation};

    return handle;
}

int
collisionUnregisterCollider(collision_handle_t handle)
{
    collider_slot_t *slot = &collidersPool[handle.index];

    if (slot->generation != handle.generation || !slot->active)
    {
        return -1;
    }

    slot->pending_remove = true;
    return 0;
}

static bool
entitiesShouldCollide(entity_type_t a, entity_type_t b)
{
    static const bool matrix[ENTITY_COUNT][ENTITY_COUNT] = {
        /* clang-format off */
        /*                   SPACESHIP  ALIEN   S_BULLET  A_BULLET  BUNKER  UFO */
        /* SPACESHIP   */  { false,     false,  false,    true,     false,  false },
        /* ALIEN       */  { false,     false,  true,     false,    false,  false },
        /* S_BULLET    */  { false,     true,   false,    true,     true,   true  },
        /* A_BULLET    */  { true,      false,  true,     false,    true,   false },
        /* BUNKER      */  { false,     false,  true,     true,     false,  false },
        /* UFO         */  { false,     false,  true,     false,    false,  false }
        /* clang-format on */
    };
    return matrix[a][b];
}

static bool
checkBoxCollision(const collider_t *c1, const collider_t *c2)
{
    if (*c1->x + c1->width < *c2->x)
    {
        return false;
    }
    if (*c1->x > *c2->x + c2->width)
    {
        return false;
    }
    if (*c1->y + c1->height < *c2->y)
    {
        return false;
    }
    if (*c1->y > *c2->y + c2->height)
    {
        return false;
    }

    return true;
}

static bool
checkPixelCollision(const collider_t *c1, const collider_t *c2)
{
    int overlap_left = (*c1->x > *c2->x) ? *c1->x : *c2->x;
    int overlap_right = (*c1->x + c1->width < *c2->x + c2->width) ? *c1->x + c1->width : *c2->x + c2->width;
    int overlap_bottom = (*c1->y > *c2->y) ? *c1->y : *c2->y;
    int overlap_top = (*c1->y + c1->height < *c2->y + c2->height) ? *c1->y + c1->height : *c2->y + c2->height;

    const uint8_t *img1 = (const uint8_t *)*c1->pixel_data_ref;
    const uint8_t *img2 = (const uint8_t *)*c2->pixel_data_ref;

    if (!img1 || !img2)
    {
        return false;
    }

    for (int y = overlap_bottom; y < overlap_top; y++)
    {
        int y1 = (c1->height - 1) - (y - *c1->y);
        int y2 = (c2->height - 1) - (y - *c2->y);

        for (int x = overlap_left; x < overlap_right; x++)
        {
            int x1 = x - *c1->x;
            int x2 = x - *c2->x;

            int idx1 = (y1 * c1->width + x1) * NUM_RGBA_CHANNELS;
            int idx2 = (y2 * c2->width + x2) * NUM_RGBA_CHANNELS;

            if (img1[idx1 + 3] && img2[idx2 + 3])
            {
                return true;
            }
        }
    }

    return false;
}

void
collisionUpdate(void)
{
    for (unsigned int i = 0; i < MAX_COLLIDERS; i++)
    {
        collider_slot_t *slot_a = &collidersPool[i];

        if (!slot_a->active || slot_a->pending_remove)
        {
            continue;
        }

        for (unsigned int j = i + 1; j < MAX_COLLIDERS; j++)
        {
            collider_slot_t *slot_b = &collidersPool[j];

            if (!slot_b->active || slot_b->pending_remove)
            {
                continue;
            }

            if (!entitiesShouldCollide(slot_a->collider.entity, slot_b->collider.entity))
            {
                continue;
            }

            if (checkBoxCollision(&slot_a->collider, &slot_b->collider))
            {
                if (checkPixelCollision(&slot_a->collider, &slot_b->collider))
                {
                    collision_event_t collision_data = {
                        .bounds_a = {*slot_a->collider.x, *slot_a->collider.y, slot_a->collider.width, slot_a->collider.height},
                        .bounds_b = {*slot_b->collider.x, *slot_b->collider.y, slot_b->collider.width, slot_b->collider.height},
                        .entity_a = slot_a->collider.entity,
                        .entity_b = slot_b->collider.entity,
                        .owner_a = slot_a->collider.owner,
                        .owner_b = slot_b->collider.owner,
                    };
                    eventEmit(EVENT_COLLISION, &collision_data);
                }
            }
        }
    }

    for (unsigned int i = 0; i < MAX_COLLIDERS; i++)
    {
        if (collidersPool[i].active && collidersPool[i].pending_remove)
        {
            collidersPool[i].active = false;
            collidersPool[i].pending_remove = false;
            collidersPool[i].generation++;
            free_slots[++free_top] = i;
        }
    }
}

bool
collisionCheckBorder(const sprite_t *const sprite, direction_t direction)
{
    if (!sprite)
        return false;

    sprite_coordinates_t coord;
    graphGetSpriteCoordinates(sprite, &coord);

    switch (direction)
    {
        case UP:
            return coord.y2 >= sprite->max_movement.up;
            break;
        case DOWN:
            return coord.y1 <= sprite->max_movement.down;
            break;
        case RIGHT:
            return coord.x2 >= sprite->max_movement.right;
            break;
        case LEFT:
            return coord.x1 <= sprite->max_movement.left;
            break;
        case INVALID_DIR:
        default:
            break;
    }

    return false;
}

#ifdef UNIT_TESTING
void
helperUT_clearCollisionState(void)
{
    memset(collidersPool, 0, sizeof(collidersPool));
    memset(free_slots, 0, sizeof(free_slots));
    free_top = -1;
}

unsigned int
helperUT_getMaxColliders(void)
{
    return MAX_COLLIDERS;
}

const collider_t *
helperUT_getCollider(unsigned int index)
{
    return &collidersPool[index].collider;
}
void
helperUT_setCollider(
    unsigned int index, int *x, int *y, int width, int height, const char **pixel_data_ref, entity_type_t entity, void *owner)
{
    collidersPool[index].collider.x = x;
    collidersPool[index].collider.y = y;
    collidersPool[index].collider.width = width;
    collidersPool[index].collider.height = height;
    collidersPool[index].collider.pixel_data_ref = pixel_data_ref;
    collidersPool[index].collider.entity = entity;
    collidersPool[index].collider.owner = owner;
}

bool
helperUT_isColliderActive(unsigned int index)
{
    return collidersPool[index].active;
}

bool
helperUT_isColliderPendingRemove(unsigned int index)
{
    return collidersPool[index].pending_remove;
}

unsigned int
helperUT_getColliderGeneration(unsigned int index)
{
    return collidersPool[index].generation;
}

unsigned int
helperUT_getFreeSlot(unsigned int index)
{
    return free_slots[index];
}

void
helperUT_setFreeSlot(unsigned int index, unsigned int value)
{
    free_slots[index] = value;
}

int
helperUT_getFreeTop(void)
{
    return free_top;
}

void
helperUT_setFreeTop(int value)
{
    free_top = value;
}

void
helperUT_setColliderState(unsigned int index, bool active, bool pending_remove, unsigned int generation)
{
    collidersPool[index].active = active;
    collidersPool[index].pending_remove = pending_remove;
    collidersPool[index].generation = generation;
}
#endif /* UNIT_TESTING */
