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

INCLUDES += -I../fonts/src \
			-I../lives/src \
			-I../score/src \
			-I../graphicglut/src 

LDFLAGS += -Wl,--wrap=fontsCreateFontsHandler \
		   -Wl,--wrap=fontsInitLayout \
		   -Wl,--wrap=fontsDraw \
		   -Wl,--wrap=fontsDrawNumber \
		   -Wl,--wrap=livesCreate \
		   -Wl,--wrap=scoreInit
