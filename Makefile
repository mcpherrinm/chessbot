CFLAGS=--std=c99 -Wall -Werror -Wno-unused
LDFLAGS=-lm
CC=gcc
LD=gcc

UNAME := $(shell uname)

GLLDFLAGS=
ifeq ($(UNAME), Darwin)
GLLDFLAGS=-framework Cocoa -framework OpenGL
endif

cli: cli.o cmd_impl.o al5d.o
	$(LD) $(LDFLAGS) -o $@ $^ -lreadline

sine: al5d.o sine.o
	$(LD) $(LDFLAGS) -o $@ $^

hobo: main.o al5d.o
	$(LD) $(LDFLAGS) $(GLLDFLAGS) -o $@ $^ -lglfw

.PHONY: clean
clean: 
	-rm cli sine hover rest touch hobo *.o
