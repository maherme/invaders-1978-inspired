/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2025 Manuel Hernández Méndez
 *
 * Authors:
 *   Manuel Hernández Méndez <maherme.dev@gmail.com>
 */

#include "testGraph.h"
#include "graph.h"
#include <cmocka.h>
#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <time.h>

void
__wrap_glGenTextures(GLsizei n, GLuint *textures)
{
    (void)textures;
    check_expected_int(n);
    function_called();
}

void
__wrap_glBindTexture(GLenum target, GLuint texture)
{
    check_expected_uint(target);
    check_expected_uint(texture);
    function_called();
}

void
__wrap_glTexImage2D(GLenum target,
                    GLint level,
                    GLint internalformat,
                    GLsizei width,
                    GLsizei height,
                    GLint border,
                    GLenum format,
                    GLenum type,
                    const void *data)
{
    check_expected_uint(target);
    check_expected_int(level);
    check_expected_int(internalformat);
    check_expected_int(width);
    check_expected_int(height);
    check_expected_int(border);
    check_expected_uint(format);
    check_expected_uint(type);
    check_expected_ptr(data);
    function_called();
}

void
__wrap_glTexSubImage2D(GLenum target,
                       GLint level,
                       GLint xoffset,
                       GLint yoffset,
                       GLsizei width,
                       GLsizei height,
                       GLenum format,
                       GLenum type,
                       const void *pixels)
{
    check_expected_uint(target);
    check_expected_int(level);
    check_expected_int(xoffset);
    check_expected_int(yoffset);
    check_expected_int(width);
    check_expected_int(height);
    check_expected_uint(format);
    check_expected_uint(type);
    check_expected_ptr(pixels);
    function_called();
}

void
__wrap_glTexParameteri(GLenum target, GLenum pname, GLint param)
{
    check_expected_uint(target);
    check_expected_uint(pname);
    check_expected_int(param);
    function_called();
}

void
__wrap_glBegin(GLenum mode)
{
    check_expected_uint(mode);
    function_called();
}

void
__wrap_glTexCoord2f(GLfloat s, GLfloat t)
{
    check_expected_float(s);
    check_expected_float(t);
    function_called();
}

void
__wrap_glVertex2i(GLint x, GLint y)
{
    check_expected_int(x);
    check_expected_int(y);
    function_called();
}

void
__wrap_glEnd(void)
{
    function_called();
}

void
__wrap_glDeleteTextures(GLsizei n, const GLuint *textures)
{
    check_expected_int(n);
    check_expected_ptr(textures);
    function_called();
}

void
__wrap_glPushAttrib(GLbitfield mask)
{
    check_expected_uint(mask);
    function_called();
}

void
__wrap_glDisable(GLenum cap)
{
    check_expected_uint(cap);
    function_called();
}

void
__wrap_glColor4ubv(const GLubyte *v)
{
    (void)v;
    function_called();
}

void
__wrap_glLineWidth(GLfloat width)
{
    check_expected_float(width);
    function_called();
}

void
__wrap_glPopAttrib(void)
{
    function_called();
}

int
__wrap_clock_gettime(clockid_t clockid, struct timespec *tp)
{
    assert_int_equal(clockid, CLOCK_MONOTONIC);
    assert_non_null(tp);
    tp->tv_sec = (time_t)mock_type(time_t);
    tp->tv_nsec = (long)mock_type(long);
    function_called();
    return 0;
}

void
testGraphCreateImageNullParameter(void **status)
{
    (void)status;

    graphCreateImage(NULL);
}

