# SPDX-License-Identifier: CC0-1.0
#
# SPDX-FileContributor: Antonio Niño Díaz, 2023

BLOCKSDS	?= /opt/blocksds/core

# User config

NAME		:= maxmod_nitrofs
GAME_TITLE	:= Maxmod NitroFS example

# Source code paths

SOURCEDIRS  := source
INCLUDEDIRS := 
GFXDIRS     := data/gfx
AUDIODIRS   := data/music
NITROFSDIR  := nitrofiles

# When a directory is added to NITROFSDIR, the Makefile rules will save the
# soundbank to that directory instead of the build directory.
NITROFSDIR	:= nitrofiles

# Libraries

LIBS		:= -lm -lnds9 -lmm9
LIBDIRS		:= $(BLOCKSDS)/libs/maxmod

include $(BLOCKSDS)/sys/default_makefiles/rom_arm9/Makefile
