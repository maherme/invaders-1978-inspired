/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2026 Manuel Hernández Méndez
 *
 * Authors:
 *   Manuel Hernández Méndez <maherme.dev@gmail.com>
 */

#ifndef __BULLET_H__
#define __BULLET_H__

#include "entity_types.h"
#include <stdbool.h>

struct sprite;
typedef struct sprite sprite_t;

typedef struct bullet_instance *bullet_t;

void
bulletCreate(int x, int y, entity_type_t type);

void
bulletDestroy(bullet_t bullet);

bool
bulletUsed(bullet_t bullet);

int
bulletGetType(const bullet_t bullet, entity_type_t *type);

void
bulletCallFunctionForEach(void (*fn)(bullet_t));

void
bulletAlienInhibit(bool inhibit);

bool
bulletNoneUsed(void);

bool
bulletSpaceshipHitBulletAliens(bool (*fn)(const sprite_t *const sprite1, const sprite_t *const sprite2));

void
bulletOnCollision(void *data);

#ifdef UNIT_TESTING
#include <string.h>

void
helperUT_bulletInitPool(void);

void
helperUT_bulletSetUsed(bullet_t bullet, bool used);

bullet_t
helperUT_bulletInjectInPool(int index, entity_type_t type);
#endif /* UNIT_TESTING */

#endif
