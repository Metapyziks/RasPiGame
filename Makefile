SRCDIR = src
BINDIR = bin

WIN-SRC = \
	$(SRCDIR)/lcd_win.c

RPI-SRC = \
	$(SRCDIR)/lcd_rpi.c

HDR = \
	$(SRCDIR)/utils.h \
	$(SRCDIR)/lcd.h \
	$(SRCDIR)/sprite.h

SRC = \
	$(SRCDIR)/main.c \
	$(SRCDIR)/sprite.c

AUX = \
	$(SRCDIR)/lcd.c

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

$(BINDIR)/$(TARGET): $(SRC) $(AUX) $(HDR) Makefile
	mkdir -p $(BINDIR)
	gcc $(SRC) $(CFLAGS) -o $(BINDIR)/$(TARGET)
