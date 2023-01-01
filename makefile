# ----------------------------
# Makefile Options
# ----------------------------

NAME = TINYAN
ICON = icon.png
DESCRIPTION = "TI NYAN ! (by SiniKraft)"
COMPRESSED = YES
ARCHIVED = YES

CFLAGS = -Wall -Wextra -Oz
CXXFLAGS = -Wall -Wextra -Oz

# ----------------------------

include $(shell cedev-config --makefile)
