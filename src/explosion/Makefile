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
			-I../events/src

LDFLAGS += -Wl,--wrap=utilsCalloc \
		   -Wl,--wrap=utilsCheckTimeout \
		   -Wl,--wrap=graphGetSprite \
		   -Wl,--wrap=graphNextImageToPrint \
		   -Wl,--wrap=clock_gettime \
		   -Wl,--wrap=graphRegisterDrawFunction \
		   -Wl,--wrap=graphUnregisterDrawFunction \
		   -Wl,--wrap=graphRegisterDrawSprite \
		   -Wl,--wrap=graphDrawSprite \
		   -Wl,--wrap=graphUnregisterDrawSprite
