SRCDIR = src
BINDIR = bin

WIN-SRC = \
	$(SRCDIR)/win/lcd.c

RPI-SRC = \
	$(SRCDIR)/rpi/lcd.c

HDR = \
	$(SRCDIR)/utils.h \
	$(SRCDIR)/lcd.h \
	$(SRCDIR)/sprite.h

SRC = \
	$(SRCDIR)/main.c \
	$(SRCDIR)/sprite.c

CFLAGS = -std=c99 -O3 -Wall -Wcast-align -Wcast-qual -Wimplicit \
	   -Wmissing-declarations -Wmissing-prototypes -Wnested-externs \
	   -Wpointer-arith -Wredundant-decls -Wshadow -Wstrict-prototypes

ifeq ($(OS),Windows_NT)
	TARGET = main.exe
	
	SRC += $(WIN-SRC)
	CFLAGS += -D WIN -lfreeglut -lglu32 -lopengl32
else
	TARGET = main

	SRC += $(RPI-SRC)
	CFLAGS += -D RPI
endif

$(BINDIR)/$(TARGET): $(SRC) $(HDR) Makefile
	mkdir -p $(BINDIR)
	gcc $(SRC) $(CFLAGS) -o $(BINDIR)/$(TARGET)
