/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2024 Manuel Hernández Méndez
 *
 * Authors:
 *   Manuel Hernández Méndez <maherme.dev@gmail.com>
 */

#include "aliens.h"
#include "bullet.h"
#include "bunkers.h"
#include "collision.h"
#include "engine.h"
#include "entity_types.h"
#include "events.h"
#include "explosion.h"
#include "graph.h"
#include "graphGlut.h"
#include "graphGlutCallbacks.h"
#include "hud.h"
#include "keyboard.h"
#include "lives.h"
#include "physic.h"
#include "score.h"
#include "spaceship.h"
#include "ufo.h"
#include "utils.h"
#include <GL/freeglut_ext.h>
#include <GL/glut.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#define ALIEN_HEIGHT_FOR_GAME_OVER 16
#define ALIEN_HEIGHT_FOR_DANGER_MID 90
#define ALIEN_HEIGHT_FOR_DANGER_HIGHT 52

static struct
{
    bool gaming;
    spaceship_t spaceship;
    ufo_t ufo;
    struct timespec timer_get_shooter;
    struct timespec timer_alien_move;
    unsigned int alien_move_delay;
} gameContext = {
    .gaming = true,
    .spaceship = NULL,
    .ufo = NULL,
    .timer_alien_move = {0},
    .timer_get_shooter = {0},
    .alien_move_delay = 0,
};

static void
bulletActionsSingle(bullet_t bullet)
{
    if (!bulletUsed(bullet))
    {
        return;
    }

    entity_type_t type;
    bulletGetType(bullet, &type);
    sprite_t *bullet_sprite = graphGetSprite((base_t *)bullet);
    if (collisionCheckBorder(bullet_sprite, UP) || collisionCheckBorder(bullet_sprite, DOWN))
    {
        if (type == ENTITY_SPACESHIP_BULLET)
        {
            explosionCreate(bullet_sprite->x,
                            GAME_HEIGHT - explosionsGetExplosionHeight(ENTITY_SPACESHIP_BULLET),
                            ENTITY_SPACESHIP_BULLET,
                            NULL);
        }
        else
        {
            explosionCreate(bullet_sprite->x, bullet_sprite->y, ENTITY_ALIEN_BULLET, NULL);
        }
        bulletDestroy(bullet);
    }
    else
    {
        if (type == ENTITY_SPACESHIP_BULLET)
        {
            physicMoveSprite(bullet_sprite, UP);
        }
        else
        {
            physicMoveSprite(bullet_sprite, DOWN);
        }
    }
}

static void
spaceshipExplosionCallback(void);

static void
spaceshipRespawnLogic(void)
{
    if (bulletNoneUsed() && explosionsAllFinished())
    {
        gameContext.spaceship = spaceshipCreate(GAME_WIDTH / 2, 0, spaceshipExplosionCallback);
        bulletAlienInhibit(false);
        engineUnregister(spaceshipRespawnLogic);
    }
}

static void
spaceshipExplosionCallback(void)
{
    eventEmit(EVENT_REMOVE_LIFE, NULL);
    engineRegister(spaceshipRespawnLogic);
}

static void
onEntityDied(void *data)
{
    entity_death_event_t *death = (entity_death_event_t *)data;

    switch (death->entity_type)
    {
        case ENTITY_SPACESHIP:
            bulletAlienInhibit(true);
            break;
        case ENTITY_ALIEN:
        case ENTITY_UFO:
            scoreAddPoints(death->points);
            break;
        case ENTITY_SPACESHIP_BULLET:
        case ENTITY_ALIEN_BULLET:
        case ENTITY_BUNKER:
        case ENTITY_COUNT:
        default:
            break;
    }
}

static void
bulletActions(void)
{
    bulletCallFunctionForEach(bulletActionsSingle);
}

static void
spaceshipFire(void)
{
    if (!spaceshipAlive(gameContext.spaceship))
    {
        return;
    }

    sprite_t *spaceship_sprite = graphGetSprite((base_t *)gameContext.spaceship);
    bulletCreate(spaceship_sprite->x + spaceship_sprite->width / 2,
                 spaceship_sprite->y + spaceship_sprite->height,
                 ENTITY_SPACESHIP_BULLET);
}

static void
aliensActions(void)
{
    unsigned int num_alive = aliensGetAlives();
    gameContext.alien_move_delay = ALIENS_BASE_MOVE_DELAY - (ALIENS_INITIAL_NUMBER - num_alive) * 8;
    if (gameContext.alien_move_delay < ALIENS_MIN_MOVE_DELAY)
    {
        gameContext.alien_move_delay = ALIENS_MIN_MOVE_DELAY;
    }

    if (utilsCheckTimeout(gameContext.timer_get_shooter, ALIENS_SHOOT_DELAY * NS_PER_MS))
    {
        alien_t alien = aliensGetShooter();
        if (alien)
        {
            sprite_t *alien_sprite = graphGetSprite((base_t *)alien);
            bulletCreate(alien_sprite->x + alien_sprite->width / 2, alien_sprite->y, ENTITY_ALIEN_BULLET);
            clock_gettime(CLOCK_MONOTONIC, &gameContext.timer_get_shooter);
        }
    }

    if (utilsCheckTimeout(gameContext.timer_alien_move, gameContext.alien_move_delay * NS_PER_MS))
    {
        aliensMove();
        clock_gettime(CLOCK_MONOTONIC, &gameContext.timer_alien_move);
    }
}

