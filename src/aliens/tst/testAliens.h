/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2026 Manuel Hernández Méndez
 *
 * Authors:
 *   Manuel Hernández Méndez <maherme.dev@gmail.com>
 */

#ifndef __TESTALIENS_H__
#define __TESTALIENS_H__

#include "events.h"

extern entity_death_event_t captured_death_event;

int
setup(void **state);

void
testAliensCreate(void **status);

void
testAlienAliveNullParameter(void **status);

void
testAlienAliveFalse(void **status);

void
testAlienAliveTrue(void **status);

void
testAliensGetAlienInstanceInvalidIndex(void **status);

void
testAliensGetAlives(void **status);

void
testAliensGetShooterFormationEmpty(void **status);

void
testAliensGetShooterFormationOneAlien(void **status);

void
testAliensGetShooterFirstInColumn(void **status);

void
testAliensMoveRight(void **status);

void
testAliensMoveLeft(void **status);

void
testAliensMoveMaxLeft(void **status);

void
testAliensMoveMaxRight(void **status);

void
testAliensOnCollisionEntityAAlien(void **status);

void
testAliensOnCollisionEntityBAlien(void **status);

void
testAliensOnCollisionNoAlien(void **status);

#endif /* __TESTALIENS_H__ */
