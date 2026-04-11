/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2026 Manuel Hernández Méndez
 *
 * Authors:
 *   Manuel Hernández Méndez <maherme.dev@gmail.com>
 */

#ifndef __TESTEXPLOSION_H__
#define __TESTEXPLOSION_H__

int
teardown(void **state);

void
testExplosionCreate(void **status);

void
testExplosionsDestroyListEmpty(void **status);

void
testExplosionsDestroyTimeoutFalse(void **status);

void
testExplosionsDestroyTimeoutTrue(void **status);

void
testExplosionsDestroyTimeoutCallback(void **status);

void
testExplosionsDestroyTwoExplosions(void **status);

void
testExplosionDestroyMoreOneFrame(void **status);

void
testExplosionDestroyMoreOneFrameTimeoutFalse(void **status);

void
testExplosionAllFinishedTrue(void **status);

void
testExplosionAllFinishedFalse(void **status);

void
testExplosionsGetExplosionHeight(void **status);

void
testExplosionOnEntityDiedOnFinishAction(void **status);

void
testExplosionOnEntityDiedOnFinishNull(void **status);

void
testExplosionOnEntityDiedNothingToDo(void **status);

void
testExplosionsGetStaticFrameEntityReturnNull(void **status);

void
testExplosionGetStaticFrameSpaceshipBullet(void **status);

void
testExplosionGetStaticFrameAlienBullet(void **status);

#endif
