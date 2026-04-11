/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2026 Manuel Hernández Méndez
 *
 * Authors:
 *   Manuel Hernández Méndez <maherme.dev@gmail.com>
 */

#include "explosion.h"
#include "entity_types.h"
#include "events.h"
#include "graph.h"
#include "graphGlutCallbacks.h"
#include "list.h"
#include "utils.h"
#include <assert.h>
#include <stdbool.h>

#define EXPLOSION_BULLET_SPACESHIP_WIDTH 8
#define EXPLOSION_BULLET_SPACESHIP_HEIGHT 8
#define EXPLOSION_BULLET_ALIEN_WIDTH 6
#define EXPLOSION_BULLET_ALIEN_HEIGHT 8
#define EXPLOSION_SPACESHIP_WIDTH 16
#define EXPLOSION_SPACESHIP_HEIGHT 8
#define EXPLOSION_SPACESHIP_NUM_FRAMES 2
#define EXPLOSION_UFO_WIDTH 24
#define EXPLOSION_UFO_HEIGHT 8
#define EXPLOSION_ALIEN_NUM_IMGS 4
#define EXPLOSION_ALIEN_WIDTH 14
#define EXPLOSION_ALIEN_HEIGHT 8

static const unsigned int explosion_heights[] = {
    [ENTITY_SPACESHIP_BULLET] = EXPLOSION_BULLET_SPACESHIP_HEIGHT,
    [ENTITY_ALIEN_BULLET] = EXPLOSION_BULLET_ALIEN_HEIGHT,
    [ENTITY_SPACESHIP] = EXPLOSION_SPACESHIP_HEIGHT,
    [ENTITY_UFO] = EXPLOSION_UFO_HEIGHT,
    [ENTITY_ALIEN] = EXPLOSION_ALIEN_HEIGHT,
};

struct explosion_instance
{
    sprite_t sprite;
    struct timespec creation_time;
    long long explosion_time;
    struct timespec update_frame_timer;
    long long update_frame_time;
    entity_type_t type;
    void (*callback)(void);
};

typedef struct explosion_node
{
    explosion_t explosion;
    struct list_head node;
} explosion_node_t;

static LIST_HEAD(explosions);

static const struct
{
    const char image[EXPLOSION_BULLET_SPACESHIP_HEIGHT][EXPLOSION_BULLET_SPACESHIP_WIDTH][NUM_RGBA_CHANNELS];
    const int image_width;
    const int image_height;
    const long long explosion_time;
} explosion_bullet_spaceship = {.image = {{B, B, R, B, R, B, B, B},
                                          {B, R, R, R, B, B, B, B},
                                          {B, B, R, R, R, B, R, B},
                                          {B, R, R, R, R, R, B, B},
                                          {B, B, R, R, R, B, B, B},
                                          {B, B, B, R, B, B, R, B},
                                          {B, B, R, B, B, R, B, B},
                                          {R, B, B, B, R, B, B, R}},
                                .image_width = EXPLOSION_BULLET_SPACESHIP_WIDTH,
                                .image_height = EXPLOSION_BULLET_SPACESHIP_HEIGHT,
                                .explosion_time = 500 * NS_PER_MS};

static const struct
{
    const char image[EXPLOSION_BULLET_ALIEN_HEIGHT][EXPLOSION_BULLET_ALIEN_WIDTH][NUM_RGBA_CHANNELS];
    const int image_width;
    const int image_height;
    const long long explosion_time;
} explosion_bullet_alien = {.image = {{B, B, G, B, B, G},
                                      {B, G, B, G, B, B},
                                      {B, B, G, G, G, G},
                                      {G, G, G, G, B, B},
                                      {B, G, G, G, G, B},
                                      {G, G, G, G, G, G},
                                      {B, B, G, G, B, G},
                                      {B, G, B, B, G, B}},
                            .image_width = EXPLOSION_BULLET_ALIEN_WIDTH,
                            .image_height = EXPLOSION_BULLET_ALIEN_HEIGHT,
                            .explosion_time = 500 * NS_PER_MS};

