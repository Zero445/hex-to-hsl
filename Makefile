CC=gcc
CFLAGS=-g --std=c99
LIBS=-lm

hextohsl: main.c
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

debug: hextohsl
	clear
	@gdb --args $^ -c "#ff0aff"

release: main.c
	$(CC) -O2 -o hextohsl $^ $(LIBS)

.PHONY: clean
clean:
	@rm hextohsl
