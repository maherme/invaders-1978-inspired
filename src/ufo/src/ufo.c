/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2026 Manuel Hernández Méndez
 *
 * Authors:
 *   Manuel Hernández Méndez <maherme.dev@gmail.com>
 */

#include "ufo.h"
#include "collision.h"
#include "entity_types.h"
#include "events.h"
#include "graph.h"
#include "graphGlutCallbacks.h"
#include "physic.h"
#include "utils.h"
#include <stdbool.h>
#include <stdlib.h>

#define UFO_WIDTH 16
#define UFO_HEIGHT 8
#define UFO_INITIAL_POINTS 50

struct ufo_instance_t
{
    sprite_t sprite;
    direction_t direction;
    long long time_to_appear;
    bool alive;
    unsigned int points;
    collision_handle_t collision_handle;
};

static struct ufo_instance_t ufo;

static const char ufoImage[UFO_HEIGHT][UFO_WIDTH][NUM_RGBA_CHANNELS] = {
    {B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B},
    {B, B, B, B, R, R, R, R, R, R, R, R, B, B, B, B},
    {B, B, B, R, R, R, R, R, R, R, R, R, R, B, B, B},
    {B, B, B, R, B, R, B, R, R, B, R, B, R, B, B, B},
    {B, R, R, R, R, R, R, R, R, R, R, R, R, R, R, B},
    {R, R, R, R, R, R, R, R, R, R, R, R, R, R, R, R},
    {B, B, B, B, B, R, R, R, R, R, R, B, B, B, B, B},
    {B, B, B, B, B, B, B, R, R, B, B, B, B, B, B, B},
};

static void
calculateDirection(ufo_t ufo)
{
    double random = (double)rand() / RAND_MAX;
    int random_direction = (random < 0.5) ? 0 : 1;
    if (random_direction)
    {
        ufo->direction = RIGHT;
    }
    else
    {
        ufo->direction = LEFT;
    }
}

static void
calculateStartingCoord(ufo_t ufo)
{
    if (ufo->direction == RIGHT)
    {
        ufo->sprite.x = -ufo->sprite.width;
    }
    else
    {
        ufo->sprite.x = GAME_WIDTH;
    }

    ufo->sprite.y = GAME_HEIGHT - 16;
}

ufo_t
ufoCreate(void)
{
    ufo_t inst = &ufo;
    calculateDirection(inst);
    inst->sprite.width = UFO_WIDTH;
    inst->sprite.height = UFO_HEIGHT;
    inst->sprite.image = (const char *)ufoImage;
    inst->sprite.pixels_to_move = 1;
    inst->sprite.time_to_move = 1000 / 60 * NS_PER_MS;
    inst->time_to_appear = 5 * NS_PER_S;
    calculateStartingCoord(inst);
    inst->sprite.max_movement.right = GAME_WIDTH + inst->sprite.width + 1;
    inst->sprite.max_movement.left = -(inst->sprite.width + 1);
    inst->sprite.layer = LAYER_GAME_OBJECTS;
    inst->sprite.visible = true;
    graphUpdateTimeSprite(&inst->sprite);
    graphRegisterPrint(&inst->sprite);
    inst->alive = true;
    inst->points = UFO_INITIAL_POINTS;

    collider_t col = {
        .x = &inst->sprite.x,
        .y = &inst->sprite.y,
        .width = inst->sprite.width,
        .height = inst->sprite.height,
        .pixel_data_ref = (const char **)&inst->sprite.image,
        .entity = ENTITY_UFO,
        .owner = inst,
    };

    inst->collision_handle = collisionRegisterCollider(&col);

    return inst;
}

void
ufoDestroy(ufo_t ufo)
{
    if (!ufo)
    {
        return;
    }

    sprite_t *ufo_sprite = graphGetSprite((base_t *)ufo);
    graphUnregisterPrint(ufo_sprite);
    collisionUnregisterCollider(ufo->collision_handle);
    ufo->alive = false;
}

bool
ufoAlive(ufo_t ufo)
{
    return ufo && ufo->alive;
}

direction_t
ufoGetDirection(const ufo_t ufo)
{
    if (!ufo)
    {
        return INVALID_DIR;
    }
    return ufo->direction;
}

bool
ufoCheckForMoving(ufo_t ufo)
{
    if (!ufo || !ufo->alive)
    {
        return false;
    }

    if (ufo->time_to_appear == 0)
    {
        return true;
    }

    if (!utilsCheckTimeout(ufo->sprite.last_update, ufo->time_to_appear))
    {
        return false;
    }
    else
    {
        ufo->time_to_appear = 0;
    }

    return true;
}

unsigned int
ufoGetPoints(void)
{
    return ufo.points;
}

void
ufoOnDangerLevelChanged(void *data)
{
    danger_level_t level = *(danger_level_t *)data;

    switch (level)
    {
        case DANGER_HIGH:
            ufo.points = 300;
            break;
        case DANGER_MID:
            ufo.points = 150;
            break;
        case DANGER_LOW:
        default:
            ufo.points = UFO_INITIAL_POINTS;
            break;
    }
}

void
ufoOnCollision(void *data)
{
    collision_event_t *collision_data = (collision_event_t *)data;
    ufo_t ufo = NULL;
    collision_bounds_t bounds = {0};

    if (collision_data->entity_a == ENTITY_UFO)
    {
        ufo = (ufo_t)collision_data->owner_a;
        bounds = collision_data->bounds_a;
    }
    else if (collision_data->entity_b == ENTITY_UFO)
    {
        ufo = (ufo_t)collision_data->owner_b;
        bounds = collision_data->bounds_b;
    }

    if (ufo)
    {
        entity_death_event_t death_data = {
            .x = bounds.x,
            .y = bounds.y,
            .width = bounds.width,
            .height = bounds.height,
            .points = ufo->points,
            .entity_type = ENTITY_UFO,
        };
        eventEmit(EVENT_ENTITY_DIED, &death_data);
        ufoDestroy(ufo);
    }
}

#ifdef UNIT_TESTING
#include <string.h>

ufo_t
helperUT_ufoGetInstance(void)
{
    ufo_t inst = &ufo;
    memset(inst, 0, sizeof(*inst));

    inst->alive = true;

    return inst;
}

void
helperUT_ufoSetAlive(ufo_t ufo, bool alive)
{
    ufo->alive = alive;
}

void
helperUT_ufoSetTimeToAppear(ufo_t ufo, long long time)
{
    ufo->time_to_appear = time;
}

void
helperUT_ufoSetPoints(unsigned int points)
{
    ufo.points = points;
}

unsigned int
helperUT_ufoGetPoints()
{
    return ufo.points;
}
#endif /* UNIT_TESTING */
