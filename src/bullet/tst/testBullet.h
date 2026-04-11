/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2026 Manuel Hernández Méndez
 *
 * Authors:
 *   Manuel Hernández Méndez <maherme.dev@gmail.com>
 */

#ifndef __TESTBULLET_H__
#define __TESTBULLET_H__

#include "events.h"
#include "mocksEvents.h"
#include <stddef.h>

extern entity_death_event_t captured_deaths[MAX_CAPTURED_DEATHS];
extern size_t captured_death_count;

int
setup(void **state);

void
testBulletCreate(void **status);

void
testBulletCreateAllTypeBulletAlien(void **status);

void
testBulletCreateTwoBulletSpaceship(void **status);

void
testBulletCreateInvalidEntity(void **status);

void
testBulletDestroyNullParameter(void **status);

void
testBulletDestroy(void **status);

void
testBulletUsedNullParameter(void **status);

void
testBulletUsedFalse(void **status);

void
testBulletUsedTrue(void **status);

void
testBulletGetTypeNullParameters(void **status);

void
testBulletGetType(void **status);

void
testBulletCallFunctionForEachNullParameter(void **status);

void
testBulletCallFunctionForEachBulletPoolEmpty(void **status);

void
testBulletCallFunctionForEach(void **status);

void
testBulletAlienInhibit(void **status);

void
testBulletNoneUsedTrue(void **status);

void
testBulletNoneUsedFalse(void **status);

void
testBulletSpaceshipHitBulletAliensNoBullets(void **status);

void
testBulletSpaceshipHitBulletAliensCmpFalse(void **status);

void
testBulletSpaceshipHitBulletAliensCmpTrue(void **status);

void
testBulletOnCollisionNonBulletEntities(void **status);

void
testBulletOnCollisionNullOwnerVsBunker(void **status);

void
testBulletOnCollisionBulletVsBullet(void **status);

void
testBulletOnCollisionSpaceshipBulletVsAlien(void **status);

void
testBulletOnCollisionSpaceshipBulletVsBunker(void **status);

void
testBulletOnCollisionBunkerVsSpaceshipBullet(void **status);

void
testBulletOnCollisionAlienBulletVsAlien(void **status);

void
testBulletOnCollisionAlienBulletVsSpaceshipBullet(void **status);

void
testBulletOnCollisionAlienBulletVsBunker(void **status);

#endif
