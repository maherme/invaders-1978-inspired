/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2026 Manuel Hernández Méndez
 *
 * Authors:
 *   Manuel Hernández Méndez <maherme.dev@gmail.com>
 */

#include "bunkers.h"
#include "collision.h"
#include "entity_types.h"
#include "explosion.h"
#include "graph.h"
#include "graphGlutCallbacks.h"
#include <assert.h>
#include <string.h>

#define BUNKER_WIDTH 22
#define BUNKER_HEIGHT 16
#define NUM_BUNKERS 4
#define BUNKER_SPACING ((GAME_WIDTH - BUNKER_WIDTH * NUM_BUNKERS) / (NUM_BUNKERS + 1))

static const char bunker_image[BUNKER_HEIGHT][BUNKER_WIDTH][NUM_RGBA_CHANNELS] = {
    {B, B, B, B, G, G, G, G, G, G, G, G, G, G, G, G, G, G, B, B, B, B},
    {B, B, B, G, G, G, G, G, G, G, G, G, G, G, G, G, G, G, G, B, B, B},
    {B, B, G, G, G, G, G, G, G, G, G, G, G, G, G, G, G, G, G, G, B, B},
    {B, G, G, G, G, G, G, G, G, G, G, G, G, G, G, G, G, G, G, G, G, B},
    {G, G, G, G, G, G, G, G, G, G, G, G, G, G, G, G, G, G, G, G, G, G},
    {G, G, G, G, G, G, G, G, G, G, G, G, G, G, G, G, G, G, G, G, G, G},
    {G, G, G, G, G, G, G, G, G, G, G, G, G, G, G, G, G, G, G, G, G, G},
    {G, G, G, G, G, G, G, G, G, G, G, G, G, G, G, G, G, G, G, G, G, G},
    {G, G, G, G, G, G, G, G, G, G, G, G, G, G, G, G, G, G, G, G, G, G},
    {G, G, G, G, G, G, G, G, G, G, G, G, G, G, G, G, G, G, G, G, G, G},
    {G, G, G, G, G, G, G, G, G, G, G, G, G, G, G, G, G, G, G, G, G, G},
    {G, G, G, G, G, G, G, G, G, G, G, G, G, G, G, G, G, G, G, G, G, G},
    {G, G, G, G, G, G, G, B, B, B, B, B, B, B, B, G, G, G, G, G, G, G},
    {G, G, G, G, G, G, B, B, B, B, B, B, B, B, B, B, G, G, G, G, G, G},
    {G, G, G, G, G, B, B, B, B, B, B, B, B, B, B, B, B, G, G, G, G, G},
    {G, G, G, G, G, B, B, B, B, B, B, B, B, B, B, B, B, G, G, G, G, G}};

struct bunker_instance_t
{
    sprite_t sprite;
    char image[BUNKER_HEIGHT][BUNKER_WIDTH][NUM_RGBA_CHANNELS];
    collision_handle_t collision_handle;
};

static struct bunker_instance_t bunkerPool[NUM_BUNKERS];

void
bunkersCreate(void)
{
    for (int i = 0; i < NUM_BUNKERS; i++)
    {
        bunker_t bunker = &bunkerPool[i];
        bunker->sprite.x = BUNKER_SPACING * (i + 1) + BUNKER_WIDTH * i;
        bunker->sprite.y = GAME_HEIGHT / 16;
        bunker->sprite.width = BUNKER_WIDTH;
        bunker->sprite.height = BUNKER_HEIGHT;
        memcpy(bunker->image, bunker_image, sizeof(bunker_image));
        bunker->sprite.image = (char *)bunker->image;
        bunker->sprite.layer = LAYER_BACKGROUND;
        bunker->sprite.visible = true;
        graphRegisterPrint(&bunker->sprite);

        collider_t col = {
            .x = &bunker->sprite.x,
            .y = &bunker->sprite.y,
            .width = bunker->sprite.width,
            .height = bunker->sprite.height,
            .pixel_data_ref = (const char **)&bunker->sprite.image,
            .entity = ENTITY_BUNKER,
            .owner = bunker,
        };

        bunker->collision_handle = collisionRegisterCollider(&col);
    }
}

void
bunkersOnCollision(void *data)
{
    collision_event_t *collision_data = (collision_event_t *)data;
    bunker_t bunker = NULL;
    struct
    {
        entity_type_t entity_type;
        collision_bounds_t bounds;
    } other = {0};

    if (collision_data->entity_a == ENTITY_BUNKER)
    {
        bunker = (bunker_t)collision_data->owner_a;
        other.entity_type = collision_data->entity_b;
        other.bounds = collision_data->bounds_b;
    }
    else if (collision_data->entity_b == ENTITY_BUNKER)
    {
        bunker = (bunker_t)collision_data->owner_b;
        other.entity_type = collision_data->entity_a;
        other.bounds = collision_data->bounds_a;
    }

    if (!bunker || (other.entity_type != ENTITY_SPACESHIP_BULLET && other.entity_type != ENTITY_ALIEN_BULLET))
    {
        return;
    }

    sprite_t mask = *explosionsGetStaticFrame(other.entity_type, 0);
    mask.x = other.bounds.x - mask.width / 2;
    mask.y = other.bounds.y;
    if (other.entity_type == ENTITY_ALIEN_BULLET)
    {
        mask.y -= other.bounds.height / 2;
    }

    graphMakeFootprintSprite(&bunker->sprite, &mask);
    graphUpdateSprite(&bunker->sprite);
}

#ifdef UNIT_TESTING
void
helperUT_bunkersInitPool(void)
{
    memset(bunkerPool, 0, sizeof(bunkerPool));
}

bunker_t
helperUT_bunkersInjectInPool(int index)
{
    assert(index >= 0);
    assert(index < NUM_BUNKERS);

    bunker_t bunker = &bunkerPool[index];
    memset(bunker, 0, sizeof(*bunker));

    bunker->sprite.width = BUNKER_WIDTH;
    bunker->sprite.height = BUNKER_HEIGHT;
    bunker->sprite.image = (char *)bunker->image;

    return bunker;
}

sprite_t *
helperUT_bunkersGetSprite(bunker_t bunker)
{
    return &bunker->sprite;
}
#endif /* UNIT_TESTING */
