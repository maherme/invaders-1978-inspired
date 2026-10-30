/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2025 Manuel Hernández Méndez
 *
 * Authors:
 *   Manuel Hernández Méndez <maherme.dev@gmail.com>
 */

#include "testGraphGlutCallbacks.h"
#include "graph.h"
#include "graphGlutCallbacks.h"
#include <cmocka.h>
#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>

#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

void
__wrap_glClear(GLbitfield mask)
{
    check_expected_uint(mask);
    function_called();
}

void
__wrap_glutSwapBuffers(void)
{
    function_called();
}

void
__wrap_glViewport(GLint x, GLint y, GLsizei width, GLsizei height)
{
    check_expected_int(x);
    check_expected_int(y);
    check_expected_int(width);
    check_expected_int(height);
    function_called();
}

void
__wrap_glMatrixMode(GLenum mode)
{
    check_expected_uint(mode);
    function_called();
}

void
__wrap_glLoadIdentity(void)
{
    function_called();
}

void
__wrap_gluOrtho2D(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top)
{
    check_expected_double(left);
    check_expected_double(right);
    check_expected_double(bottom);
    check_expected_double(top);
    function_called();
}

int
setup(void **state)
{
    (void)state;
    graphRegisterDrawInit();
    helperUT_graphGlutResetRegisteredDrawables();
    return 0;
}

int
teardown(void **state)
{
    (void)state;
    helperUT_graphGlutResetRegisteredDrawables();
    return 0;
}

void
testGraphRegisterDrawFunctionFailNullParameter(void **status)
{
    (void)status;

    graph_draw_id_t result = graphRegisterDrawFunction(NULL);
    assert_int_equal(result, GRAPH_DRAW_ID_INVALID);
}

static void
register_drawables(graph_drawable_t **drawables, int n)
{
    for (int i = 0; i < n; i++)
    {
        expect_function_call(__wrap_utilsCalloc);
        int result = graphRegisterDrawFunction(drawables[i]);
        assert_int_equal(result, 0);
    }
}

void
testGraphRegisterDrawFunctionSuccess(void **status)
{
    (void)status;
    graph_drawable_t drawable = {0};
    drawable.layer = LAYER_BACKGROUND;
    graph_drawable_t *expected_drawables[] = {&drawable};

    register_drawables(expected_drawables, ARRAY_SIZE(expected_drawables));
}

typedef struct
{
    int x, y;
    int w, h;
} expected_glviewport_t;

typedef struct
{
    double left, right;
    double top, bottom;
} expected_gluortho2d_t;

static void
checkViewport(expected_glviewport_t *glviewport, expected_gluortho2d_t *gluortho2d)
{
    expect_int_value(__wrap_glViewport, x, glviewport->x);
    expect_int_value(__wrap_glViewport, y, glviewport->y);
    expect_int_value(__wrap_glViewport, width, glviewport->w);
    expect_int_value(__wrap_glViewport, height, glviewport->h);
    expect_function_call(__wrap_glViewport);

    expect_uint_value(__wrap_glMatrixMode, mode, GL_PROJECTION);
    expect_function_call(__wrap_glMatrixMode);

    expect_function_call(__wrap_glLoadIdentity);

    expect_double(__wrap_gluOrtho2D, left, gluortho2d->left, 0);
    expect_double(__wrap_gluOrtho2D, right, gluortho2d->right, 0);
    expect_double(__wrap_gluOrtho2D, bottom, gluortho2d->bottom, 0);
    expect_double(__wrap_gluOrtho2D, top, gluortho2d->top, 0);
    expect_function_call(__wrap_gluOrtho2D);

    expect_uint_value(__wrap_glMatrixMode, mode, GL_MODELVIEW);
    expect_function_call(__wrap_glMatrixMode);

    expect_function_call(__wrap_glLoadIdentity);
}

