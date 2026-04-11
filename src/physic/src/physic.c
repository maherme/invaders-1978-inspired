/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2026 Manuel Hernández Méndez
 *
 * Authors:
 *   Manuel Hernández Méndez <maherme.dev@gmail.com>
 */

#include "physic.h"
#include "graph.h"
#include "utils.h"
#include <stdbool.h>

void
physicMoveSprite(sprite_t *sprite, direction_t direction)
{
    if (!sprite)
    {
        return;
    }

    if (!utilsCheckTimeout(sprite->last_update, sprite->time_to_move))
    {
        return;
    }

    sprite_coordinates_t coord;
    graphGetSpriteCoordinates(sprite, &coord);

    switch (direction)
    {
        case RIGHT:
            if (coord.x2 + sprite->pixels_to_move >= sprite->max_movement.right)
            {
                sprite->x = sprite->max_movement.right - sprite->width;
                return;
            }
            sprite->x += sprite->pixels_to_move;
            break;
        case LEFT:
            if (coord.x1 - sprite->pixels_to_move <= sprite->max_movement.left)
            {
                sprite->x = sprite->max_movement.left;
                return;
            }
            sprite->x -= sprite->pixels_to_move;
            break;
        case UP:
            if (coord.y2 + sprite->pixels_to_move >= sprite->max_movement.up)
            {
                sprite->y = sprite->max_movement.up - sprite->height;
                return;
            }
            sprite->y += sprite->pixels_to_move;
            break;
        case DOWN:
            if (coord.y1 - sprite->pixels_to_move <= sprite->max_movement.down)
            {
                sprite->y = sprite->max_movement.down;
                return;
            }
            sprite->y -= sprite->pixels_to_move;
            break;
        case INVALID_DIR:
        default:
            return;
            break;
    }

    graphNextImageToPrint(sprite);
    graphUpdateTimeSprite(sprite);
}
