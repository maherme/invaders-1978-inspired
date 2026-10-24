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
#include <string.h>

static int expected_lives_counter;
static int expected_score_letters;
static int expected_score_counter;

static void
checkLivesCounter(void)
{
    int expected_num_digit = 2;
    int expected_lives_counter_x = 5;
    int expected_lives_counter_y = 5;
    int expected_lives_images_x = 16;
    int expected_lives_images_y = 5;

    will_return(__wrap_fontsCreateFontsHandler, &expected_lives_counter);
    expect_int_value(__wrap_fontsCreateFontsHandler, size, expected_num_digit);
    expect_function_call(__wrap_fontsCreateFontsHandler);
    expect_uint_value(__wrap_fontsInitLayout, h, (uintptr_t)&expected_lives_counter);
    expect_int_value(__wrap_fontsInitLayout, x, expected_lives_counter_x);
    expect_int_value(__wrap_fontsInitLayout, y, expected_lives_counter_y);
    expect_function_call(__wrap_fontsInitLayout);
    expect_int_value(__wrap_livesCreate, x, expected_lives_images_x);
    expect_int_value(__wrap_livesCreate, y, expected_lives_images_y);
    expect_function_call(__wrap_livesCreate);
}

static int
checkString(CMockaValueData value, CMockaValueData check_data)
{
    const char *const *actual = value.const_ptr;
    const char *expected = check_data.const_ptr;

    return actual != NULL && *actual != NULL && expected != NULL && strcmp(*actual, expected) == 0;
}

static void
checkScoreLetters(void)
{
    int expected_num_letters = 5;
    int expected_score_letters_x = 100;
    int expected_score_letters_y = 5;
    const char *expected_text = "SCORE";

    will_return(__wrap_fontsCreateFontsHandler, &expected_score_letters);
    expect_int_value(__wrap_fontsCreateFontsHandler, size, expected_num_letters);
    expect_function_call(__wrap_fontsCreateFontsHandler);
    expect_uint_value(__wrap_fontsInitLayout, h, (uintptr_t)&expected_score_letters);
    expect_int_value(__wrap_fontsInitLayout, x, expected_score_letters_x);
    expect_int_value(__wrap_fontsInitLayout, y, expected_score_letters_y);
    expect_function_call(__wrap_fontsInitLayout);
    expect_check_data(__wrap_fontsDraw, text, checkString, (CMockaValueData){.const_ptr = expected_text});
    expect_uint_value(__wrap_fontsDraw, fonts_handler, (uintptr_t)&expected_score_letters);
    expect_function_call(__wrap_fontsDraw);
}

static void
checkScoreCounter(void)
{
    int expected_num_digit = 6;
    int expected_score_counter_x = 140;
    int expected_score_counter_y = 5;

    will_return(__wrap_fontsCreateFontsHandler, &expected_score_counter);
    expect_int_value(__wrap_fontsCreateFontsHandler, size, expected_num_digit);
    expect_function_call(__wrap_fontsCreateFontsHandler);
    expect_uint_value(__wrap_fontsInitLayout, h, (uintptr_t)&expected_score_counter);
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
    checkScoreLetters();
    checkScoreCounter();

    hudInit();
}

void
testOnLivesChanged(void **status)
{
    (void)status;
    int expected_num_lives = 3;

    expect_int_value(__wrap_fontsDrawNumber, num, expected_num_lives);
    expect_uint_value(__wrap_fontsDrawNumber, fonts_handler, (uintptr_t)&expected_lives_counter);
    expect_function_call(__wrap_fontsDrawNumber);

    onLivesChanged(&expected_num_lives);
}

void
testOnScoreChanged(void **status)
{
    (void)status;
    int expected_score = 100;

    expect_int_value(__wrap_fontsDrawNumber, num, expected_score);
    expect_uint_value(__wrap_fontsDrawNumber, fonts_handler, (uintptr_t)&expected_score_counter);
    expect_function_call(__wrap_fontsDrawNumber);

    onScoreChanged(&expected_score);
}
