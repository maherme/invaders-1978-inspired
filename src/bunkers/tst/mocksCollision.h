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

void
helperUT_resetCapturedBunkerOwners(void);

void *
helperUT_getCapturedBunkerOwner(size_t index);

#endif /* __MOCKSCOLLISION_H__ */
