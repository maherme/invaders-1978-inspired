/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2026 Manuel Hernández Méndez
 *
 * Authors:
 *   Manuel Hernández Méndez <maherme.dev@gmail.com>
 */

#ifndef __FONTS_H__
#define __FONTS_H__

#include <stddef.h>

struct fonts;
typedef struct fonts fonts_handler_t;

fonts_handler_t *
fontsCreateFontsHandler(size_t size);

void
fontsInitLayout(fonts_handler_t *h, int x, int y);

void
fontsDestroyFontsHandler(fonts_handler_t *fonts_handler);

void
fontsDraw(const char *const text, fonts_handler_t *fonts_handler);

void
fontsDrawNumber(int num, fonts_handler_t *fonts_handler);

#ifdef UNIT_TESTING
struct sprite;
typedef struct sprite sprite_t;

fonts_handler_t *
helperUT_fontsMakeFakeHandler(size_t size, int x, int y);

sprite_t *
helperUT_fontsGetSpritesPointer(fonts_handler_t *h, int index);

size_t
helperUT_fontsGetSize(fonts_handler_t *fonts_handler);

void
helperUT_fontsFreeHandler(fonts_handler_t *fonts_handler);
#endif /* UNIT_TESTING */

#endif /* __FONTS_H__ */
