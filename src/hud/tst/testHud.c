/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2026 Manuel Hernández Méndez
 *
 * Authors:
 *   Manuel Hernández Méndez <maherme.dev@gmail.com>
 */

#include "testHud.h"
#include "hud.h"
#include <cmocka.h>
#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>

static void
checkLivesCounter(void)
{
    int expected_num_digit = 2;
    int expected_live_counter_x = 5;
    int expected_live_counter_y = 5;
    fonts_handler_t *expected_h = helperUT_hudGetLivesCounterPointer();
    int expected_live_images_x = 16;
    int expected_live_images_y = 5;

    expect_int_value(__wrap_fontsCreateFontsHandler, size, expected_num_digit);
    expect_function_call(__wrap_fontsCreateFontsHandler);
    expect_uint_value(__wrap_fontsInitLayout, h, (uintptr_t)expected_h);
    expect_int_value(__wrap_fontsInitLayout, x, expected_live_counter_x);
    expect_int_value(__wrap_fontsInitLayout, y, expected_live_counter_y);
    expect_function_call(__wrap_fontsInitLayout);
    expect_int_value(__wrap_livesCreate, x, expected_live_images_x);
    expect_int_value(__wrap_livesCreate, y, expected_live_images_y);
    expect_function_call(__wrap_livesCreate);
}

static void
checkScoreCounter(void)
{
    int expected_num_digit = 6;
    int expected_score_counter_x = 120;
    int expected_score_counter_y = 5;
    fonts_handler_t *expected_h = helperUT_hudGetScoreCounterPointer();

    expect_int_value(__wrap_fontsCreateFontsHandler, size, expected_num_digit);
    expect_function_call(__wrap_fontsCreateFontsHandler);
    expect_uint_value(__wrap_fontsInitLayout, h, (uintptr_t)expected_h);
    expect_int_value(__wrap_fontsInitLayout, x, expected_score_counter_x);
    expect_int_value(__wrap_fontsInitLayout, y, expected_score_counter_y);
    expect_function_call(__wrap_fontsInitLayout);
    expect_function_call(__wrap_scoreInit);
}

void
testHudInit(void **status)
{
    (void)status;
    checkLivesCounter();
    checkScoreCounter();
    hudInit();
}

void
testOnLivesChanged(void **status)
{
    (void)status;
    int expected_num_lives = 3;
    fonts_handler_t *expected_live_counter_h = helperUT_hudGetLivesCounterPointer();

    expect_int_value(__wrap_fontsDrawNumber, num, expected_num_lives);
    expect_uint_value(__wrap_fontsDrawNumber, fonts_handler, (uintptr_t)expected_live_counter_h);
    expect_function_call(__wrap_fontsDrawNumber);

    onLivesChanged(&expected_num_lives);
}

void
testOnScoreChanged(void **status)
{
    (void)status;
    int expected_score = 100;
    fonts_handler_t *expected_score_counter_h = helperUT_hudGetScoreCounterPointer();

    expect_int_value(__wrap_fontsDrawNumber, num, expected_score);
    expect_uint_value(__wrap_fontsDrawNumber, fonts_handler, (uintptr_t)expected_score_counter_h);
    expect_function_call(__wrap_fontsDrawNumber);

    onScoreChanged(&expected_score);
}
