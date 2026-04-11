/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2026 Manuel Hernández Méndez
 *
 * Authors:
 *   Manuel Hernández Méndez <maherme.dev@gmail.com>
 */

#ifndef __TESTUFO_H__
#define __TESTUFO_H__

#include "events.h"

extern entity_death_event_t death_event_data;

void
testUfoCreateLeftDirection(void **status);

void
testUfoCreateRightDirection(void **status);

void
testUfoDestroyNullParameter(void **status);

void
testUfoDestroy(void **status);

void
testUfoAliveNullParameter(void **status);

void
testUfoAliveFalse(void **status);

void
testUfoAliveTrue(void **status);

void
testUfoGetDirectionNullParameter(void **status);

void
testUfoCheckForMovingNullParameter(void **status);

void
testUfoCheckForMovingAliveFalse(void **status);

void
testUfoCheckForMovingTimeoutFalse(void **status);

void
testUfoCheckForMovingTrue(void **status);

void
testUfoCheckForMovingTrueMoreCalls(void **status);

void
testUfoGetPoints(void **status);

void
testUfoPointsByDangerLevel(void **status);

void
testUfoOnCollisionEntityA(void **status);

void
testUfoOnCollisionEntityB(void **status);

void
testUfoOnCollisionNoUfo(void **status);

#endif /* __TESTUFO_H__ */
