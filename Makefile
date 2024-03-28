CFLAGS ?= -O2 -Wall

binary = defender

objects = \
	src/term.o \
	src/util.o \
	src/title.o \
	src/sprite.o \
	src/game.o \
	src/hiscore.o \
	src/starfield.o \
	src/canvas.o \
	src/scroller.o \
	src/keys.o \
	src/main.o

all: $(binary)

$(binary): $(objects)
	$(CC) $(LDFLAGS) -o $(binary) $(objects)

clean:
	rm -f $(binary) $(objects)

.PHONY: all clean
