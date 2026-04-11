/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2026 Manuel Hernández Méndez
 *
 * Authors:
 *   Manuel Hernández Méndez <maherme.dev@gmail.com>
 */

#ifndef __UFO_H__
#define __UFO_H__

#include "physic.h"
#include <stdbool.h>

typedef struct ufo_instance_t *ufo_t;

ufo_t
ufoCreate(void);

void
ufoDestroy(ufo_t ufo);

bool
ufoAlive(ufo_t ufo);

direction_t
ufoGetDirection(const ufo_t ufo);

bool
ufoCheckForMoving(ufo_t ufo);

unsigned int
ufoGetPoints(void);

void
ufoOnDangerLevelChanged(void *data);

void
ufoOnCollision(void *data);

#ifdef UNIT_TESTING
ufo_t
helperUT_ufoGetInstance(void);

void
helperUT_ufoSetAlive(ufo_t ufo, bool alive);

void
helperUT_ufoSetTimeToAppear(ufo_t ufo, long long time);

void
helperUT_ufoSetPoints(unsigned int points);

unsigned int
helperUT_ufoGetPoints(void);
#endif /* UNIT_TESTING */

#endif /* __UFO_H__ */
