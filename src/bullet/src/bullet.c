/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2026 Manuel Hernández Méndez
 *
 * Authors:
 *   Manuel Hernández Méndez <maherme.dev@gmail.com>
 */

#include "bullet.h"
#include "collision.h"
#include "entity_types.h"
#include "events.h"
#include "graph.h"
#include "graphGlutCallbacks.h"
#include "utils.h"
#include <assert.h>
#include <stdbool.h>

#define MAX_BULLETS 4
#define BULLET_SPACESHIP_WIDTH 1
#define BULLET_SPACESHIP_HEIGHT 4
#define BULLET_ALIEN_WIDTH 3
#define BULLET_ALIEN_HEIGHT 7
#define BULLET_ALIEN_NUM_FRAMES 4
#define BULLET_SPACESHIP_SLOT 0
#define BULLET_ALIEN_SLOT 1

typedef enum
{
    CRACKLE,
    PLASMA,
    COIL,
    BULLET_ALIEN_NUM_TYPES
} bullet_alien_type_t;

struct bullet_instance
{
    sprite_t sprite;
    entity_type_t type;
    bool used;
    collision_handle_t collision_handle;
};

typedef struct
{
    const char *image;
    long long time_to_move;
    int pixels_to_move;
} bullet_alien_t;

typedef struct
{
    int width, height;
    const char *image_base, *image;
    int num_frames;
    int pixels_to_move;
    int max_movement_up, max_movement_down;
    long long time_to_move;
} bullet_config_t;

static struct
{
    struct bullet_instance bullets[MAX_BULLETS];
    bool inhibitBulletAlien;
} bulletPool;

static const char bulletSpaceshipImage[BULLET_SPACESHIP_HEIGHT][BULLET_SPACESHIP_WIDTH][NUM_RGBA_CHANNELS] = {
    {W}, {W}, {W}, {W}};
static const char bulletAlienImage[BULLET_ALIEN_NUM_TYPES][BULLET_ALIEN_NUM_FRAMES][BULLET_ALIEN_HEIGHT][BULLET_ALIEN_WIDTH]
                                  [NUM_RGBA_CHANNELS] = {
                                      /* crackle */
                                      /* frame 1 */
                                      {{{B, W, B}, {B, W, B}, {W, B, B}, {B, W, B}, {B, B, W}, {B, W, B}, {B, W, B}},
                                       /* frame 2 */
                                       {{B, W, B}, {B, W, B}, {B, W, B}, {B, W, B}, {B, W, B}, {B, W, B}, {B, W, B}},
                                       /* frame 3 */
                                       {{B, W, B}, {B, W, B}, {B, B, W}, {B, W, B}, {W, B, B}, {B, W, B}, {B, W, B}},
                                       /* frame 4 */
                                       {{B, W, B}, {B, W, B}, {B, W, B}, {B, W, B}, {B, W, B}, {B, W, B}, {B, W, B}}},
                                      /* plasma */
                                      /* frame 1 */
                                      {{{B, W, B}, {B, W, B}, {B, W, B}, {B, W, B}, {B, W, B}, {W, W, W}, {W, W, W}},
                                       /* frame 2 */
                                       {{B, W, B}, {B, W, B}, {B, W, B}, {W, W, W}, {B, W, B}, {B, W, B}, {W, W, W}},
                                       /* frame 3 */
                                       {{B, W, B}, {W, W, W}, {B, W, B}, {B, W, B}, {B, W, B}, {B, W, B}, {W, W, W}},
                                       /* frame 4 */
                                       {{B, W, B}, {B, W, B}, {B, W, B}, {W, W, W}, {B, W, B}, {B, W, B}, {W, W, W}}},
                                      /* coil */
                                      /* frame 1 */
                                      {{{B, W, W}, {B, W, B}, {W, W, B}, {B, W, B}, {B, W, W}, {B, W, B}, {W, W, B}},
                                       /* frame 2 */
                                       {{B, W, B}, {W, W, B}, {B, W, B}, {B, W, W}, {B, W, B}, {W, W, B}, {B, W, B}},
                                       /* frame 3 */
                                       {{W, W, B}, {B, W, B}, {B, W, W}, {B, W, B}, {W, W, B}, {B, W, B}, {B, W, W}},
                                       /* frame 4 */
                                       {{B, W, B}, {B, W, W}, {B, W, B}, {W, W, B}, {B, W, B}, {B, W, W}, {B, W, B}}}};