void
testGraphCreateImageSuccess(void **status)
{
    (void)status;
    sprite_t sprite = {0};

    expect_int_value(__wrap_glGenTextures, n, 1);
    expect_function_call(__wrap_glGenTextures);

    expect_uint_value(__wrap_glBindTexture, target, GL_TEXTURE_2D);
    expect_uint_value(__wrap_glBindTexture, texture, sprite.textureId);
    expect_function_call(__wrap_glBindTexture);

    expect_uint_value(__wrap_glTexImage2D, target, GL_TEXTURE_2D);
    expect_int_value(__wrap_glTexImage2D, level, 0);
    expect_int_value(__wrap_glTexImage2D, internalformat, GL_RGBA);
    expect_int_value(__wrap_glTexImage2D, width, sprite.width);
    expect_int_value(__wrap_glTexImage2D, height, sprite.height);
    expect_int_value(__wrap_glTexImage2D, border, 0);
    expect_uint_value(__wrap_glTexImage2D, format, GL_RGBA);
    expect_uint_value(__wrap_glTexImage2D, type, GL_UNSIGNED_BYTE);
    expect_uint_value(__wrap_glTexImage2D, data, (uintptr_t)sprite.image);
    expect_function_call(__wrap_glTexImage2D);

    expect_uint_value(__wrap_glTexParameteri, target, GL_TEXTURE_2D);
    expect_uint_value(__wrap_glTexParameteri, pname, GL_TEXTURE_MIN_FILTER);
    expect_int_value(__wrap_glTexParameteri, param, GL_NEAREST);
    expect_function_call(__wrap_glTexParameteri);

    expect_uint_value(__wrap_glTexParameteri, target, GL_TEXTURE_2D);
    expect_uint_value(__wrap_glTexParameteri, pname, GL_TEXTURE_MAG_FILTER);
    expect_int_value(__wrap_glTexParameteri, param, GL_NEAREST);
    expect_function_call(__wrap_glTexParameteri);

    graphCreateImage(&sprite);
}

void
testGraphDestroyImageNullParameter(void **status)
{
    (void)status;

    graphDestroyImage(NULL);
}

void
testGraphDestroyImage(void **status)
{
    (void)status;
    sprite_t sprite = {0};

    expect_int_value(__wrap_glDeleteTextures, n, 1);
    expect_uint_value(__wrap_glDeleteTextures, textures, (uintptr_t)&sprite.textureId);
    expect_function_call(__wrap_glDeleteTextures);

    graphDestroyImage(&sprite);
}

void
testGraphGetSpriteFail(void **status)
{
    (void)status;

    assert_null(graphGetSprite(NULL));
}

void
testGraphGetSpriteSuccess(void **status)
{
    (void)status;
    base_t base = {0};
    sprite_t *sprite = graphGetSprite(&base);

    assert_ptr_equal((void *)&base.sprite, (void *)sprite);
}

void
testGraphGetSpriteCoordinatesNullParameters(void **status)
{
    (void)status;
    sprite_t sprite = {0};
    sprite_coordinates_t coordinates = {0};

    assert_int_equal(1, graphGetSpriteCoordinates(&sprite, NULL));
    assert_int_equal(1, graphGetSpriteCoordinates(NULL, &coordinates));
    assert_int_equal(1, graphGetSpriteCoordinates(NULL, NULL));
}

void
testGraphGetSpriteCoordinates(void **status)
{
    (void)status;
    sprite_t sprite = {.x = 1, .y = 2, .height = 3, .width = 4};
    sprite_coordinates_t coordinates = {0};

    assert_int_equal(0, graphGetSpriteCoordinates(&sprite, &coordinates));
    assert_int_equal(sprite.x, coordinates.x1);
    assert_int_equal(sprite.x + sprite.width, coordinates.x2);
    assert_int_equal(sprite.y, coordinates.y1);
    assert_int_equal(sprite.y + sprite.height, coordinates.y2);
}

void
testGraphUpdateImageToPrintNullParameter(void **status)
{
    (void)status;

    graphNextImageToPrint(NULL);
}

void
testGraphUpdateImageToPrintOneFrame(void **status)
{
    (void)status;
    /* Image with one frame */
    static const char images[1][1][NUM_RGBA_CHANNELS] = {{B}};

    sprite_t sprite = {.selected_image = 0,
                       .num_frames = 1,
                       .image_base = (const char *)images,
                       .image = (const char *)images,
                       .width = 1,
                       .height = 1};

    graphNextImageToPrint(&sprite);

    assert_int_equal(sprite.selected_image, 0);
    assert_uint_equal(sprite.image, (uintptr_t)images);
}

