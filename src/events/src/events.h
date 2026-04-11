/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2026 Manuel Hernández Méndez
 *
 * Authors:
 *   Manuel Hernández Méndez <maherme.dev@gmail.com>
 */

#ifndef __EVENTS_H__
#define __EVENTS_H__

#include "entity_types.h"

#define EVENTS_MAX_NUM_CB 16

typedef enum
{
    EVENT_GAME_OVER,
    EVENT_LIVES_DEPLETED,
    EVENT_ALIENS_HEIGHT,
    EVENT_REMOVE_LIFE,
    EVENT_LIVES_CHANGED,
    EVENT_SCORE_CHANGED,
    EVENT_DANGER_LEVEL_CHANGED,
    EVENT_COLLISION,
    EVENT_ENTITY_DIED,
    EVENT_COUNT
} event_type_t;

typedef enum
{
    DANGER_LOW,
    DANGER_MID,
    DANGER_HIGH
} danger_level_t;

typedef struct
{
    int x, y;
    int width, height;
    entity_type_t entity_type;
    unsigned int points;
    void (*on_finish)(void);
} entity_death_event_t;

typedef void (*event_cb_t)(void *data);

void
eventRegister(event_type_t type, event_cb_t cb);

void
eventEmit(event_type_t type, void *data);

#ifdef UNIT_TESTING
event_cb_t
helperUT_eventGetCallback(event_type_t type, unsigned int index);

void
helperUT_eventSetCallback(event_type_t type, unsigned int index, event_cb_t cb);

void
helperUT_eventsClear(void);
#endif /* UNIT_TESTING */

#endif /* __EVENTS_H__ */
