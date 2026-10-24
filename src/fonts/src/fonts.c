/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2026 Manuel Hernández Méndez
 *
 * Authors:
 *   Manuel Hernández Méndez <maherme.dev@gmail.com>
 */

#include "fonts.h"
#include "graph.h"
#include "utils.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define FONTS_NUM_DIGITS 10
#define FONTS_DIGIT_WIDTH 4
#define FONTS_DIGIT_HEIGHT 6
#define FONTS_NUM_LETTERS 26
#define FONTS_LETTERS_WIDTH 5
#define FONTS_LETTERS_HEIGHT 6
#define FONTS_PIXELS_SPACE 1

static const char fontsDigits[FONTS_NUM_DIGITS][FONTS_DIGIT_HEIGHT][FONTS_DIGIT_WIDTH][NUM_RGBA_CHANNELS] = {
    /* clang-format off */
    /* digit 0 */
    {{B, W, W, B},
     {W, B, B, W},
     {W, B, B, W},
     {W, B, B, W},
     {W, B, B, W},
     {B, W, W, B}},
    /* digit 1 */
    {{B, B, W, B},
     {B, W, W, B},
     {B, B, W, B},
     {B, B, W, B},
     {B, B, W, B},
     {B, B, W, B}},
    /* digit 2 */
    {{B, W, W, B},
     {W, B, B, W},
     {B, B, B, W},
     {B, B, W, B},
     {B, W, B, B},
     {W, W, W, W}},
    /* digit 3 */
    {{B, W, W, B},
     {W, B, B, W},
     {B, B, W, B},
     {B, B, B, W},
     {W, B, B, W},
     {B, W, W, B}},
    /* digit 4 */
    {{B, B, B, W},
     {B, B, W, W},
     {B, W, B, W},
     {W, B, B, W},
     {W, W, W, W},
     {B, B, B, W}},
    /* digit 5 */
    {{W, W, W, W}, 
     {W, B, B, B},
     {W, W, W, B},
     {B, B, B, W},
     {B, B, B, W},
     {W, W, W, B}},
    /* digit 6 */
    {{B, W, W, B},
     {W, B, B, B},
     {W, W, W, B},
     {W, B, B, W},
     {W, B, B, W},
     {B, W, W, B}},
    /* digit 7 */
    {{W, W, W, W},
     {B, B, B, W},
     {B, B, B, W},
     {B, B, B, W},
     {B, B, B, W},
     {B, B, B, W}},
    /* digit 8 */
    {{B, W, W, B},
     {W, B, B, W},
     {B, W, W, B},
     {W, B, B, W},
     {W, B, B, W},
     {B, W, W, B}},
    /* digit 9 */
    {{B, W, W, B},
     {W, B, B, W},
     {W, B, B, W},
     {B, W, W, W},
     {B, B, B, W},
     {B, W, W, B}}};