void
testGraphGlutDisplay(void **status)
{
    (void)status;
    graph_drawable_t drawable1, drawable2 = {0};
    drawable1.layer = LAYER_BACKGROUND;
    drawable2.layer = LAYER_HUD;
    graph_drawable_t *expected_drawables[] = {&drawable1, &drawable2};
    expected_glviewport_t glviewport_game = {.x = 1, .y = 2, .w = 3, .h = 4};
    expected_glviewport_t glviewport_hud = {.x = 1, .y = 2, .w = 3, .h = 4};
    expected_gluortho2d_t gluortho2d_game = {.left = 0, .right = GAME_WIDTH, .bottom = 0, .top = GAME_HEIGHT};
    expected_gluortho2d_t gluortho2d_hud = {.left = 0, .right = GAME_WIDTH, .bottom = 0, .top = HUD_HEIGHT};

    expect_function_call(__wrap_utilsCalloc);
    helperUT_graphGlutInjectDrawable(expected_drawables[0]);
    expect_function_call(__wrap_utilsCalloc);
    helperUT_graphGlutInjectDrawable(expected_drawables[1]);

    helperUT_graphGlutSetGameViewport((void *)&glviewport_game);
    helperUT_graphGlutSetHudViewport((void *)&glviewport_hud);

    expect_uint_value(__wrap_glClear, mask, GL_COLOR_BUFFER_BIT);
    expect_function_call(__wrap_glClear);

    checkViewport(&glviewport_game, &gluortho2d_game);
    //    expect_uint_value(__wrap_graphPrintImage, sprite, (uintptr_t)expected_sprites[0]);
    //    expect_function_call(__wrap_graphPrintImage);
    expect_int_value(__wrap_graphPrintBorder, x, 0);
    expect_int_value(__wrap_graphPrintBorder, y, 0);
    expect_int_value(__wrap_graphPrintBorder, w, GAME_WIDTH);
    expect_int_value(__wrap_graphPrintBorder, h, GAME_HEIGHT);
    expect_function_call(__wrap_graphPrintBorder);

    checkViewport(&glviewport_hud, &gluortho2d_hud);
    //    expect_uint_value(__wrap_graphPrintImage, sprite, (uintptr_t)expected_sprites[1]);
    //    expect_function_call(__wrap_graphPrintImage);
    expect_int_value(__wrap_graphPrintBorder, x, 0);
    expect_int_value(__wrap_graphPrintBorder, y, 0);
    expect_int_value(__wrap_graphPrintBorder, w, GAME_WIDTH);
    expect_int_value(__wrap_graphPrintBorder, h, HUD_HEIGHT);
    expect_function_call(__wrap_graphPrintBorder);

    expect_function_call(__wrap_glutSwapBuffers);

    graphGlutDisplay();
}

static void
check_viewports(int w, int h, int expected_scale)
{
    expected_glviewport_t expected_vp_game = {.x = (w - GAME_WIDTH * expected_scale) / 2,
                                              .y = HUD_HEIGHT * expected_scale +
                                                   (h - (GAME_HEIGHT + HUD_HEIGHT) * expected_scale) / 2,
                                              .w = GAME_WIDTH * expected_scale,
                                              .h = GAME_HEIGHT * expected_scale};
    expected_glviewport_t expected_vp_hud = {.x = (w - GAME_WIDTH * expected_scale) / 2,
                                             .y = (h - (GAME_HEIGHT + HUD_HEIGHT) * expected_scale) / 2,
                                             .w = GAME_WIDTH * expected_scale,
                                             .h = HUD_HEIGHT * expected_scale};

    expected_glviewport_t *glviewport_game = (expected_glviewport_t *)helperUT_graphGlutGetGameViewport();
    assert_int_equal(expected_vp_game.x, glviewport_game->x);
    assert_int_equal(expected_vp_game.y, glviewport_game->y);
    assert_int_equal(expected_vp_game.w, glviewport_game->w);
    assert_int_equal(expected_vp_game.h, glviewport_game->h);

    expected_glviewport_t *glviewport_hud = (expected_glviewport_t *)helperUT_graphGlutGetHudViewport();
    assert_int_equal(expected_vp_hud.x, glviewport_hud->x);
    assert_int_equal(expected_vp_hud.y, glviewport_hud->y);
    assert_int_equal(expected_vp_hud.w, glviewport_hud->w);
    assert_int_equal(expected_vp_hud.h, glviewport_hud->h);
}

void
testGraphGlutReshapeScaleLowerThanOne(void **status)
{
    (void)status;
    int width = GAME_WIDTH / 2;
    int height = (GAME_HEIGHT + HUD_HEIGHT) / 2;

    graphGlutReshape(width, height);

    int expected_scale = 1;
    check_viewports(width, height, expected_scale);
}

void
testGraphGlutReshapeScaleHigherThanOne(void **status)
{
    (void)status;
    int width = GAME_WIDTH * 2;
    int height = (GAME_HEIGHT + HUD_HEIGHT) * 2;

    graphGlutReshape(width, height);

    int expected_scale = 2;
    check_viewports(width, height, expected_scale);
}

void
testGraphRegisterDrawSprite(void **status)
{
    (void)status;
    sprite_t expected_sprite = {0};

    expect_uint_value(__wrap_graphCreateImage, sprite, (uintptr_t)&expected_sprite);
    expect_function_call(__wrap_graphCreateImage);
    graphRegisterDrawSprite(&expected_sprite);
}

void
testGraphDrawSprite(void **status)
{
    (void)status;
    sprite_t expected_sprite = {0};

    expect_uint_value(__wrap_graphPrintImage, sprite, (uintptr_t)&expected_sprite);
    expect_function_call(__wrap_graphPrintImage);
    graphDrawSprite(&expected_sprite);
}

void
testGraphUnregisterDrawSprite(void **status)
{
    (void)status;
    sprite_t expected_sprite = {0};

    expect_uint_value(__wrap_graphDestroyImage, sprite, (uintptr_t)&expected_sprite);
    expect_function_call(__wrap_graphDestroyImage);
    graphUnregisterDrawSprite(&expected_sprite);
}
