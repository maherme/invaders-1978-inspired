/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2026 Manuel Hernández Méndez
 *
 * Authors:
 *   Manuel Hernández Méndez <maherme.dev@gmail.com>
 */

#ifndef __ALIENS_H__
#define __ALIENS_H__

#include <stdbool.h>

#define ALIENS_ROWS 5
#define ALIENS_COLS 11
#define ALIENS_INITIAL_NUMBER (ALIENS_ROWS * ALIENS_COLS)
#define ALIENS_BASE_MOVE_DELAY 500
#define ALIENS_MIN_MOVE_DELAY 60
#define ALIENS_SHOOT_DELAY 500

typedef enum
{
    SQUID,
    CRAB,
    OCTOPUS
} alien_type_t;

typedef struct alien_instance *alien_t;

void
aliensCreate(void);

bool
alienAlive(alien_t alien);

unsigned int
aliensGetAlives(void);

alien_t
aliensGetAlienInstance(int alien_index);

alien_t
aliensGetShooter(void);

void
aliensMove(void);

void
aliensOnCollision(void *data);

#ifdef UNIT_TESTING
#include "physic.h"

void
helperUT_alienInitPool(void);

void
helperUT_alienSetAlive(alien_t alien, bool alive);

alien_t
helperUT_alienInjectInPool(int row, int col, sprite_t *sprite);

void
helperUT_alienSetCurrentDirection(direction_t dir);

void
helperUT_aliensSetType(alien_t alien, alien_type_t type);

bool
helperUT_alienGetAlive(alien_t alien);

unsigned int
helperUT_aliensGetAlives(void);

#endif /* UNIT_TESTING */

#endif /* __ALIENS_H__ */