void
testGraphUpdateImageToPrintMoreOneFrame(void **status)
{
    (void)status;
    /* Image with two frames */
    static const char images[2][1][1][NUM_RGBA_CHANNELS] = {/* frame 1 */
                                                            {{B}},
                                                            /* frame 2 */
                                                            {{B}}};

    sprite_t sprite = {.selected_image = 0,
                       .num_frames = 2,
                       .image_base = (const char *)images,
                       .image = (const char *)images,
                       .width = 1,
                       .height = 1};

    expect_uint_value(__wrap_glBindTexture, target, GL_TEXTURE_2D);
    expect_uint_value(__wrap_glBindTexture, texture, sprite.textureId);
    expect_function_call(__wrap_glBindTexture);

    expect_uint_value(__wrap_glTexSubImage2D, target, GL_TEXTURE_2D);
    expect_int_value(__wrap_glTexSubImage2D, level, 0);
    expect_int_value(__wrap_glTexSubImage2D, xoffset, 0);
    expect_int_value(__wrap_glTexSubImage2D, yoffset, 0);
    expect_int_value(__wrap_glTexSubImage2D, width, sprite.width);
    expect_int_value(__wrap_glTexSubImage2D, height, sprite.height);
    expect_uint_value(__wrap_glTexSubImage2D, format, GL_RGBA);
    expect_uint_value(__wrap_glTexSubImage2D, type, GL_UNSIGNED_BYTE);
    expect_uint_value(__wrap_glTexSubImage2D, pixels, (uintptr_t)&images[1]);
    expect_function_call(__wrap_glTexSubImage2D);

    graphNextImageToPrint(&sprite);

    assert_int_equal(sprite.selected_image, 1);
    assert_uint_equal(sprite.image, (uintptr_t)&images[1]);
}

void
testGraphUpdateTimeSpriteNullParameter(void **status)
{
    (void)status;

    graphUpdateTimeSprite(NULL);
}

void
testGraphUpdateTimeSprite(void **status)
{
    (void)status;
    sprite_t sprite = {0};

    will_return(__wrap_clock_gettime, (time_t)9);
    will_return(__wrap_clock_gettime, (long)(99));
    expect_function_call(__wrap_clock_gettime);

    graphUpdateTimeSprite(&sprite);
    assert_int_equal((int)sprite.last_update.tv_sec, 9);
    assert_int_equal((int)sprite.last_update.tv_nsec, 99);
}

void
testGraphPrintImageNullParameter(void **status)
{
    (void)status;

    graphPrintImage(NULL);
}

void
testGraphPrintImageVisibleFalse(void **status)
{
    (void)status;
    sprite_t sprite = {
        .visible = false,
    };

    graphPrintImage(&sprite);
}

