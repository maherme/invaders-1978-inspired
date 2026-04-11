/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2026 Manuel Hernández Méndez
 *
 * Authors:
 *   Manuel Hernández Méndez <maherme.dev@gmail.com>
 */

#ifndef __TESTPHYSIC_H__
#define __TESTPHYSIC_H__

void
testPhysicMoveSpriteFail(void **status);

void
testPhysicMoveSpriteWrongDirection(void **status);

void
testPhysicMoveSpriteRight(void **status);

void
testPhysicMoveSpriteRightMax(void **status);

void
testPhysicMoveSpriteLeft(void **status);

void
testPhysicMoveSpriteLeftMax(void **status);

void
testPhysicMoveSpriteUp(void **status);

void
testPhysicMoveSpriteUpMax(void **status);

void
testPhysicMoveSpriteDown(void **status);

void
testPhysicMoveSpriteDownMax(void **status);

void
testPhysicMoveSpriteTooEarlyToMove(void **status);

#endif /* __TESTPHYSIC_H__ */
