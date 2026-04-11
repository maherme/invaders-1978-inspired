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
			-I../graphicglut/src \
			-I../collision/src \
			-I../physic/src \
			-I../explosion/src

LDFLAGS += -Wl,--wrap=graphRegisterPrint \
		   -Wl,--wrap=collisionRegisterCollider \
		   -Wl,--wrap=graphMakeFootprintSprite \
		   -Wl,--wrap=graphUpdateSprite \
		   -Wl,--wrap=explosionsGetStaticFrame