static const struct
{
    const char image[EXPLOSION_SPACESHIP_NUM_FRAMES][EXPLOSION_SPACESHIP_HEIGHT][EXPLOSION_SPACESHIP_WIDTH][NUM_RGBA_CHANNELS];
    const int image_width;
    const int image_height;
    const long long explosion_time;
    const long long update_frame_time;
} explosion_spaceship = {.image =
                             {/* frame 1 */
                              {{B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B},
                               {B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B},
                               {B, B, B, G, B, B, B, B, B, B, G, B, B, B, B, B},
                               {B, B, B, G, B, G, B, B, B, G, G, B, G, B, B, B},
                               {B, B, G, B, G, B, G, B, B, B, B, G, B, G, B, B},
                               {B, B, B, G, B, G, G, B, B, G, B, G, G, B, B, B},
                               {B, B, B, G, G, B, B, B, B, B, B, G, G, B, B, B},
                               {B, B, G, G, G, G, B, B, B, B, G, G, G, G, B, B}},
                              /* frame 2 */
                              {{G, B, B, G, B, B, G, B, B, G, B, B, G, B, B, G},
                               {B, B, G, B, G, B, B, B, B, G, B, G, B, B, G, B},
                               {B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B},
                               {B, B, B, B, B, B, G, B, B, G, B, B, G, B, B, B},
                               {B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B},
                               {B, B, B, G, B, B, B, B, B, B, B, G, B, B, B, B},
                               {B, B, B, G, G, B, B, B, B, B, B, G, G, B, B, B},
                               {B, B, G, G, G, G, B, B, B, B, G, G, G, G, B, B}}},
                         .image_width = EXPLOSION_SPACESHIP_WIDTH,
                         .image_height = EXPLOSION_SPACESHIP_HEIGHT,
                         .explosion_time = 2000 * NS_PER_MS,
                         .update_frame_time = 125 * NS_PER_MS};

static const struct
{
    const char image[EXPLOSION_UFO_HEIGHT][EXPLOSION_UFO_WIDTH][NUM_RGBA_CHANNELS];
    const int image_width;
    const int image_height;
    const long long explosion_time;
} explosion_ufo = {.image = {{B, B, B, B, B, B, B, B, B, B, B, R, B, R, B, B, B, B, B, B, B, B, B, B},
                             {B, B, B, R, R, R, R, R, B, B, B, B, B, B, B, B, R, R, R, R, R, B, B, B},
                             {B, B, R, R, R, R, R, R, R, B, B, B, R, B, B, R, R, R, R, R, R, R, B, B},
                             {B, B, R, B, R, B, R, R, B, B, R, B, B, R, B, B, R, R, B, R, B, R, B, B},
                             {B, R, R, R, R, R, R, R, B, R, B, B, B, B, R, B, R, R, R, R, R, R, R, B},
                             {R, R, R, R, R, R, R, B, B, R, B, B, B, B, B, B, R, R, R, R, R, R, R, R},
                             {B, B, B, B, R, R, R, R, B, B, B, R, R, B, B, B, R, R, R, R, B, B, B, B},
                             {B, B, B, B, B, B, R, B, B, B, B, B, B, B, R, B, B, R, B, B, B, B, B, B}},
                   .image_width = EXPLOSION_UFO_WIDTH,
                   .image_height = EXPLOSION_UFO_HEIGHT,
                   .explosion_time = 500 * NS_PER_MS};

