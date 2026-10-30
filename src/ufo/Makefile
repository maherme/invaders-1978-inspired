#
# SPDX-License-Identifier: MIT
# 
# Copyright (c) 2025 Manuel Hernández Méndez
# 
# Authors:
#   Manuel Hernández Méndez <maherme.dev@gmail.com>
# 
 
include ../../toolchain.mk
include ../common.mk

INCLUDES += -I../../src \
			-I../graphic/src \
			-I../utils/src \
			-I../graphicglut/src \
			-I../physic/src \
			-I../events/src \
			-I../collision/src

LDFLAGS += -Wl,--wrap=utilsCalloc \
		   -Wl,--wrap=utilsCheckTimeout \
		   -Wl,--wrap=graphGetSprite \
		   -Wl,--wrap=graphUpdateTimeSprite \
		   -Wl,--wrap=graphRegisterDrawFunction \
		   -Wl,--wrap=graphUnregisterDrawFunction \
		   -Wl,--wrap=graphRegisterDrawSprite \
		   -Wl,--wrap=graphDrawSprite \
		   -Wl,--wrap=graphUnregisterDrawSprite \
		   -Wl,--wrap=rand \
		   -Wl,--wrap=eventEmit \
		   -Wl,--wrap=collisionRegisterCollider \
		   -Wl,--wrap=collisionUnregisterCollider
