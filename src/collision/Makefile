#
# SPDX-License-Identifier: MIT
# 
# Copyright (c) 2026 Manuel Hernández Méndez
# 
# Authors:
#   Manuel Hernández Méndez <maherme.dev@gmail.com>
# 
 
include ../../toolchain.mk
include ../common.mk

INCLUDES += -I../../src \
			-I../physic/src \
			-I../events/src \
			-I../graphic/src \
			-I../graphicglut/src

LDFLAGS += -Wl,--wrap=eventEmit \
		   -Wl,--wrap=graphGetSpriteCoordinates