static const struct
{
    const char image[EXPLOSION_ALIEN_NUM_IMGS][EXPLOSION_ALIEN_HEIGHT][EXPLOSION_ALIEN_WIDTH][NUM_RGBA_CHANNELS];
    const int image_width;
    const int image_height;
    const long long explosion_time;
} explosion_alien = {.image =
                         {/* image 1 */
                          {{B, B, B, B, B, B, B, B, B, B, W, B, B, B},
                           {B, W, B, B, W, B, B, B, B, B, B, B, W, B},
                           {B, B, B, B, B, W, B, B, W, B, B, B, B, B},
                           {B, B, B, B, B, B, W, W, B, B, B, B, B, W},
                           {B, B, W, B, B, B, W, W, B, W, B, B, B, B},
                           {B, B, B, B, W, W, B, B, W, B, B, W, B, B},
                           {W, B, B, B, B, B, B, B, B, B, B, B, B, B},
                           {B, B, W, B, B, B, W, B, B, B, W, B, B, W}},
                          /* image 2 */
                          {{B, B, B, B, B, B, B, B, B, B, W, B, B, B},
                           {B, B, B, B, W, B, B, W, B, B, B, B, B, B},
                           {B, B, B, B, B, W, B, B, B, B, B, B, B, B},
                           {B, B, B, B, B, B, W, W, B, B, W, B, B, B},
                           {B, B, B, B, B, B, W, W, B, B, B, B, B, B},
                           {B, B, B, B, W, W, B, B, W, B, B, W, B, B},
                           {B, B, B, B, B, B, B, B, B, B, B, B, B, B},
                           {B, B, B, W, B, B, W, B, B, B, W, B, B, B}},
                          /* image 3 */
                          {{B, B, B, B, B, B, B, B, B, B, B, B, B, B},
                           {B, W, B, W, B, B, B, B, B, B, B, B, B, B},
                           {B, B, B, B, B, W, B, B, W, B, B, B, B, B},
                           {B, B, B, B, W, B, W, W, B, B, B, B, B, B},
                           {B, B, W, B, B, B, W, W, B, B, B, B, B, B},
                           {B, B, B, B, B, W, B, B, W, B, B, W, B, B},
                           {B, B, B, B, B, B, B, B, B, B, B, B, B, B},
                           {B, B, B, W, B, B, W, B, B, B, B, B, B, B}},
                          /* image 4 */
                          {{B, B, B, B, B, B, B, B, B, B, B, B, W, B},
                           {B, B, B, B, B, B, W, B, B, B, W, B, B, B},
                           {W, B, B, W, B, W, B, B, W, B, B, B, B, B},
                           {B, B, B, B, B, B, W, W, B, B, B, B, B, B},
                           {B, B, B, B, B, B, W, W, B, W, B, W, B, B},
                           {B, B, B, B, W, W, B, B, W, B, B, B, B, B},
                           {B, W, B, B, B, B, B, B, B, B, B, B, B, B},
                           {B, B, B, B, B, B, B, W, B, B, B, W, B, B}}},
                     .image_width = EXPLOSION_ALIEN_WIDTH,
                     .image_height = EXPLOSION_ALIEN_HEIGHT,
                     .explosion_time = 500 * NS_PER_MS};

static void
setExplosionType(explosion_t instance, entity_type_t type)
{
    static int explosion_alien_next = 0;

    instance->type = type;
    switch (type)
    {
        case ENTITY_SPACESHIP_BULLET:
            instance->sprite.width = explosion_bullet_spaceship.image_width;
            instance->sprite.height = explosion_bullet_spaceship.image_height;
            instance->sprite.image = (const char *)explosion_bullet_spaceship.image;
            instance->explosion_time = explosion_bullet_spaceship.explosion_time;
            break;
        case ENTITY_ALIEN_BULLET:
            instance->sprite.width = explosion_bullet_alien.image_width;
            instance->sprite.height = explosion_bullet_alien.image_height;
            instance->sprite.image = (const char *)explosion_bullet_alien.image;
            instance->explosion_time = explosion_bullet_alien.explosion_time;
            break;
        case ENTITY_UFO:
            instance->sprite.width = explosion_ufo.image_width;
            instance->sprite.height = explosion_ufo.image_height;
            instance->sprite.image = (const char *)explosion_ufo.image;
            instance->explosion_time = explosion_ufo.explosion_time;
            break;
        case ENTITY_SPACESHIP:
            instance->sprite.width = explosion_spaceship.image_width;
            instance->sprite.height = explosion_spaceship.image_height;
            instance->sprite.image_base = (const char *)explosion_spaceship.image;
            instance->sprite.image = instance->sprite.image_base;
            instance->sprite.num_frames = EXPLOSION_SPACESHIP_NUM_FRAMES;
            instance->explosion_time = explosion_spaceship.explosion_time;
            instance->update_frame_time = explosion_spaceship.update_frame_time;
            clock_gettime(CLOCK_MONOTONIC, &instance->update_frame_timer);
            break;
        case ENTITY_ALIEN:
            instance->sprite.width = explosion_alien.image_width;
            instance->sprite.height = explosion_alien.image_height;
            instance->sprite.image = (const char *)explosion_alien.image[explosion_alien_next];
            explosion_alien_next = (explosion_alien_next + 1) % EXPLOSION_ALIEN_NUM_IMGS;
            instance->explosion_time = explosion_alien.explosion_time;
            break;
            /* GCOVR_EXCL_START */
        case ENTITY_BUNKER:
        case ENTITY_COUNT:
        default:
            assert(!"invalid explosion type");
            UNREACHABLE();
            break; /* GCOVR_EXCL_BR_SOURCE */
                   /* GCOVR_EXCL_STOP */
    }
}

