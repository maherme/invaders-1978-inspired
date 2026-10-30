/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2024 Manuel Hernández Méndez
 *
 * Authors:
 *   Manuel Hernández Méndez <maherme.dev@gmail.com>
 */

#ifndef __GRAPHGLUTCALLBACKS_H__
#define __GRAPHGLUTCALLBACKS_H__

#include <stdint.h>

#define GRAPH_DRAW_ID_INVALID UINT32_MAX

struct sprite;
typedef struct sprite sprite_t;

typedef uint32_t graph_draw_id_t;

typedef enum
{
    LAYER_BACKGROUND = 0,
    LAYER_GAME_OBJECTS,
    LAYER_EFFECTS,
    LAYER_HUD,
    NUM_RENDER_LAYERS,
} render_layer_t;

typedef void (*graph_on_register_fn)(void *on_register_data);
typedef void (*graph_on_draw_fn)(void *on_draw_data);
typedef void (*graph_on_unregister_fn)(void *on_unregister_data);

typedef struct
{
    graph_on_register_fn on_register_fn;
    void *on_register_data;
    graph_on_draw_fn on_draw_fn;
    void *on_draw_data;
    graph_on_unregister_fn on_unregister_fn;
    void *on_unregister_data;
    render_layer_t layer;
} graph_drawable_t;

void
graphRegisterDrawInit(void);

graph_draw_id_t
graphRegisterDrawFunction(graph_drawable_t *drawable);

int
graphUnregisterDrawFunction(graph_draw_id_t id);

void
graphGlutDisplay(void);

void
graphGlutReshape(int w, int h);

void
graphRegisterDrawSprite(void *data);

void
graphDrawSprite(void *data);

void
graphUnregisterDrawSprite(void *data);

#ifdef UNIT_TESTING
void
helperUT_graphGlutResetRegisteredDrawables(void);

void
helperUT_graphGlutInjectDrawable(graph_drawable_t *drawable);

void
helperUT_graphGlutSetGameViewport(void *game_vp);

void
helperUT_graphGlutSetHudViewport(void *hud_vp);

void *
helperUT_graphGlutGetGameViewport(void);

void *
helperUT_graphGlutGetHudViewport(void);
#endif

#endif
