#
# SPDX-License-Identifier: MIT
# 
# Copyright (c) 2025 Manuel Hernández Méndez
# 
# Authors:
#   Manuel Hernández Méndez <maherme.dev@gmail.com>
# 
 
DIR_BIN := $(PROJECT_ROOT)/bin
DIR_BIN_RLS := $(DIR_BIN)/rel
DIR_COV := $(DIR_BIN)/cov
DIR_SRC := $(PROJECT_ROOT)/src

DIR_TST := $(filter-out %/src/ %/bin/,$(dir $(wildcard $(DIR_SRC)/*/*/.)))
GCOV_EXCLUDE := $(foreach d,$(DIR_TST),$(addprefix -e ,$(d)))

VPATH := $(DIR_SRC) $(filter-out %/tst/ %/bin/,$(dir $(wildcard $(DIR_SRC)/*/*/.)))

INCLUDES := $(VPATH:%=-I%)

find_c_files = $(wildcard $(addsuffix /*.c,$(1)))
SRC:= $(foreach d,$(VPATH),$(call find_c_files,$(d)))
OBJ := $(notdir $(patsubst %.c, %.o, $(SRC)))
OBJ := $(addprefix $(DIR_BIN_RLS)/,$(OBJ))

DIR_MAKES := $(PROJECT_ROOT)/src/utils \
			 $(PROJECT_ROOT)/src/spaceship\
			 $(PROJECT_ROOT)/src/bullet \
			 $(PROJECT_ROOT)/src/graphic \
			 $(PROJECT_ROOT)/src/graphicglut \
			 $(PROJECT_ROOT)/src/keyboard \
			 $(PROJECT_ROOT)/src/engine \
			 $(PROJECT_ROOT)/src/physic \
			 $(PROJECT_ROOT)/src/explosion \
			 $(PROJECT_ROOT)/src/ufo \
			 $(PROJECT_ROOT)/src/aliens \
			 $(PROJECT_ROOT)/src/bunkers \
			 $(PROJECT_ROOT)/src/lives \
			 $(PROJECT_ROOT)/src/events \
			 $(PROJECT_ROOT)/src/fonts \
			 $(PROJECT_ROOT)/src/hud \
			 $(PROJECT_ROOT)/src/score \
			 $(PROJECT_ROOT)/src/collision