explosion_t
explosionCreate(int x, int y, entity_type_t type, void (*callback)(void))
{
    explosion_node_t *new_node = utilsCalloc(1, sizeof(explosion_node_t));
    explosion_t new_explosion = utilsCalloc(1, sizeof(struct explosion_instance));
    setExplosionType(new_explosion, type);
    new_explosion->sprite.x = x - new_explosion->sprite.width / 2;
    new_explosion->sprite.y = y;
    new_explosion->sprite.pixels_to_move = 0;
    new_explosion->sprite.time_to_move = 0;
    new_explosion->sprite.layer = LAYER_EFFECTS;
    new_explosion->sprite.visible = true;
    clock_gettime(CLOCK_MONOTONIC, &new_explosion->creation_time);
    graphRegisterPrint(graphGetSprite((base_t *)new_explosion));
    new_explosion->callback = callback;
    new_node->explosion = new_explosion;
    list_add(&new_node->node, &explosions);

    return new_explosion;
}

static bool
explosionTimeout(explosion_t explosion)
{
    assert(explosion); /* GCOVR_EXCL_LINE */

    if (utilsCheckTimeout(explosion->creation_time, explosion->explosion_time))
    {
        sprite_t *explosion_sprite = graphGetSprite((base_t *)explosion);
        graphUnregisterPrint(explosion_sprite);
        if (explosion->callback)
        {
            explosion->callback();
        }
        UTILS_FREE(explosion);
        return true;
    }

    return false;
}

static void
explosionUpdateImageToPrint(explosion_t explosion)
{
    assert(explosion); /* GCOVR_EXCL_LINE */

    if (explosion->sprite.num_frames > 1 && utilsCheckTimeout(explosion->update_frame_timer, explosion->update_frame_time))
    {
        graphNextImageToPrint(&explosion->sprite);
        clock_gettime(CLOCK_MONOTONIC, &explosion->update_frame_timer);
    }
}

void
explosionsDestroy(void)
{
    explosion_node_t *n, *tmp;

    list_for_each_entry_safe(n, tmp, &explosions, node)
    {
        explosionUpdateImageToPrint(n->explosion);
        if (explosionTimeout(n->explosion))
        {
            list_del(&n->node);
            UTILS_FREE(n);
        }
    }
}

bool
explosionsAllFinished(void)
{
    return list_empty(&explosions);
}

unsigned int
explosionsGetExplosionHeight(entity_type_t type)
{
    return explosion_heights[type];
}

void
explosionsOnEntityDied(void *data)
{
    entity_death_event_t *death_data = (entity_death_event_t *)data;

    switch (death_data->entity_type)
    {
        case ENTITY_ALIEN:
        case ENTITY_UFO:
        case ENTITY_SPACESHIP:
            explosionCreate(death_data->x + death_data->width / 2,
                            death_data->y,
                            death_data->entity_type,
                            death_data->on_finish);
            break;
        case ENTITY_SPACESHIP_BULLET:
        case ENTITY_ALIEN_BULLET:
            explosionCreate(death_data->x, death_data->y, death_data->entity_type, NULL);
            break;
        case ENTITY_BUNKER:
        case ENTITY_COUNT:
        default:
            break;
    }
}

