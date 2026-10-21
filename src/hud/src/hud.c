/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2026 Manuel Hernández Méndez
 *
 * Authors:
 *   Manuel Hernández Méndez <maherme.dev@gmail.com>
 */

#include "hud.h"
#include "fonts.h"
#include "lives.h"
#include "score.h"

#define LIVES_COUNTER_NUM_DIGIT 2
#define LIVES_COUNTER_X_POS 5
#define LIVES_COUNTER_Y_POS 5
#define SCORE_COUNTER_NUM_LETTERS 5
#define SCORE_COUNTER_LETTERS_X_POS 100
#define SCORE_COUNTER_LETTERS_Y_POS 5
#define SCORE_COUNTER_NUM_DIGIT 6
#define SCORE_COUNTER_DIGITS_X_POS 140
#define SCORE_COUNTER_DIGITS_Y_POS 5

static fonts_handler_t *lives_counter;
static fonts_handler_t *score_letters;
static fonts_handler_t *score_counter;

void
hudInit(void)
{
    lives_counter = fontsCreateFontsHandler(LIVES_COUNTER_NUM_DIGIT);
    fontsInitLayout(lives_counter, LIVES_COUNTER_X_POS, LIVES_COUNTER_Y_POS);
    livesCreate(16, 5);

    score_letters = fontsCreateFontsHandler(SCORE_COUNTER_NUM_LETTERS);
    fontsInitLayout(score_letters, SCORE_COUNTER_LETTERS_X_POS, SCORE_COUNTER_LETTERS_Y_POS);
    fontsDraw("SCORE", score_letters);

    score_counter = fontsCreateFontsHandler(SCORE_COUNTER_NUM_DIGIT);
    fontsInitLayout(score_counter, SCORE_COUNTER_DIGITS_X_POS, SCORE_COUNTER_DIGITS_Y_POS);
    scoreInit();
}

void
onLivesChanged(void *data)
{
    int lives = *(int *)data;
    fontsDrawNumber(lives, lives_counter);
}

void
onScoreChanged(void *data)
{
    int score = *(int *)data;
    fontsDrawNumber(score, score_counter);
}

#ifdef UNIT_TESTING
fonts_handler_t *
helperUT_hudGetLivesCounterPointer(void)
{
    return lives_counter;
}

fonts_handler_t *
helperUT_hudGetScoreLettersPointer(void)
{
    return score_letters;
}

fonts_handler_t *
helperUT_hudGetScoreCounterPointer(void)
{
    return score_counter;
}
#endif /* UNIT_TESTING */
