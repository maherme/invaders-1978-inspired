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

INCLUDES += -I../graphic/src \
			-I../graphicglut/src \
			-I../utils/src

LDFLAGS += -Wl,--wrap=utilsCalloc \
		   -Wl,--wrap=utilsFree \
		   -Wl,--wrap=graphRegisterPrint \
		   -Wl,--wrap=graphUpdateSprite
