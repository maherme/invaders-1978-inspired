/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2026 Manuel Hernández Méndez
 *
 * Authors:
 *   Manuel Hernández Méndez <maherme.dev@gmail.com>
 */

#include "testFonts.h"
#include "fonts.h"
#include "graph.h"
#include "graphGlutCallbacks.h"
#include <cmocka.h>
#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

void
testFontsCreateFontsHandler(void **status)
{
    (void)status;
    size_t expected_size = 10;

    expect_function_call(__wrap_utilsCalloc);
    expect_function_call(__wrap_utilsCalloc);

    fonts_handler_t *fonts_handler = fontsCreateFontsHandler(expected_size);
    assert_uint_equal(expected_size, helperUT_fontsGetSize(fonts_handler));

    helperUT_fontsFreeHandler(fonts_handler);
}

void
testFontsInitLayout(void **status)
{
    (void)status;
    size_t expected_size = 2;
    int expected_x = 1, expected_y = 2;

    fonts_handler_t *fonts_handler = helperUT_fontsMakeFakeHandler(expected_size, 0, 0);

    for (size_t i = 0; i < expected_size; i++)
    {
        sprite_t *expected_sprite = helperUT_fontsGetSpritesPointer(fonts_handler, i);
        expect_uint_value(__wrap_graphRegisterDrawFunction, drawable->on_register_data, (uintptr_t)expected_sprite);
        expect_uint_value(__wrap_graphRegisterDrawFunction, drawable->on_draw_data, (uintptr_t)expected_sprite);
        expect_uint_value(__wrap_graphRegisterDrawFunction, drawable->on_unregister_data, (uintptr_t)expected_sprite);
        expect_function_call(__wrap_graphRegisterDrawFunction);
    }

    fontsInitLayout(fonts_handler, expected_x, expected_y, LAYER_BACKGROUND);

    assert_int_equal(helperUT_fontsGetLayer(fonts_handler), LAYER_BACKGROUND);

    helperUT_fontsFreeHandler(fonts_handler);
}

void
testFontsDestroyFontsHandlerNullParameter(void **status)
{
    (void)status;

    fontsDestroyFontsHandler(NULL);
}

void
testFontsDestroyFontsHandler(void **status)
{
    (void)status;
    size_t expected_size = 2;
    fonts_handler_t *fonts_handler = helperUT_fontsMakeFakeHandler(expected_size, 0, 0);

    for (size_t i = 0; i < expected_size; i++)
    {
        expect_function_call(__wrap_graphUnregisterDrawFunction);
    }

    fontsDestroyFontsHandler(fonts_handler);

    /* No asserts, use ASAN */
}

void
testFontsDraw(void **status)
{
    (void)status;
    int expected_x = 2;
    int expected_y = 3;
    fonts_handler_t *fonts_handler = helperUT_fontsMakeFakeHandler(1, expected_x, expected_y);

    sprite_t *expected_sprite = helperUT_fontsGetSpritesPointer(fonts_handler, 0);
    expect_uint_value(__wrap_graphUpdateSprite, sprite, (uintptr_t)expected_sprite);
    expect_function_call(__wrap_graphUpdateSprite);

    fontsDraw("A", fonts_handler);

    assert_int_equal(expected_sprite->x, expected_x);
    assert_int_equal(expected_sprite->y, expected_y);
    assert_int_equal(expected_sprite->width, 5);
    assert_int_equal(expected_sprite->height, 6);
    assert_true(expected_sprite->visible);

    helperUT_fontsFreeHandler(fonts_handler);
}

void
testFontsDrawNotSupported(void **status)
{
    (void)status;
    fonts_handler_t *fonts_handler = helperUT_fontsMakeFakeHandler(1, 0, 0);

    sprite_t *sprite = helperUT_fontsGetSpritesPointer(fonts_handler, 0);
    sprite->visible = true;

    fontsDraw("a", fonts_handler);

    assert_false(sprite->visible);

    helperUT_fontsFreeHandler(fonts_handler);
}

void
testFontsDrawBiggerBuffer(void **status)
{
    (void)status;
    size_t expected_size = 2;
    int expected_x = 10;
    int expected_y = 20;
    fonts_handler_t *fonts_handler = helperUT_fontsMakeFakeHandler(expected_size, expected_x, expected_y);

    sprite_t *expected_sprite = helperUT_fontsGetSpritesPointer(fonts_handler, 0);
    expect_uint_value(__wrap_graphUpdateSprite, sprite, (uintptr_t)expected_sprite);
    expect_function_call(__wrap_graphUpdateSprite);

    fontsDraw("A", fonts_handler);

    sprite_t *sprite_a = helperUT_fontsGetSpritesPointer(fonts_handler, 0);
    assert_int_equal(sprite_a->x, expected_x);
    assert_int_equal(sprite_a->y, expected_y);
    assert_int_equal(sprite_a->width, 5);
    assert_int_equal(sprite_a->height, 6);
    assert_true(sprite_a->visible);

    sprite_t *sprite_unused = helperUT_fontsGetSpritesPointer(fonts_handler, 1);
    assert_false(sprite_unused->visible);

    helperUT_fontsFreeHandler(fonts_handler);
}

