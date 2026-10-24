/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2026 Manuel Hernández Méndez
 *
 * Authors:
 *   Manuel Hernández Méndez <maherme.dev@gmail.com>
 */

#ifndef __TESTFONTS_H__
#define __TESTFONTS_H__

void
testFontsCreateFontsHandler(void **status);

void
testFontsDestroyFontsHandler(void **status);

void
testFontsInitLayout(void **status);

void
testFontsDraw(void **status);

void
testFontsDrawNotSupported(void **status);

void
testFontsDrawBiggerBuffer(void **status);

void
testFontsDrawSmallerBuffer(void **status);

void
testFontsDrawMixedCharacters(void **status);

void
testFontsDrawNumber(void **status);

#endif /* __TESTFONTS_H__ */
