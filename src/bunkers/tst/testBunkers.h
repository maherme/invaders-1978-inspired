/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2026 Manuel Hernández Méndez
 *
 * Authors:
 *   Manuel Hernández Méndez <maherme.dev@gmail.com>
 */

#ifndef __TESTBUNKERS_H__
#define __TESTBUNKERS_H__

/* Shall be 4 bunkers */
#define NUM_TEST_BUNKERS 4

int
setup(void **state);

void
testBunkersCreate(void **status);

void
testBunkersOnCollisionShipBulletEntityA(void **status);

void
testBunkersOnCollisionAlienBulletEntityA(void **status);

void
testBunkersOnCollisionShipBulletEntityB(void **status);

void
testBunkersOnCollisionAlienBulletEntityB(void **status);

void
testBunkersOnCollisionNoBunker(void **status);

void
testBunkersOnCollisionNullOwner(void **status);

void
testBunkersOnCollisionUnsupportedEntity(void **status);

#endif /* __TESTBUNKERS_H__ */
