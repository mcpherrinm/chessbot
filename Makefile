CFLAGS=--std=c99 -Wall -Werror -Wno-unused
LDFLAGS=-lm
CC=gcc
LD=gcc

cli: cli.o cmd_impl.o al5d.o
	$(LD) $(LDFLAGS) -o $@ $^ -lreadline

sine: al5d.o sine.o
	$(LD) $(LDFLAGS) -o $@ $^

hover: al5d.o hover.o
	$(LD) $(LDFLAGS) -o $@ $^

rest: al5d.o rest.o
	$(LD) $(LDFLAGS) -o $@ $^

touch: al5d.o touch.o
	$(LD) $(LDFLAGS) -o $@ $^

hobo: main.o al5d.o
	$(LD) $(LDFLAGS) -o $@ $^ -lglfw

.PHONY: clean
clean: 
	-rm cli sine hover rest touch hobo *.o