static const char fontsLetters[FONTS_NUM_LETTERS][FONTS_LETTERS_HEIGHT][FONTS_LETTERS_WIDTH][NUM_RGBA_CHANNELS] = {
    /* letter A */
    {{B, W, W, W, B},
     {W, B, B, B, W},
     {W, B, B, B, W},
     {W, W, W, W, W},
     {W, B, B, B, W},
     {W, B, B, B, W}},
    /* letter B */
    {{W, W, W, W, B},
     {W, B, B, B, W},
     {W, W, W, W, B},
     {W, B, B, B, W},
     {W, B, B, B, W},
     {W, W, W, W, B}},
    /* letter C */
    {{B, W, W, W, B},
     {W, B, B, B, W},
     {W, B, B, B, B},
     {W, B, B, B, B},
     {W, B, B, B, W},
     {B, W, W, W, B}},
    /* letter D */
    {{W, W, W, W, B},
     {W, B, B, B, W},
     {W, B, B, B, W},
     {W, B, B, B, W},
     {W, B, B, B, W},
     {W, W, W, W, B}},
    /* letter E */
    {{W, W, W, W, W},
     {W, B, B, B, B},
     {W, W, W, W, B},
     {W, B, B, B, B},
     {W, B, B, B, B},
     {W, W, W, W, W}},
    /* letter F */
    {{W, W, W, W, W},
     {W, B, B, B, B},
     {W, W, W, W, B},
     {W, B, B, B, B},
     {W, B, B, B, B},
     {W, B, B, B, B}},
    /* letter G */
    {{B, W, W, W, B},
     {W, B, B, B, W},
     {W, B, B, B, B},
     {W, B, W, W, W},
     {W, B, B, B, W},
     {B, W, W, W, B}},
    /* letter H */
    {{W, B, B, B, W},
     {W, B, B, B, W},
     {W, W, W, W, W},
     {W, B, B, B, W},
     {W, B, B, B, W},
     {W, B, B, B, W}},
    /* letter I */
    {{W, W, W, W, W},
     {B, B, W, B, B},
     {B, B, W, B, B},
     {B, B, W, B, B},
     {B, B, W, B, B},
     {W, W, W, W, W}},
    /* letter J */
    {{B, B, B, B, W},
     {B, B, B, B, W},
     {B, B, B, B, W},
     {W, B, B, B, W},
     {W, B, B, B, W},
     {B, W, W, W, B}},
    /* letter K */
    {{W, B, B, B, W},
     {W, B, B, W, B},
     {W, W, W, B, B},
     {W, B, W, B, B},
     {W, B, B, W, B},
     {W, B, B, B, W}},
    /* letter L */
    {{W, B, B, B, B},
     {W, B, B, B, B},
     {W, B, B, B, B},
     {W, B, B, B, B},
     {W, B, B, B, B},
     {W, W, W, W, W}},
    /* letter M */
    {{W, B, B, B, W},
     {W, W, B, W, W},
     {W, B, W, B, W},
     {W, B, B, B, W},
     {W, B, B, B, W},
     {W, B, B, B, W}},
    /* letter N */
    {{W, B, B, B, W},
     {W, W, B, B, W},
     {W, B, W, B, W},
     {W, B, B, W, W},
     {W, B, B, B, W},
     {W, B, B, B, W}},
    /* letter O */
    {{B, W, W, W, B},
     {W, B, B, B, W},
     {W, B, B, B, W},
     {W, B, B, B, W},
     {W, B, B, B, W},
     {B, W, W, W, B}},
    /* letter P */
    {{W, W, W, W, B},
     {W, B, B, B, W},
     {W, B, B, B, W},
     {W, W, W, W, B},
     {W, B, B, B, B},
     {W, B, B, B, B}},
    /* letter Q */
    {{B, W, W, W, B},
     {W, B, B, B, W},
     {W, B, B, B, W},
     {W, B, B, B, W},
     {B, W, W, W, B},
     {B, B, B, B, W}},
    /* letter R */
    {{W, W, W, W, B},
     {W, B, B, B, W},
     {W, B, B, B, W},
     {W, W, W, W, B},
     {W, B, B, B, W},
     {W, B, B, B, W}},
    /* letter S */
    {{B, W, W, W, W},
     {W, B, B, B, B},
     {B, W, W, W, B},
     {B, B, B, B, W},
     {B, B, B, B, W},
     {W, W, W, W, B}},
    /* letter T */
    {{W, W, W, W, W},
     {B, B, W, B, B},
     {B, B, W, B, B},
     {B, B, W, B, B},
     {B, B, W, B, B},
     {B, B, W, B, B}},
    /* letter U */
    {{W, B, B, B, W},
     {W, B, B, B, W},
     {W, B, B, B, W},
     {W, B, B, B, W},
     {W, B, B, B, W},
     {B, W, W, W, B}},
    /* letter V */
    {{W, B, B, B, W},
     {W, B, B, B, W},
     {W, B, B, B, W},
     {W, B, B, B, W},
     {B, W, B, W, B},
     {B, B, W, B, B}},
    /* letter W */
    {{W, B, B, B, W},
     {W, B, B, B, W},
     {W, B, B, B, W},
     {W, B, W, B, W},
     {W, W, B, W, W},
     {W, B, B, B, W}},
    /* letter X */
    {{W, B, B, B, W},
     {B, W, B, W, B},
     {B, B, W, B, B},
     {B, B, W, B, B},
     {B, W, B, W, B},
     {W, B, B, B, W}},
    /* letter Y */
    {{W, B, B, B, W},
     {W, B, B, B, W},
     {W, B, B, B, W},
     {B, W, W, W, W},
     {B, B, B, B, W},
     {B, W, W, W, B}},
    /* letter Z */
    {{W, W, W, W, W},
     {B, B, B, B, W},
     {B, B, B, W, B},
     {B, B, W, B, B},
     {B, W, B, B, B},
     {W, W, W, W, W}},
};
/* clang-format on */

