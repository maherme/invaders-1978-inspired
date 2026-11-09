/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2026 Manuel Hernández Méndez
 *
 * Authors:
 *   Manuel Hernández Méndez <maherme.dev@gmail.com>
 */

#ifndef __MOCKSGRAPHGLUTCALLBACKS_H__
#define __MOCKSGRAPHGLUTCALLBACKS_H__

#include <stddef.h>

void
mocksGraphGlutCallbacksResetExpectedDrawCalls(void);

size_t
mocksGraphGlutCallbacksGetExpectedDrawCalls(void);

#endif /* __MOCKSGRAPHGLUTCALLBACKS_H__ */
