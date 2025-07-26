TARGET = PSP_Recording_Example
OBJS = logging.o psp_audio_ext.o main.o

INCDIR = 
CFLAGS = -O2 -G0 -Wall
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti
ASFLAGS = $(CFLAGS)

LIBDIR = 
LDFLAGS =
LIBS = -lpspaudiolib -lpspaudio -lpsphprm

EXTRA_TARGETS = EBOOT.PBP
PSP_EBOOT_TITLE = PSP Recording Example v0.2

PSPSDK=$(shell psp-config --pspsdk-path)

include $(PSPSDK)/lib/build.mak

copy:
	cp -r __SCE__${TARGET} /media/psp/psp/GAME/
	cp -r %__SCE__${TARGET} /media/psp/psp/GAME/

