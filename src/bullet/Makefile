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
			-I../graphic/src \
			-I../utils/src \
			-I../graphicglut/src \
			-I../events/src \
			-I../collision/src \
			-I../physic/src

LDFLAGS += -Wl,--wrap=utilsCalloc \
		   -Wl,--wrap=graphGetSprite \
		   -Wl,--wrap=graphPrintImage \
		   -Wl,--wrap=graphRegisterPrint \
		   -Wl,--wrap=graphUnregisterPrint \
		   -Wl,--wrap=rand \
		   -Wl,--wrap=eventEmit \
		   -Wl,--wrap=collisionRegisterCollider \
		   -Wl,--wrap=collisionUnregisterCollider