static const bullet_config_t bullet_configs[] = {
    /* clang-format off */
    [ENTITY_SPACESHIP_BULLET] = {
        .height = BULLET_SPACESHIP_HEIGHT,
        .width = BULLET_SPACESHIP_WIDTH,
        .image = (const char *)bulletSpaceshipImage,
        .image_base = NULL,
        .num_frames = 0,
        .pixels_to_move = 1,
        .max_movement_down = 0,
        .max_movement_up = GAME_HEIGHT,
        .time_to_move = 2 * NS_PER_MS,
    },
    [ENTITY_ALIEN_BULLET] = {
        .height = BULLET_ALIEN_HEIGHT,
        .width = BULLET_ALIEN_WIDTH,
        .image = NULL,
        .image_base = NULL,
        .num_frames = BULLET_ALIEN_NUM_FRAMES,
        .pixels_to_move = 0,
        .max_movement_up = GAME_HEIGHT,
        .max_movement_down = 0,
        .time_to_move = 0,
    }};
/* clang-format on */

static bullet_alien_t bulletAlien[BULLET_ALIEN_NUM_TYPES] = {
    {.image = (const char *)&bulletAlienImage[CRACKLE], .time_to_move = 1000 / 60 * NS_PER_MS, .pixels_to_move = 1},
    {.image = (const char *)&bulletAlienImage[PLASMA], .time_to_move = 2000 / 60 * NS_PER_MS, .pixels_to_move = 1},
    {.image = (const char *)&bulletAlienImage[COIL], .time_to_move = 1000 / 60 * NS_PER_MS, .pixels_to_move = 1}};

static bullet_alien_type_t
getBulletAlienType(void)
{
    bullet_alien_type_t result;
    int r = rand() % 100;

    if (r < 50)
    {
        result = CRACKLE;
    }
    else if (r < 80)
    {
        result = COIL;
    }
    else
    {
        result = PLASMA;
    }

    return result;
}

static void
setBulletType(bullet_t bullet, entity_type_t type)
{
    bullet->sprite.width = bullet_configs[type].width;
    bullet->sprite.height = bullet_configs[type].height;
    bullet->sprite.num_frames = bullet_configs[type].num_frames;
    bullet->sprite.max_movement.up = bullet_configs[type].max_movement_up;
    bullet->sprite.max_movement.down = bullet_configs[type].max_movement_down;
    bullet->sprite.time_to_move = bullet_configs[type].time_to_move;
    bullet->sprite.pixels_to_move = bullet_configs[type].pixels_to_move;
    bullet->sprite.image_base = bullet_configs[type].image_base;
    bullet->sprite.image = bullet_configs[type].image;

    if (type == ENTITY_ALIEN_BULLET)
    {
        bullet_alien_type_t alien_bullet_type = getBulletAlienType();
        bullet->sprite.time_to_move = bulletAlien[alien_bullet_type].time_to_move;
        bullet->sprite.pixels_to_move = bulletAlien[alien_bullet_type].pixels_to_move;
        bullet->sprite.image_base = bulletAlien[alien_bullet_type].image;
        bullet->sprite.image = bullet->sprite.image_base;
    }

    bullet->type = type;
}

void
bulletCreate(int x, int y, entity_type_t type)
{
    if (type != ENTITY_SPACESHIP_BULLET && type != ENTITY_ALIEN_BULLET)
    {
        return;
    }

    for (int i = 0; i < MAX_BULLETS; i++)
    {
        if (type == ENTITY_SPACESHIP_BULLET && i != BULLET_SPACESHIP_SLOT)
        {
            continue;
        }

        if (type == ENTITY_ALIEN_BULLET && (i == BULLET_SPACESHIP_SLOT || bulletPool.inhibitBulletAlien))
        {
            continue;
        }

        if (!bulletPool.bullets[i].used)
        {
            struct bullet_instance *inst = &bulletPool.bullets[i];

            setBulletType(inst, type);
            inst->sprite.x = x;
            inst->sprite.y = y;
            inst->sprite.layer = LAYER_GAME_OBJECTS;
            inst->sprite.visible = true;
            graphRegisterPrint(&inst->sprite);

            inst->used = true;

            collider_t col = {
                .x = &inst->sprite.x,
                .y = &inst->sprite.y,
                .width = inst->sprite.width,
                .height = inst->sprite.height,
                .pixel_data_ref = (const char **)&inst->sprite.image,
                .owner = inst,
                .entity = type,
            };

            inst->collision_handle = collisionRegisterCollider(&col);

            return;
        }
    }
}

