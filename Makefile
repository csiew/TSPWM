PREFIX?=/usr/include/X11
CFLAGS?=-Os -pedantic -Wall
COMPILER?=g++

all:
	$(COMPILER) $(CFLAGS) -I$(PREFIX)/include src/tspwm.cpp -L$(PREFIX)/lib -lX11 -o out/tspwm

clean:
	rm -f out/tspwm

