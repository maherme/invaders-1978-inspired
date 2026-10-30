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
			-I../events/src

LDFLAGS += -Wl,--wrap=graphRegisterDrawFunction \
		   -Wl,--wrap=graphUnregisterDrawFunction \
		   -Wl,--wrap=graphRegisterDrawSprite \
		   -Wl,--wrap=graphDrawSprite \
		   -Wl,--wrap=graphUnregisterDrawSprite \
		   -Wl,--wrap=graphUnregisterPrint \
		   -Wl,--wrap=eventEmit
