TARGET = Audio_Mechanica_2025

PSP_EBOOT_ICON = ICON0.PNG

OBJS = logging.o psp_audio_ext.o main.o graphics.o framebuffer.o

INCDIR                 := ./include 
CFLAGS = -O2 -G0 -Wall
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti
ASFLAGS = $(CFLAGS)

LIBDIR = 
LDFLAGS =
LIBS = -lpspaudiolib -lpspaudio -lpsppower -lpsphprm -lpng -lz -lpspgu -lm

EXTRA_TARGETS = EBOOT.PBP
PSP_EBOOT_TITLE = Audio Mechanica 2025

PSPSDK=$(shell psp-config --pspsdk-path)

include $(PSPSDK)/lib/build.mak