sprite_t *
explosionsGetStaticFrame(entity_type_t type, int num_frame)
{
    (void)num_frame;
    static sprite_t result;
    result.x = 0;
    result.y = 0;

    switch (type)
    {
        case ENTITY_SPACESHIP_BULLET:
            result.width = explosion_bullet_spaceship.image_width;
            result.height = explosion_bullet_spaceship.image_height;
            result.image = (const char *)explosion_bullet_spaceship.image;
            break;
        case ENTITY_ALIEN_BULLET:
            result.width = explosion_bullet_alien.image_width;
            result.height = explosion_bullet_alien.image_height;
            result.image = (const char *)explosion_bullet_alien.image;
            break;
        case ENTITY_ALIEN:
        case ENTITY_UFO:
        case ENTITY_BUNKER:
        case ENTITY_SPACESHIP:
        case ENTITY_COUNT:
        default:
            return NULL;
            break;
    }

    return &result;
}

#ifdef UNIT_TESTING
void
helperUT_explosionResetList(void)
{
    explosion_node_t *n, *tmp;

    list_for_each_entry_safe(n, tmp, &explosions, node)
    {
        list_del(&n->node);
        free((void *)n->explosion);
        free((void *)n);
    }
}

explosion_t
helperUT_getExplosionFromPoolByType(entity_type_t type)
{
    explosion_node_t *n;

    /* GCOVR_EXCL_START */
    list_for_each_entry(n, &explosions, node)
    {
        if (n->explosion->type == type)
        {
            return n->explosion;
        }
    }

    return NULL;
    /* GCOVR_EXCL_STOP */
}

sprite_t
helperUT_getSpriteFromExplosion(explosion_t explosion)
{
    return explosion->sprite;
}

callback_t
helperUT_getCallbackFromExplosion(explosion_t explosion)
{
    return explosion->callback;
}

entity_type_t
helperUT_getEntityTypeFromExplosion(explosion_t explosion)
{
    return explosion->type;
}

int
helperUT_expectedExplosionWidth(entity_type_t type)
{
    int result = 0;

    switch (type)
    {
        case ENTITY_SPACESHIP_BULLET:
            result = explosion_bullet_spaceship.image_width;
            break;
        case ENTITY_ALIEN_BULLET:
            result = explosion_bullet_alien.image_width;
            break;
            /* GCOVR_EXCL_START */
        default:
            result = -1;
            break; /* GCOVR_EXCL_BR_SOURCE */
                   /* GCOVR_EXCL_STOP */
    }

    return result;
}

int
helperUT_expectedExplosionHeight(entity_type_t type)
{
    int result = 0;

    switch (type)
    {
        case ENTITY_SPACESHIP_BULLET:
            result = explosion_bullet_spaceship.image_height;
            break;
        case ENTITY_ALIEN_BULLET:
            result = explosion_bullet_alien.image_height;
            break;
            /* GCOVR_EXCL_START */
        default:
            result = -1;
            break; /* GCOVR_EXCL_BR_SOURCE */
                   /* GCOVR_EXCL_STOP */
    }

    return result;
}

const char *
helperUT_expectedExplosionImage(entity_type_t type)
{
    const char *result = 0;

    switch (type)
    {
        case ENTITY_SPACESHIP_BULLET:
            result = (const char *)explosion_bullet_spaceship.image;
            break;
        case ENTITY_ALIEN_BULLET:
            result = (const char *)explosion_bullet_alien.image;
            break;
            /* GCOVR_EXCL_START */
        default:
            result = NULL;
            break; /* GCOVR_EXCL_BR_SOURCE */
                   /* GCOVR_EXCL_STOP */
    }

    return result;
}
#endif /* UNIT_TESTING */
