LOCAL_DIR := $(GET_LOCAL_DIR)

TARGET := xenon-ppc64-stage1

MODULES += app/shell
#MODULES += app/tests
MODULES += lib/debugcommands
MODULES += lib/gfx
MODULES += unittest
#MODULES += lib/gfxconsole

DEBUG := 2
# WITH_TESTS := false
WITH_LINKER_GC := true
