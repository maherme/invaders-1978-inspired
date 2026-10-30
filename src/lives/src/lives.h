/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2026 Manuel Hernández Méndez
 *
 * Authors:
 *   Manuel Hernández Méndez <maherme.dev@gmail.com>
 */

#ifndef __LIVES_H__
#define __LIVES_H__

void
livesCreate(int x, int y);

void
livesRemove(void);

void
livesAdd(void);

#ifdef UNIT_TESTING
#include "graphGlutCallbacks.h"

int
helperUT_livesGetNumLives(void);

void
helperUT_livesSetNumLives(int n);

struct sprite;
typedef struct sprite sprite_t;

sprite_t *
helperUT_livesGetSpriteByInstanceIndex(int idx);

graph_draw_id_t
helperUT_livesGetGraphDrawIdByInstanceIndex(int idx);
#endif /* UNIT_TESTING */

#endif /* __LIVES_H__ */
