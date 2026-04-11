/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2026 Manuel Hernández Méndez
 *
 * Authors:
 *   Manuel Hernández Méndez <maherme.dev@gmail.com>
 */

#ifndef __TESTCOLLISION_H__
#define __TESTCOLLISION_H__

#include "collision.h"

extern collision_event_t last_collision_event;

int
teardown(void **state);

void
testCollisionInit(void **status);

void
testCollisionRegisterCollider(void **status);

void
testCollisionRegisterColliderPoolFull(void **status);

void
testCollisionUnregisterColliderValid(void **status);

void
testCollisionUnregisterColliderNonExisting(void **status);

void
testCollisionUnregisterColliderWrongGeneration(void **status);

void
testCollisionUpdateNoActiveColliders(void **status);

void
testCollisionUpdatePendingRemoveCollider(void **status);

void
testCollisionUpdateInactiveSlotB(void **status);

void
testCollisionUpdateActiveSlotB(void **status);

void
testCollisionUpdateEntitiesShouldNotCollide(void **status);

void
testCollisionUpdateNoBoxCollision(void **status);

void
testCollisionUpdateNoBoxCollisionLeft(void **status);

void
testCollisionUpdateNoBoxCollisionBelow(void **status);

void
testCollisionUpdateNoBoxCollisionAbove(void **status);

void
testCollisionUpdateNoPixelCollisionNullImage(void **status);

void
testCollisionUpdateNoPixelCollisionSecondImageNull(void **status);

void
testCollisionUpdateNoPixelCollisionTransparent(void **status);

void
testCollisionUpdatePixelCollisionFirstOpaqueSecondTransparent(void **status);

void
testCollisionUpdatePixelCollision(void **status);

void
testCollisionCheckBorderNullParameter(void **status);

void
testCollisionCheckBorderUpTrue(void **status);

void
testCollisionCheckBorderDownTrue(void **status);

void
testCollisionCheckBorderRightTrue(void **status);

void
testCollisionCheckBorderLeftTrue(void **status);

void
testCollisionCheckBorderInvalidDir(void **status);

#endif /* __TESTCOLLISION_H__ */