void
testGraphPrintImageSuccess(void **status)
{
    (void)status;
    sprite_t sprite = {
        .width = 10,
        .height = 20,
        .visible = true,
    };

    expect_uint_value(__wrap_glBindTexture, target, GL_TEXTURE_2D);
    expect_uint_value(__wrap_glBindTexture, texture, sprite.textureId);
    expect_function_call(__wrap_glBindTexture);

    expect_uint_value(__wrap_glBegin, mode, GL_QUADS);
    expect_function_call(__wrap_glBegin);

    expect_float(__wrap_glTexCoord2f, s, 0.0, 0);
    expect_float(__wrap_glTexCoord2f, t, 1.0, 0);
    expect_function_call(__wrap_glTexCoord2f);

    expect_int_value(__wrap_glVertex2i, x, sprite.x);
    expect_int_value(__wrap_glVertex2i, y, sprite.y);
    expect_function_call(__wrap_glVertex2i);

    expect_float(__wrap_glTexCoord2f, s, 1.0, 0);
    expect_float(__wrap_glTexCoord2f, t, 1.0, 0);
    expect_function_call(__wrap_glTexCoord2f);

    expect_int_value(__wrap_glVertex2i, x, sprite.x + sprite.width);
    expect_int_value(__wrap_glVertex2i, y, sprite.y);
    expect_function_call(__wrap_glVertex2i);

    expect_float(__wrap_glTexCoord2f, s, 1.0, 0);
    expect_float(__wrap_glTexCoord2f, t, 0.0, 0);
    expect_function_call(__wrap_glTexCoord2f);

    expect_int_value(__wrap_glVertex2i, x, sprite.x + sprite.width);
    expect_int_value(__wrap_glVertex2i, y, sprite.y + sprite.height);
    expect_function_call(__wrap_glVertex2i);

    expect_float(__wrap_glTexCoord2f, s, 0.0, 0);
    expect_float(__wrap_glTexCoord2f, t, 0.0, 0);
    expect_function_call(__wrap_glTexCoord2f);

    expect_int_value(__wrap_glVertex2i, x, sprite.x);
    expect_int_value(__wrap_glVertex2i, y, sprite.y + sprite.height);
    expect_function_call(__wrap_glVertex2i);

    expect_function_call(__wrap_glEnd);

    graphPrintImage(&sprite);
}

void
testGraphUpdateSpriteNullParameter(void **status)
{
    (void)status;

    graphUpdateSprite(NULL);
}

void
testGraphUpdateSprite(void **status)
{
    (void)status;
    sprite_t sprite = {0};

    expect_uint_value(__wrap_glBindTexture, target, GL_TEXTURE_2D);
    expect_uint_value(__wrap_glBindTexture, texture, sprite.textureId);
    expect_function_call(__wrap_glBindTexture);

    expect_uint_value(__wrap_glTexSubImage2D, target, GL_TEXTURE_2D);
    expect_int_value(__wrap_glTexSubImage2D, level, 0);
    expect_int_value(__wrap_glTexSubImage2D, xoffset, 0);
    expect_int_value(__wrap_glTexSubImage2D, yoffset, 0);
    expect_int_value(__wrap_glTexSubImage2D, width, sprite.width);
    expect_int_value(__wrap_glTexSubImage2D, height, sprite.height);
    expect_uint_value(__wrap_glTexSubImage2D, format, GL_RGBA);
    expect_uint_value(__wrap_glTexSubImage2D, type, GL_UNSIGNED_BYTE);
    expect_uint_value(__wrap_glTexSubImage2D, pixels, (uintptr_t)sprite.image);
    expect_function_call(__wrap_glTexSubImage2D);

    graphUpdateSprite(&sprite);
}

void
testGraphPrintGameBorder(void **status)
{
    (void)status;

    expect_uint_value(__wrap_glPushAttrib, mask, GL_CURRENT_BIT | GL_ENABLE_BIT);
    expect_function_call(__wrap_glPushAttrib);
    expect_uint_value(__wrap_glDisable, cap, GL_TEXTURE_2D);
    expect_function_call(__wrap_glDisable);
    expect_function_call(__wrap_glColor4ubv);
    expect_float(__wrap_glLineWidth, width, 2.0, 0);
    expect_function_call(__wrap_glLineWidth);
    expect_uint_value(__wrap_glBegin, mode, GL_LINE_LOOP);
    expect_function_call(__wrap_glBegin);
    expect_int_value(__wrap_glVertex2i, x, 0);
    expect_int_value(__wrap_glVertex2i, y, 0);
    expect_function_call(__wrap_glVertex2i);
    expect_int_value(__wrap_glVertex2i, x, GAME_WIDTH);
    expect_int_value(__wrap_glVertex2i, y, 0);
    expect_function_call(__wrap_glVertex2i);
    expect_int_value(__wrap_glVertex2i, x, GAME_WIDTH);
    expect_int_value(__wrap_glVertex2i, y, GAME_HEIGHT);
    expect_function_call(__wrap_glVertex2i);
    expect_int_value(__wrap_glVertex2i, x, 0);
    expect_int_value(__wrap_glVertex2i, y, GAME_HEIGHT);
    expect_function_call(__wrap_glVertex2i);
    expect_function_call(__wrap_glEnd);
    expect_function_call(__wrap_glPopAttrib);

    graphPrintBorder(0, 0, GAME_WIDTH, GAME_HEIGHT);
}

