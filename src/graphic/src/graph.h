/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2025 Manuel Hernández Méndez
 *
 * Authors:
 *   Manuel Hernández Méndez <maherme.dev@gmail.com>
 */

#ifndef __GRAPH_H__
#define __GRAPH_H__

#include "graphGlutCallbacks.h"
#include <GL/glut.h>
#include <stdbool.h>
#include <stdint.h>

#define GAME_WIDTH 224
#define GAME_HEIGHT 256
#define HUD_HEIGHT 16

#define NUM_RGBA_CHANNELS 4
#define W {(uint8_t)255, (uint8_t)255, (uint8_t)255, (uint8_t)255}
#define R {(uint8_t)255, 0, 0, (uint8_t)255}
#define G {0, (uint8_t)255, 0, (uint8_t)255}
#define B {0, 0, 0, 0}

typedef struct
{
    int up;
    int down;
    int right;
    int left;
} max_movement_t;

typedef struct
{
    int x1;
    int x2;
    int y1;
    int y2;
} sprite_coordinates_t;

typedef struct sprite
{
    int x;
    int y;
    int width;
    int height;
    const char *image_base;
    const char *image;
    render_layer_t layer;
    int selected_image;
    int num_frames;
    GLuint textureId;
    struct timespec last_update;
    long long time_to_move;
    int pixels_to_move;
    max_movement_t max_movement;
    bool visible;
} sprite_t;

typedef struct
{
    sprite_t sprite;
} base_t;

void
graphCreateImage(sprite_t *sprite);

void
graphDestroyImage(sprite_t *sprite);

sprite_t *
graphGetSprite(base_t *b);

int
graphGetSpriteCoordinates(const sprite_t *const sprite, sprite_coordinates_t *coordinates);

void
graphNextImageToPrint(sprite_t *sprite);

void
graphUpdateTimeSprite(sprite_t *sprite);

void
graphPrintImage(const sprite_t *const sprite);

void
graphUpdateSprite(const sprite_t *const sprite);

void
graphPrintBorder(int x, int y, int w, int h);

void
graphMakeFootprintSprite(const sprite_t *const sprite1, const sprite_t *const sprite2);

#endif
