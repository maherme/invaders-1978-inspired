/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2026 Manuel Hernández Méndez
 *
 * Authors:
 *   Manuel Hernández Méndez <maherme.dev@gmail.com>
 */

#ifndef __COLLISION_H__
#define __COLLISION_H__

#include "entity_types.h"
#include "physic.h"
#include <stdbool.h>

typedef struct
{
    unsigned int index;
    unsigned int generation;
} collision_handle_t;

typedef struct
{
    int *x, *y;
    int width, height;
    const char **pixel_data_ref;
    entity_type_t entity;
    void *owner;
} collider_t;

typedef struct
{
    int x, y;
    int width, height;
} collision_bounds_t;

typedef struct
{
    collision_bounds_t bounds_a, bounds_b;
    entity_type_t entity_a, entity_b;
    void *owner_a, *owner_b;
} collision_event_t;

void
collisionInit(void);

collision_handle_t
collisionRegisterCollider(collider_t *collider);

int
collisionUnregisterCollider(collision_handle_t handle);

void
collisionUpdate(void);

bool
collisionCheckBorder(const sprite_t *const sprite, direction_t direction);

#ifdef UNIT_TESTING
void
helperUT_clearCollisionState(void);

unsigned int
helperUT_getMaxColliders(void);

const collider_t *
helperUT_getCollider(unsigned int index);

void
helperUT_setCollider(
    unsigned int index, int *x, int *y, int width, int height, const char **pixel_data_ref, entity_type_t entity, void *owner);

bool
helperUT_isColliderActive(unsigned int index);

bool
helperUT_isColliderPendingRemove(unsigned int index);

unsigned int
helperUT_getColliderGeneration(unsigned int index);

unsigned int
helperUT_getFreeSlot(unsigned int index);

void
helperUT_setFreeSlot(unsigned int index, unsigned int value);

int
helperUT_getFreeTop(void);

void
helperUT_setFreeTop(int value);

void
helperUT_setColliderState(unsigned int index, bool active, bool pending_remove, unsigned int generation);
#endif /* UNIT_TESTING */

#endif /* __COLLISION_H__ */