void
testGraphMakeFootprintSpriteNullArguments(void **status)
{
    (void)status;

    _Alignas(uint32_t) uint8_t image[1][1][NUM_RGBA_CHANNELS] = {{W}};

    sprite_t sprite = {
        .image = (const char *)image,
        .width = 1,
        .height = 1,
        .x = 0,
        .y = 0,
    };

    sprite_t mask = {
        .image = (const char *)image,
        .width = 1,
        .height = 1,
        .x = 0,
        .y = 0,
    };

    sprite_t null_image_mask = {
        .image = NULL,
        .width = 1,
        .height = 1,
        .x = 0,
        .y = 0,
    };

    graphMakeFootprintSprite(NULL, &mask);
    graphMakeFootprintSprite(&sprite, NULL);
    graphMakeFootprintSprite(&sprite, &null_image_mask);
}

void
testGraphMakeFootprintSpriteInvalidDimensions(void **status)
{
    (void)status;

    _Alignas(uint32_t) uint8_t image1[1][1][NUM_RGBA_CHANNELS] = {{W}};
    _Alignas(uint32_t) uint8_t image2[1][1][NUM_RGBA_CHANNELS] = {{W}};

    sprite_t sprite = {
        .image = (const char *)image1,
        .width = 1,
        .height = 1,
        .x = 0,
        .y = 0,
    };

    sprite_t mask = {
        .image = (const char *)image2,
        .width = 1,
        .height = 1,
        .x = 0,
        .y = 0,
    };

    sprite.width = 0;
    graphMakeFootprintSprite(&sprite, &mask);

    sprite.width = 1;
    sprite.height = 0;
    graphMakeFootprintSprite(&sprite, &mask);

    sprite.height = 1;
    mask.width = 0;
    graphMakeFootprintSprite(&sprite, &mask);

    mask.width = 1;
    mask.height = 0;
    graphMakeFootprintSprite(&sprite, &mask);

    const uint8_t expected[NUM_RGBA_CHANNELS] = W;
    assert_memory_equal(image1[0][0], expected, sizeof(expected));
}

void
testGraphMakeFootprintSpriteNoHorizontalOverlap(void **status)
{
    (void)status;

    char image1[2][2][NUM_RGBA_CHANNELS] = {0};
    char image2[2][2][NUM_RGBA_CHANNELS] = {0};

    sprite_t sprite = {
        .image = (const char *)image1,
        .width = 2,
        .height = 2,
        .x = 0,
        .y = 0,
    };

    sprite_t mask = {
        .image = (const char *)image2,
        .width = 2,
        .height = 2,
        .x = 2,
        .y = 0,
    };

    graphMakeFootprintSprite(&sprite, &mask);
}

void
testGraphMakeFootprintSpriteNoVerticalOverlap(void **status)
{
    (void)status;

    char image1[2][2][NUM_RGBA_CHANNELS] = {0};
    char image2[2][2][NUM_RGBA_CHANNELS] = {0};

    sprite_t sprite = {
        .image = (const char *)image1,
        .width = 2,
        .height = 2,
        .x = 0,
        .y = 0,
    };

    sprite_t mask = {
        .image = (const char *)image2,
        .width = 2,
        .height = 2,
        .x = 0,
        .y = 2,
    };

    graphMakeFootprintSprite(&sprite, &mask);
}

