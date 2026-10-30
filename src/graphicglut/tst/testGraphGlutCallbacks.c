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

static void
testRegisterCallback(void *data)
{
    check_expected_ptr(data);
    function_called();
}

static void
testUnregisterCallback(void *data)
{
    check_expected_ptr(data);
    function_called();
}

static int draw_order[16];
static size_t draw_count;

static void
testDrawCallback(void *data)
{
    assert_true(draw_count < ARRAY_SIZE(draw_order));

    draw_order[draw_count++] = *(int *)data;
}

void
testGraphRegisterDrawFunctionFailNotInitialized(void **status)
{
    (void)status;

    helperUT_graphGlutReset();

    graph_drawable_t drawable = {
        .layer = LAYER_BACKGROUND,
    };

    assert_int_equal(graphRegisterDrawFunction(&drawable), GRAPH_DRAW_ID_INVALID);
}

void
testGraphRegisterDrawFunctionFailNullParameter(void **status)
{
    (void)status;

    graph_draw_id_t result = graphRegisterDrawFunction(NULL);
    assert_int_equal(result, GRAPH_DRAW_ID_INVALID);
}

void
testGraphRegisterDrawFunctionFailNegativeLayer(void **status)
{
    (void)status;

    graph_drawable_t drawable = {0};
    drawable.layer = -1;

    graph_draw_id_t result = graphRegisterDrawFunction(&drawable);

    assert_int_equal(result, GRAPH_DRAW_ID_INVALID);
}

void
testGraphRegisterDrawFunctionFailLayerOutOfRange(void **status)
{
    (void)status;

    graph_drawable_t drawable = {
        .layer = NUM_RENDER_LAYERS,
    };

    graph_draw_id_t result = graphRegisterDrawFunction(&drawable);

    assert_int_equal(result, GRAPH_DRAW_ID_INVALID);
}

void
testGraphRegisterDrawFunctionUniqueIds(void **status)
{
    (void)status;

    graph_drawable_t drawable1 = {
        .layer = LAYER_BACKGROUND,
    };
    graph_drawable_t drawable2 = {
        .layer = LAYER_BACKGROUND,
    };

    expect_function_call(__wrap_utilsCalloc);
    graph_draw_id_t id1 = graphRegisterDrawFunction(&drawable1);

    expect_function_call(__wrap_utilsCalloc);
    graph_draw_id_t id2 = graphRegisterDrawFunction(&drawable2);

    assert_int_not_equal(id1, GRAPH_DRAW_ID_INVALID);
    assert_int_not_equal(id2, GRAPH_DRAW_ID_INVALID);
    assert_int_not_equal(id1, id2);
}

void
testGraphRegisterDrawFunctionCallback(void **status)
{
    (void)status;

    int expected_data = 42;

    graph_drawable_t drawable = {
        .layer = LAYER_BACKGROUND,
        .on_register_fn = testRegisterCallback,
        .on_register_data = &expected_data,
    };

    expect_function_call(__wrap_utilsCalloc);

    expect_uint_value(testRegisterCallback, data, (uintptr_t)&expected_data);
    expect_function_call(testRegisterCallback);

    graph_draw_id_t id = graphRegisterDrawFunction(&drawable);

    assert_int_not_equal(id, GRAPH_DRAW_ID_INVALID);
}

void
testGraphUnregisterDrawFunctionFailNotInitialized(void **status)
{
    (void)status;

    graph_drawable_t drawable = {
        .layer = LAYER_BACKGROUND,
    };

    expect_function_call(__wrap_utilsCalloc);
    graph_draw_id_t id = helperUT_graphGlutInjectDrawable(&drawable);

    helperUT_graphGlutReset();

    assert_int_equal(graphUnregisterDrawFunction(id), -1);
}

void
testGraphUnregisterDrawFunctionSuccess(void **status)
{
    (void)status;

    graph_drawable_t drawable = {
        .layer = LAYER_BACKGROUND,
    };

    expect_function_call(__wrap_utilsCalloc);
    graph_draw_id_t id = helperUT_graphGlutInjectDrawable(&drawable);

    assert_int_equal(graphUnregisterDrawFunction(id), 1);
}

void
testGraphUnregisterDrawFunctionNotFound(void **status)
{
    (void)status;

    assert_int_equal(graphUnregisterDrawFunction(1234), 0);
}

void
testGraphUnregisterDrawFunctionInvalidId(void **status)
{
    (void)status;

    assert_int_equal(graphUnregisterDrawFunction(GRAPH_DRAW_ID_INVALID), -1);
}

void
testGraphUnregisterDrawFunctionCallback(void **status)
{
    (void)status;

    int expected_data = 42;

    graph_drawable_t drawable = {
        .layer = LAYER_BACKGROUND,
        .on_unregister_fn = testUnregisterCallback,
        .on_unregister_data = &expected_data,
    };

    expect_function_call(__wrap_utilsCalloc);
    graph_draw_id_t id = helperUT_graphGlutInjectDrawable(&drawable);

    expect_uint_value(testUnregisterCallback, data, (uintptr_t)&expected_data);
    expect_function_call(testUnregisterCallback);

    assert_int_equal(graphUnregisterDrawFunction(id), 1);
}

