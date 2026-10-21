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

#ifdef UNIT_TESTING
struct fonts;
typedef struct fonts fonts_handler_t;

fonts_handler_t *
helperUT_hudGetLivesCounterPointer(void);

fonts_handler_t *
helperUT_hudGetScoreLettersPointer(void);

fonts_handler_t *
helperUT_hudGetScoreCounterPointer(void);
#endif /* UNIT_TESTING */

#endif /* __HUD_H__ */