static void
ufoActions(void)
{
    if (!ufoAlive(gameContext.ufo))
    {
        gameContext.ufo = ufoCreate();
        return;
    }
    else
    {
        sprite_t *ufo_sprite = graphGetSprite((base_t *)gameContext.ufo);
        if (ufoCheckForMoving(gameContext.ufo))
        {
            physicMoveSprite(ufo_sprite, ufoGetDirection(gameContext.ufo));
        }
        if (collisionCheckBorder(ufo_sprite, RIGHT) || collisionCheckBorder(ufo_sprite, LEFT))
        {
            ufoDestroy(gameContext.ufo);
        }
    }
}

static void
keyboardUpdate(void)
{
    if (keyboardGetSpecialKeyState(SPECIAL_KEY_LEFT))
    {
        physicMoveSprite(graphGetSprite((base_t *)gameContext.spaceship), LEFT);
    }
    if (keyboardGetSpecialKeyState(SPECIAL_KEY_RIGHT))
    {
        physicMoveSprite(graphGetSprite((base_t *)gameContext.spaceship), RIGHT);
    }
}

static void
gameOver(void *data)
{
    (void)data;
    /* NOTE: TBD */
}

static void
onLivesRemove(void *data)
{
    (void)data;
    livesRemove();
}

static void
deriveGameStateFromAliensHeight(void *data)
{
    int height = *(int *)data;
    danger_level_t danger_lv = DANGER_LOW;

    if (height <= ALIEN_HEIGHT_FOR_GAME_OVER)
    {
        eventEmit(EVENT_GAME_OVER, NULL);
    }
    else if (height <= ALIEN_HEIGHT_FOR_DANGER_HIGHT)
    {
        danger_lv = DANGER_HIGH;
        eventEmit(EVENT_DANGER_LEVEL_CHANGED, &danger_lv);
    }
    else if (height <= ALIEN_HEIGHT_FOR_DANGER_MID)
    {
        danger_lv = DANGER_MID;
        eventEmit(EVENT_DANGER_LEVEL_CHANGED, &danger_lv);
    }
}

int
main(int argc, char **argv)
{
    graphInitGlutConfig_t initGlutConfig = {
        .main_argc = &argc,
        .main_argv = argv,
        .displayMode = GLUT_DOUBLE | GLUT_RGB,
        .scale = 2,
        .windowPositionX = 0,
        .windowPositionY = 0,
        .windowName = "Space Invaders",
        .displayFunc = graphGlutDisplay,
        .reshapeFunc = graphGlutReshape,
    };

    srand(time(NULL));

    graphRegisterPrintInit();
    graphInitGlut(&initGlutConfig);
    keyboardRegisterAction(spaceshipFire, ACTION_KEY_DOWN, ' ');
    keyboardInit();
    collisionInit();

    eventRegister(EVENT_GAME_OVER, gameOver);
    eventRegister(EVENT_LIVES_DEPLETED, gameOver);
    eventRegister(EVENT_ALIENS_HEIGHT, deriveGameStateFromAliensHeight);
    eventRegister(EVENT_REMOVE_LIFE, onLivesRemove);
    eventRegister(EVENT_LIVES_CHANGED, onLivesChanged);
    eventRegister(EVENT_SCORE_CHANGED, onScoreChanged);
    eventRegister(EVENT_DANGER_LEVEL_CHANGED, ufoOnDangerLevelChanged);
    eventRegister(EVENT_COLLISION, aliensOnCollision);
    eventRegister(EVENT_COLLISION, bulletOnCollision);
    eventRegister(EVENT_COLLISION, ufoOnCollision);
    eventRegister(EVENT_COLLISION, spaceshipOnCollision);
    eventRegister(EVENT_COLLISION, bunkersOnCollision);
    eventRegister(EVENT_ENTITY_DIED, explosionsOnEntityDied);
    eventRegister(EVENT_ENTITY_DIED, onEntityDied);

    gameContext.spaceship = spaceshipCreate(GAME_WIDTH / 2, 0, spaceshipExplosionCallback);
    aliensCreate();
    bunkersCreate();
    hudInit();
    engineRegister(keyboardUpdate);
    engineRegister(explosionsDestroy);
    engineRegister(ufoActions);
    engineRegister(aliensActions);
    engineRegister(bulletActions);
    engineRegister(collisionUpdate);

    clock_gettime(CLOCK_MONOTONIC, &gameContext.timer_get_shooter);
    clock_gettime(CLOCK_MONOTONIC, &gameContext.timer_alien_move);

    while (gameContext.gaming)
    {
        engineRun(ENGINE_RATE);
        glutMainLoopEvent();
        graphGlutDisplay();
    }

    exit(EXIT_SUCCESS);
}
