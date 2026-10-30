/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2025 Manuel Hernández Méndez
 *
 * Authors:
 *   Manuel Hernández Méndez <maherme.dev@gmail.com>
 */

#include "graphGlutCallbacks.h"
#include "graph.h"
#include "list.h"
#include "utils.h"
#include <GL/glut.h>
#include <stdbool.h>

static bool graph_draw_initialized = false;
static int next_draw_id = 0;

typedef struct ctx_node
{
    graph_draw_id_t id;
    graph_on_draw_fn on_draw_fn;
    void *on_draw_data;
    graph_on_unregister_fn on_unregister_fn;
    void *on_unregister_data;
    struct list_head node;
} ctx_node_t;

struct list_head render_layers[NUM_RENDER_LAYERS];

typedef struct viewport
{
    int x, y;
    int w, h;
} viewport_t;

static struct
{
    int width;
    int height;
    viewport_t game_vp;
    viewport_t hud_vp;
} window;

void
graphRegisterDrawInit(void)
{
    if (graph_draw_initialized)
    {
        return;
    }

    for (int i = 0; i < NUM_RENDER_LAYERS; i++)
    {
        init_list_head(&render_layers[i]);
    }

    graph_draw_initialized = true;
}

graph_draw_id_t
graphRegisterDrawFunction(graph_drawable_t *drawable)
{
    if (!graph_draw_initialized || !drawable || drawable->layer < 0 || drawable->layer >= NUM_RENDER_LAYERS)
    {
        return GRAPH_DRAW_ID_INVALID;
    }

    ctx_node_t *n = utilsCalloc(1, sizeof(*n));
    n->on_draw_fn = drawable->on_draw_fn;
    n->on_draw_data = drawable->on_draw_data;
    n->on_unregister_fn = drawable->on_unregister_fn;
    n->on_unregister_data = drawable->on_unregister_data;
    n->id = next_draw_id++;
    list_add_tail(&n->node, &render_layers[drawable->layer]);

    if (drawable->on_register_fn)
    {
        drawable->on_register_fn(drawable->on_register_data);
    }

    return n->id;
}

int
graphUnregisterDrawFunction(graph_draw_id_t id)
{
    int removed = 0;

    if (!graph_draw_initialized || id == GRAPH_DRAW_ID_INVALID)
    {
        return -1;
    }

    for (int i = 0; i < NUM_RENDER_LAYERS; i++)
    {
        ctx_node_t *n, *tmp;

        list_for_each_entry_safe(n, tmp, &render_layers[i], node)
        {
            if (n->id == id)
            {
                list_del(&n->node);
                if (n->on_unregister_fn)
                {
                    n->on_unregister_fn(n->on_unregister_data);
                }
                UTILS_FREE(n);
                removed++;
            }
        }
    }

    return removed;
}

static void
drawLayers(int first, int last)
{
    for (int i = first; i <= last; i++)
    {
        ctx_node_t *n;
        list_for_each_entry(n, &render_layers[i], node)
        {
            if (n->on_draw_fn)
            {
                n->on_draw_fn(n->on_draw_data);
            }
        }
    }
}

static void
setupGameViewport(void)
{
    glViewport(window.game_vp.x, window.game_vp.y, window.game_vp.w, window.game_vp.h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, GAME_WIDTH, 0, GAME_HEIGHT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

static void
setupHudViewport(void)
{
    glViewport(window.hud_vp.x, window.hud_vp.y, window.hud_vp.w, window.hud_vp.h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, GAME_WIDTH, 0, HUD_HEIGHT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void
graphGlutDisplay(void)
{
    glClear(GL_COLOR_BUFFER_BIT);

    setupGameViewport();
    drawLayers(LAYER_BACKGROUND, LAYER_EFFECTS);
    graphPrintBorder(0, 0, GAME_WIDTH, GAME_HEIGHT);

    setupHudViewport();
    drawLayers(LAYER_HUD, LAYER_HUD);
    graphPrintBorder(0, 0, GAME_WIDTH, HUD_HEIGHT);

    glutSwapBuffers();
}

static void
calculateViewports(void)
{
    int scale_x = window.width / GAME_WIDTH;
    int scale_y = window.height / (GAME_HEIGHT + HUD_HEIGHT);
    int scale = scale_x < scale_y ? scale_x : scale_y;
    if (scale < 1)
    {
        scale = 1;
    }

    int game_width = GAME_WIDTH * scale;
    int game_height = GAME_HEIGHT * scale;
    int hud_width = game_width;
    int hud_height = HUD_HEIGHT * scale;

    int vp_x = (window.width - game_width) / 2;
    int vp_y = (window.height - (game_height + hud_height)) / 2;

    window.game_vp.x = vp_x;
    window.game_vp.y = hud_height + vp_y;
    window.game_vp.w = game_width;
    window.game_vp.h = game_height;

    window.hud_vp.x = vp_x;
    window.hud_vp.y = vp_y;
    window.hud_vp.w = hud_width;
    window.hud_vp.h = hud_height;
}

void
graphGlutReshape(int w, int h)
{
    window.width = w;
    window.height = h;

    calculateViewports();
}

void
graphRegisterDrawSprite(void *data)
{
    graphCreateImage((sprite_t *)data);
}

void
graphDrawSprite(void *data)
{
    graphPrintImage((sprite_t *)data);
}

void
graphUnregisterDrawSprite(void *data)
{
    graphDestroyImage((sprite_t *)data);
}

#ifdef UNIT_TESTING
void
helperUT_graphGlutResetRegisteredDrawables(void)
{
    for (int i = 0; i < NUM_RENDER_LAYERS; i++)
    {
        list_clear_with_free(&render_layers[i], ctx_node_t, node);
    }
}

void
helperUT_graphGlutInjectDrawable(graph_drawable_t *drawable)
{
    ctx_node_t *n = utilsCalloc(1, sizeof(*n));
    n->on_draw_fn = drawable->on_draw_fn;
    n->on_draw_data = drawable->on_draw_data;
    n->on_unregister_fn = drawable->on_unregister_fn;
    n->on_unregister_data = drawable->on_unregister_data;
    list_add(&n->node, &render_layers[drawable->layer]);
}

void
helperUT_graphGlutSetGameViewport(void *game_vp)
{
    window.game_vp = *(viewport_t *)game_vp;
}

void
helperUT_graphGlutSetHudViewport(void *hud_vp)
{
    window.hud_vp = *(viewport_t *)hud_vp;
}

void *
helperUT_graphGlutGetGameViewport(void)
{
    return &window.game_vp;
}

void *
helperUT_graphGlutGetHudViewport(void)
{
    return &window.hud_vp;
}
#endif
