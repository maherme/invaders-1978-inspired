/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2025 Manuel Hernández Méndez
 *
 * Authors:
 *   Manuel Hernández Méndez <maherme.dev@gmail.com>
 */

#include "spaceship.h"
#include "collision.h"
#include "entity_types.h"
#include "events.h"
#include "graph.h"
#include "graphGlutCallbacks.h"
#include "utils.h"
#include <stdbool.h>

#define SPACESHIP_WIDTH 13
#define SPACESHIP_HEIGHT 8

struct spaceship_instance_t
{
    sprite_t sprite;
    bool alive;
    collision_handle_t collision_handle;
    void (*death_callback)(void);
    graph_draw_id_t draw_id;
};

static struct spaceship_instance_t spaceship;

static const char spaceshipImage[SPACESHIP_HEIGHT][SPACESHIP_WIDTH][NUM_RGBA_CHANNELS] = {
    {B, B, B, B, B, B, G, B, B, B, B, B, B},
    {B, B, B, B, B, G, G, G, B, B, B, B, B},
    {B, B, B, B, B, G, G, G, B, B, B, B, B},
    {B, G, G, G, G, G, G, G, G, G, G, G, B},
    {G, G, G, G, G, G, G, G, G, G, G, G, G},
    {G, G, G, G, G, G, G, G, G, G, G, G, G},
    {B, B, G, G, B, B, B, B, B, G, G, B, B},
    {B, G, G, G, G, B, B, B, G, G, G, G, B},
};

spaceship_t
spaceshipCreate(int x, int y, void (*death_callback)(void))
{
    spaceship_t inst = &spaceship;
    inst->sprite.x = x;
    inst->sprite.y = y;
    inst->sprite.width = SPACESHIP_WIDTH;
    inst->sprite.height = SPACESHIP_HEIGHT;
    inst->sprite.image = (const char *)spaceshipImage;
    inst->sprite.pixels_to_move = 1;
    inst->sprite.time_to_move = 1000 / 60 * NS_PER_MS;
    inst->sprite.max_movement.right = GAME_WIDTH;
    inst->sprite.max_movement.left = 0;
    inst->sprite.visible = true;

    graph_drawable_t drawable = {
        .on_register_fn = graphRegisterDrawSprite,
        .on_register_data = &inst->sprite,
        .on_draw_fn = graphDrawSprite,
        .on_draw_data = &inst->sprite,
        .on_unregister_fn = graphUnregisterDrawSprite,
        .on_unregister_data = &inst->sprite,
        .layer = LAYER_GAME_OBJECTS,
    };

    inst->draw_id = graphRegisterDrawFunction(&drawable);

    inst->alive = true;
    inst->death_callback = death_callback;

    collider_t col = {
        .x = &inst->sprite.x,
        .y = &inst->sprite.y,
        .width = inst->sprite.width,
        .height = inst->sprite.height,
        .pixel_data_ref = (const char **)&inst->sprite.image,
        .entity = ENTITY_SPACESHIP,
        .owner = inst,
    };

    inst->collision_handle = collisionRegisterCollider(&col);

    return inst;
}

static void
spaceshipDestroy(spaceship_t spaceship)
{
    graphUnregisterDrawFunction(spaceship->draw_id);
    collisionUnregisterCollider(spaceship->collision_handle);
    spaceship->alive = false;
}

bool
spaceshipAlive(spaceship_t spaceship)
{
    return (spaceship && spaceship->alive);
}

void
spaceshipOnCollision(void *data)
{
    collision_event_t *collision_data = (collision_event_t *)data;
    spaceship_t spaceship = NULL;
    collision_bounds_t bounds = {0};

    if (collision_data->entity_a == ENTITY_SPACESHIP)
    {
        spaceship = (spaceship_t)collision_data->owner_a;
        bounds = collision_data->bounds_a;
    }
    else if (collision_data->entity_b == ENTITY_SPACESHIP)
    {
        spaceship = (spaceship_t)collision_data->owner_b;
        bounds = collision_data->bounds_b;
    }

    if (spaceship)
    {
        entity_death_event_t death_data = {
            .x = bounds.x,
            .y = bounds.y,
            .width = bounds.width,
            .height = bounds.height,
            .entity_type = ENTITY_SPACESHIP,
            .on_finish = spaceship->death_callback,
        };
        eventEmit(EVENT_ENTITY_DIED, &death_data);
        spaceshipDestroy(spaceship);
    }
}

#ifdef UNIT_TESTING
#include <string.h>

spaceship_t
helperUT_spaceshipGetInstance(void)
{
    spaceship_t inst = &spaceship;
    memset(inst, 0, sizeof(*inst));

    inst->alive = true;

    return inst;
}

void
helperUT_spaceshipSetAlive(spaceship_t spaceship, bool alive)
{
    spaceship->alive = alive;
}
#endif /* UNIT_TESTING */