void
bulletDestroy(bullet_t bullet)
{
    if (!bullet)
    {
        return;
    }

    sprite_t *bullet_sprite = graphGetSprite((base_t *)bullet);
    graphUnregisterPrint(bullet_sprite);
    collisionUnregisterCollider(bullet->collision_handle);
    bullet->used = false;
}

bool
bulletUsed(bullet_t bullet)
{
    return bullet && bullet->used;
}

int
bulletGetType(const bullet_t bullet, entity_type_t *type)
{
    if (!bullet || !type)
    {
        return -1;
    }

    *type = bullet->type;

    return 0;
}

void
bulletCallFunctionForEach(void (*fn)(bullet_t))
{
    if (!fn)
    {
        assert(!"pointer to fn must not be NULL");
        return;
    }

    for (int i = 0; i < MAX_BULLETS; i++)
    {
        if (bulletPool.bullets[i].used)
        {
            fn(&bulletPool.bullets[i]);
        }
    }
}

void
bulletAlienInhibit(bool inhibit)
{
    bulletPool.inhibitBulletAlien = inhibit;
}

bool
bulletNoneUsed(void)
{
    for (int i = 0; i < MAX_BULLETS; i++)
    {
        if (bulletPool.bullets[i].used)
        {
            return false;
        }
    }

    return true;
}

bool
bulletSpaceshipHitBulletAliens(bool (*fn)(const sprite_t *const sprite1, const sprite_t *const sprite2))
{
    for (int i = BULLET_ALIEN_SLOT; i < MAX_BULLETS; i++)
    {
        if (bulletPool.bullets[i].used)
        {
            if (fn(&bulletPool.bullets[BULLET_SPACESHIP_SLOT].sprite, &bulletPool.bullets[i].sprite))
            {
                bulletDestroy(&bulletPool.bullets[i]);
                return true;
            }
        }
    }

    return false;
}

static void
processBulletCollison(
    entity_type_t my_type, void *owner, collision_bounds_t my_bounds, entity_type_t other_type, bool should_explode)
{
    if (my_type != ENTITY_SPACESHIP_BULLET && my_type != ENTITY_ALIEN_BULLET)
    {
        return;
    }

    if (owner)
    {
        if (should_explode)
        {
            entity_death_event_t death = {
                .x = my_bounds.x,
                .y = my_bounds.y,
                .width = my_bounds.width,
                .height = my_bounds.height,
                .entity_type = my_type,
            };

            if (my_type == ENTITY_ALIEN_BULLET && other_type == ENTITY_BUNKER)
            {
                death.y -= my_bounds.height / 2;
            }

            eventEmit(EVENT_ENTITY_DIED, &death);
        }
        bulletDestroy((bullet_t)owner);
    }
}

void
bulletOnCollision(void *data)
{
    collision_event_t *collision_data = (collision_event_t *)data;

    bool is_bullet_vs_bullet =
        (collision_data->entity_a == ENTITY_SPACESHIP_BULLET && collision_data->entity_b == ENTITY_ALIEN_BULLET) ||
        (collision_data->entity_a == ENTITY_ALIEN_BULLET && collision_data->entity_b == ENTITY_SPACESHIP_BULLET);
    bool is_bullet_vs_bunker = (collision_data->entity_a == ENTITY_BUNKER || collision_data->entity_b == ENTITY_BUNKER);
    bool should_explode = is_bullet_vs_bullet || is_bullet_vs_bunker;

    processBulletCollison(collision_data->entity_a,
                          collision_data->owner_a,
                          collision_data->bounds_a,
                          collision_data->entity_b,
                          should_explode);
    processBulletCollison(collision_data->entity_b,
                          collision_data->owner_b,
                          collision_data->bounds_b,
                          collision_data->entity_a,
                          should_explode);
}

#ifdef UNIT_TESTING
void
helperUT_bulletInitPool(void)
{
    memset(bulletPool.bullets, 0, sizeof(bulletPool.bullets));
}

void
helperUT_bulletSetUsed(bullet_t bullet, bool used)
{
    bullet->used = used;
}

bullet_t
helperUT_bulletInjectInPool(int index, entity_type_t type)
{
    bullet_t bullet = &bulletPool.bullets[index];
    memset(bullet, 0, sizeof(*bullet));

    bulletPool.bullets[index].used = true;
    bulletPool.bullets[index].type = type;

    return bullet;
}
#endif /* UNIT_TESTING */