void
testGraphUnregisterDrawFunctionKeepsOtherDrawables(void **status)
{
    (void)status;

    graph_drawable_t drawable1 = {
        .layer = LAYER_BACKGROUND,
    };
    graph_drawable_t drawable2 = {
        .layer = LAYER_BACKGROUND,
    };

    expect_function_call(__wrap_utilsCalloc);
    graph_draw_id_t id1 = helperUT_graphGlutInjectDrawable(&drawable1);

    expect_function_call(__wrap_utilsCalloc);
    graph_draw_id_t id2 = helperUT_graphGlutInjectDrawable(&drawable2);

    assert_int_equal(graphUnregisterDrawFunction(id1), 1);
    assert_int_equal(graphUnregisterDrawFunction(id2), 1);
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

static void
expectGraphGlutDisplay(void)
{
    expected_glviewport_t game_vp = {
        .x = 1,
        .y = 2,
        .w = 3,
        .h = 4,
    };

    expected_glviewport_t hud_vp = {
        .x = 5,
        .y = 6,
        .w = 7,
        .h = 8,
    };

    expected_gluortho2d_t game_ortho = {
        .left = 0,
        .right = GAME_WIDTH,
        .bottom = 0,
        .top = GAME_HEIGHT,
    };

    expected_gluortho2d_t hud_ortho = {
        .left = 0,
        .right = GAME_WIDTH,
        .bottom = 0,
        .top = HUD_HEIGHT,
    };

    helperUT_graphGlutSetGameViewport(&game_vp);
    helperUT_graphGlutSetHudViewport(&hud_vp);

    expect_uint_value(__wrap_glClear, mask, GL_COLOR_BUFFER_BIT);
    expect_function_call(__wrap_glClear);

    checkViewport(&game_vp, &game_ortho);

    expect_int_value(__wrap_graphPrintBorder, x, 0);
    expect_int_value(__wrap_graphPrintBorder, y, 0);
    expect_int_value(__wrap_graphPrintBorder, w, GAME_WIDTH);
    expect_int_value(__wrap_graphPrintBorder, h, GAME_HEIGHT);
    expect_function_call(__wrap_graphPrintBorder);

    checkViewport(&hud_vp, &hud_ortho);

    expect_int_value(__wrap_graphPrintBorder, x, 0);
    expect_int_value(__wrap_graphPrintBorder, y, 0);
    expect_int_value(__wrap_graphPrintBorder, w, GAME_WIDTH);
    expect_int_value(__wrap_graphPrintBorder, h, HUD_HEIGHT);
    expect_function_call(__wrap_graphPrintBorder);

    expect_function_call(__wrap_glutSwapBuffers);
}

void
testGraphGlutDisplayDrawLayersOrder(void **status)
{
    (void)status;

    int layer_data[NUM_RENDER_LAYERS];
    graph_drawable_t drawables[NUM_RENDER_LAYERS];

    draw_count = 0;

    for (int layer = 0; layer < NUM_RENDER_LAYERS; layer++)
    {
        layer_data[layer] = layer;

        drawables[layer] = (graph_drawable_t){
            .layer = layer,
            .on_draw_fn = testDrawCallback,
            .on_draw_data = &layer_data[layer],
        };

        expect_function_call(__wrap_utilsCalloc);
        helperUT_graphGlutInjectDrawable(&drawables[layer]);
    }

    expectGraphGlutDisplay();

    graphGlutDisplay();

    assert_int_equal(draw_count, NUM_RENDER_LAYERS);

    for (size_t i = 0; i < ARRAY_SIZE(drawables); i++)
    {
        assert_int_equal(draw_order[i], (int)i);
    }
}

void
testGraphGlutDisplayDrawLayersNullCallback(void **status)
{
    (void)status;

    int expected_draw = 42;

    graph_drawable_t drawable1 = {
        .layer = LAYER_BACKGROUND,
        .on_draw_fn = NULL,
        .on_draw_data = NULL,
    };

    graph_drawable_t drawable2 = {
        .layer = LAYER_EFFECTS,
        .on_draw_fn = testDrawCallback,
        .on_draw_data = &expected_draw,
    };

    draw_count = 0;

    expect_function_call(__wrap_utilsCalloc);
    graphRegisterDrawFunction(&drawable1);

    expect_function_call(__wrap_utilsCalloc);
    graphRegisterDrawFunction(&drawable2);

    expectGraphGlutDisplay();

    graphGlutDisplay();

    assert_int_equal(draw_count, 1);
    assert_int_equal(draw_order[0], expected_draw);
}

void
testGraphGlutDisplayDrawLayersEmpty(void **status)
{
    (void)status;

    draw_count = 0;

    expectGraphGlutDisplay();

    graphGlutDisplay();

    assert_int_equal(draw_count, 0);
}

#if 0
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
    expect_int_value(__wrap_graphPrintBorder, x, 0);
    expect_int_value(__wrap_graphPrintBorder, y, 0);
    expect_int_value(__wrap_graphPrintBorder, w, GAME_WIDTH);
    expect_int_value(__wrap_graphPrintBorder, h, GAME_HEIGHT);
    expect_function_call(__wrap_graphPrintBorder);

    checkViewport(&glviewport_hud, &gluortho2d_hud);
    expect_int_value(__wrap_graphPrintBorder, x, 0);
    expect_int_value(__wrap_graphPrintBorder, y, 0);
    expect_int_value(__wrap_graphPrintBorder, w, GAME_WIDTH);
    expect_int_value(__wrap_graphPrintBorder, h, HUD_HEIGHT);
    expect_function_call(__wrap_graphPrintBorder);

    expect_function_call(__wrap_glutSwapBuffers);

    graphGlutDisplay();
}
#endif

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
