CC="gcc"
CFLAGS="-Wall"
SDL2="`sdl2-config --cflags --libs`"

chip8: chip8.o display.o
	${CC} ${CFLAGS} chip8.o display.o -o chip8 ${SDL2}
chip8.o: chip8.c chip8.h
	${CC} ${CFLAGS} chip8.c -c

display.o: display.c display.h
	${CC} ${CFLAGS} display.c -c

clean:
	rm *.o
	rm chip8
