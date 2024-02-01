# SPDX-License-Identifier: CC0-1.0
#
# SPDX-FileContributor: Antonio Niño Díaz, 2023

BLOCKSDS	?= /opt/blocksds/core

# User config

NAME		:= DSDonut
GAME_ICON      :=      icon.bmp
GAME_TITLE	:=	DSDonut
GAME_SUBTITLE1	:=	Hold on to your donut...
GAME_SUBTITLE2	:=	Wii Donut but on the DS

# Source code paths

SOURCEDIRS  := source
INCLUDEDIRS := include
GFXDIRS     := data/gfx
AUDIODIRS   := data/music
NITROFSDIR  := nitrofiles

# When a directory is added to NITROFSDIR, the Makefile rules will save the
# soundbank to that directory instead of the build directory.
NITROFSDIR	:= nitrofiles

# Libraries

LIBS		:= -lmm9 -lc -lnds9 -lm
LIBDIRS		:= $(BLOCKSDS)/libs/maxmod

include $(BLOCKSDS)/sys/default_makefiles/rom_arm9/Makefile