void
testFontsDrawSmallerBuffer(void **status)
{
    (void)status;
    size_t expected_size = 1;
    const char *expected_image = "test";
    fonts_handler_t *fonts_handler = helperUT_fontsMakeFakeHandler(expected_size, 10, 20);

    sprite_t *sprite = helperUT_fontsGetSpritesPointer(fonts_handler, 0);
    sprite->image = expected_image;
    sprite->width = 7;
    sprite->height = 8;
    sprite->visible = true;

    fontsDraw("AB", fonts_handler);

    assert_int_equal(sprite->x, 0);
    assert_int_equal(sprite->y, 0);
    assert_int_equal(sprite->width, 7);
    assert_int_equal(sprite->height, 8);
    assert_true(sprite->visible);
    assert_ptr_equal(sprite->image, expected_image);

    helperUT_fontsFreeHandler(fonts_handler);
}

void
testFontsDrawMixedCharacters(void **status)
{
    (void)status;
    int expected_x = 10;
    int expected_y = 20;
    size_t expected_size = 3;
    fonts_handler_t *fonts_handler = helperUT_fontsMakeFakeHandler(expected_size, expected_x, expected_y);

    for (size_t i = 0; i < expected_size; i++)
    {
        sprite_t *expected_sprite = helperUT_fontsGetSpritesPointer(fonts_handler, i);
        expect_uint_value(__wrap_graphUpdateSprite, sprite, (uintptr_t)expected_sprite);
        expect_function_call(__wrap_graphUpdateSprite);
    }

    fontsDraw("A0B", fonts_handler);

    sprite_t *sprite_a = helperUT_fontsGetSpritesPointer(fonts_handler, 0);
    assert_int_equal(sprite_a->x, 10);
    assert_int_equal(sprite_a->y, 20);
    assert_int_equal(sprite_a->width, 5);
    assert_int_equal(sprite_a->height, 6);
    assert_true(sprite_a->visible);

    sprite_t *sprite_0 = helperUT_fontsGetSpritesPointer(fonts_handler, 1);
    assert_int_equal(sprite_0->x, 16); /* 10 + 5 + FONTS_PIXELS_SPACE */
    assert_int_equal(sprite_0->y, 20);
    assert_int_equal(sprite_0->width, 4);
    assert_int_equal(sprite_0->height, 6);
    assert_true(sprite_0->visible);

    sprite_t *sprite_b = helperUT_fontsGetSpritesPointer(fonts_handler, 2);
    assert_int_equal(sprite_b->x, 21); /* 16 + 4 + FONTS_PIXELS_SPACE */
    assert_int_equal(sprite_b->y, 20);
    assert_int_equal(sprite_b->width, 5);
    assert_int_equal(sprite_b->height, 6);
    assert_true(sprite_b->visible);

    helperUT_fontsFreeHandler(fonts_handler);
}

void
testFontsDrawNumber(void **status)
{
    (void)status;
    size_t expected_size = 5;
    fonts_handler_t *fonts_handler = helperUT_fontsMakeFakeHandler(expected_size, 10, 20);

    for (size_t i = 0; i < expected_size; i++)
    {
        sprite_t *sprite = helperUT_fontsGetSpritesPointer(fonts_handler, i);
        expect_uint_value(__wrap_graphUpdateSprite, sprite, (uintptr_t)sprite);
        expect_function_call(__wrap_graphUpdateSprite);
    }

    fontsDrawNumber(42, fonts_handler);

    const int expected_x[] = {10, 15, 20, 25, 30};
    const int expected_width[] = {4, 4, 4, 4, 4};

    for (size_t i = 0; i < expected_size; i++)
    {
        sprite_t *sprite = helperUT_fontsGetSpritesPointer(fonts_handler, i);

        assert_int_equal(sprite->x, expected_x[i]);
        assert_int_equal(sprite->y, 20);
        assert_int_equal(sprite->width, expected_width[i]);
        assert_int_equal(sprite->height, 6);
        assert_true(sprite->visible);
    }

    helperUT_fontsFreeHandler(fonts_handler);
}
