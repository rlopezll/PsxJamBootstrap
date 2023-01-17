TARGET = MAIN
TYPE = ps-exe

SRCS = main.c meshes\FType_PSX.tim data/accept.vag data/beep.vag data/bell.vag \
../../third_party/common/syscalls/printf.s \
../../third_party/common/crt0/crt0.s \
../../source/dcLibPsx/src/dcMemory.c\
../../source/dcLibPsx/src/dcCamera.c\
../../source/dcLibPsx/src/dcMath.c\
../../source/dcLibPsx/src/dcRender.c\
../../source/dcLibPsx/src/dcAudio.c\

CPPFLAGS += -I../../third_party/common
CPPFLAGS += -I../../third_party/psyq/include
CPPFLAGS += -I../../source/dcLibPsx/include
CPPFLAGS += -Wno-unused-variable
LDFLAGS += -L../../third_party/psyq/lib
LDFLAGS += -Wl,--start-group
LDFLAGS += -lapi
LDFLAGS += -lc
LDFLAGS += -lcd
LDFLAGS += -lc2
LDFLAGS += -lcard
LDFLAGS += -lcomb
LDFLAGS += -lds
LDFLAGS += -letc
LDFLAGS += -lgpu
LDFLAGS += -lgs
LDFLAGS += -lgte
LDFLAGS += -lgpu
LDFLAGS += -lgun
LDFLAGS += -lhmd
LDFLAGS += -lmath
LDFLAGS += -lmcrd
LDFLAGS += -lmcx
LDFLAGS += -lpad
LDFLAGS += -lpress
LDFLAGS += -lsio
LDFLAGS += -lsnd
LDFLAGS += -lspu
LDFLAGS += -ltap
LDFLAGS += -Wl,--end-group

include ../../third_party/common.mk
