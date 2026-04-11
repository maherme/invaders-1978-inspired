/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2025 Manuel Hernández Méndez
 *
 * Authors:
 *   Manuel Hernández Méndez <maherme.dev@gmail.com>
 */

#ifndef __TESTSPACESHIP_H__
#define __TESTSPACESHIP_H__

#include "events.h"

extern entity_death_event_t death_event_data;

void
testSpaceshipCreate(void **status);

void
testSpaceshipAliveNullParameter(void **status);

void
testSpaceshipAliveFalse(void **status);

void
testSpaceshipAliveTrue(void **status);

void
testSpaceshipOnCollisionEntityA(void **status);

void
testSpaceshipOnCollisionEntityB(void **status);

void
testSpaceshipOnCollisionNoSpaceship(void **status);

#endif
