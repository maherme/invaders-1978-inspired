/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2026 Manuel Hernández Méndez
 *
 * Authors:
 *   Manuel Hernández Méndez <maherme.dev@gmail.com>
 */

#ifndef __BUNKERS__H__
#define __BUNKERS__H__

typedef struct bunker_instance_t *bunker_t;

void
bunkersCreate(void);

void
bunkersOnCollision(void *data);

#ifdef UNIT_TESTING
#include "graph.h"

void
helperUT_bunkersInitPool(void);

bunker_t
helperUT_bunkersInjectInPool(int index);

sprite_t *
helperUT_bunkersGetSprite(bunker_t bunker);
#endif /* UNIT_TESTING */

#endif /* __BUNKERS__H__ */
