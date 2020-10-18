CC = gcc
LD = gcc
AS = nasm

CFLAGS = -c -m32
#CFLAGS = -c -m32 -DNO_XPM
#CFLAGS = -c -m32 -DHAVE_SDL2
LFLAGS = -m32 -lX11 -lXpm -lXm -lXt
#LFLAGS = -m32 -lX11 -lXm -lXt
#LFLAGS = -m32 -lX11 -lXpm -lSDL2 -lXm -lXt
AFLAGS = -f elf

all: fractald fractalf

fractald: fractald.o routinesd.o
	$(LD) fractald.o routinesd.o -o fractald $(LFLAGS)

fractalf: fractalf.o routinesf.o
	$(LD) fractalf.o routinesf.o -o fractalf $(LFLAGS)

routinesd.o: routinesd.asm
	$(AS) $(AFLAGS) routinesd.asm

routinesf.o: routinesf.asm
	$(AS) $(AFLAGS) routinesf.asm

fractald.o: fractald.c 
	$(CC) $(CFLAGS) fractald.c

fractalf.o: fractalf.c 
	$(CC) $(CFLAGS) fractalf.c

clean:
	rm -f *.o fractald fractalf