static bool
fontsSetInBuffer(char c, sprite_t *sprite)
{
    /* NOTE:
     * condition false && false is not possible
     */
    if (c >= '0' && c <= '9') /* GCOVR_EXCL_BR_WITHOUT_HIT: 1/4 */
    {
        sprite->image = (const char *)fontsDigits[c - '0'];
        sprite->width = FONTS_DIGIT_WIDTH;
        sprite->height = FONTS_DIGIT_HEIGHT;
        return true;
    }

    /* NOTE:
     * condition false && false is not possible
     */
    if (c >= 'A' && c <= 'Z') /* GCOVR_EXCL_BR_WITHOUT_HIT: 1/4 */
    {
        sprite->image = (const char *)fontsLetters[c - 'A'];
        sprite->width = FONTS_LETTERS_WIDTH;
        sprite->height = FONTS_LETTERS_HEIGHT;
        return true;
    }

    sprite->image = NULL;
    return false;
}

typedef struct fonts
{
    sprite_t *sprites;
    size_t size;
    int x;
    int y;
} fonts_handler_t;

fonts_handler_t *
fontsCreateFontsHandler(size_t size)
{
    fonts_handler_t *h = utilsCalloc(1, sizeof(*h));
    h->sprites = utilsCalloc(size, sizeof(sprite_t));
    h->size = size;

    return h;
}

static void
fontsRegisterSprites(fonts_handler_t *h)
{

    for (size_t i = 0; i < h->size; i++)
    {
        graphRegisterPrint(&h->sprites[i]);
    }
}

void
fontsInitLayout(fonts_handler_t *h, int x, int y)
{
    h->x = x;
    h->y = y;

    for (size_t i = 0; i < h->size; i++)
    {
        h->sprites[i].layer = LAYER_HUD;
    }

    fontsRegisterSprites(h);
}

void
fontsDestroyFontsHandler(fonts_handler_t *fonts_handler)
{
    UTILS_FREE(fonts_handler->sprites);
    UTILS_FREE(fonts_handler);
}

void
fontsDraw(const char *const text, fonts_handler_t *fonts_handler)
{
    size_t len = strlen(text);

    if (fonts_handler->size < len)
    {
        return;
    }

    int x = fonts_handler->x;

    for (size_t i = 0; i < len; i++)
    {
        sprite_t *sprite = &fonts_handler->sprites[i];
        bool supported = fontsSetInBuffer(text[i], sprite);

        sprite->x = x;
        sprite->y = fonts_handler->y;
        sprite->visible = supported;

        if (!supported)
        {
            continue;
        }

        x += sprite->width + FONTS_PIXELS_SPACE;
        graphUpdateSprite(sprite);
    }

    for (size_t i = len; i < fonts_handler->size; i++)
    {
        fonts_handler->sprites[i].visible = false;
    }
}

void
fontsDrawNumber(int num, fonts_handler_t *fonts_handler)
{
    char buf[fonts_handler->size + 1];
    snprintf(buf, sizeof(buf), "%0*d", (int)fonts_handler->size, num);
    fontsDraw(buf, fonts_handler);
}

#ifdef UNIT_TESTING
fonts_handler_t *
helperUT_fontsMakeFakeHandler(size_t size, int x, int y)
{
    fonts_handler_t *fonts_handler = calloc(1, sizeof(fonts_handler_t));
    fonts_handler->sprites = calloc(size, sizeof(sprite_t));
    fonts_handler->size = size;
    fonts_handler->x = x;
    fonts_handler->y = y;

    return fonts_handler;
}

sprite_t *
helperUT_fontsGetSpritesPointer(fonts_handler_t *h, int index)
{
    return &h->sprites[index];
}

size_t
helperUT_fontsGetSize(fonts_handler_t *fonts_handler)
{
    return fonts_handler->size;
}

void
helperUT_fontsFreeHandler(fonts_handler_t *fonts_handler)
{
    free(fonts_handler->sprites);
    free(fonts_handler);
}
#endif /* UNIT_TESTING */
