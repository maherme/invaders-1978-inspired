/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2026 Manuel Hernández Méndez
 *
 * Authors:
 *   Manuel Hernández Méndez <maherme.dev@gmail.com>
 */

#ifndef __MOCKSCOLLISION_H__
#define __MOCKSCOLLISION_H__

#include <stddef.h>

size_t
mocksCollisionGetExpectedAlienIndex(void);

void
mocksCollisionResetExpectedAlienIndex(void);

size_t
mocksCollisionGetExpectedColliderCalls(void);

void
mocksCollisionResetExpectedColliderCalls(void);

#endif /* __MOCKSCOLLISION_H__ */
