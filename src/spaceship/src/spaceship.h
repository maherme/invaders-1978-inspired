/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2025 Manuel Hernández Méndez
 *
 * Authors:
 *   Manuel Hernández Méndez <maherme.dev@gmail.com>
 */

#ifndef __SPACESHIP_H__
#define __SPACESHIP_H__

#include <stdbool.h>

typedef struct spaceship_instance_t *spaceship_t;

spaceship_t
spaceshipCreate(int x, int y, void (*death_callback)(void));

bool
spaceshipAlive(spaceship_t spaceship);

void
spaceshipOnCollision(void *data);

#ifdef UNIT_TESTING
spaceship_t
helperUT_spaceshipGetInstance(void);

void
helperUT_spaceshipSetAlive(spaceship_t spaceship, bool alive);
#endif /* UNIT_TESTING */

#endif
