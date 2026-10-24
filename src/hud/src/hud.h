/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2026 Manuel Hernández Méndez
 *
 * Authors:
 *   Manuel Hernández Méndez <maherme.dev@gmail.com>
 */

#ifndef __HUD_H__
#define __HUD_H__

void
hudInit(void);

void
onLivesChanged(void *data);

void
onScoreChanged(void *data);

#endif /* __HUD_H__ */
