/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2026 Manuel Hernández Méndez
 *
 * Authors:
 *   Manuel Hernández Méndez <maherme.dev@gmail.com>
 */

#ifndef __EXPLOSION_H__
#define __EXPLOSION_H__

#include "entity_types.h"
#include <stdbool.h>

struct sprite;
typedef struct sprite sprite_t;

typedef struct explosion_instance *explosion_t;

explosion_t
explosionCreate(int x, int y, entity_type_t type, void (*callback)(void));

void
explosionsDestroy(void);

bool
explosionsAllFinished(void);

unsigned int
explosionsGetExplosionHeight(entity_type_t type);

void
explosionsOnEntityDied(void *data);

sprite_t *
explosionsGetStaticFrame(entity_type_t type, int num_frame);

#ifdef UNIT_TESTING
void
helperUT_explosionResetList(void);

explosion_t
helperUT_getExplosionFromPoolByType(entity_type_t type);

sprite_t
helperUT_getSpriteFromExplosion(explosion_t explosion);

typedef void (*callback_t)(void);

callback_t
helperUT_getCallbackFromExplosion(explosion_t explosion);

entity_type_t
helperUT_getEntityTypeFromExplosion(explosion_t explosion);

int
helperUT_expectedExplosionWidth(entity_type_t type);

int
helperUT_expectedExplosionHeight(entity_type_t type);

const char *
helperUT_expectedExplosionImage(entity_type_t type);
#endif /* UNIT_TESTING */

#endif /* __EXPLOSION_H__ */