void
testGraphMakeFootprintSpriteOverlap(void **status)
{
    (void)status;

    char image1[2][2][NUM_RGBA_CHANNELS] = {0};
    char image2[2][2][NUM_RGBA_CHANNELS] = {0};

    sprite_t sprite = {
        .image = (const char *)image1,
        .width = 2,
        .height = 2,
        .x = 0,
        .y = 0,
    };

    sprite_t mask = {
        .image = (const char *)image2,
        .width = 2,
        .height = 2,
        .x = 0,
        .y = 0,
    };

    graphMakeFootprintSprite(&sprite, &mask);
}

void
testGraphMakeFootprintSpriteOpaquePixels(void **status)
{
    (void)status;

    _Alignas(uint32_t) uint8_t image1[1][1][NUM_RGBA_CHANNELS] = {{R}};
    _Alignas(uint32_t) uint8_t image2[1][1][NUM_RGBA_CHANNELS] = {{W}};

    sprite_t sprite = {
        .image = (const char *)image1,
        .width = 1,
        .height = 1,
        .x = 0,
        .y = 0,
    };

    sprite_t mask = {
        .image = (const char *)image2,
        .width = 1,
        .height = 1,
        .x = 0,
        .y = 0,
    };

    graphMakeFootprintSprite(&sprite, &mask);

    const uint8_t expected[NUM_RGBA_CHANNELS] = B;
    assert_memory_equal(image1[0][0], expected, sizeof(expected));
}

void
testGraphMakeFootprintSpriteTransparentSpritePixel(void **status)
{
    (void)status;

    _Alignas(uint32_t) uint8_t image1[1][1][NUM_RGBA_CHANNELS] = {{B}};
    _Alignas(uint32_t) uint8_t image2[1][1][NUM_RGBA_CHANNELS] = {{W}};

    sprite_t sprite = {
        .image = (const char *)image1,
        .width = 1,
        .height = 1,
        .x = 0,
        .y = 0,
    };

    sprite_t mask = {
        .image = (const char *)image2,
        .width = 1,
        .height = 1,
        .x = 0,
        .y = 0,
    };

    graphMakeFootprintSprite(&sprite, &mask);

    const uint8_t expected[NUM_RGBA_CHANNELS] = B;
    assert_memory_equal(image1[0][0], expected, sizeof(expected));
}

void
testGraphMakeFootprintSpriteTransparentMaskPixel(void **status)
{
    (void)status;

    _Alignas(uint32_t) uint8_t image1[1][1][NUM_RGBA_CHANNELS] = {{R}};
    _Alignas(uint32_t) uint8_t image2[1][1][NUM_RGBA_CHANNELS] = {{B}};

    sprite_t sprite = {
        .image = (const char *)image1,
        .width = 1,
        .height = 1,
        .x = 0,
        .y = 0,
    };

    sprite_t mask = {
        .image = (const char *)image2,
        .width = 1,
        .height = 1,
        .x = 0,
        .y = 0,
    };

    graphMakeFootprintSprite(&sprite, &mask);

    const uint8_t expected[NUM_RGBA_CHANNELS] = R;
    assert_memory_equal(image1[0][0], expected, sizeof(expected));
}

void
testGraphMakeFootprintSpriteSemiTransparentPixels(void **status)
{
    (void)status;

    _Alignas(uint32_t) uint8_t image1[1][1][NUM_RGBA_CHANNELS] = {{{255, 0, 0, 128}}};
    _Alignas(uint32_t) uint8_t image2[1][1][NUM_RGBA_CHANNELS] = {{{0, 255, 0, 128}}};

    sprite_t sprite = {
        .image = (const char *)image1,
        .width = 1,
        .height = 1,
        .x = 0,
        .y = 0,
    };

    sprite_t mask = {
        .image = (const char *)image2,
        .width = 1,
        .height = 1,
        .x = 0,
        .y = 0,
    };

    graphMakeFootprintSprite(&sprite, &mask);

    const uint8_t expected[NUM_RGBA_CHANNELS] = B;
    assert_memory_equal(image1[0][0], expected, sizeof(expected));
}
